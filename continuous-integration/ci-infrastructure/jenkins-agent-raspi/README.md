# Example: Jenkins and Raspberry Pi 5 Agent

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


## Setup Java on Raspi 5

When you use SSH launch, Jenkins connects to the Raspberry Pi, copies over 
`agent.jar`, and then runs it with Java:

```bash
$ java -jar agent.jar
```

So even though the connection starts with SSH, the actual agent process is 
still a Java application. Without Java, Jenkins cannot start the agent.

Therefore, we have to **install the Java Development Kit (JDK)**:

```bash
$ sudo apt install openjdk-21-jdk -y
```


## Setup Linux VM Agent

Try to **connect to the Raspi via SSH**:

```bash
$ ssh <username>@<ip address>
password: <password>

$ exit 
```

With SSH connectivity verified, we can proceed to **configure the Jenkins agent**:

**Add a new Jenkins agent** (node):

* On the Jenkins web UI:	
	- Go to `Manage Jenkins / Nodes`
		- Click `[New Node]`
			- Node name: raspi-5
			- Choose “Permanent Agent” 
			- `[Create]`

	- On the configuration screen:
		- Number of executors: 1
		- Remote root directory: `/home/student`
		- Labels: 
		- Usage: `Use this node as much as possible`
		- Launch method: ` Launch agents via SSH`
			- Host: <ip address>
			- Credentials: <username> / <password>> 
				- Add Credentials
					- Domain: Global credentials 
					- Kind: Username with password 
					- Scope: Global 
			- Host Key Verification Strategy: Manually trusted key Verification Strategy
				- Check: Require manual verification of initial connection
		- Availability: `Keep this agent online as much as possible` 
		- `[Save]`

	- Go to the Nodes overview, ensure your new Node `linux-vm` is listed.


## Configure Jenkins Job

We use the `jenkins-pipeline-cxx` with `Jenkinsfile-agent-raspi`:

* New Item
    - Enter an item Name "**C++ Pipeline (Raspi Agent)**"
	- Pipeline
    - Description (optional)
    - Build Triggers 
	    - Poll SCM: `H/1 * * * *`
    - Pipeline / Definition: 
      - Pipeleine: Pipeline Script from SCM: 
        - SCM: Git 
        - Repository URL: `https://github.com/teiniker/jenkins-pipeline-cxx.git` 
        - Branches to build: `*/main` 
      - Script Path: **Jenkinsfile-agent-raspi**


After running the build on the Raspi5 agent, we can **download the executable and analyze it**:

```bash
$ file Downloads/test 
Downloads/test: ELF 64-bit LSB pie executable, ARM aarch64, version 1 (GNU/Linux), 
dynamically linked, interpreter /lib/ld-linux-aarch64.so.1, BuildID[sha1]=7c0f3b90efad19219953202ede7f85e40b6cf725, for GNU/Linux 3.7.0, with debug_info, not stripped
```

As we can see, it is an **ARM aarch64 binary**.

_Nicoletta Kähling, Egon Teiniker, 2025, GPL v3.0_    



