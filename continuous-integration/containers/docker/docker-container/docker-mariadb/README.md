# Example: MariaDB Database Server

MariaDB is an **open-source relational database management system (RDBMS)** used 
to store and manage structured data. It’s designed to be fast, scalable, and 
reliable—suitable for everything from small projects to large enterprise systems.

MariaDB was created in 2009 after Oracle acquired Sun Microsystems (owner of MySQL). 
The MySQL founders were worried Oracle might make MySQL closed-source, so they 
forked the codebase and created MariaDB to ensure an open, community-driven alternative.


## Create and Run a MariaDB Container 


```bash
# Pull the latest MariaDB image from Docker Hub
$ docker pull mariadb:latest                 

# List all local Docker images (including intermediate images)
$ docker image ls -a                          
REPOSITORY      TAG       IMAGE ID       CREATED          SIZE
mariadb         latest    dfbea441e6fc   2 months ago     330MB    

# Show detailed JSON metadata for the mariadb:latest image
$ docker image inspect mariadb:latest        

"Volumes": {
                "/var/lib/mysql": {}
            },
```

We **create a container** named `mariadb` and set some environmental variables as well as the name of its volume:

```bash
# Run MariaDB in the background, name the container, map port, mount the volume, and set environment variables
$ docker run -d --name mariadb -p 3306:3306 -v mariadb-db:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=root66 -e MYSQL_DATABASE=testdb -e MYSQL_USER=student -e MYSQL_PASSWORD=student mariadb:latest

# Inspect the container's JSON metadata (verify mounts, env, ports, etc.)
$ docker container inspect mariadb

# List Docker volumes to confirm the mariadb-db volume exists
$ docker volume ls
DRIVER    VOLUME NAME
local     mariadb-db
```


## Connecting to the Database from Outside the Container

Using the `mysql` client, installed in the Debian VM, we access the running MariaDB container:

```bash
# Connect to MariaDB on localhost:3306 as root; prompts for root password ('root66')
$ mysql -h 127.0.0.1 -P 3306 -u root -p  

# Connect as user 'student'; prompts for password ('student')
$ mysql -h 127.0.0.1 -P 3306 -u student -p testdb

# Inside the MySQL client: switch to the 'testdb' database
MariaDB [(none)]> use testdb;  

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

MariaDB [testdb]> select * from user;

MariaDB [testdb]> exit
```

## Clean Up the Scene

Stop and remove the container, then optionally remove the data volume and image:

```bash
# Stop and remove the container
docker container stop mariadb
docker container rm mariadb

# List volumes
docker volume ls

# Optionally, remove the data volume (this deletes all DB data)
docker volume rm mariadb-db

# Optionally, remove the image
docker image rm mariadb:latest
```

## References

* [Volumes](https://docs.docker.com/engine/storage/volumes/)

* [DockerHub: mariadb](https://hub.docker.com/_/mariadb)


*Egon Teiniker, 2025, GPL v3.0*
