import threading
import time
from collections import deque
from dataclasses import dataclass, asdict


# Immutable value object for one measurement
@dataclass(frozen=True)
class Measurement:
    time: str           # "HH:MM:SS"
    temperature: float  # degree Celsius, one decimal place
    humidity: float     # %RH, one decimal place

    def to_dict(self):
        return asdict(self)


# Periodically reads the sensor and stores the last measurements
class MeasurementService:
    def __init__(self, sensor, interval=1.0, max_size=60,
                 clock=lambda: time.strftime('%H:%M:%S')):
        self.sensor = sensor
        self.interval = interval
        self.clock = clock
        self.measurements = deque(maxlen=max_size)  # ring buffer
        self.lock = threading.Lock()
        self.thread = None

    def measure(self):
        temperature, humidity = self.sensor.read()
        measurement = Measurement(self.clock(), temperature, humidity)
        with self.lock:
            self.measurements.append(measurement)
        return measurement

    def latest(self):
        with self.lock:
            return self.measurements[-1] if self.measurements else None

    def history(self):
        # Return a copy, so callers get a consistent list
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
