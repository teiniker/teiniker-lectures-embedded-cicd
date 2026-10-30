# Design: Climate Dashboard

This document describes the architecture and design of the Climate
Dashboard specified in [uc.md](uc.md). The application is implemented
with **Flask** on the server and **plain JavaScript** in the browser and
follows the **Model-View-Controller (MVC)** pattern.


## Architecture Overview

```
  Browser                               Flask Server
 +-----------------------+             +------------------------------+
 | View                  |   GET /     | Controller (app.py)          |
 |  templates/index.html |<------------|  index()                     |
 |  static/chart.js      |             |  measurements()              |
 |  static/dashboard.js  |  GET /api/  |  latest()                    |
 |   (polls every 1 s)   |------------>|                              |
 +-----------------------+   JSON      +--------------+---------------+
                                                      | uses
                                       +--------------v---------------+
                                       | Model                        |
                                       |  MeasurementService          |
                                       |   (sampling thread, 1 s,     |
                                       |    ring buffer of 60)        |
                                       |        | reads               |
                                       |  ClimateSensor (interface)   |
                                       |   SimulatedClimateSensor     |
                                       +------------------------------+
```

Two independent loops decouple measuring from displaying:

* **Server side:** a background thread reads the sensor once per second
    and stores the measurement, whether or not a browser is connected
    (User Story 2).
* **Client side:** JavaScript polls the JSON API once per second and
    redraws the charts. The HTML page is loaded only once (User
    Story 6).

The browser never accesses the model directly. All data flows through
the JSON API, which is also used by other programs (User Stories 3, 4).


## Project Layout

```
dashboard/
├── app.py                  Controller: Flask routes, application factory
├── sensor.py               Model: sensor interface and simulation
├── service.py              Model: Measurement, MeasurementService
├── pytest.ini              Makes the modules importable from test/
├── templates/
│   └── index.html          View: page skeleton with two <canvas> elements
├── static/
│   ├── chart.js            View: generic line chart drawing (no libraries)
│   └── dashboard.js        View: polling and updating both charts
└── test/
    ├── sensor_test.py
    ├── service_test.py
    └── app_test.py
```


## Model

The model contains the domain logic and knows nothing about Flask or
HTML. It can be tested without a web server.

### Measurement (`service.py`)

```python
@dataclass(frozen=True)
class Measurement:
    time: str           # "HH:MM:SS"
    temperature: float  # °C, one decimal place
    humidity: float     # %RH, one decimal place

    def to_dict(self):
        return asdict(self)
```

* An immutable value object for one measurement.
* `to_dict()` produces exactly the JSON format defined in `uc.md`.

### ClimateSensor (`sensor.py`)

```python
class ClimateSensor(ABC):
    @abstractmethod
    def read(self):
        """Return (temperature, humidity) in °C and %RH."""
        pass
```

* The interface for every sensor (User Story 1). A hardware sensor
    (e.g. an I2C sensor such as SHT31) implements `read()` and replaces
    the simulation without changes to the rest of the application.

### SimulatedClimateSensor (`sensor.py`)

```python
class SimulatedClimateSensor(ClimateSensor):
    def __init__(self, temp_base=22.0, temp_amplitude=3.0,
                 hum_base=50.0, hum_amplitude=10.0,
                 period=60.0, noise=0.2, clock=time.monotonic):
        ...

    def read(self):
        ...
        return round(temperature, 1), round(humidity, 1)
```

* Each value is a slow sine wave around its base value (one period per
    minute) plus Gaussian noise. The humidity curve is phase shifted,
    so the two charts look different.
* Humidity is clamped to the range `0.0` to `100.0`.
* The `clock` parameter allows tests to control the time; `noise=0`
    makes the values deterministic.

### MeasurementService (`service.py`)

```python
class MeasurementService:
    def __init__(self, sensor, interval=1.0, max_size=60,
                 clock=lambda: time.strftime('%H:%M:%S')):
        self.measurements = deque(maxlen=max_size)
        self.lock = threading.Lock()
        ...

    def measure(self):   # read sensor once, store and return Measurement
    def latest(self):    # newest Measurement or None
    def history(self):   # list of Measurements, oldest first
    def start(self):     # start the daemon sampling thread (once)
```

* `deque(maxlen=60)` acts as a ring buffer: appending the 61st value
    drops the oldest one (User Story 2).
* A `Lock` protects the buffer, because the sampling thread and the
    Flask request threads access it concurrently. `history()` returns a
    copy, so callers always get a consistent list.
* `measure()` is public, so tests can take measurements synchronously
    without starting the thread.
* The sensor and the clock are injected (dependency injection), which
    makes the service independent of the concrete sensor and testable
    with a stub.


## Controller (`app.py`)

The controller maps HTTP requests to model operations and selects the
response (HTML page or JSON).

```python
def create_app(service):
    app = Flask(__name__)

    @app.route('/')
    def index():
        return render_template('index.html')

    @app.route('/api/measurements', methods=['GET'])
    def measurements():
        return jsonify([m.to_dict() for m in service.history()])

    @app.route('/api/measurements/latest', methods=['GET'])
    def latest():
        measurement = service.latest()
        if measurement is None:
            return jsonify({'error': 'No measurement available'}), 503
        return jsonify(measurement.to_dict())

    return app


if __name__ == '__main__':
    service = MeasurementService(SimulatedClimateSensor())
    service.start()
    create_app(service).run(port=8080, debug=True, use_reloader=False)
```

