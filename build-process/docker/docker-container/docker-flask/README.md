# Example: Flask Image 

This example demonstrates how to build a Python image for a simple Python 
Flask application and its dependencies.

## Dockerfile

```dockerfile
# Use the official Python 3 base image
FROM python:3-alpine

# Set the working directory inside the container
WORKDIR /app  

# Inform Docker that the container listens on port 8080
EXPOSE 8080

# Copy the requirements file into the working directory
COPY requirements.txt ./  
# Install Python dependencies without caching wheels
RUN pip install --no-cache-dir -r requirements.txt  

# Copy the entire project into the container's working directory
COPY ./src .  
# Default command: run the hello.py script
CMD [ "python", "./book_service.py" ]  
```

The **Flask server's host, port, and debug settings** are configured in `book_service.py`:

```python 
if __name__ == '__main__':
    app.run(port=8080, debug=True, host='0.0.0.0')
```    

* `port=8080`: Sets the port number where the server will listen. 
    Default Flask port is `5000`.

* `debug=True`: Enables Flask’s debug mode:
    - Automatic code reloading when files change
    - Shows detailed error pages (debugger)

    Only use `debug=True` in development, not production, because 
    the debugger can execute arbitrary code.

* `host='0.0.0.0'`: Makes the app accessible externally, not just on your local machine.
    It listens on all network interfaces.
    Without it (default `127.0.0.1`), only our own computer can reach the app.


```bash
# Build the Docker image with the tag "flask-book-service"
$ docker build -t flask-book-service .

# List local images to confirm the image was created
$ docker image ls -a
REPOSITORY     TAG       IMAGE ID       CREATED          SIZE
flask-book-service   latest    707546f38c68   2 hours ago     60.7MB
```

## Start the Container with the REST Service 

```bash
# Run the image (container removed after exit with --rm)
$ docker run --rm -p 8080:8080 flask-book-service 
```

## Access the REST Service

We access the REST service in the Docker container using **curl**:

```bash
# Find all Books
$ curl -i http://localhost:8080/books

# Find a particular Book
$ curl -i http://localhost:8080/books/1

# Insert a Book
$ curl -i -X POST http://localhost:8080/books -H "Content-Type: application/json" -d '{"id":7, "author":"Wes McKinney ", "title":"Python for Data Analysis", "isbn":"978-1098104030"}'

# Update a Book
$ curl -i -X PUT http://localhost:8080/books/2 -H "Content-Type: application/json" -d '{"author":"Brett Slatkin","title":"Effective Python", "isbn":"0134853989"}'

# Delete a Book
$ curl -i -X DELETE http://localhost:8080/books/1
```

The `--rm` option removes the container immediately after use. 
To also remove the image, run the following command:

```bash
$ docker image rm flask-book-service
```


## References

* [DockerHub: Docker Official Image - python](https://hub.docker.com/_/python)

* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

*Egon Teiniker, 2025, GPL v3.0*
