# Example: MariaDB + mysql Client 

In this example we connect a MariaDB database server to a `mysql` client so 
we can easily work with the database.

The `mysql` client represents various applications that require access to the database.

## Docker Compose File

Because we want to use the Flask image together with Redis, we use the following 
`docker-compose.yml` file, stored in a top-level directory called `compose-flask`:

This `docker-compose.yml` file defines two services: `mariadb` and `mysql-client`:

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

Now we can work with the `testdb` database.

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