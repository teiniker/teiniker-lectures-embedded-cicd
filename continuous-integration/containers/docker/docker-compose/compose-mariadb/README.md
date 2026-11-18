# Example: MariaDB + mysql Client 

In this example we connect a MariaDB database server to a `mysql` client so 
we can easily work with the database.

The `mysql` client represents various applications that require access to
the database.

## Docker Compose File

This `docker-compose.yml` file defines two services: `mariadb` and
`mysql-client`. It also creates a persistent volume so the database data
isn’t lost when the container is removed.

```yaml
services:
  mariadb:
    image: mariadb:13.0.2
    container_name: mariadb
    environment:
      MYSQL_ROOT_PASSWORD: root66
      MYSQL_DATABASE: testdb
      MYSQL_USER: student
      MYSQL_PASSWORD: student
    ports:
      - "3306:3306"
    volumes:
      - mariadb_data:/var/lib/mysql

  mysql-client:
    image: mysql:26.10
    container_name: mysql-client
    depends_on:
      - mariadb
    command: sh -c "sleep 10 && mysql -h mariadb -u student -pstudent testdb"
    # keeps the container running so you can exec into it if you want
    tty: true
    stdin_open: true

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
  - **Port mapping:** Exposes the database on port 3306 of the host.
  - **Volume:** 
    - Stores database files outside the container.
    - Ensures data persists even if the container is removed.

* **Service: `mysql-client`**
  - **Image:** Uses the official MySQL image, which contains the `mysql`
    command line client.
  - **Dependency:** `depends_on` ensures the database container starts
    before the client does. Note that it only waits for the container to
    start, not for the database server to be ready.
  - **Startup command:** 
    - Waits 10 seconds to allow MariaDB to fully start.
    - Then runs a MySQL client command:
      - `-h mariadb`: connects by service name over the Docker network
      - `-u student -pstudent`: logs in with the created user
      - `testdb`: opens the database
  - **Keeps the container open:**  
    - `tty` and `stdin_open` prevent the client from exiting right after
      it starts.
    - This allows us to exec into the container and manually run commands:
    
      ```bash
      $ docker exec -it mysql-client mysql -h mariadb -u student -pstudent testdb
      ```

* **Volume definition**
  - Declares the named volume used by MariaDB.
  - Data stored here survives container deletion.


## Run the Services

From the project directory, start the services and open an interactive
MySQL session:

```bash
# Start services in the background
$ docker compose up -d

# Verify services are running
$ docker compose ps

# Open an interactive MySQL shell in the mysql-client container
$ docker exec -it mysql-client mysql -h mariadb -u student -pstudent testdb
mysql> 
```

The `docker exec` command **starts an additional process inside an already
running container**. Here it starts the `mysql` client program inside the
`mysql-client` container:

* `docker exec`: Runs a command in a running container (the container is
  not created or restarted).

* `-it`: Combination of two options which we need for an interactive shell:
  - `-i` (interactive): Keeps `stdin` open, so we can type SQL commands.
  - `-t` (tty): Allocates a pseudo terminal, so we get a prompt, line
    editing and formatted output.

* `mysql-client`: The name of the container in which the command runs
  (set by `container_name` in `docker-compose.yml`).

* `mysql -h mariadb -u student -pstudent testdb`: The command executed in
  the container, which starts the MySQL command line client:
  - `-h mariadb`: Connects to the host `mariadb`. Docker Compose puts both
    services into a **common network** and its DNS resolves the service
    name `mariadb` to the IP address of the database container. Therefore,
    we don't need the port mapping `3306:3306` for this connection; it is
    only needed for clients running on the host.
  - `-u student`: Logs in as user `student`.
  - `-pstudent`: Uses the password `student`. Note that there is **no space**
    between `-p` and the password. With `-p` alone, the client asks for the
    password, which is safer because the password does not appear in the
    shell history or the process list.
  - `testdb`: Selects `testdb` as the default database (like `USE testdb;`).

Why do we need `docker exec` at all, when the container already starts a
`mysql` client in its `command`? That client runs as the container's main
process and is not connected to our terminal. With `docker exec` we start
a second client which is connected to our terminal.

Instead of the container name, we can also use the **service name** with
the `docker compose exec` command (the `-it` options are the default here):

```bash
$ docker compose exec mysql-client mysql -h mariadb -u student -pstudent testdb
```


Now we can work with the `testdb` database:

```sql
mysql> CREATE TABLE user
(
	id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	firstname VARCHAR(255) NOT NULL,
	lastname VARCHAR(255) NOT NULL,
	username VARCHAR(255) NOT NULL,
	password VARCHAR(255) NOT NULL
) ENGINE = INNODB;

mysql> INSERT INTO user (firstname, lastname, username, password) VALUES ('Homer', 'Simpson', 'homer', '2aaab795b3836904f82efc6ca2285d927aed75206214e1da383418eb90c9052f');
mysql> INSERT INTO user (firstname, lastname, username, password) VALUES ('Bart', 'Simpson', 'bart', '9551dadbf76a27457946e70d1aebebe2132f8d3bce6378d216c11853524dd3a6');
mysql> INSERT INTO user (firstname, lastname, username, password) VALUES ('Lisa', 'Simpson', 'lisa', 'd84fe7e07bedb227cffff10009151d96fc944f6a1bd37cff60e8e4626a1eb1c3');

mysql> SELECT * FROM user;
+----+-----------+----------+----------+------------------------------------------------------------------+
| id | firstname | lastname | username | password                                                         |
+----+-----------+----------+----------+------------------------------------------------------------------+
|  1 | Homer     | Simpson  | homer    | 2aaab795b3836904f82efc6ca2285d927aed75206214e1da383418eb90c9052f |
|  2 | Bart      | Simpson  | bart     | 9551dadbf76a27457946e70d1aebebe2132f8d3bce6378d216c11853524dd3a6 |
|  3 | Lisa      | Simpson  | lisa     | d84fe7e07bedb227cffff10009151d96fc944f6a1bd37cff60e8e4626a1eb1c3 |
+----+-----------+----------+----------+------------------------------------------------------------------+
3 rows in set (0.00 sec)

mysql> exit
```

The `password` column stores hash values instead of plain text passwords.

If we use the `mysql` client again, we can still access the data:

```bash
$ docker exec -it mysql-client mysql -h mariadb -u student -pstudent testdb

mysql> SELECT * FROM user;
```

Even when we **remove all containers and recreate them** using Docker
Compose, the database data is still there, because it is stored in the
`mariadb_data` volume:

```bash
$ docker compose down

$ docker compose up -d

$ docker exec -it mysql-client mysql -h mariadb -u student -pstudent testdb
mysql> SELECT * FROM user;
```

If the client reports that it can't connect, the database server is still
starting. Wait a few seconds and try again.


## Clean Up the Scene

To clean up the containers created by Docker Compose, run the following
command:

```bash
$ docker compose down

$ docker container ls -a
CONTAINER ID   IMAGE            COMMAND   CREATED      STATUS                  PORTS     NAMES
```

The `docker compose down` command stops the services defined in
`docker-compose.yml` and removes their containers and the network. It
**does not remove the images and the volume**:

```bash
$ docker image ls
REPOSITORY   TAG       IMAGE ID       CREATED       SIZE
mariadb      13.0.2    ...
mysql        26.10     ...

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
* [DockerHub: mariadb](https://hub.docker.com/_/mariadb)
* [DockerHub: mysql](https://hub.docker.com/_/mysql)

*Egon Teiniker, 2025-2026, GPL v3.0*
