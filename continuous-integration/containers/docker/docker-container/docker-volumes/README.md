# Docker Volumes 

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
$ docker volume create mysql-db

# List volumes
$ docker volume ls
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
$ docker run -d --name mysql -e MYSQL_ROOT_PASSWORD=root66 -p 3306:3306 mysql

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

We can use a local `mysql` client to access the database server in the docker container:

```bash
$ mysql -h 127.0.0.1 -P 3306 -u root -proot66
Welcome to the MariaDB monitor.  Commands end with ; or \g.
Your MySQL connection id is 9
Server version: 26.10.0 MySQL Community Server - GPL

MySQL [(none)]> SHOW DATABASES;
MySQL [(none)]> quit
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
$ docker run -d --name mysql -e MYSQL_ROOT_PASSWORD=root66 -v mysql-db:/var/lib/mysql -p 3306:3306 mysql

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

Again, we use the local `mysql` client to access the database server in the docker container:

```bash
$ mysql -h 127.0.0.1 -P 3306 -u root -proot66

-- Create the database and switch to it
CREATE DATABASE testdb;
USE testdb;

-- Create the table
CREATE TABLE users (
    id       INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50)  NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL
);

-- Insert test users
INSERT INTO users (username, password) VALUES
    ('alice', 'alice123'),
    ('bob',   'bob123'),
    ('carol', 'carol123');

-- Check the result
SELECT * FROM users;
+----+----------+----------+
| id | username | password |
+----+----------+----------+
|  1 | alice    | alice123 |
|  2 | bob      | bob123   |
|  3 | carol    | carol123 |
+----+----------+----------+

MySQL [testdb]> quit
```

After inserting data in our database, let's stop and remove the container - then start it again:

```bash
$ docker container ls -a
$ docker container stop mysql
$ docker container rm mysql
```

Our docker volume is still there:

```bash
$ docker volume ls
DRIVER    VOLUME NAME
local     mysql-db
```

Let's restart the container with the same volume:
```bash
$ docker run -d --name mysql -e MYSQL_ROOT_PASSWORD=root66 -v mysql-db:/var/lib/mysql -p 3306:3306 mysql

$ mysql -h 127.0.0.1 -P 3306 -u root -proot66
MySQL [(none)]> USE testdb;

MySQL [testdb]> select * from users;
+----+----------+----------+
| id | username | password |
+----+----------+----------+
|  1 | alice    | alice123 |
|  2 | bob      | bob123   |
|  3 | carol    | carol123 |
+----+----------+----------+
```

We can use our database table as before.


## References

* [Volumes](https://docs.docker.com/engine/storage/volumes/)

* [DockerHub: mysql](https://hub.docker.com/_/mysql)


*Egon Teiniker, 2025-2026, GPL v3.0*
