# Temperature Measurement

Flask web application that simulates a temperature sensor and shows the
measured values in a **live diagram** in the browser.

## Setup 

We start the web application from the command line:
```
$ python app.py
```

Using a Browser, we can access the web page at:
```
http://localhost:8080/
```

The JSON API can also be used directly, e.g. from another program:
```
$ curl http://localhost:8080/api/temperature
{"temperature":23.0,"time":"16:02:32"}

$ curl http://localhost:8080/api/measurements
[{"temperature":22.0,"time":"16:02:29"},{"temperature":22.4,"time":"16:02:30"}, ...]
```

The **measurement** (server) and the **visualization** (browser) are
decoupled:

* On the server, a **background thread** reads the sensor once per second,
  independently of any client, and keeps the last 60 values.

* In the browser, JavaScript **polls** the JSON API once per second and
  redraws the diagram. The page itself is loaded only once.


## Implementation

* **Model** (`sensor.py`)

```python
class TemperatureSensor(ABC):
    @abstractmethod
    def read(self):
        pass
```

- `TemperatureSensor` is the interface for a sensor.
- `SimulatedTemperatureSensor` implements it: a sine wave around 22 °C
  (amplitude 3 °C, one period per minute) plus Gaussian measurement noise.
  On real hardware, we would add e.g. an `I2CTemperatureSensor` that
  implements `read()`, without changing the rest of the application.
- `MeasurementService` reads the sensor periodically in a daemon thread and
  stores the values in a `deque(maxlen=60)` (ring buffer). A `Lock` protects
  the buffer, because the sampling thread and the Flask request handlers
  access it concurrently.

* **Controller** (`app.py`)

```python
service = MeasurementService(SimulatedTemperatureSensor(), interval=1.0, max_size=60)

@app.route('/')
def index():
    return render_template('index.html', measurement=service.latest())

@app.route('/api/measurements', methods=['GET'])
def measurements():
    return jsonify(service.history())
```

- `index()` renders the HTML page with the latest measurement.
- `measurements()` returns the stored values as JSON - this is the data
  source for the live diagram.
- `temperature()` returns only the latest value (`503 Service Unavailable`
  if no value has been measured yet).

```python
if __name__ == '__main__':
    service.start()
    app.run(port=8080, debug=True, use_reloader=False)
```

- `use_reloader=False`: In debug mode, Flask's reloader runs the application
  in a second process, which would start a second sampling thread.

* **View** (`templates/index.html`, `static/chart.js`)

```html
<canvas id="chart" width="800" height="400"></canvas>

<script src="{{ url_for('static', filename='chart.js') }}"></script>
<script>
    startLiveChart("{{ url_for('measurements') }}", 1000);
</script>
```

- Files in the `static/` directory are delivered unchanged by Flask.
- `startLiveChart()` uses `fetch()` to request `/api/measurements` every
  1000 ms and draws the values on the `<canvas>` element.
- The diagram is drawn with plain JavaScript, **without external libraries**, 
  so the page also works on an embedded device without internet access.


*Egon Teiniker, 2020-2026, GPL v3.0*
