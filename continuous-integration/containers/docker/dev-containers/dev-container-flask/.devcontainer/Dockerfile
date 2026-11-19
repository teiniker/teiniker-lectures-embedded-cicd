# Use the official Python image (Alpine variant) as the base image
FROM python:3-alpine

# Install git and bash for the development workflow
RUN apk add --no-cache git bash

# Install the Python libraries
# (the build context is the project directory, see devcontainer.json)
COPY requirements.txt /tmp/requirements.txt
RUN pip install --no-cache-dir --root-user-action=ignore \
        -r /tmp/requirements.txt \
    && rm /tmp/requirements.txt
