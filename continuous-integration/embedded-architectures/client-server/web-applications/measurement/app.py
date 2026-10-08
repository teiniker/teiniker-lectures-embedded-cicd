from flask import Flask, render_template, jsonify
from sensor import SimulatedTemperatureSensor, MeasurementService

app = Flask(__name__)

service = MeasurementService(SimulatedTemperatureSensor(), interval=1.0, max_size=60)

# http://localhost:8080/
@app.route('/')
def index():
    return render_template('index.html', measurement=service.latest())

# http://localhost:8080/api/temperature
@app.route('/api/temperature', methods=['GET'])
def temperature():
    measurement = service.latest()
    if measurement is None:
        return jsonify({'error': 'No measurement available'}), 503
    return jsonify(measurement)

# http://localhost:8080/api/measurements
@app.route('/api/measurements', methods=['GET'])
def measurements():
    return jsonify(service.history())


if __name__ == '__main__':
    service.start()
    # use_reloader=False: the reloader would start the sampling thread twice
    app.run(port=8080, debug=True, use_reloader=False)
