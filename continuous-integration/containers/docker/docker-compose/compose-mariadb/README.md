# Example: MariaDB

In this example we run a MariaDB database server in a container and
access it with a `mysql` client which is installed **locally** on the
host.

The local `mysql` client represents various applications that require
access to the database. We use Docker Compose only to configure the
database container in a reproducible way, instead of typing a long
`docker run` command.

## Docker Compose File

This `docker-compose.yml` file defines a single service: `mariadb`.
It also creates a persistent volume so the database data isn't lost when
the container is removed.

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
    ports:
      - "3306:3306"
    volumes:
      - mariadb_data:/var/lib/mysql
    healthcheck:
      test: ["CMD", "healthcheck.sh", "--connect", "--innodb_initialized"]
      interval: 5s
      timeout: 5s
      retries: 10

volumes:
  mariadb_data:
```

* **Service: `mariadb`**
  - **Image:** Runs the official MariaDB image. We use a fixed version tag
    instead of `latest`, so every build uses the same server version.

  - **Container name:** The container is named `mariadb`, so we can refer
    to it by name in Docker commands.
  
  - **Environment variables:** Set up the database when the server
    initializes for the first time (with an empty volume):
    - Root password = **root66**
    - Creates a database named **testdb**
    - Creates a user named **student** with password **student**

    The image also accepts the older names `MYSQL_ROOT_PASSWORD`,
    `MYSQL_DATABASE`, ..., but `MARIADB_*` are the documented ones.
  
  - **Port mapping:** Publishes the database on port 3306 of the host.
    This is required, because our `mysql` client runs **on the host** and
    not inside the Docker network. If another database server already
    uses port 3306 on the host, `docker compose up` fails with
    "address already in use". In this case we use another host port, for
    example `"3307:3306"`, and connect with `-P 3307`.
  
  - **Volume:**
    - Stores database files outside the container.
    - Ensures data persists even if the container is removed.
  
  - **Health check:** The image contains the script `healthcheck.sh`.
    With `--connect --innodb_initialized`, it succeeds only when the
    server accepts connections and the storage engine is ready. Docker
    runs this check every 5 seconds and shows the container as
    **healthy** in `docker compose ps`.

* **Volume definition**
  - Declares the named volume used by MariaDB.
  - Data stored here survives container deletion.


## Run the Database Server

From the project directory, we start the database server in the
background:

```bash
$ docker compose up -d --wait
 Container mariadb Healthy

$ docker compose ps
NAME      IMAGE            COMMAND                  SERVICE   CREATED          STATUS                    PORTS
mariadb   mariadb:13.0.2   "docker-entrypoint.s…"   mariadb   12 seconds ago   Up 11 seconds (healthy)   0.0.0.0:3306->3306/tcp
```

* `-d` (`--detach`): Runs the container in the background and returns to
    the shell.

* `--wait`: Waits until the container is **healthy**, which means the
    health check succeeded. So when the command returns, the database is
    ready and the client can connect immediately.

On the first start (with an empty volume), the server initializes the
database: it starts a temporary server, creates `testdb` and the user
`student`, and restarts. Without the health check, a client which only
waits a fixed time (for example `sleep 10`) might connect during this
restart and fail with:

```
ERROR 2003 (HY000): Can't connect to MySQL server on ...
```


## Connect with the mysql Client

```bash
$ mysql -h 127.0.0.1 -P 3306 -u student -p testdb
Enter password: student
MariaDB [testdb]>
```

* `-h 127.0.0.1`: Connects to the host `127.0.0.1` via TCP. The port
    mapping forwards the connection to the container.
    Note that we **can't use `-h localhost`** here: for `localhost`, the
    client uses a local Unix socket instead of TCP, which doesn't exist
    on the host, because the server runs inside the container:

    ```
    ERROR 2002 (HY000): Can't connect to local server through socket '/run/mysqld/mysqld.sock' (2)
    ```
* `-P 3306`: Uses port 3306 (the host port of the port mapping). Since
    3306 is the default port, this option can be omitted.

* `-u student`: Logs in as user `student`.

* `-p`: Asks for the password. We could also write `-pstudent` (**no
    space** between `-p` and the password), but then the password appears
    in the shell history and the process list.

* `testdb`: Selects `testdb` as the default database (like `USE testdb;`).

Now we can work with the `testdb` database:

```sql
MariaDB [testdb]> CREATE TABLE user
(
	id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	firstname VARCHAR(255) NOT NULL,
	lastname VARCHAR(255) NOT NULL,
	username VARCHAR(255) NOT NULL,
	password VARCHAR(255) NOT NULL
) ENGINE = INNODB;

