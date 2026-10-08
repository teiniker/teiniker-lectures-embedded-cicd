import math
import random
import threading
import time
from abc import ABC, abstractmethod
from collections import deque


# Interface equivalent in Python
class TemperatureSensor(ABC):
    @abstractmethod
    def read(self):
        """Return the current temperature in degree Celsius."""
        pass


# Simulated sensor: slow sine wave (one period per minute) plus measurement noise
class SimulatedTemperatureSensor(TemperatureSensor):
    def __init__(self, base=22.0, amplitude=3.0, period=60.0, noise=0.2):
        self.base = base
        self.amplitude = amplitude
        self.period = period
        self.noise = noise
        self.start = time.time()

    def read(self):
        t = time.time() - self.start
        value = self.base + self.amplitude * math.sin(2 * math.pi * t / self.period)
        value += random.gauss(0, self.noise)
        return round(value, 1)


# Periodically reads the sensor and stores the last measurements
class MeasurementService:
    def __init__(self, sensor, interval=1.0, max_size=60):
        self.sensor = sensor
        self.interval = interval
        self.measurements = deque(maxlen=max_size)
        self.lock = threading.Lock()
        self.thread = None

    def measure(self):
        measurement = {'time': time.strftime('%H:%M:%S'), 'temperature': self.sensor.read()}
        with self.lock:
            self.measurements.append(measurement)
        return measurement

    def latest(self):
        with self.lock:
            return self.measurements[-1] if self.measurements else None

    def history(self):
        with self.lock:
            return list(self.measurements)

    def start(self):
        # Sampling loop runs in a background (daemon) thread
        def run():
            while True:
                self.measure()
                time.sleep(self.interval)

        if self.thread is None:
            self.thread = threading.Thread(target=run, daemon=True)
            self.thread.start()
