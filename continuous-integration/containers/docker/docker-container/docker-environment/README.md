# Docker Environment Variables

Environment variables are the common way to **configure a containerized
application from outside**, without changing the image.

The idea is that we build an image **once** and run it in different
environments (development, test, production). Only the configuration
changes: database host, user names, log level, feature flags. If these
values were hard coded or copied into the image, we would need a
separate image for every environment.

Every process in Linux has a set of environment variables (`KEY=value`
pairs) which it inherits from its parent process. For a container, Docker
sets this environment when it starts the main process. The values come
from three places:
* the **image** (`ENV` instructions in the Dockerfile and its base images),
* the **`docker run` command** (`-e` and `--env-file`),
* the **Docker Compose file** (`environment` and `env_file`).

The application reads them like any other environment variable, for
example with `os.environ` in Python or `getenv()` in C/C++.

## Project Structure

```
docker-environment/
├── app.env
├── app.py
└── Dockerfile
```

The small Python program `app.py` prints a greeting. Its configuration
is read from environment variables, with a default value for each one:

```python
import os

# Read the configuration from environment variables (with default values)
greeting = os.environ.get('GREETING', 'Hello')
name = os.environ.get('NAME', 'World')
count = int(os.environ.get('COUNT', '1'))

for _ in range(count):
    print(f'{greeting} {name}!')
```

Without any environment variables, the program prints `Hello World!`.


## Environment Variables of an Image

Every image already defines some environment variables. We can list them
with `docker image inspect`:

```bash
$ docker image inspect python:3-alpine --format '{{json .Config.Env}}'
["PATH=/usr/local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin","LANG=C.UTF-8","PYTHON_VERSION=3.15.2","PYTHON_SHA256=8b55..."]
```

The `env` command, executed in a container, shows the complete
environment of the container's process:

```bash
$ docker run --rm alpine env
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
HOSTNAME=18d508af5aef
HOME=/root
```

`PATH` comes from the image. `HOSTNAME` (the container ID) and `HOME` are
set by Docker when the container starts.


## Default Values in the Dockerfile: `ENV`

With the `ENV` instruction, we set default values in the image:

```dockerfile
FROM python:3-alpine

# Default values for the configuration of app.py
ENV GREETING="Hello" \
    NAME="Docker"

WORKDIR /app
COPY app.py .

CMD ["python", "app.py"]
```

We build the image and run it without further configuration:

```bash
$ docker build -t env-demo .

$ docker run --rm env-demo
Hello Docker!
```

`NAME` is set to `Docker` by the image, so the default value `World` in
`app.py` isn't used. `COUNT` is not defined in the image, so the program
uses its default value `1`.

The `ENV` values are stored in the image and are also available in all
following Dockerfile instructions (`RUN`, `WORKDIR`, ...).


## Setting Variables with `docker run -e`

The `-e` (`--env`) flag sets an environment variable for the container.
It **overrides** a value from the image:

```bash
$ docker run --rm -e NAME=Graz env-demo
Hello Graz!
```

We can use `-e` several times:

```bash
$ docker run --rm -e NAME=Graz -e GREETING=Servus -e COUNT=3 env-demo
Servus Graz!
Servus Graz!
Servus Graz!
```

The same image, started with different environment variables, behaves
differently. This is exactly what we need for different environments.

### Passing a Variable from the Host

If we only write the name of the variable (`-e NAME`, without `=`),
Docker takes the value from the **shell on the host**:

```bash
$ export NAME=Student
$ docker run --rm -e NAME env-demo
Hello Student!
```

This avoids writing values (for example passwords) into the command
line, so they don't end up in the shell history.

**Pitfall:** If the variable is **not set** on the host, Docker removes
the variable from the container's environment, including the default
value from the image:

```bash
$ unset NAME
$ docker run --rm -e NAME env-demo
Hello World!
```

The program now prints its own default `World`, not `Docker` from the
`ENV` instruction. In contrast, `-e NAME=` sets the variable to an
**empty string**:

```bash
$ docker run --rm -e NAME= env-demo
Hello !
```


## Environment Files: `--env-file`

With many variables, the command line becomes long. Instead, we write
the variables into a file, one `KEY=value` pair per line:

```bash
$ cat app.env
# Configuration for app.py
GREETING=Servus
NAME=Graz
COUNT=2
```

