# Example: Message Service + MariaDB

In this example we use the Message Service from
`embedded-architectures/microservices/message-service` and run it with
Docker Compose. Instead of a local SQLite file, the messages are stored
in a **MariaDB** database server.

We use two containers:
* `mariadb`: The MariaDB database server.
* `message-service`: The Python interpreter with Flask and the Message
    Service implementation.

## Project Structure

```
docker-compose-message-service/
├── docker-compose.yml
├── message-service/
│   ├── Dockerfile
│   ├── requirements.txt
│   ├── message.py
│   ├── message_dao.py
│   └── message_service.py
└── sql/
    └── setup.sql
```


## Database Setup

The official MariaDB image executes all `*.sql` files found in the
directory `/docker-entrypoint-initdb.d` when the server initializes for
the first time (with an empty volume). We mount our `sql/` directory to
this location, so `sql/setup.sql` creates the table and the test data:

```sql
CREATE TABLE messages
(
    address  INT          NOT NULL PRIMARY KEY,
    priority INT          NOT NULL,
    data     VARCHAR(255) NOT NULL
) ENGINE = INNODB;

INSERT INTO messages (address, priority, data) VALUES (1123, 2, 'voltage=3.4V');
INSERT INTO messages (address, priority, data) VALUES (1124, 2, 'voltage=1.3V');
INSERT INTO messages (address, priority, data) VALUES (1125, 1, 'voltage=-1.7V');
```

Compared to the SQLite version, the column types are changed to MariaDB
types (`INT`, `VARCHAR(255)`).


## Message Service

### Database Access

The only class which has to change is `MessageDao`. Instead of the
`sqlite3` module, we use the **PyMySQL** library, a pure Python driver
for MySQL and MariaDB:

```python
import os
import pymysql
from pymysql.constants import CLIENT
from message import Message


class MessageDao:
    """Data Access Object for the messages MariaDB table."""

    def __init__(self) -> None:
        # The connection parameters are passed as environment variables
        # (see docker-compose.yml)
        self.config = {
            'host': os.environ.get('DB_HOST', 'localhost'),
            'port': int(os.environ.get('DB_PORT', '3306')),
            'user': os.environ.get('DB_USER', 'student'),
            'password': os.environ.get('DB_PASSWORD', 'student'),
            'database': os.environ.get('DB_NAME', 'testdb'),
            'autocommit': True,
            # Report matched rows (not changed rows) for UPDATE, so an
            # UPDATE with unchanged values still returns rowcount 1
            'client_flag': CLIENT.FOUND_ROWS,
        }

    def _connect(self) -> pymysql.connections.Connection:
        return pymysql.connect(**self.config)

    def find_by_id(self, address: int) -> Message | None:
        with self._connect() as conn, conn.cursor() as cursor:
            cursor.execute(
                'SELECT address, priority, data FROM messages '
                'WHERE address = %s',
                (address,)
            )
            row = cursor.fetchone()
        if row is None:
            return None
        return Message(address=row[0], priority=row[1], data=row[2])
    ...
```

* **Connection parameters:** The database host, port, user, password and
    database name are read from **environment variables**. So the same
    image can be used with different databases, without changing the code.
* **Placeholders:** PyMySQL uses `%s` instead of `?` as placeholder in
    SQL statements. The values are still passed separately, so they are
    escaped by the driver (no SQL injection).
* **`autocommit`:** Every statement is committed immediately, like the
    `with sqlite3.connect(...)` block in the SQLite version.
* **`CLIENT.FOUND_ROWS`:** By default, MariaDB returns the number of
    **changed** rows for an `UPDATE`. If the new values are equal to the
    old ones, this would be 0 and the service would respond with
    `404 NOT FOUND`. With this flag, MariaDB returns the number of
    **matched** rows, like SQLite.

The classes `Message` and the Flask routes in `message_service.py` are
the same as in the original service. The only change in
`message_service.py` is the `host` parameter:

```python
if __name__ == '__main__':
    app.run(port=8080, debug=True, host='0.0.0.0')
```

