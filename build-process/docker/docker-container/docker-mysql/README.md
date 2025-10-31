# Example: MySQL Database Server

MySQL is a widely used, open-source relational database management system (RDBMS).

```bash
$ docker pull  mysql:9.5.0

$ docker image ls -a
REPOSITORY      TAG       IMAGE ID       CREATED          SIZE
mysql           9.5.0     f6b0ca07d79d   9 days ago       934MB

$ docker image inspect mysql:9.5.0

"Volumes": {
                "/var/lib/mysql": {}
            },
```

## Docker Volumes 

Docker volumes are a built-in mechanism for 
**persisting and sharing data used by Docker containers**. 

Normally, anything written inside a container’s filesystem disappears when 
the container stops or is removed. 

Volumes solve that problem by storing data outside the container’s 
internal filesystem, they:
* Persist data beyond container lifecycles (database files, uploads, logs).
* Share data between multiple containers.
* Isolate storage from container images (smaller images, easier upgrades).
* Better performance than bind mounts on many systems.

Docker manages them under `/var/lib/docker/volumes/` on Linux 
(or equivalent directories on Mac/Windows).

We can **create a volume explicitly** using the `docker volume create` 
command, or Docker can create a volume during container or service creation:

```bash
# Create a named volume
$ docker volume create mydata

# List volumes
$  docker volume ls

# Pass a named volume to a starting container
$ docker run -d -v mydata:/var/lib/mysql mysql
```

If we start a container and specify a volume which does not already exist, 
an empty volume is created for us. 

When we mount the volume into a container, this directory is what's mounted 
into the container. This is **similar to the way that bind mounts work**, 
except that volumes are managed by Docker and are **isolated from the core 
functionality of the host machine**.

While bind mounts are dependent on the directory structure and OS of the 
host machine, volumes are completely managed by Docker. 

## Anonymous Volumes

Anonymous volumes persist even if we remove the container that uses them, 
except if we use the `--rm` flag when creating the container, in which case 
the anonymous volume associated with the container is destroyed.

If we create multiple containers consecutively that each use anonymous volumes, 
each container creates its own volume. Anonymous volumes aren't reused or shared 
between containers automatically. To share an anonymous volume between two or 
more containers, we must mount the anonymous volume using the random volume ID.

We create a container named `mysql`:

```bash
$ docker run -d --name mysql -e MYSQL_ROOT_PASSWORD=root66 mysql:9.5.0

$ docker container inspect mysql
	"Mounts": [
		{
			"Type": "volume",
			"Name": "55eda1e315665d3b5d832424951e1b69dba665cf0225e1d7ceebd97f4494618d",
			"Source": "/var/lib/docker/volumes/55eda1e315665d3b5d832424951e1b69dba665cf0225e1d7ceebd97f4494618d/_data",
			"Destination": "/var/lib/mysql",
			"Driver": "local",
			"Mode": "",
			"RW": true,
			"Propagation": ""
		}
	],
```

**Starting the container creates a volume** in the `/var/lib/docker/volumes`folder:

```bash
$ docker volume ls 
DRIVER    VOLUME NAME
local     55eda1e315665d3b5d832424951e1b69dba665cf0225e1d7ceebd97f4494618d

$ docker volume inspect  55eda1e315665d3b5d832424951e1b69dba665cf0225e1d7ceebd97f4494618d
[
    {
        "CreatedAt": "2025-10-30T19:35:59+01:00",
        "Driver": "local",
        "Labels": {
            "com.docker.volume.anonymous": ""
        },
        "Mountpoint": "/var/lib/docker/volumes/55eda1e315665d3b5d832424951e1b69dba665cf0225e1d7ceebd97f4494618d/_data",
        "Name": "55eda1e315665d3b5d832424951e1b69dba665cf0225e1d7ceebd97f4494618d",
        "Options": null,
        "Scope": "local"
    }
]
```

Starting a new MySQL container creates an anonymous volume automatically. 
**Stopping and removing the container does not remove that volume**, volumes persist to keep data. 

```bash
$ docker container stop mysql
$ docker container rm mysql
$ docker container ls -a
CONTAINER ID   IMAGE     COMMAND   CREATED   STATUS    PORTS     NAMES

$ docker volume ls 
DRIVER    VOLUME NAME
local     55eda1e315665d3b5d832424951e1b69dba665cf0225e1d7ceebd97f4494618d
```

Docker does not delete unused volumes automatically, we have to remove them manually 
when they are no longer needed:

```bash
$ docker volume rm 55eda1e315665d3b5d832424951e1b69dba665cf0225e1d7ceebd97f4494618d
```


## Named Volumes 

When creating a container we can assign a name to the volume with the `-v` flag, 
for example `-v mysql-db:/var/lib/mysql`, which creates (or reuses) a persistent 
named volume stored outside the container. 

```bash
$ docker run -d --name mysql -e MYSQL_ROOT_PASSWORD=root66 -v mysql-db:/var/lib/mysql mysql:9.5.0

$ docker volume ls
DRIVER    VOLUME NAME
local     mysql-db

$ docker container inspect mysql

	"Mounts": [
		{
			"Type": "volume",
			"Name": "mysql-db",
			"Source": "/var/lib/docker/volumes/mysql-db/_data",
			"Destination": "/var/lib/mysql",
			"Driver": "local",
			"Mode": "z",
			"RW": true,
			"Propagation": ""
		}
	],
```

Now we have a persistent named volume with a human-readable name.


## Connecting to MySQL from inside the container

To use the container's MySQL client, run the client inside the running 
container with `docker exec`:

```bash
$ docker exec -it mysql mysql -u root

mysql> create database testdb;
mysql> create user 'student'@'localhost' identified by 'student';
mysql> ALTER USER 'student'@'localhost' IDENTIFIED BY 'student';
mysql> grant alter, create, delete, drop, index, insert, select, update on testdb.* to 'student'@'localhost';
mysql> exit 
```

In SQL, we created a database named `testdb` and a user `student`.
We can now connect to the MySQL server using the `student` account:

```bash
$ docker exec -it mysql mysql -ustudent -pstudent

mysql> use testdb;
mysql> show tables;

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

mysql> select* from user;
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


## Connecting to the Database from Outside the Container

To apply new settings to the container, we have to remove the existing container 
and its associated volume:

```bash
$ docker rm -f mysql

$ docker volume rm mysql-db
```

To allow connections to MySQL from the host or other machines, we start the 
container with port publishing and set the root host:

* `-p 3306:3306` Publish the container’s MySQL port on the host so clients can reach port 3306.
* `-e MYSQL_ROOT_HOST=%` Permit root connections from any client IP (not limited to `localhost`).

Note: using `%` allows connections from any IP and is insecure for production; prefer a specific host or proper network security.

```bash
$ docker run -d --name mysql -p 3306:3306 -e MYSQL_ROOT_HOST=% -e MYSQL_ROOT_PASSWORD=root66 -v mysql-db:/var/lib/mysql mysql:9.5.0
```

Now we can use a local `mysql`client to access the database server:

```bash
$ mysql -h 127.0.0.1 -P 3306 -u root -p

mysql> create database testdb;
mysql> use testdb;
mysql> show tables;
...
mysql> exit
```


## References

* [Volumes](https://docs.docker.com/engine/storage/volumes/)

* [DockerHub: mysql](https://hub.docker.com/_/mysql)


*Egon Teiniker, 2025, GPL v3.0*
