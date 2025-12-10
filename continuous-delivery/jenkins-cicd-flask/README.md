# Example: CI/CD for REST Service

In this example, we see the combination of a CI and a CD pipeline.

* **CI Pipeline**: A Python REST service is built into a wheel file.

* **CD Pipeline**: The wheel file is installed, the Flask server is 
    started, and the acceptance tests are executed.

The example can be found in the following GitHub repository:

```
https://github.com/teiniker/jenkins-pipeline-flask
```


## CI pipeline

The CI pipeline is defined by the `Jenkinsfile-ci` file and contains the 
following stages:

* `unit test`: This stage is where potential unit and component tests (white-box tests) 
    would be executed. These tests run locally and do not require any remote communication.

* `package`: If the tests complete successfully, a wheel file is built from the Python 
    source code, which can then be installed with the `pip` tool.

After a successful build, the wheel file is archived.


### Configure Jenkins Job (CI Pipeline)

We use the `jenkins-pipeline-flask` with `Jenkinsfile-ci`:

* New Item
    - Enter an item Name "**Flask CI Pipeline**"
	- Pipeline
    - Description (optional)
    - Build Triggers 
	    - Poll SCM: `H/1 * * * *`
    - Pipeline / Definition: 
      - Pipeleine: Pipeline Script from SCM: 
        - SCM: Git 
        - Repository URL: `https://github.com/teiniker/jenkins-pipeline-flask.git` 
        - Branches to build: `*/main` 
      - Script Path: **Jenkinsfile-ci**



## CD Pipeline

The CD pipeline is defined by the `Jenkinsfile-cd` file and contains the 
following stages:

* `download wheel`: The wheel file stored in the Jenkins archive (Artifactory) 
    is downloaded. For this, the Jenkins **copyArtifacts** plugin must be installed.

* `install wheel`: The wheel file, and therefore the application under test, 
    is installed into a virtual environment. 

* `start service`: The Flask server with the REST service is started.
    - `fuser -k ${FLASK_PORT}/tcp || true`: Kill any process currently using that port.
    - `nohup ./${VENV_DIR}/bin/python -m ${FLASK_APP_MODULE} --port=${FLASK_PORT} &`: Start Flask service in background
        - `nohup`: Prevents the process from being killed when the shell exits. 
            Every `sh """..."""` block runs in a temporary shell.
        - `&`: Runs the process in the background, so the pipeline can continue 
    - `sleep 5`: Wait for the service to start

* `acceptance tests`: The full suite of acceptance tests is executed against the running REST service.

In all cases, the Flask server is stopped: `fuser -k ${FLASK_PORT}/tcp || true`


### Configure Jenkins Job (CD Pipeline)

* New Item
    - Enter an item Name "**Flask CD Pipeline**"
	- Pipeline
    - Description (optional)
    - Build Triggers 
	    - Poll SCM: `H/1 * * * *`
    - Pipeline / Definition: 
      - Pipeleine: Pipeline Script from SCM: 
        - SCM: Git 
        - Repository URL: `https://github.com/teiniker/jenkins-pipeline-flask.git` 
        - Branches to build: `*/main` 
      - Script Path: **Jenkinsfile-cd**


_Nicoletta Kähling, Egon Teiniker, 2025, GPL v3.0_  
