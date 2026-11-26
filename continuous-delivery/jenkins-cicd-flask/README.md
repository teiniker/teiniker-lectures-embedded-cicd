# Example: CI/CD for REST Services

In this example, we implement the **CI/CD Pipeline** with
two Jenkins pipelines:

* **CI Pipeline (Commit Stage)**: The Python REST service is tested with
    unit tests and built into a **wheel file**. This wheel file is our
    **Release Candidate**, and Jenkins archives it.

* **CD Pipeline (Acceptance Stage)**: The Release Candidate is copied
    from the CI pipeline and installed, the Flask server is started, and
    the **acceptance tests** are executed against the running service.

```
 Flask CI Pipeline (Commit Stage)        Flask CD Pipeline (Acceptance Stage)
+--------------------------------+       +----------------------------------+
| setup                          |       | download wheel  (copyArtifacts)  |
| unit test                      |       | install wheel                    |
| package    -> book_service.whl | ----> | start service                    |
| archive the wheel              |       | acceptance tests                 |
+--------------------------------+       +----------------------------------+
         triggered by a commit             triggered by a successful CI build
```

The example can be found in the following GitHub repository:

```
https://github.com/teiniker/jenkins-pipeline-flask
```

The acceptance tests are specified as user stories with acceptance
criteria in `UserStories.md`.


## Setup Jenkins

Start the native Jenkins service:

```bash
$ sudo systemctl start jenkins
$ sudo systemctl status jenkins
```

Access the Jenkins Web GUI:

```bash
URL: http://localhost:9000/
```

### Install the Copy Artifact Plugin

The CD pipeline uses the **Copy Artifact** plugin (`copyArtifacts()`),
which allows a pipeline to retrieve the build artifacts of another job:

* `(Gear) Manage Jenkins / Plugins / Available plugins`
    - Search for `Copy Artifact`
    - Select it and click `[Install]`


## CI Pipeline

### Configure Jenkins Job (CI Pipeline)

We use the `jenkins-pipeline-flask` with `Jenkinsfile-ci`:

* New Item
    - Enter an item Name "**Flask CI Pipeline**"
    - Pipeline
    - Description (optional)
    - Build Triggers
        - Poll SCM: `H/2 * * * *`
    - Pipeline / Definition:
        - Pipeline: Pipeline Script from SCM:
            - SCM: Git
            - Repository URL: `https://github.com/teiniker/jenkins-pipeline-flask.git`
            - Branches to build: `*/main`
        - Script Path: **Jenkinsfile-ci**

The name of the job is important: the CD pipeline refers to it.

With **Poll SCM**, Jenkins checks the repository every two minutes and
only starts a build if there is a **new commit**. In contrast,
**Build periodically** starts a build at the given times, even if nothing
has changed.

### Stages of the CI Pipeline

```groovy
stage('setup')
{
    steps
    {
        sh """
            python3 -m venv ${VENV_DIR}
            ./${VENV_DIR}/bin/pip install build
            ./${VENV_DIR}/bin/pip install --editable .
        """
    }
}
```

* `setup`: Create a virtual environment, and install the project with
    its dependencies (Flask). `--editable` installs the source code
    directly from the workspace, so the unit tests can import it.

* `unit test`: Run the unit tests in `tests/unit`:

    ```bash
    $ python -m unittest discover --start-directory tests/unit --pattern '*_test.py'
    ```

    The unit tests use the **Flask test client**, which calls the request
    handlers directly. No server is started, and there is no network
    communication, so these tests are fast.

* `package`: If the tests complete successfully, build the wheel file:

    ```bash
    $ python -m build --wheel --outdir dist
    ```

After a successful build, the `post` section archives the wheel file:

```groovy
archiveArtifacts artifacts: 'dist/*.whl', fingerprint: true
```

`fingerprint: true` stores a checksum of the file, so Jenkins can track
in which other builds this exact file is used.


## CD Pipeline

### Configure Jenkins Job (CD Pipeline)

* New Item
    - Enter an item Name "**Flask CD Pipeline**"
    - Pipeline
    - Description (optional)
    - Build Triggers: **none** (the trigger is defined in the Jenkinsfile)
    - Pipeline / Definition:
        - Pipeline: Pipeline Script from SCM:
            - SCM: Git
            - Repository URL: `https://github.com/teiniker/jenkins-pipeline-flask.git`
            - Branches to build: `*/main`
        - Script Path: **Jenkinsfile-cd**

After creating the job, we start it **once manually** (`Build Now`).
Jenkins reads the `triggers` directive of the Jenkinsfile only when the
pipeline runs, and registers the trigger afterwards.

### Triggered by the CI Pipeline

