# Use the official Python image (Alpine variant) as the base image
FROM python:3-alpine

WORKDIR /app

# Install the Python libraries (Flask and the MariaDB driver PyMySQL)
COPY requirements.txt ./
RUN pip install --no-cache-dir --root-user-action=ignore -r requirements.txt

EXPOSE 8080

# Copy the service implementation
COPY *.py ./

CMD [ "python", "./message_service.py" ]
