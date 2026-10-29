# Example: Message Service With Swagger

This example extends the message service with an **OpenAPI** description and
a **Swagger UI**. Instead of plain Flask we use **APIFlask**, a thin wrapper
around Flask which generates the OpenAPI description from the decorated view
functions and their marshmallow schemas.

## Setup

Setup database table and test data:

```bash
$ sqlite3 message.db 
sqlite> 

CREATE TABLE messages
(
    address  INTEGER PRIMARY KEY,
    priority INTEGER NOT NULL,
    data     TEXT    NOT NULL
);

INSERT INTO messages (address, priority, data) VALUES (1123, 2, 'voltage=3.4V');
INSERT INTO messages (address, priority, data) VALUES (1124, 2, 'voltage=1.3V');
INSERT INTO messages (address, priority, data) VALUES (1125, 1, 'voltage=-1.7V');

sqlite> select * from messages;
1123|2|voltage=3.4V
1124|2|voltage=1.3V
1125|1|voltage=-1.7V

sqlite> .quit
```

Alternatively, the table can be created from the SQL scripts:

```bash
$ sqlite3 message.db < sql/setup.sql
```

Start the service:

```bash
# Install APIFlask (includes Flask)
$ pip install apiflask

# Run service
$ python message_service.py
```

## OpenAPI Description and Swagger UI

APIFlask generates the OpenAPI description at runtime and serves two
additional endpoints:

* `http://localhost:8080/openapi.json`: OpenAPI description (JSON)
* `http://localhost:8080/docs`: interactive API documentation (Swagger UI)

```bash
$ curl http://localhost:8080/openapi.json
```

The description can also be exported to a file without starting the server:

```bash
$ flask --app message_service spec --output openapi.json
```

**Swagger UI** renders the OpenAPI description as interactive documentation.
For each endpoint it shows the HTTP method and path, the parameters, the
request body schema and all documented responses. The **Schemas** section at
the bottom of the page lists all data structures.
Note that the Swagger UI JavaScript and CSS files are loaded from a CDN
(`cdn.jsdelivr.net`), so the browser needs internet access.

Swagger UI can also send requests to the running service:

1. Open `http://localhost:8080/docs` in a browser.
2. Expand an operation, e.g. `POST /v1/messages`.
3. Click **Try it out** and enter the request body, e.g.:
   ```json
   {
     "address": 1127,
     "priority": 3,
     "data": "voltage=5.0V"
   }
   ```
4. Click **Execute**.

Swagger UI shows the equivalent `curl` command, the request URL and the
server response (status code, body and headers).

## Implementation

### APIFlask Application

`APIFlask` is a subclass of `Flask`. The `title` and `version` arguments end up
in the `info` section of the OpenAPI description.

All resources are published under a version prefix in the URL
(e.g. `/v1/messages`). This allows us to introduce incompatible changes in a
new version (`/v2/messages`) while existing clients keep using `/v1`.
The routes of one API version are grouped in an `APIBlueprint` (the APIFlask
variant of a Flask `Blueprint`) with a `url_prefix`. The blueprint is then
registered at the application.

```python
from apiflask import APIBlueprint, APIFlask, Schema, abort

api_v1 = APIBlueprint('api_v1', __name__, url_prefix='/v1')

@api_v1.get('/messages')
@api_v1.output(MessagesOut)
def find_all():
    ...

app = APIFlask(__name__, title='Message Service', version='1.0.0')
app.register_blueprint(api_v1)
```

The OpenAPI endpoints `/openapi.json` and `/docs` belong to the application,
not to the blueprint, so they are not prefixed with `/v1`.

### Schemas

Schemas describe the structure of request and response data. Each schema
becomes an entry in the `components/schemas` section of the OpenAPI
description.

```python
from apiflask.fields import Integer, String, List, Nested

class MessageIn(Schema):
    priority = Integer(required=True)
    data = String(required=True)


class MessageCreateIn(MessageIn):
    address = Integer(required=True)


class MessageOut(Schema):
    address = Integer()
    priority = Integer()
    data = String()


class MessagesOut(Schema):
    data = List(Nested(MessageOut))
```