By default, Flask only listens on `127.0.0.1`, which is the loopback
interface **inside** the container. With `host='0.0.0.0'`, the service
listens on all interfaces and can be reached via the port mapping.


### Dockerfile

```dockerfile
FROM python:3-alpine

WORKDIR /app

# Install the Python libraries (Flask and the MariaDB driver PyMySQL)
COPY requirements.txt ./
RUN pip install --no-cache-dir --root-user-action=ignore -r requirements.txt

EXPOSE 8080

# Copy the service implementation
COPY *.py ./

CMD [ "python", "./message_service.py" ]
```

`requirements.txt` is copied and installed **before** the source code.
So when we only change the Python files, Docker reuses the cached layer
with the installed libraries and the build is fast.

`requirements.txt` pins the versions of Flask and its dependencies, plus
`PyMySQL==1.2.4`.


## Docker Compose File

```yaml
services:
  mariadb:
    image: mariadb:13.0.2
    container_name: mariadb
    environment:
      MARIADB_ROOT_PASSWORD: root66
      MARIADB_DATABASE: testdb
      MARIADB_USER: student
      MARIADB_PASSWORD: student
    volumes:
      - mariadb_data:/var/lib/mysql
      - ./sql:/docker-entrypoint-initdb.d:ro
    healthcheck:
      test: ["CMD", "healthcheck.sh", "--connect", "--innodb_initialized"]
      interval: 5s
      timeout: 5s
      retries: 10

  message-service:
    build: ./message-service
    container_name: message-service
    depends_on:
      mariadb:
        condition: service_healthy
    environment:
      DB_HOST: mariadb
      DB_PORT: 3306
      DB_USER: student
      DB_PASSWORD: student
      DB_NAME: testdb
    ports:
      - "8080:8080"

volumes:
  mariadb_data:
```

* **Service: `mariadb`**
  - **Image:** The official MariaDB image with a fixed version tag.
  - **Environment variables:** Create the root password, the database
    **testdb** and the user **student** (password **student**) when the
    server initializes for the first time.
  - **No port mapping:** The database is only used by the Message Service,
    which reaches it over the Docker network. So port 3306 is **not**
    published on the host.
  - **Volumes:**
    - `mariadb_data`: A named volume which stores the database files, so
      the data survives when the container is removed.
    - `./sql`: The SQL scripts for the first start, mounted read only
      (`:ro`).
  - **Health check:** The image contains the script `healthcheck.sh`.
    With `--connect --innodb_initialized`, it succeeds only when the
    server accepts connections and the storage engine is ready. Docker
    runs this check every 5 seconds and marks the container as
    **healthy** or **unhealthy**.

* **Service: `message-service`**
  - **Build:** The image is built from `message-service/Dockerfile`.
  - **Dependency:** In the `compose-mariadb` example, `depends_on` only
    waited for the database container to **start**, so the client needed
    a `sleep 10`. Here we use `condition: service_healthy`: the Message
    Service is started only after the health check of `mariadb` succeeds.
  - **Environment variables:** The connection parameters for `MessageDao`.
    `DB_HOST: mariadb` is the **service name**, which the Docker DNS
    resolves to the IP address of the database container.
  - **Port mapping:** Publishes the service on port 8080 of the host.


## Run the Services

From the project directory, we build the image and start both services:

```bash
$ docker compose up -d --build

$ docker compose ps
NAME              IMAGE                                            SERVICE           STATUS                    PORTS
mariadb           mariadb:13.0.2                                   mariadb           Up 20 seconds (healthy)   3306/tcp
message-service   docker-compose-message-service-message-service   message-service   Up 5 seconds              0.0.0.0:8080->8080/tcp
```

The `docker compose up` command uses two options:

* `-d` (`--detach`): Runs the containers in the **background** and
    returns to the shell immediately. Without `-d`, `docker compose up`
    stays in the foreground and shows the log output of all services;
    `Ctrl+C` then stops the services. The logs of detached containers
    can be shown with `docker compose logs` (`-f` follows the output).

