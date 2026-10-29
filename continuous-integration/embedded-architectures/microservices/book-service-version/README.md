# Book Service 

In this example, we can see how simple a RESTful service can be implemented with
**Flask**.

## Setup 

We start the web service from the command line:

```bash 
# Run service
$ python book_service.py
```

## API Versioning

All resources are published under a version prefix in the URL
(e.g. `/v1/books`). This allows us to introduce incompatible changes in a
new version (`/v2/books`) while existing clients keep using `/v1`.

In Flask, the routes of one API version are grouped in a **Blueprint** with a
`url_prefix`:

```python
api_version = Blueprint('api_v1', __name__, url_prefix='/v1')

@api_version.route('/books', methods = ['GET'])
def find_all():
    ...

app = Flask(__name__)
app.register_blueprint(api_version)
```

## Access the REST Service

### Find all Articles
```
$ curl -i http://localhost:8080/v1/books

HTTP/1.1 200 OK
Server: Werkzeug/3.1.8 Python/3.13.5
Date: Tue, 21 Apr 2026 11:00:31 GMT
Content-Type: application/json
Content-Length: 401
Connection: close

{
  "data": [
    {
      "author": "Eric Matthes",
      "id": 1,
      "isbn": "978-1718502703",
      "title": "Python Crash Course"
    },
    {
      "author": "Brett Slatkin",
      "id": 2,
      "isbn": "978-0134853987",
      "title": "Effective Python"
    },
    {
      "author": "Luciano Ramalho",
      "id": 3,
      "isbn": "978-1492056355",
      "title": "Fluent Python"
    }
  ]
}
```

### Find a particular Article
```
$ curl -i http://localhost:8080/v1/books/1

HTTP/1.1 200 OK
Server: Werkzeug/3.1.8 Python/3.13.5
Date: Tue, 21 Apr 2026 11:04:13 GMT
Content-Type: application/json
Content-Length: 104
Connection: close

{
  "author": "Eric Matthes",
  "id": 1,
  "isbn": "978-1718502703",
  "title": "Python Crash Course"
}
```

### Insert an Article

```
$ curl -i -X POST localhost:8080/v1/books -H "Content-Type: application/json" -d '{"id":7, "author": "Mark Lutz", "isbn": "978-1098171308", "title": "Learning Python: Powerful Object-Oriented Programming"}'

HTTP/1.1 201 CREATED
Server: Werkzeug/3.1.8 Python/3.13.5
Date: Tue, 21 Apr 2026 11:08:16 GMT
Content-Type: application/json
Content-Length: 135
Connection: close

{
  "author": "Mark Lutz",
  "id": 7,
  "isbn": "978-1098171308",
  "title": "Learning Python: Powerful Object-Oriented Programming"
}
```

### Update an Article

```
$ curl -i -X PUT localhost:8080/v1/books/2 -H "Content-Type: application/json" -d '{"author": "Brett Slatkin","isbn": "978-0134853987","title": "Effective Python, 3rd Edition"}'

HTTP/1.1 200 OK
Server: Werkzeug/3.1.8 Python/3.13.5
Date: Tue, 21 Apr 2026 11:12:01 GMT
Content-Type: application/json
Content-Length: 115
Connection: close

{
  "author": "Brett Slatkin",
  "id": 2,
  "isbn": "978-0134853987",
  "title": "Effective Python, 3rd Edition"
}
```

### Delete an Article

```
$ curl -i -X DELETE http://localhost:8080/v1/books/1

HTTP/1.1 204 NO CONTENT
Server: Werkzeug/3.1.8 Python/3.13.5
Date: Tue, 21 Apr 2026 11:12:39 GMT
Content-Type: text/html; charset=utf-8
Connection: close
```

## References
* [Python-API-Development-Fundamentals: Lesson01](https://github.com/TrainingByPackt/Python-API-Development-Fundamentals/blob/master/Lesson01/Activity02/basic-api/app.py)


*Egon Teiniker, 2025-2026, GPL v3.0*