```groovy
triggers
{
    upstream(upstreamProjects: 'Flask CI Pipeline',
             threshold: hudson.model.Result.SUCCESS)
}
```

Every **successful** build of `Flask CI Pipeline` starts the CD pipeline.
So a commit flows through both pipelines automatically, and a failed
Commit Stage never reaches the Acceptance Stage.

Note that we don't configure **Poll SCM** for the CD pipeline. If both
pipelines polled the repository, they would start at the same time, and
the CD pipeline would test the wheel of the **previous** CI build.

### Copy the Release Candidate

```groovy
copyArtifacts(
    projectName: 'Flask CI Pipeline',
    filter: 'dist/*.whl',
    target: '.',
    fingerprintArtifacts: true,
    selector: upstream(fallbackToLastSuccessful: true)
)
```

* `projectName`: The job to copy from.
* `filter`: Which files to copy.
* `target`: Where to place them inside our workspace.
* `selector: upstream(...)`: Take the wheel of the CI build which
    **triggered** this pipeline. If we start the pipeline manually, take
    the wheel of the last successful build.
* `fingerprintArtifacts: true`: Record the fingerprint of the copied file.

Thanks to the fingerprints, Jenkins shows on the build page of the CI
pipeline (`See Fingerprints`) in which builds of the CD pipeline its
wheel was used, and vice versa.

### Install and Start the Release Candidate

```groovy
sh """
    rm -rf ${VENV_DIR}
    python3 -m venv ${VENV_DIR}
    ./${VENV_DIR}/bin/pip install requests
    ./${VENV_DIR}/bin/pip install dist/*.whl
"""
```

The wheel is installed into a **new** virtual environment, as a user
would install it. Then the **installed** service is started:

```bash
nohup ./${VENV_DIR}/bin/book-service --port ${FLASK_PORT} > service.log 2>&1 &
echo $! > service.pid
```

* `book-service`: A command which pip creates when it installs the wheel.
    It is defined in `pyproject.toml`:

    ```toml
    [project.scripts]
    book-service = "book_service.book_service:main"
    ```

* `nohup`: Prevents the process from being killed when the shell exits.
    Every `sh """..."""` block runs in its own shell.
* `&`: Runs the process in the background, so the pipeline can continue.
* `> service.log 2>&1`: The output of the service is written into a file.
* `echo $! > service.pid`: Store the process ID of the service, so we can
    stop exactly this process later.

Note that the CD pipeline also checks out the Git repository, because it
needs the `Jenkinsfile-cd` and the acceptance tests. We must not start
the service from this source code (for example with
`python -m src.book_service.book_service`): then we would test the
latest commit, but not the Release Candidate.

> **We test exactly the bits and bytes which we will release.**

### Wait Until the Service Is Ready

Starting a server takes some time. Instead of waiting a fixed time
(`sleep 5`), the pipeline sends a request every 0.5 seconds until the
service answers:

```bash
for i in $(seq 1 20); do
    if ./.venv/bin/python -c "import urllib.request; urllib.request.urlopen('http://localhost:9090/books', timeout=1)" 2>/dev/null; then
        echo "Service is up"
        exit 0
    fi
    sleep 0.5
done
echo "Service did not start:"
cat service.log
exit 1
```

* If the service is ready earlier, we don't waste time.
* If the service doesn't start within 10 seconds (for example, because
    the port is already in use), the stage fails with the output of the
    service, and not with a `Connection refused` in the acceptance tests.

### Acceptance Tests

```bash
$ python -m unittest discover --start-directory tests/acceptance --pattern '*_test.py'
```

The acceptance tests access the service only via its **REST API** (black
box tests), like any other client. The URL comes from the environment
variable `BOOK_SERVICE_URL`, which is set in the `environment` section of
the Jenkinsfile.

Every test leaves the data as it was: `test_insert` deletes the new book
again, and `test_update` restores the original data. So the tests don't
depend on each other or on their order.

### Stop the Service

```groovy
post
{
    always
    {
        sh """
            if [ -f service.pid ]; then
                kill \$(cat service.pid) || true
                rm service.pid
            fi
        """
    }
}
```

The service is stopped in all cases, also when a test fails. We only
stop the process we started.


## Run the Pipelines

A commit to the repository starts the CI pipeline (log excerpt):

