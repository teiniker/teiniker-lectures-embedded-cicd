# Use the official Python 3 base image
FROM python:3-alpine

# Set the working directory inside the container
WORKDIR /app

# Copy the requirements file into the working directory
COPY requirements.txt ./
# Install Python dependencies without caching wheels
RUN pip install --no-cache-dir -r requirements.txt

# Copy the Python source code into the container's working directory
COPY ./src .
# Default command: run the password_bcrypt.py script
CMD [ "python", "./password_bcrypt.py" ]
