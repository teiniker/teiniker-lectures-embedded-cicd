import threading
import unittest
from service import Measurement, MeasurementService


# Stub sensor: returns increasing temperature values
class StubSensor:
    def __init__(self):
        self.count = 0

    def read(self):
        self.count += 1
        return float(self.count), 50.0


class MeasurementServiceTest(unittest.TestCase):

    def setUp(self):
        self.service = MeasurementService(StubSensor(), max_size=60, clock=lambda: '12:00:00')

    def test_measurement_to_dict(self):
        m = Measurement('16:02:32', 22.4, 48.3)
        self.assertEqual({'time': '16:02:32', 'temperature': 22.4, 'humidity': 48.3}, m.to_dict())

    def test_empty_service(self):
        self.assertIsNone(self.service.latest())
        self.assertEqual([], self.service.history())

    def test_measure(self):
        m = self.service.measure()
        self.assertEqual(Measurement('12:00:00', 1.0, 50.0), m)
        self.assertEqual(m, self.service.latest())

    def test_history_oldest_first(self):
        for _ in range(3):
            self.service.measure()
        self.assertEqual([1.0, 2.0, 3.0], [m.temperature for m in self.service.history()])

    def test_ring_buffer_keeps_last_60(self):
        for _ in range(61):
            self.service.measure()
        history = self.service.history()
        self.assertEqual(60, len(history))
        self.assertEqual(2.0, history[0].temperature)
        self.assertEqual(61.0, history[-1].temperature)

    def test_history_returns_copy(self):
        self.service.measure()
        history = self.service.history()
        self.service.measure()
        self.assertEqual(1, len(history))

    def test_concurrent_access(self):
        def writer():
            for _ in range(1000):
                self.service.measure()

        thread = threading.Thread(target=writer)
        thread.start()
        while thread.is_alive():
            history = self.service.history()
            self.assertLessEqual(len(history), 60)
        thread.join()
        self.assertEqual(1000.0, self.service.latest().temperature)


if __name__ == '__main__':
    unittest.main()
