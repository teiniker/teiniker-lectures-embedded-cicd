# Example: CI/CD for REST Service

In this example, we see the combination of a CI and a CD pipeline.

* **CI Pipeline**: A Python REST service is built into a wheel file.

* **CD Pipeline**: The wheel file is installed, the Flask server is 
    started, and the acceptance tests are executed.

The example can be found in the following GitHub repository:

```
https://github.com/teiniker/jenkins-pipeline-flask
```

## CI Pipeline

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
