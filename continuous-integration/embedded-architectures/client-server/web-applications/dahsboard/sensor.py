import math
import random
import time
from abc import ABC, abstractmethod


# Interface equivalent in Python
class ClimateSensor(ABC):
    @abstractmethod
    def read(self):
        """Return (temperature, humidity) in degree Celsius and %RH."""
        pass


# Simulated sensor: slow sine waves (one period per minute) plus measurement noise
class SimulatedClimateSensor(ClimateSensor):
    def __init__(self, temp_base=22.0, temp_amplitude=3.0,
                 hum_base=50.0, hum_amplitude=10.0,
                 period=60.0, noise=0.2, clock=time.monotonic):
        self.temp_base = temp_base
        self.temp_amplitude = temp_amplitude
        self.hum_base = hum_base
        self.hum_amplitude = hum_amplitude
        self.period = period
        self.noise = noise
        self.clock = clock
        self.start = clock()

    def read(self):
        phase = 2 * math.pi * (self.clock() - self.start) / self.period
        temperature = self.temp_base + self.temp_amplitude * math.sin(phase)
        # Humidity is phase shifted: it falls when the temperature rises
        humidity = self.hum_base - self.hum_amplitude * math.sin(phase)
        if self.noise > 0:
            temperature += random.gauss(0, self.noise)
            humidity += random.gauss(0, self.noise)
        humidity = min(max(humidity, 0.0), 100.0)
        return round(temperature, 1), round(humidity, 1)
