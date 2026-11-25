# Example: Jenkins and Linux VM Agent

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


## Setup Linux VM Agent

**Add a new Jenkins agent** (node):

* On the Jenkins web UI:	
	- Go to `(Gear) Manage Jenkins / Nodes`
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
        $ curl -sO http://localhost:9000/jnlpJars/agent.jar
        $ java -jar agent.jar -url http://localhost:9000/ -secret <YOURSECRET> -name "linux-vm" -webSocket -workDir "/home/student"
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
      - Pipeline: Pipeline Script from SCM: 
        - SCM: Git 
        - Repository URL: `https://github.com/teiniker/jenkins-pipeline-cxx.git` 
        - Branches to build: `*/main` 
      - Script Path: **Jenkinsfile-agent-vm**


## Any vs. Linux-VM Node

The `jenkins-pipeline-cxx` repository contains two pipelines which only
differ in the `agent` directive:

```groovy
// Jenkinsfile
pipeline
{
    agent any
    ...
}
```

```groovy
// Jenkinsfile-agent-vm
pipeline
{
    agent { label 'linux-vm' }
    ...
}
```

* `agent any`: Run the pipeline on **any node** with a free executor.
    In our setup, this can be the **built-in node** (the Jenkins
    controller itself, which has 2 executors) or the `linux-vm` agent.
    We can't control which one is used.

* `agent { label 'linux-vm' }`: Run the pipeline **only** on a node with
    the label `linux-vm`. The name of a node is always also one of its
    labels, so this works although we left the `Labels` field empty.

In our lab, the Jenkins controller and the `linux-vm` agent run on the
**same machine**. So both pipelines are executed on the same host. 
Still, they run in completely different environments.

### Two Processes, Two Users

On the VM, we find two Java processes:

```bash
$ ps -eo user,args | grep -E "jenkins.war|agent.jar"
jenkins  /usr/bin/java ... -jar /usr/share/java/jenkins.war --webroot=/var/cache/jenkins/war --httpPort=9000
student  java -jar agent.jar -url http://localhost:9000/ -secret ... -name linux-vm -webSocket -workDir /home/student
```

* The **Jenkins controller** is started by systemd as the system user
    `jenkins`. Builds on the built-in node are executed **inside this
    process**, with its user, its environment and its file permissions.

* The **agent** is a separate process, started by us in a terminal as
    the user `student`. It connects to the controller (via WebSocket)
    and receives the build steps from there. Builds on `linux-vm` are
    executed with the user, environment and permissions of `student`.

The console output of the builds shows where they were executed:

```
# agent any (here: c-pipeline)
Running on Jenkins in /var/lib/jenkins/workspace/c-pipeline

# agent { label 'linux-vm' }
Running on linux-vm in /home/student/workspace/C++ Pipeline (Linux VM Agent)
```

`Jenkins` is the name of the built-in node in the console output.


### Why Does This Matter?

* **Tools and environment:** The cross compiler for the Raspberry Pi is
    installed in `/opt/x-tools` and added to the `PATH` in
    `/home/student/.bashrc`. The agent, started from a terminal of
    `student`, inherits this `PATH`, so a build step like
    `aarch64-rpi5-linux-gnu-gcc ...` works. The Jenkins service doesn't
    read `.bashrc`, so the same step fails on the built-in node with
    `command not found`. The same applies to tools installed in the home
    directory of `student`, which the user `jenkins` can't even read.

* **Reproducibility:** With `agent any`, the build may run on the
    built-in node once and on `linux-vm` the next time. A build which
    depends on installed tools then succeeds or fails depending on the
    node. With a label, the pipeline states **exactly** which
    environment it needs.

* **Security:** Builds on the built-in node run with the permissions of
    the Jenkins controller. A build script (for example from a pull
    request) can read and change everything in `/var/lib/jenkins`: the
    configuration of all jobs, the stored credentials and the encryption
    keys in `/var/lib/jenkins/secrets`. The user `student` has no access
    to this directory:

    ```bash
    $ ls /var/lib/jenkins/secrets
    ls: cannot open directory '/var/lib/jenkins/secrets': Permission denied
    ```

    Therefore, the Jenkins documentation recommends to **not run builds
    on the built-in node** and to set its number of executors to 0
    (`Manage Jenkins / Nodes / Built-In Node / Configure`). Then
    `agent any` always selects an agent.

* **Availability:** A labeled pipeline needs the agent. If `agent.jar`
    is not running, `linux-vm` is shown as offline, and the build waits
    in the queue until the agent connects again. With `agent any`, the
    build would simply run on another node.

Note that the agent runs only as long as the `java -jar agent.jar`
command in our terminal. Closing the terminal stops the agent. For a
permanent setup, the agent is started as a systemd service as well.

Also note that the secret is visible in the process list (`ps`) for all
users on the machine. Instead of `-secret <YOURSECRET>`, we can store the
secret in a file which only we can read, and pass
`-secret @secret-file` to the agent.

_Nicoletta Kähling, Egon Teiniker, 2025-2026, GPL v3.0_    



