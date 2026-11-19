import os

# Read the configuration from environment variables (with default values)
greeting = os.environ.get('GREETING', 'Hello')
name = os.environ.get('NAME', 'World')
count = int(os.environ.get('COUNT', '1'))

for _ in range(count):
    print(f'{greeting} {name}!')