```bash
$ docker run --rm --env-file app.env env-demo
Servus Graz!
Servus Graz!
```

Lines starting with `#` are comments and empty lines are ignored.

If a variable is set in the file **and** with `-e`, the `-e` value wins:

```bash
$ docker run --rm --env-file app.env -e NAME=FH env-demo
Servus FH!
Servus FH!
```

**Pitfall:** `docker run --env-file` doesn't interpret the content like
a shell. **Quotes are part of the value**, and spaces are kept:

```
A="quoted"
B=with space
```

```bash
$ docker run --rm --env-file q.env alpine sh -c 'echo "A=[$A] B=[$B]"'
A=["quoted"] B=[with space]
```

So we write values in an env file **without quotes**. (Docker Compose
behaves differently and removes the quotes, see below.)


## Looking into a Running Container

The environment of a container is part of its configuration and can be
shown with `docker container inspect`:

```bash
$ docker run -d --name env-demo -e COUNT=2 env-demo sh -c 'sleep 60'

$ docker container inspect env-demo --format '{{json .Config.Env}}'
["COUNT=2","PATH=/usr/local/bin:...","LANG=C.UTF-8","PYTHON_VERSION=3.15.2","PYTHON_SHA256=8b55...","GREETING=Hello","NAME=Docker"]
```

The list contains our `-e` variable and all variables of the image.
With `docker exec`, we can read single values inside the container:

```bash
$ docker exec env-demo printenv NAME COUNT
Docker
2

$ docker rm -f env-demo
```

The environment is set when the container is **created**. To change a
value, we have to remove the container and create a new one.


## Pitfall: Variables in the Command

Variables in a `docker run` command are expanded by the **shell on the
host**, before Docker even sees the command:

```bash
$ docker run --rm alpine echo $HOME
/home/student
```

The output is the home directory of the host user, not of the container.
To use the variable of the container, a shell **inside the container**
has to expand it. So we use single quotes, which prevent the expansion
on the host:

```bash
$ docker run --rm alpine sh -c 'echo $HOME'
/root
```


## Build Arguments: `ARG` vs. `ENV`

Besides `ENV`, a Dockerfile can declare **build arguments** with `ARG`.
They look similar, but are used at a different time:

| | `ARG` | `ENV` |
|---|---|---|
| Set with | `docker build --build-arg` | `ENV` in Dockerfile, `docker run -e` |
| Available during `docker build` | yes | yes |
| Available in the running container | **no** | yes |

The following Dockerfile shows the difference. To make a build argument
available at run time, we copy it into an `ENV` variable:

```dockerfile
FROM alpine
ARG VERSION=1.0
ENV APP_VERSION=$VERSION
RUN echo "build time: VERSION=$VERSION APP_VERSION=$APP_VERSION"
CMD sh -c 'echo "run time: VERSION=[$VERSION] APP_VERSION=[$APP_VERSION]"'
```

```bash
$ docker build --progress=plain --build-arg VERSION=2.1 -t arg-demo .
...
#5 0.272 build time: VERSION=2.1 APP_VERSION=2.1
...

$ docker run --rm arg-demo
run time: VERSION=[] APP_VERSION=[2.1]
```

Build arguments are visible in the image history, so they must not be
used for passwords either:

```bash
$ docker history arg-demo --no-trunc --format '{{.CreatedBy}}' | grep VERSION
RUN |1 VERSION=2.1 /bin/sh -c echo "build time: ..."
```


## Example: Configuring a MariaDB Container

Many official images are configured completely with environment
variables. The MariaDB image, for example, **requires** a root password
setting:

```bash
$ docker run --rm mariadb:13.0.2
[ERROR] [Entrypoint]: Database is uninitialized and password option is not specified
	You need to specify one of MARIADB_ROOT_PASSWORD, MARIADB_ROOT_PASSWORD_HASH, MARIADB_ALLOW_EMPTY_ROOT_PASSWORD and MARIADB_RANDOM_ROOT_PASSWORD
```

```bash
$ docker run -d --name db -e MARIADB_ROOT_PASSWORD=root66 -v db-data:/var/lib/mysql mariadb:13.0.2
```

**Pitfall:** The entrypoint script of the image uses these variables
**only to initialize** an empty database. If we later create a new
container with the same volume and another password, the variable is
ignored, because the database already exists:

