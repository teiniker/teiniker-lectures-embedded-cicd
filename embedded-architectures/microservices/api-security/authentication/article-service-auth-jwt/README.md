# JWT Authentication

In this example, we can see how simple a JWT authentication can be implemented 
in a REST service using **Flask**.

## Setup 
```bash
$ pip install flask-jwt-extended
```

We start the web service from the command line:
```bash
$ python article_service.py
```


## Access the REST Service

### Create a JWT

We need to log in to obtain a valid JWT:

```bash
$ curl -i -k -X POST https://localhost:8443/login -H "Content-Type: application/json" -d '{"username": "student", "password": "student"}'
```

The received JWT must be included in all subsequent requests as an Authorization header.

```
-H "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJmcmVzaCI6ZmFsc2UsImlhdCI6MTc2NTk4NjQxNywianRpIjoiZjY2NGE1OTEtY2I1MC00Yzk1LThiZjgtZDZlYmYzNzllZTEwIiwidHlwZSI6ImFjY2VzcyIsInN1YiI6InN0dWRlbnQiLCJuYmYiOjE3NjU5ODY0MTcsImNzcmYiOiIwOWNjYmMwNC01OWNlLTRmOTQtYjZjYS0wYzY2MDQzZjUxNGYiLCJleHAiOjE3NjU5ODczMTd9.Qk4kGFpuwq7Ss5HMYetWuNCFTum-eh7NVPmCASvdhaE"
```
We can analyze the generated JWT with an online 
[JSON Web Token (JWT) Debugger](https://www.jwt.io/)

![JWT Analysis](figures/JWT-Token.png)

Note that a new **JWT is generated with each login**.


### Find all Articles

```bash
$ curl -i -k https://localhost:8443/articles 
HTTP/1.1 401 UNAUTHORIZED
Server: Werkzeug/2.2.2 Python/3.9.2
Date: Thu, 12 Jan 2023 14:33:18 GMT
Content-Type: text/html; charset=utf-8
Content-Length: 19
WWW-Authenticate: Basic realm="Authentication Required"
Connection: close
```

```bash
$ curl -i -k -u student:student https://localhost:8443/articles -H "Authorization: Bearer <JWT>
HTTP/1.1 200 OK
Server: Werkzeug/2.2.2 Python/3.9.2
Date: Sat, 31 Dec 2022 12:13:49 GMT
Content-Type: application/json
Content-Length: 286
Connection: close

{
  "data": [
    {
      "description": "Effective Python",
      "id": 1,
      "price": 2390
    },
    {
      "description": "Effective Python",
      "id": 2,
      "price": 2390
    },
    {
      "description": "Effective Python",
      "id": 3,
      "price": 2390
    }
  ]
}
```

### Find a particular Article

```bash
$ curl -i -k -u student:student https://localhost:8443/articles/1 -H "Authorization: Bearer <JWT>
HTTP/1.1 200 OK
Server: Werkzeug/2.2.2 Python/3.9.2
Date: Sat, 31 Dec 2022 12:14:13 GMT
Content-Type: application/json
Content-Length: 68
Connection: close

{
  "description": "Effective Python",
  "id": 1,
  "price": 2390
}
```

### Insert an Article

```bash
$ curl -i -k -u student:student -X POST https://localhost:8443/articles -H "Authorization: Bearer <JWT> -H "Content-Type: application/json" -d '{"id":7, "description":"Learning Python", "price":5448}' 
HTTP/1.1 201 CREATED
Server: Werkzeug/2.2.2 Python/3.9.2
Date: Sat, 31 Dec 2022 12:16:59 GMT
Content-Type: application/json
Content-Length: 67
Connection: close

{
  "description": "Learning Python",
  "id": 7,
  "price": 5448
}
```

### Update an Article

```bash
$ curl -i -k -u student:student -X PUT https://localhost:8443/articles/2 -H "Authorization: Bearer <JWT> -H "Content-Type: application/json" -d '{"description":"Clean Code in Python", "price":3700}'
HTTP/1.1 200 OK
Server: Werkzeug/2.2.2 Python/3.9.2
Date: Sat, 31 Dec 2022 12:32:29 GMT
Content-Type: application/json
Content-Length: 72
Connection: close

{
  "description": "Clean Code in Python",
  "id": 2,
  "price": 3700
}
```

### Delete an Article

```bash
$ curl -i -k -u student:student -X DELETE https://localhost:8443/articles/1 -H "Authorization: Bearer <JWT>
HTTP/1.1 204 NO CONTENT
Server: Werkzeug/2.2.2 Python/3.9.2
Date: Sat, 31 Dec 2022 12:39:33 GMT
Content-Type: text/html; charset=utf-8
Connection: close
```


## References

* [Flask-JWT-Extended’s Documentation](https://flask-jwt-extended.readthedocs.io/en/stable/)

*Egon Teiniker, 2025, GPL v3.0*