* **Application factory:** `create_app(service)` receives the service
    as a parameter. Tests pass a service with a stub sensor and use
    Flask's test client; no sampling thread and no network are needed.
* The sampling thread is started only in `__main__`, not on import.
* `use_reloader=False`: in debug mode the reloader would run the
    application in a second process and start a second sampling thread.

### REST API

| Method | Path                       | Success           | Error             |
|--------|----------------------------|-------------------|-------------------|
| `GET`  | `/`                        | `200` HTML page   |                   |
| `GET`  | `/api/measurements`        | `200` JSON array  |                   |
| `GET`  | `/api/measurements/latest` | `200` JSON object | `503` JSON error  |

`/api/measurements` returns `[]` if no measurement exists yet
(User Story 4).


## View

The view runs in the browser. It consists of a static HTML skeleton and
two JavaScript files. No external libraries or CDNs are used, so the
dashboard works on a device without internet access (User Story 5).

### `templates/index.html`

```html
<h2>Climate Dashboard</h2>

<h3>Temperature</h3>
<canvas id="temperature-chart" width="800" height="300"></canvas>

<h3>Humidity</h3>
<canvas id="humidity-chart" width="800" height="300"></canvas>

<script src="{{ url_for('static', filename='chart.js') }}"></script>
<script src="{{ url_for('static', filename='dashboard.js') }}"></script>
<script>
    startDashboard("{{ url_for('measurements') }}", 1000);
</script>
```

* The page contains no measurement data; all values are loaded via the
    JSON API. `url_for()` keeps the API URL in one place (the
    controller).

### `static/chart.js`

```javascript
// Draws a line chart of one field of the measurements on a <canvas>.
function drawChart(canvas, measurements, options) {
    // options: { field: 'temperature', unit: '°C', color: '#c0392b' }
}
```

* A generic, reusable function: the same code draws both charts, only
    `options` differ (`temperature` / `°C` / red, `humidity` / `%RH` /
    blue).
* x-axis: measurement time (first, middle and last timestamp).
* y-axis: grid lines with value and unit, range derived from the shown
    values.
* With an empty list it draws the message "Waiting for measurements..."
    instead of a line (User Story 5).

### `static/dashboard.js`

```javascript
// Polls the JSON API periodically and redraws both charts.
function startDashboard(url, interval) {
    async function update() {
        try {
            const response = await fetch(url);
            if (!response.ok) throw new Error(response.status);
            const measurements = await response.json();
            drawChart(temperatureCanvas, measurements, TEMPERATURE);
            drawChart(humidityCanvas, measurements, HUMIDITY);
        } catch (error) {
            console.error('Update failed:', error);
        }
    }
    update();
    setInterval(update, interval);
}
```

* `update()` runs immediately and then every `interval` ms
    (User Story 6).
* One request provides the data for both charts, so they always show
    the same points in time.
* **Error handling:** if the request fails, the charts are not
    redrawn, so they keep the last values. The next interval simply
    tries again, so the dashboard recovers automatically once the
    server is reachable (User Story 6).


## Dynamic Behavior

```
Sampling thread        MeasurementService        Browser            Controller
      |                       |                     |                   |
      |--- measure() -------->|                     |                   |
      |                       |-- sensor.read()     |                   |
      |                       |-- append (locked)   |                   |
      |   sleep(1 s)          |                     |                   |
      |                       |                     |-- GET /api/ ----->|
      |                       |<------------------------- history() ----|
      |                       |------------------------ list ---------->|
      |                       |                     |<-- JSON array ----|
      |                       |                     |-- drawChart() x2  |
      |                       |                     |   wait 1 s        |
```


## Testing

Tests are unittest-style `*_test.py` files executed with `pytest`.

| Test file         | Tested unit              | Technique                    |
|-------------------|--------------------------|------------------------------|
| `sensor_test.py`  | `SimulatedClimateSensor` | `noise=0`, fake `clock`      |
| `service_test.py` | `MeasurementService`     | stub sensor, `measure()`     |
| `app_test.py`     | Controller, REST API     | `create_app()`, test client  |

Examples:

* The sensor returns values rounded to one decimal place; humidity
    stays within 0 to 100 %RH even with extreme parameters.
* After 61 calls of `measure()`, `history()` has 60 entries and the
    first measurement is gone.
* `GET /api/measurements/latest` returns `503` on an empty service and
    `200` with the newest measurement otherwise.
* `GET /api/measurements` returns `[]` on an empty service.

The JavaScript view is tested manually in the browser (charts are
drawn, update every second, survive a server restart).


## Traceability

| User Story                         | Components                         |
|------------------------------------|------------------------------------|
| 1 Simulate a Climate Sensor        | `sensor.py`                        |
| 2 Sample Measurements Periodically | `MeasurementService`               |
| 3 Get the Latest Measurement       | `latest()` route                   |
| 4 Get the Measurement History      | `measurements()` route             |
| 5 View the Dashboard               | `index.html`, `chart.js`           |
| 6 Update the Dashboard Live        | `dashboard.js`                     |
