# Example: Jenkins and Linux VM Agent

## Setup Jenkins

Start the native Jenkins service:

```bash
$ sudo systemctl start jenkins
$ sudo systemctl status jenkins	
```

Access the Jenkins Web GUI:

```bash
URL: http://localhost:8080/
```


## Setup Linux VM Agent

**Add a new Jenkins agent** (node):

* On the Jenkins web UI:	
	- Go to `Manage Jenkins / Nodes`
		- Click `[New Node]`
			- Node name: linux-vm
			- Choose “Permanent Agent” 
			- `[Create]`

	- On the configuration screen:
		- Number of executors: 1
		- Remote root directory: `/home/student`
		- Labels: 
		- Usage: `Use this node as much as possible`
		- Launch method: `Launch agent by connecting it to the controller`
		- Availability: `Keep this agent online as much as possible` 
		- `[Save]`

	- Go to the Nodes overview, ensure your new Node `linux-vm` is listed.

* Setup the **agent software on the Linux VM**:

	- In Jenkins, navigate to: `Manage Jenkins / Nodes / linux-vm`:

	- Run from Linux agent command line: 
	
        ```bash
        $ cd Downloads 
        $ curl -sO http://localhost:8080/jnlpJars/agent.jar
        $ java -jar agent.jar -url http://localhost:8080/ -secret <YOURSECRET> -name "linux-vm" -webSocket -workDir "/home/student"
        ```
    
    The Linux VM agent can now use pre-installed tools (e.g., crosstool-ng) for builds.



## Configure Jenkins Job

We use the `jenkins-pipeline-cxx` with `Jenkinsfile-agent-vm`:

* New Item
    - Enter an item Name "**C++ Pipeline (Linux VM Agent)**"
	- Pipeline
    - Description (optional)
    - Build Triggers 
	    - Poll SCM: `H/1 * * * *`
    - Pipeline / Definition: 
      - Pipeleine: Pipeline Script from SCM: 
        - SCM: Git 
        - Repository URL: `https://github.com/teiniker/jenkins-pipeline-cxx.git` 
        - Branches to build: `*/main` 
      - Script Path: **Jenkinsfile-agent-vm**




_Nicoletta Kähling, Egon Teiniker, 2025, GPL v3.0_    



