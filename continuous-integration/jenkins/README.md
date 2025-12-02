# Jenkins 

> Jenkins is an **open-source automation server** used mainly for 
> **continuous integration (CI)** and **continuous delivery (CD)**.


## Setup Jenkins 

Install Jenkins directly on the Debian virtual machine, similar to how the 
Docker Engine was installed.

```bash
$ sudo apt update

$ sudo apt install fontconfig 

# jdk21 is already installed
$ java -version 
openjdk version "21" 2023-09-19
OpenJDK Runtime Environment (build 21+35-2513)
OpenJDK 64-Bit Server VM (build 21+35-2513, mixed mode, sharing)


$ sudo wget -O /etc/apt/keyrings/jenkins-keyring.asc https://pkg.jenkins.io/debian-stable/jenkins.io-2023.key
$ echo "deb [signed-by=/etc/apt/keyrings/jenkins-keyring.asc]" \
  https://pkg.jenkins.io/debian-stable binary/ | sudo tee \
  /etc/apt/sources.list.d/jenkins.list > /dev/null
$ sudo apt update

$ sudo apt install jenkins
```

Configure the Jenkins service (not enabled at Debian startup):

```bash
$ sudo systemctl enable jenkins # default setting
$ sudo systemctl disable jenkins

$ sudo systemctl start jenkins
$ sudo systemctl status jenkins

$ sudo systemctl stop jenkins
```

Finally, access the Jenkins wep page:

```bash
URL: http://localhost:8080

# Copy & paste this password as Administrator Password 
sudo cat /var/lib/jenkins/secrets/initialAdminPassword

# Install suggested plugins

# Create an admin user
root/root66

# Install plugin: Docker Pipeline
#  Go to Manage Jenkins → Manage Plugins → Available
#    Install: Docker Pipeline
```

To allow Jenkins to access Docker, we need to add the `jenkins` 
user to the `docker` group:

```bash
# Add jenkins to the docker group
sudo usermod -aG docker jenkins

# Restart Jenkins
sudo systemctl restart jenkins
```


## Jenkins Pipeline

Jenkins uses a `Jenkinsfile` which is part of a project to configure the 
steps needed in the CI/CD pipeleine.

A **pipeline** is a sequence of automated operations that usually represents 
a part of the software delivery and quality assurance process.

A Jenkins pipeline consists of two kinds of elements:

* **Step**: A single operation that tells Jenkins what to do.
	For example, check out code from the repository, execute a script .

* **Stage**: A logical separation of steps that groups conceptually distinct sequences of steps.
 	For example, Build, Test, and Deploy, used to visualize the Jenkins pipeline progress.

_Example_: Jenkins commit pipeline (checkout, build, and unit test)

```yml
pipeline 
{
    agent any 
    
    stages 
    {
        stage('build') 
        {
            steps 
            {
                echo 'Build stage: compile all code and build an executable' 
                sh 'make'
            }
        }
        stage('test') 
        {
            steps 
            {
                echo 'Test stage: run the test cases' 
                sh 'build/stack_test'
            }
        }
    }
    post 
    {
        always 
        {
            echo 'I will always execute this!'
        }
    }
}
```

## Jenkinsfile Syntax

A **declarative pipeline** is always specified inside the `pipeline` block 
and contains sections, directives, and steps.


### Directives

Directives express the configuration of a pipeline or its parts: 

* **Agent**: This specifies where the execution takes place and can define 
the label to match the equally-labeled agents, or docker to specify a 
container that is dynamically provisioned to provide an environment for 
the pipeline execution. 

* **Triggers**: This defines automated ways to trigger the pipeline and 
can use cron to set the time-based scheduling, or pollSCM to check the 
repository for changes.

* **Options**: This specifies pipeline-specific options, e.g. timeout 
(maximum time of pipeline run) or retry (number of times the pipeline 
should be re-run after failure).

* **Environment**: This defines a set of key values used as environment 
variables during the build.

* **Parameters**: This defines a list of user-input parameters.

* **Stage**: This allows for the logical grouping of steps.

* **When**: This determines whether the stage should be executed depending 
on the given condition.


### Sections

Sections define the pipeline structure and usually contain one or more 
directives or steps. 
They are defined with the following keywords: 

* **Stages**: This defines a series of one or more stage directives.

* **Steps**: This defines a series of one or more step instructions.

* **Post**: This defines a series of one or more step instructions that are 
run at the end of the pipeline build; they are marked with a condition 
(e.g., `always`, `success`, or `failure`), and usually used to send 
notifications after the pipeline build (we will cover this in detail in 
the Triggers and notifications section).


### Steps 

Steps define the operations that are executed, so they actually tell 
Jenkins what to do: 

* **sh**: This executes the shell command; actually, it's possible 
to define almost any operation using `sh`. 

* **custom**: Jenkins offers a lot of operations that can be used as 
steps (for example, `echo`); many of them are simply wrappers over 
the sh command used for convenience; plugins can also define their 
own operations.

* **script**: This executes a block of the Groovy-based code that 
can be used for some non-trivial scenarios where flow control is needed.



## References

* [YouTube: How to Setup Docker Containers As Build Agents for Jenkins](https://youtu.be/ymI02j-hqpU?si=rHKmVRh3_S4euWDG)

* [Jenkins Homepage: Installing Jenkins](https://www.jenkins.io/doc/book/installing/linux/)


_Egon Teiniker, 2025, GPL v3.0_