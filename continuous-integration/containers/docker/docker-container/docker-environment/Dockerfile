FROM python:3-alpine

# Default values for the configuration of app.py
ENV GREETING="Hello" \
    NAME="Docker"

WORKDIR /app
COPY app.py .

CMD ["python", "app.py"]