* **Input schemas** (`MessageIn`, `MessageCreateIn`) describe request bodies.
  The `address` is only part of the request body when a message is created.
  For an update, it is taken from the URL path.

* **Output schemas** (`MessageOut`, `MessagesOut`) describe response bodies.
  APIFlask serializes the `Message` objects returned by the DAO directly, so
  the `_to_dict()` helper function is no longer needed.

### Decorators

```python
@api_v1.put('/messages/<int:address>')
@api_v1.input(MessageIn)
@api_v1.output(MessageOut)
@api_v1.doc(responses=[HTTPStatus.NOT_FOUND])
def update(address, json_data):
    ...
```

* `@api_v1.get`, `@api_v1.post`, `@api_v1.put` and `@api_v1.delete` replace
  `@app.route(..., methods=[...])`.
* `@api_v1.input()` validates the JSON request body against the schema and
  passes the data as `json_data` argument. Invalid requests are rejected with
  `422` before the view function is called.
* `@api_v1.output()` defines the response schema and status code.
* `@api_v1.doc(responses=[...])` documents additional responses like `404`,
  which are created with `abort()`:
  ```json
  {
    "detail": {},
    "message": "message not found"
  }
  ```

## Access Message Service Using curl

### Find all Messages

```bash
$ curl -i http://localhost:8080/v1/messages 

HTTP/1.1 200 OK
Server: Werkzeug/3.1.8 Python/3.13.5
Date: Wed, 29 Apr 2026 05:39:50 GMT
Content-Type: application/json
Content-Length: 278
Connection: close

{
  "data": [
    {
      "address": 1123,
      "data": "voltage=3.4V",
      "priority": 2
    },
    {
      "address": 1124,
      "data": "voltage=1.3V",
      "priority": 2
    },
    {
      "address": 1125,
      "data": "voltage=-1.7V",
      "priority": 1
    }
  ]
}
```

### Find a particular Message

```bash
$ curl -i http://localhost:8080/v1/messages/1124

HTTP/1.1 200 OK
Server: Werkzeug/3.1.8 Python/3.13.5
Date: Wed, 29 Apr 2026 06:04:15 GMT
Content-Type: application/json
Content-Length: 65
Connection: close

{
  "address": 1124,
  "data": "voltage=1.3V",
  "priority": 2
}
```

### Insert a Message

```bash
$ curl -i -X POST localhost:8080/v1/messages -H "Content-Type: application/json" -d '{"address": 1127, "priority": 3, "data":"voltage=5.0V"}'

HTTP/1.1 201 CREATED
Server: Werkzeug/3.1.8 Python/3.13.5
Date: Wed, 29 Apr 2026 06:04:41 GMT
Content-Type: application/json
Content-Length: 65
Connection: close

{
  "address": 1127,
  "data": "voltage=5.0V",
  "priority": 3
}
```

### Update a Message

```bash
$ curl -i -X PUT localhost:8080/v1/messages/1124 -H "Content-Type: application/json" -d '{"priority": 9, "data":"voltage=1.0V"}'

HTTP/1.1 200 OK
Server: Werkzeug/3.1.8 Python/3.13.5
Date: Wed, 29 Apr 2026 06:07:25 GMT
Content-Type: application/json
Content-Length: 65
Connection: close

{
  "address": 1124,
  "data": "voltage=1.0V",
  "priority": 9
}
```

### Delete a Message

```bash
$ curl -i -X DELETE http://localhost:8080/v1/messages/1124

HTTP/1.1 204 NO CONTENT
Server: Werkzeug/3.1.8 Python/3.13.5
Date: Wed, 29 Apr 2026 06:07:56 GMT
Content-Type: text/html; charset=utf-8
Connection: close
```

## References
* [APIFlask Documentation](https://apiflask.com/)
* [Swagger UI](https://swagger.io/tools/swagger-ui/)

*Egon Teiniker, 2025-2026, GPL v3.0*