```bash
$ docker rm -f db
$ docker run -d --name db -e MARIADB_ROOT_PASSWORD=newpw -v db-data:/var/lib/mysql mariadb:13.0.2

$ docker exec db mariadb -u root -pnewpw -e "SELECT 1"
ERROR 1045 (28000): Access denied for user 'root'@'localhost' (using password: YES)

$ docker exec db mariadb -u root -proot66 -e "SELECT 1"
1
1
```

(The first `1` is the column header, the second one the value.)

To change the password, we use SQL (`ALTER USER ...`) or remove the
volume and start with a new database.

```bash
$ docker rm -f db
$ docker volume rm db-data
```


## Environment Variables and Secrets

Environment variables are **not secret**. Everyone who can access the
Docker Engine can read them:

```bash
$ docker container inspect db --format '{{json .Config.Env}}'
["MARIADB_ROOT_PASSWORD=newpw", ...]
```

Therefore:
* We never write passwords into a Dockerfile with `ENV` or `ARG`, because
    they are stored in the image (and in every registry we push it to).
* For local development, environment variables (or env files which are
    **not** committed to Git) are fine.
* For production, many images support a `_FILE` variant, for example
    `MARIADB_ROOT_PASSWORD_FILE=/run/secrets/db_root_password`. The
    variable only contains the **path** to a file with the password,
    which is provided by a secret management system (for example Docker
    Compose `secrets` or Kubernetes Secrets).


## Environment Variables in Docker Compose

In a `docker-compose.yml` file, we use `environment` instead of `-e` and
`env_file` instead of `--env-file`:

```yaml
services:
  app:
    build: .
    environment:
      GREETING: Hi
      NAME: ${USER_NAME:-Compose}
  app-file:
    build: .
    env_file:
      - app.env
```

```bash
$ docker compose run --rm app
Hi Compose!

$ docker compose run --rm app-file
Servus Graz!
Servus Graz!
```

### Variable Substitution

Docker Compose itself also reads environment variables: an expression
like `${USER_NAME:-Compose}` in `docker-compose.yml` is replaced **before**
the container is created. The value comes from the shell or from a file
named `.env` in the project directory. `:-Compose` is the default value
if the variable is not set:

```bash
$ USER_NAME=Lisa docker compose run --rm app
Hi Lisa!

$ echo 'USER_NAME=Bart' > .env
$ docker compose run --rm app
Hi Bart!
```

`docker compose config` shows the file after the substitution:

```bash
$ docker compose config
...
    environment:
      GREETING: Hi
      NAME: Bart
```

Note the difference between the two files:
* `.env`: Read by **Docker Compose** for `${...}` substitution in
    `docker-compose.yml`. Its values are **not** passed to the containers
    automatically.
* `env_file`: Read for a **service**. Its values are passed to the
    container. Unlike `docker run --env-file`, quotes are removed
    (`A="quoted"` becomes `quoted`).


## Summary

| Where                    | How                              | When          |
|--------------------------|----------------------------------|---------------|
| Dockerfile               | `ENV KEY=value`                  | default value |
| Dockerfile               | `ARG KEY=value`                  | build only    |
| `docker build`           | `--build-arg KEY=value`          | build only    |
| `docker run`             | `-e KEY=value`, `-e KEY`         | run time      |
| `docker run`             | `--env-file app.env`             | run time      |
| `docker-compose.yml`     | `environment:`, `env_file:`      | run time      |
| `.env` (Compose project) | `${KEY}` in `docker-compose.yml` | Compose only  |

Precedence at run time (highest first): `-e` / `environment`, then
`--env-file` / `env_file`, then `ENV` in the image.


## References

* [Docker: Set environment variables (docker run)](https://docs.docker.com/reference/cli/docker/container/run/#env)
* [Dockerfile reference: ENV](https://docs.docker.com/reference/dockerfile/#env)
* [Dockerfile reference: ARG](https://docs.docker.com/reference/dockerfile/#arg)
* [Docker Compose: Environment variables](https://docs.docker.com/compose/how-tos/environment-variables/)
* [DockerHub: mariadb](https://hub.docker.com/_/mariadb)

*Egon Teiniker, 2025-2026, GPL v3.0*
