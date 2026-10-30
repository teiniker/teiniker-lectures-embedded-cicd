import unittest
from app import create_app
from service import MeasurementService


class StubSensor:
    def read(self):
        return 22.4, 48.3


class AppTest(unittest.TestCase):

    def setUp(self):
        self.service = MeasurementService(StubSensor(), clock=lambda: '16:02:32')
        self.client = create_app(self.service).test_client()

    def test_index(self):
        response = self.client.get('/')
        self.assertEqual(200, response.status_code)
        html = response.get_data(as_text=True)
        self.assertIn('id="temperature-chart"', html)
        self.assertIn('id="humidity-chart"', html)
        self.assertNotIn('http', html)  # no external resources

    def test_measurements_empty(self):
        response = self.client.get('/api/measurements')
        self.assertEqual(200, response.status_code)
        self.assertEqual([], response.get_json())

    def test_measurements(self):
        self.service.measure()
        self.service.measure()
        response = self.client.get('/api/measurements')
        self.assertEqual(200, response.status_code)
        expected = {'time': '16:02:32', 'temperature': 22.4, 'humidity': 48.3}
        self.assertEqual([expected, expected], response.get_json())

    def test_latest_empty(self):
        response = self.client.get('/api/measurements/latest')
        self.assertEqual(503, response.status_code)
        self.assertIn('error', response.get_json())

    def test_latest(self):
        self.service.measure()
        response = self.client.get('/api/measurements/latest')
        self.assertEqual(200, response.status_code)
        self.assertEqual({'time': '16:02:32', 'temperature': 22.4, 'humidity': 48.3},
                         response.get_json())


if __name__ == '__main__':
    unittest.main()