MariaDB [testdb]> INSERT INTO user (firstname, lastname, username, password) VALUES ('Homer', 'Simpson', 'homer', '2aaab795b3836904f82efc6ca2285d927aed75206214e1da383418eb90c9052f');
MariaDB [testdb]> INSERT INTO user (firstname, lastname, username, password) VALUES ('Bart', 'Simpson', 'bart', '9551dadbf76a27457946e70d1aebebe2132f8d3bce6378d216c11853524dd3a6');
MariaDB [testdb]> INSERT INTO user (firstname, lastname, username, password) VALUES ('Lisa', 'Simpson', 'lisa', 'd84fe7e07bedb227cffff10009151d96fc944f6a1bd37cff60e8e4626a1eb1c3');

MariaDB [testdb]> SELECT * FROM user;
+----+-----------+----------+----------+------------------------------------------------------------------+
| id | firstname | lastname | username | password                                                         |
+----+-----------+----------+----------+------------------------------------------------------------------+
|  1 | Homer     | Simpson  | homer    | 2aaab795b3836904f82efc6ca2285d927aed75206214e1da383418eb90c9052f |
|  2 | Bart      | Simpson  | bart     | 9551dadbf76a27457946e70d1aebebe2132f8d3bce6378d216c11853524dd3a6 |
|  3 | Lisa      | Simpson  | lisa     | d84fe7e07bedb227cffff10009151d96fc944f6a1bd37cff60e8e4626a1eb1c3 |
+----+-----------+----------+----------+------------------------------------------------------------------+
3 rows in set (0.00 sec)

MariaDB [testdb]> exit
```

The `password` column stores hash values instead of plain text passwords.

For a single query, we don't need an interactive session. The `-e`
option executes the given SQL statement and exits:

```bash
$ mysql -h 127.0.0.1 -u student -pstudent testdb -e "SELECT id, username FROM user"
+----+----------+
| id | username |
+----+----------+
|  1 | homer    |
|  2 | bart     |
|  3 | lisa     |
+----+----------+
```


## Persistent Data

Even when we **remove the container and recreate it** using Docker
Compose, the database data is still there, because it is stored in the
`mariadb_data` volume:

```bash
$ docker compose down

$ docker compose up -d --wait

$ mysql -h 127.0.0.1 -u student -p testdb
MariaDB [testdb]> SELECT * FROM user;
```


## Clean Up the Scene

To clean up the container created by Docker Compose, run the following
command:

```bash
$ docker compose down

$ docker container ls -a
CONTAINER ID   IMAGE            COMMAND   CREATED      STATUS                  PORTS     NAMES
```

The `docker compose down` command stops the services defined in
`docker-compose.yml` and removes their containers and the network. It
**does not remove the image and the volume**:

```bash
$ docker image ls
REPOSITORY   TAG       IMAGE ID       CREATED       SIZE
mariadb      13.0.2    ...

$ docker volume ls
DRIVER    VOLUME NAME
local     compose-mariadb_mariadb_data
```

Docker Compose prefixes the volume name with the project name, which is
the name of the project directory by default.

To also remove the volume (and therefore **all database data**), we use
the `-v` option:

```bash
$ docker compose down -v
```


## References
* [Docker Compose Quickstart](https://docs.docker.com/compose/gettingstarted/)
* [Docker Compose: Control startup order](https://docs.docker.com/compose/how-tos/startup-order/)
* [DockerHub: mariadb](https://hub.docker.com/_/mariadb)

*Egon Teiniker, 2025-2026, GPL v3.0*