* `--build`: **Rebuilds the images** of all services with a `build:`
    entry (here `message-service`) before the containers are started.
    Services which use a ready-made image from a registry (`image:`, here
    `mariadb`) are not affected.
  - Without `--build`, Docker Compose builds an image only if it does not
    exist yet. Later starts reuse the existing image, even if we changed
    the Python files, `requirements.txt` or the `Dockerfile`. The
    container would still run the **old code**.
  - With `--build`, our changes end up in the image. The layer cache is
    still used: if only the `.py` files changed, the `pip install` layer
    is reused and the rebuild takes only a few seconds.

So `docker compose up -d --build` means: rebuild the service image with
the current code, then start all services in the background.

The `message-service` container starts a few seconds after `mariadb`,
because it waits for the health check.

The log output of the Flask service can be shown with:

```bash
$ docker compose logs message-service
message-service  |  * Serving Flask app 'message_service'
message-service  |  * Debug mode: on
message-service  |  * Running on all addresses (0.0.0.0)
message-service  |  * Running on http://127.0.0.1:8080
message-service  |  * Running on http://172.18.0.3:8080
```

If port 8080 is already used on the host, change the port mapping, for
example to `"8080:8080"`, and use that port in the `curl` commands.


## Access Message Service Using curl

### Find all Messages

```bash
$ curl -i http://localhost:8080/messages

HTTP/1.1 200 OK
Content-Type: application/json

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
$ curl -i http://localhost:8080/messages/1124

HTTP/1.1 200 OK
Content-Type: application/json

{
  "address": 1124,
  "data": "voltage=1.3V",
  "priority": 2
}
```

### Insert a Message

```bash
$ curl -i -X POST localhost:8080/messages \
    -H "Content-Type: application/json" \
    -d '{"address": 1127, "priority": 3, "data":"voltage=5.0V"}'

HTTP/1.1 201 CREATED
Content-Type: application/json

{
  "address": 1127,
  "data": "voltage=5.0V",
  "priority": 3
}
```

### Update a Message

```bash
$ curl -i -X PUT localhost:8080/messages/1124 \
    -H "Content-Type: application/json" \
    -d '{"address": 1124, "priority": 9, "data":"voltage=1.0V"}'

HTTP/1.1 200 OK
Content-Type: application/json

{
  "address": 1124,
  "data": "voltage=1.0V",
  "priority": 9
}
```

### Delete a Message

```bash
$ curl -i -X DELETE http://localhost:8080/messages/1124

HTTP/1.1 204 NO CONTENT
```


## Look into the Database

We can start the `mariadb` command line client inside the database
container to verify that the service really works with MariaDB:

```bash
$ docker exec -it mariadb mariadb -u student -pstudent testdb

MariaDB [testdb]> SELECT * FROM messages;
+---------+----------+---------------+
| address | priority | data          |
+---------+----------+---------------+
|    1123 |        2 | voltage=3.4V  |
|    1125 |        1 | voltage=-1.7V |
|    1127 |        3 | voltage=5.0V  |
+---------+----------+---------------+

MariaDB [testdb]> exit
```

For a single query, we don't need an interactive session. The `-e`
option executes the given SQL statement and exits, which is useful in
scripts:

```bash
$ docker exec mariadb mariadb -u student -pstudent testdb -e "SELECT * FROM messages"
address	priority	data
1123	2	voltage=3.4V
1125	1	voltage=-1.7V
1127	3	voltage=5.0V
```

Because there is no `-it`, no terminal is attached and the client prints
the result as tab separated values instead of a formatted table.


## Clean Up the Scene

```bash
$ docker compose down
```

This removes the containers and the network, but **not** the image and
the `mariadb_data` volume. When we start the services again, the changed
data is still there and `sql/setup.sql` is **not** executed again.

To start again with the initial test data, we also remove the volume:

```bash
$ docker compose down -v
```


## References
* [Docker Compose Quickstart](https://docs.docker.com/compose/gettingstarted/)
* [Docker Compose: Control startup order](https://docs.docker.com/compose/how-tos/startup-order/)
* [DockerHub: mariadb](https://hub.docker.com/_/mariadb)
* [PyMySQL Documentation](https://pymysql.readthedocs.io/)

*Egon Teiniker, 2025-2026, GPL v3.0*
