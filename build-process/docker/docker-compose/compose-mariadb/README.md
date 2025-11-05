# Example: MariaDB + mysql Client 

In this example we connect a MariaDB database server to a `mysql` client so 
we can easily work with the database.

The `mysql` client represents various applications that require access to the database.

## Docker Compose File

This `docker-compose.yml` file defines two services: `mariadb` and `mysql-client`.
It also creates a persistent volume so the database data isn’t lost when the container stops.

```yaml
services:
  mariadb:
    image: mariadb:latest
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
    image: mysql:latest
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
  - Runs the official MariaDB image
  - The container is named `mariadb` so other services can reference it by name

*  Environment variables set up the database when the server initializes:
  - Root password = **root66**
  - Creates a database named **testdb**
  - Creates a user named **student** with password **student**

* Port mapping: Exposes the database on port 3306 

* Volume: 
  - Stores database files outside the container
  - Ensures data persists even if the container is removed

* **Service: `mysql-client`**
  - Uses the official MySQL client tools
  - Helpful for testing or manually running SQL commands

* Container dependency: Ensures the database container starts before the client does

* Startup command: 
  - Waits 10 seconds to allow MariaDB to fully start
  - Then runs a MySQL client command:
    - `-h mariadb`: connects by container name over Docker network
    - `-u student -pstudent`: logs in with the created user
    - `testdb`: opens the database

* Keeps container open:  
  - Prevents container from exiting after running the command
  - Allows you to exec into the container and manually run commands:
    
    ```bash
    docker exec -it mysql-client mysql -h mariadb -u student -pstudent testdb
    ```

* **Volume definition**
  - Declares the named volume used by MariaDB
  - Data stored here survives container deletion


## Run the Services

From the project directory, start the services and open an interactive MySQL session:

```bash
# Start services in the background
$ docker compose up -d

# Verify services are running
$ docker compose ps

# Open an interactive MySQL shell in the mysql-client container
$ docker exec -it mysql-client mysql -h mariadb -u student -pstudent testdb
mysql> 
```

Now we can work with the `testdb` database:

```SQL
CREATE TABLE user
(
	id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	firstname VARCHAR(255) NOT NULL,
	lastname VARCHAR(255) NOT NULL,
	username VARCHAR(255) NOT NULL,
	password VARCHAR(255) NOT NULL
) ENGINE = INNODB;

INSERT INTO user (firstname,lastname,username, password) VALUES ('Homer', 'Simpson', 'homer', '2aaab795b3836904f82efc6ca2285d927aed75206214e1da383418eb90c9052f');
INSERT INTO user (firstname,lastname,username, password) VALUES ('Bart', 'Simpson','bart', '9551dadbf76a27457946e70d1aebebe2132f8d3bce6378d216c11853524dd3a6');
INSERT INTO user (firstname,lastname,username, password) VALUES ('Lisa', 'Simpson','lisa', 'd84fe7e07bedb227cffff10009151d96fc944f6a1bd37cff60e8e4626a1eb1c3');

mysql> select * from user;
+----+-----------+----------+----------+------------------------------------------------------------------+
| id | firstname | lastname | username | password                                                         |
+----+-----------+----------+----------+------------------------------------------------------------------+
|  1 | Homer     | Simpson  | homer    | 2aaab795b3836904f82efc6ca2285d927aed75206214e1da383418eb90c9052f |
|  2 | Bart      | Simpson  | bart     | 9551dadbf76a27457946e70d1aebebe2132f8d3bce6378d216c11853524dd3a6 |
|  3 | Lisa      | Simpson  | lisa     | d84fe7e07bedb227cffff10009151d96fc944f6a1bd37cff60e8e4626a1eb1c3 |
+----+-----------+----------+----------+------------------------------------------------------------------+
3 rows in set (0.001 sec)

mysql> exit
```

If we use the `mysql` client again, we can access the database data.

```bash
$ docker exec -it mysql-client mysql -h mariadb -u student -pstudent testdb

mysql> select * from user;
```

Even when we **remove all containers, and recreate them** using compose, 
the database data is still there:

```bash
$ docker compose down

$ docker compose up -d

$ docker exec -it mysql-client mysql -h mariadb -u student -pstudent testdb
mysql> select * from user;
```

## Clean Up the Scene

To clean up the containers created by docker compose, run the following command:

```bash
$ docker compose down

$ docker container ls -a
CONTAINER ID   IMAGE            COMMAND   CREATED      STATUS                  PORTS     NAMES

$ docker image ls -a
REPOSITORY          TAG       IMAGE ID       CREATED         SIZE
```

The `docker compose down` command stops the services defined in `docker-compose.yml` and 
removes their containers, but it does not remove the Docker images.



## References
* [Docker Compose Quickstart](https://docs.docker.com/compose/gettingstarted/)
* [DockerHub: mariadb](https://hub.docker.com/_/mariadb)

*Egon Teiniker, 2025, GPL v3.0*