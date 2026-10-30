# Epic: Climate Dashboard

A Flask web application that periodically measures temperature and
humidity (simulated values) and shows them over time as live diagrams
in the browser. Other programs access the measurements through a JSON
API.

Each measurement is a JSON object with the following fields:

```json
{"time": "16:02:32", "temperature": 22.4, "humidity": 48.3}
```

* `time`: local time of the measurement (`HH:MM:SS`)
* `temperature`: degree Celsius, rounded to one decimal place
* `humidity`: relative humidity in percent (%RH), rounded to one
    decimal place


## User Story 1: Simulate a Climate Sensor

**As a** developer

**I want to** read temperature and humidity from a simulated sensor

**So that** I can develop and test the dashboard without real hardware

* **Acceptance Criteria:**
* **Given** a simulated sensor
* **When** the sensor is read
* **Then** it returns a temperature and a humidity value, each rounded
    to one decimal place

* **Given** a simulated sensor
* **When** the sensor is read repeatedly over time
* **Then** the values change slowly around a base value (e.g. 22 °C
    and 50 %RH) with small random measurement noise

* **Given** a simulated sensor
* **When** the humidity is read
* **Then** the value is always within 0.0 and 100.0 %RH

* **Given** the sensor is defined by an interface with a `read()`
    method
* **When** a real hardware sensor implements this interface
* **Then** it can replace the simulated sensor without changes to the
    rest of the application


## User Story 2: Sample Measurements Periodically

**As an** operator

**I want** the application to measure temperature and humidity once
per second, independently of any browser

**So that** a continuous history is available whenever I open the
dashboard

* **Acceptance Criteria:**
* **Given** the application has been started
* **When** one second has passed
* **Then** a new measurement is stored, even if no client is connected

* **Given** 60 measurements are already stored
* **When** a new measurement is taken
* **Then** the oldest measurement is removed, so that at most the last
    60 measurements are kept

* **Given** a measurement is being stored by the sampling thread
* **When** a client requests the measurements at the same time
* **Then** the client receives a consistent list without errors


## User Story 3: Get the Latest Measurement

**As an** API client

**I want to** retrieve the most recent measurement

**So that** I can process the current temperature and humidity in
another program

* **Acceptance Criteria:**
* **Given** at least one measurement exists
* **When** a `GET` request is sent to `/api/measurements/latest`
* **Then** the system returns HTTP `200 OK` with the newest measurement
    object

* **Given** no measurement has been taken yet
* **When** a `GET` request is sent to `/api/measurements/latest`
* **Then** the system returns HTTP `503 Service Unavailable` with a
    JSON error message


## User Story 4: Get the Measurement History

**As an** API client

**I want to** retrieve all stored measurements in a single request

**So that** I can display or analyze the values over time

* **Acceptance Criteria:**
* **Given** one or more measurements exist
* **When** a `GET` request is sent to `/api/measurements`
* **Then** the system returns HTTP `200 OK` with a JSON array of the
    stored measurements, ordered from oldest to newest

* **Given** no measurement has been taken yet
* **When** a `GET` request is sent to `/api/measurements`
* **Then** the system returns HTTP `200 OK` with an empty JSON array
    `[]`


## User Story 5: View the Dashboard

**As a** user

**I want to** see temperature and humidity over time in two separate
diagrams

**So that** I can recognize trends of both values at a glance

* **Acceptance Criteria:**
* **Given** the application is running
* **When** I open `http://localhost:8080/` in a browser
* **Then** the page shows one line chart for temperature and one line
    chart for humidity

* **Given** the dashboard is displayed
* **When** I look at a chart
* **Then** the x-axis shows the measurement time, and the y-axis shows
    the value with its unit (°C for temperature, %RH for humidity)

* **Given** no measurement has been taken yet
* **When** I open the dashboard
* **Then** the page is displayed without errors and the charts are
    empty

* **Given** the device has no internet access
* **When** I open the dashboard
* **Then** the page and both charts are displayed correctly, because no
    external libraries or resources are loaded


## User Story 6: Update the Dashboard Live

**As a** user

**I want** the diagrams to update automatically

**So that** I always see the current values without reloading the page

* **Acceptance Criteria:**
* **Given** the dashboard is open in the browser
* **When** one second has passed
* **Then** the browser requests `/api/measurements` and redraws both
    charts with the returned values, without reloading the page

* **Given** the dashboard is open and the server becomes unreachable
* **When** a request to `/api/measurements` fails
* **Then** the charts keep the last displayed values and the dashboard
    resumes updating once the server is reachable again
