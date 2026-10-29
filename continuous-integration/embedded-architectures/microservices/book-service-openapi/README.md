# Book Service With Open API

In this example, we can see how simple a RESTful service can be implemented with
**APIFlask**, a thin wrapper around Flask which generates an **OpenAPI**
description of the service from the decorated view functions and their
marshmallow schemas.

## Setup 

We start the web service from the command line:

```bash 
# Install APIFlask (includes Flask)
$ pip install apiflask

# Run service
$ python book_service.py
```


## OpenAPI Description

APIFlask generates the OpenAPI description at runtime:

* `http://localhost:8080/openapi.json`: OpenAPI description (JSON)
* `http://localhost:8080/docs`: interactive API documentation (Swagger UI)

```
$ curl http://localhost:8080/openapi.json
```

The description can also be exported to a file without starting the server:

```bash
$ flask --app book_service spec --output openapi.json
```

## Using the Swagger UI 

**Swagger UI** is a web application which renders an OpenAPI description as
interactive documentation. APIFlask serves it at `http://localhost:8080/docs`
and points it to the generated `/openapi.json`.
Note that the Swagger UI JavaScript and CSS files are loaded from a CDN
(`cdn.jsdelivr.net`), so the browser needs internet access.

The page lists all endpoints of the service, grouped by path. For each
operation we can see:

* The HTTP method and path, e.g. `GET /books/{oid}`.
* The summary, which APIFlask derives from the view function name
  (`find_by_id` becomes "Find By Id").
* The parameters, e.g. the path parameter `oid` of type `integer`.
* The request body schema, e.g. `BookCreateIn` for `POST /books`.
* All documented responses with their status codes and schemas,
  e.g. `200`, `404` and `422`.

At the bottom of the page, the **Schemas** section shows all data
structures used in requests and responses.

### Manual Testing

Swagger UI can also send real requests to the running service, so we can
test the API without `curl` or a separate REST client:

1. Open `http://localhost:8080/docs` in a browser.
2. Expand an operation, e.g. `POST /books`.
3. Click **Try it out**. The parameter and request body fields become
   editable. The request body is prefilled with an example derived from
   the schema.
4. Enter the values, e.g.:
   ```json
   {
     "id": 7,
     "author": "Mark Lutz",
     "title": "Learning Python",
     "isbn": "978-1098171308"
   }
   ```
5. Click **Execute**.

Swagger UI then shows:

* The equivalent `curl` command, which can be copied into a terminal.
* The request URL.
* The server response: status code, response body and response headers.

This makes it easy to try out error cases as well: requesting
`GET /books/{oid}` with an unknown `oid` returns `404`, and sending a
request body with missing fields returns `422` together with a list of the
validation errors.


## Implementation

The OpenAPI description is generated from the following artifacts in
`book_service.py`.

### APIFlask Application

Instead of a `Flask` object we create an `APIFlask` object. `APIFlask` is a
subclass of `Flask`, so everything else works as before. The `title` and
`version` arguments end up in the `info` section of the OpenAPI description.

```python
from apiflask import APIFlask, Schema, abort

app = APIFlask(__name__, title='Book Service', version='1.0.0')
```

The `APIFlask` object also registers the `/openapi.json` and `/docs`
endpoints.

### Schemas

Schemas describe the structure of request and response data. APIFlask uses
**marshmallow** schemas: a class derived from `Schema` with one attribute
per field. Each schema becomes an entry in the `components/schemas` section
of the OpenAPI description.

```python
from apiflask.fields import Integer, String, List, Nested

class BookIn(Schema):
    author = String(required=True)
    title = String(required=True)
    isbn = String(required=True)


class BookCreateIn(BookIn):
    id = Integer(required=True)


class BookOut(Schema):
    id = Integer()
    author = String()
    title = String()
    isbn = String()


class BooksOut(Schema):
    data = List(Nested(BookOut))
```

* **Input schemas** (`BookIn`, `BookCreateIn`) describe request bodies.
  `required=True` marks mandatory fields.
  `BookCreateIn` inherits the fields of `BookIn` and adds the `id`, which is
  only needed when a new book is created.
  
* **Output schemas** (`BookOut`, `BooksOut`) describe response bodies.
  `Nested` and `List` are used to describe composed structures like the
  `{"data": [...]}` response of `GET /books`.

Schemas are not only documentation: they are also used to validate and
serialize the data at runtime.

### Route Decorators

APIFlask provides shortcuts for the HTTP methods: `@app.get`, `@app.post`,
`@app.put` and `@app.delete` instead of
`@app.route(..., methods=[...])`. Path parameters like `<int:oid>` are
documented automatically, including their type.

### Input Decorator

`@app.input()` assigns an input schema to an endpoint.

```python
@app.post('/books')
@app.input(BookCreateIn)
@app.output(BookOut, status_code=HTTPStatus.CREATED)
def insert(json_data):
    ...
```

* The schema is added as `requestBody` to the OpenAPI description.
* The JSON request body is validated against the schema. If the validation
  fails, APIFlask returns a `422` response and the view function is not
  called. This `422` response is also documented automatically.
* The validated data is passed to the view function as the `json_data`
  argument (instead of calling `request.get_json()`).

### Output Decorator

`@app.output()` assigns an output schema and a status code (default `200`)
to an endpoint.

```python
@app.get('/books/<int:oid>')
@app.output(BookOut)
@app.doc(responses=[HTTPStatus.NOT_FOUND])
def find_by_id(oid):
    ...
            return item
    abort(HTTPStatus.NOT_FOUND, 'book not found')
```

* The schema and status code are added to the `responses` section of the
  OpenAPI description.
* The view function returns a plain Python object (a `dict`) instead of
  calling `jsonify()`. APIFlask serializes it with the schema; fields which
  are not defined in the schema are not part of the response.
* For `DELETE` we use an empty schema `{}` with status code `204`, because
  the response has no body.

### Documenting Error Responses

Error responses are not visible from the code, so we document them
explicitly with `@app.doc(responses=[...])`. Here we add `404` to every
endpoint which accesses a single book.

At runtime, `abort()` creates the error response, which has the same
structure as the documented `HTTPError` schema:

```json
{
  "detail": {},
  "message": "book not found"
}
```



## References
* [APIFlask Documentation](https://apiflask.com/)

* [Python-API-Development-Fundamentals: Lesson01](https://github.com/TrainingByPackt/Python-API-Development-Fundamentals/blob/master/Lesson01/Activity02/basic-api/app.py)


*Egon Teiniker, 2025-2026, GPL v3.0*