```
Started by an SCM change
Running on Jenkins in /var/lib/jenkins/workspace/Flask CI Pipeline
...
[Pipeline] { (unit test)
+ ./.venv/bin/python -m unittest discover --start-directory tests/unit --pattern *_test.py --verbose
test_delete (book_service_test.BookServiceUnitTest.test_delete) ... ok
test_find_all (book_service_test.BookServiceUnitTest.test_find_all) ... ok
test_find_by_id (book_service_test.BookServiceUnitTest.test_find_by_id) ... ok
test_find_by_id_not_found (book_service_test.BookServiceUnitTest.test_find_by_id_not_found) ... ok
test_insert (book_service_test.BookServiceUnitTest.test_insert) ... ok
test_update (book_service_test.BookServiceUnitTest.test_update) ... ok
test_update_not_found (book_service_test.BookServiceUnitTest.test_update_not_found) ... ok
----------------------------------------------------------------------
Ran 7 tests in 0.010s
OK
[Pipeline] { (package)
+ ./.venv/bin/python -m build --wheel --outdir dist
...
Successfully built book_service-0.1.0-py3-none-any.whl
...
Archive the Release Candidate
Archiving artifacts
Recording fingerprints
Finished: SUCCESS
```

The first build of the CD pipeline is started manually (`Build Now`), so
it takes the wheel of the last successful CI build (log excerpt):

```
Started by user root
Running on raspi-5 in /home/student/workspace/Flask CD Pipeline
...
[Pipeline] { (download wheel)
Copied 1 artifact from "Flask CI Pipeline" build number 31
+ ls -l dist
-rw-r--r-- 1 student student 14850 Nov 26 16:45 book_service-0.1.0-py3-none-any.whl
[Pipeline] { (install wheel)
...
Successfully installed Flask-3.1.3 blinker-1.9.0 book-service-0.1.0 click-8.5.1 itsdangerous-2.2.0 jinja2-3.1.6 markupsafe-3.0.4 werkzeug-3.1.9
[Pipeline] { (start service)
+ echo 14580
+ seq 1 20
+ nohup ./.venv/bin/book-service --port 9090
+ ./.venv/bin/python -c import urllib.request; urllib.request.urlopen('http://localhost:9090/books', timeout=1)
+ sleep 0.5
+ ./.venv/bin/python -c import urllib.request; urllib.request.urlopen('http://localhost:9090/books', timeout=1)
+ echo Service is up
Service is up
[Pipeline] { (acceptance tests)
+ ./.venv/bin/python -m unittest discover --start-directory tests/acceptance --pattern *_test.py --verbose
test_find_all (book_service_test.BookServiceTest.test_find_all) ... ok
test_find_by_id (book_service_test.BookServiceTest.test_find_by_id) ... ok
test_insert (book_service_test.BookServiceTest.test_insert) ... ok
test_update (book_service_test.BookServiceTest.test_update) ... ok
----------------------------------------------------------------------
Ran 4 tests in 0.020s
OK
[Pipeline] { (Declarative: Post Actions)
Stop the book service
+ cat service.pid
+ kill 14580
Finished: SUCCESS
```

* `Copied 1 artifact from "Flask CI Pipeline" build number 31`: The CD
    pipeline uses the Release Candidate of CI build #31.
* `+ echo 14580`: The process ID of the service, which is stored in
    `service.pid`. The shell prints the commands in its own order: the
    background command `nohup ...` appears after the `echo`.
* The first request of the readiness check fails, because the service is
    not ready yet. The second request, 0.5 seconds later, succeeds.
* `+ kill 14580`: At the end, exactly the started process is stopped.

The **fingerprint** of the wheel file shows that both pipelines used the
same file: `Flask CI Pipeline #31` (built and archived) and
`Flask CD Pipeline #31` (tested).

Note that both pipelines use `agent any`. Here, the CI pipeline ran on the
built-in node (x86_64) and the CD pipeline on the Raspberry Pi agent
(aarch64). This works because the wheel contains only Python code
(`py3-none-any`: any Python 3, no ABI, any platform).

After this first run, Jenkins has registered the `upstream` trigger, and
every successful CI build starts the CD pipeline automatically. The log
of the CD pipeline then starts with
`Started by upstream project "Flask CI Pipeline"`.


## CI vs. CD Pipeline

|               | Flask CI Pipeline           | Flask CD Pipeline            |
|---------------|-----------------------------|------------------------------|
| Stage         | Commit Stage                | Acceptance Stage             |
| Triggered by  | a new commit (Poll SCM)     | a successful CI build        |
| Input         | source code                 | Release Candidate (wheel)    |
| Tests         | unit tests (test client)    | acceptance tests (HTTP)      |
| Server        | no                          | installed service is started |
| Output        | archived wheel file         | tested Release Candidate     |


## References

* [Jenkins Copy Artifact](https://plugins.jenkins.io/copyartifact/)
* [Jenkins Pipeline Syntax: Triggers](https://www.jenkins.io/doc/book/pipeline/syntax/#triggers)
* [Python Packaging: Entry Points](https://packaging.python.org/en/latest/specifications/entry-points/)

_Nicoletta Kähling, Egon Teiniker, 2025-2026, GPL v3.0_
