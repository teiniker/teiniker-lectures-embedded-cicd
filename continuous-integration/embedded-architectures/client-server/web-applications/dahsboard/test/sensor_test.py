import unittest
from sensor import ClimateSensor, SimulatedClimateSensor


# Fake clock: returns the time set by the test
class FakeClock:
    def __init__(self):
        self.now = 0.0

    def __call__(self):
        return self.now


class SimulatedClimateSensorTest(unittest.TestCase):

    def setUp(self):
        self.clock = FakeClock()

    def test_is_climate_sensor(self):
        self.assertIsInstance(SimulatedClimateSensor(), ClimateSensor)

    def test_read_base_values(self):
        sensor = SimulatedClimateSensor(noise=0, clock=self.clock)
        self.assertEqual((22.0, 50.0), sensor.read())

    def test_read_after_quarter_period(self):
        sensor = SimulatedClimateSensor(noise=0, clock=self.clock)
        self.clock.now = 15.0
        self.assertEqual((25.0, 40.0), sensor.read())

    def test_read_rounded_to_one_decimal(self):
        sensor = SimulatedClimateSensor(clock=self.clock)
        for t in range(100):
            self.clock.now = t * 0.7
            temperature, humidity = sensor.read()
            self.assertEqual(temperature, round(temperature, 1))
            self.assertEqual(humidity, round(humidity, 1))

    def test_humidity_clamped(self):
        sensor = SimulatedClimateSensor(hum_base=50.0, hum_amplitude=200.0, clock=self.clock)
        for t in range(60):
            self.clock.now = t
            _, humidity = sensor.read()
            self.assertTrue(0.0 <= humidity <= 100.0)


if __name__ == '__main__':
    unittest.main()
