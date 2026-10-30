from flask import Flask, render_template, jsonify
from sensor import SimulatedClimateSensor
from service import MeasurementService


def create_app(service):
    app = Flask(__name__)

    # http://localhost:8080/
    @app.route('/')
    def index():
        return render_template('index.html')

    # http://localhost:8080/api/measurements
    @app.route('/api/measurements', methods=['GET'])
    def measurements():
        return jsonify([m.to_dict() for m in service.history()])

    # http://localhost:8080/api/measurements/latest
    @app.route('/api/measurements/latest', methods=['GET'])
    def latest():
        measurement = service.latest()
        if measurement is None:
            return jsonify({'error': 'No measurement available'}), 503
        return jsonify(measurement.to_dict())

    return app


if __name__ == '__main__':
    service = MeasurementService(SimulatedClimateSensor(), interval=1.0, max_size=60)
    service.start()
    # use_reloader=False: the reloader would start the sampling thread twice
    create_app(service).run(port=8080, debug=True, use_reloader=False)
