# GitHub Actions

**GitHub Actions** is the CI/CD platform which is built into GitHub.
Instead of operating our own CI server (like Jenkins), we describe the
build in a YAML file in our repository, and GitHub executes it on its
own machines whenever something happens in the repository, for example
a `git push`.

## Main Concepts

* **Workflow:** An automated process, described in a YAML file in the
    directory `.github/workflows/` of the repository. A repository can
    contain several workflows, for example one for building and testing,
    and another one for a release. GitHub finds them automatically; we
    don't have to register them anywhere.

* **Event (Trigger):** Something that starts a workflow, declared with
    `on:`. Typical events are:
    - `push`: commits are pushed to a branch,
    - `pull_request`: a pull request is opened or updated,
    - `schedule`: a time-based trigger (cron syntax),
    - `workflow_dispatch`: a manual start via a button in the web UI.

* **Job:** A set of steps which run **on the same runner**. A workflow
    consists of one or more jobs. By default, jobs run **in parallel**;
    with `needs:` we define dependencies between them.

* **Step:** A single task within a job. A step either executes a shell
    command (`run:`) or uses an action (`uses:`). The steps of a job run
    one after another and share the same file system.

* **Action:** A reusable building block, published in a Git repository
    and referenced by `<owner>/<repo>@<version>`, for example
    `actions/checkout@v4`. The
    [GitHub Marketplace](https://github.com/marketplace?type=actions)
    lists thousands of actions, similar to the plugins of Jenkins.

* **Runner:** The machine which executes a job.
    - **GitHub-hosted runners** are virtual machines provided by GitHub
        (`ubuntu-latest`, `windows-latest`, `macos-latest`, and also ARM
        machines like `ubuntu-24.04-arm`). Every job gets a **fresh VM**,
        which is deleted after the job.
    - **Self-hosted runners** are our own machines (for example a
        Raspberry Pi in the lab) with the GitHub runner software
        installed. Like the `linux-vm` agent in Jenkins, the runner
        connects to GitHub and waits for jobs.

* **Artifacts:** Files produced by a job (executables, test reports),
    which are stored by GitHub and can be downloaded from the web UI.

* **Secrets and variables:** Passwords and tokens are stored in the
    repository settings and accessed with `${{ secrets.NAME }}`. They are
    masked in the log output.

* **Expressions and contexts:** `${{ ... }}` expressions access
    information about the run, for example `${{ github.sha }}` (the
    commit) or `${{ env.IMAGE }}` (an environment variable).

The relation between the concepts:

```
Repository
└── .github/workflows/build-docker.yml   (Workflow)
    ├── on: push, workflow_dispatch       (Events)
    └── jobs:
        └── build-and-test                (Job, runs on a Runner)
            ├── Checkout                  (Step using an Action)
            ├── Build stage               (Step running a command)
            └── ...
```


## Workflow

As an example, we use the workflow
[build-docker.yml](https://github.com/teiniker/jenkins-pipeline-cxx/blob/main/.github/workflows/build-docker.yml)
of the `jenkins-pipeline-cxx` repository. It does the same as the
Jenkins pipeline `Jenkinsfile-agent-docker`: it builds the toolchain
image from the `Dockerfile`, then compiles and tests `file_service`
inside this image, and archives the test executable.

```yaml
name: build-docker

on:
  push:
    branches: [main]
  workflow_dispatch:

permissions:
  contents: read

env:
  IMAGE: file-service-build:ci

jobs:
  build-and-test:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Set up Docker Buildx
        uses: docker/setup-buildx-action@v3

      - name: Build toolchain image from Dockerfile
        uses: docker/build-push-action@v6
        with:
          context: .
          file: ./Dockerfile
          load: true
          tags: ${{ env.IMAGE }}
          cache-from: type=gha
          cache-to: type=gha,mode=max

      - name: Build stage
        run: |
          echo 'Build stage: compile all code and build an executable'
          docker run --rm -v "$GITHUB_WORKSPACE:/app" -w /app/file_service "$IMAGE" \
            sh -c 'mkdir -p build && cd build && cmake .. && make'

      - name: Test stage
        run: |
          echo 'Test stage: run the test cases'
          docker run --rm -v "$GITHUB_WORKSPACE:/app" -w /app/file_service/build "$IMAGE" \
            ./test/test

      - name: Archive build artifacts
        if: success()
        uses: actions/upload-artifact@v4
        with:
          name: file-service-test
          path: file_service/build/test/test
```

### Workflow Settings

* `name: build-docker`: The name shown in the **Actions** tab of the
    repository.
* `on:`: The workflow is started
    - on every `push` to the `main` branch,
    - manually (`workflow_dispatch`): **Actions / build-docker / Run
        workflow**.
* `permissions: contents: read`: The workflow gets a temporary access
    token (`GITHUB_TOKEN`). Here we restrict it to reading the
    repository. A good default: give a workflow only the permissions it
    needs.
* `env: IMAGE: ...`: An environment variable for all jobs and steps.
    It is used as `${{ env.IMAGE }}` in the YAML file and as `$IMAGE` in
    the shell commands.

### The Job

* `build-and-test`: The ID of the only job of this workflow.
* `runs-on: ubuntu-latest`: The job runs on a GitHub-hosted runner with
    the latest Ubuntu LTS version. Docker is already installed.
* `steps:` The steps run one after another on this runner.

### The Steps

1. **Checkout** (`actions/checkout@v4`): Clones the repository into the
    workspace of the runner (`$GITHUB_WORKSPACE`). In contrast to
    Jenkins, this is **not** done automatically: a job starts on an
    empty VM.

2. **Set up Docker Buildx** and **Build toolchain image**: Build the
    image from our `Dockerfile`, like `agent { dockerfile true }` in
    Jenkins.
    - `load: true`: Load the image into the local Docker engine of the
        runner, so `docker run` can use it.
    - `tags`: The name of the image (`file-service-build:ci`).
    - `cache-from` / `cache-to: type=gha`: Because every job starts on a
        fresh VM, there is no local Docker layer cache. Without these
        options, Google Test would be compiled again in every run. With
        `type=gha`, the layers are stored in the **GitHub Actions
        cache** and reused by the next runs.

3. **Build stage** and **Test stage**: Run the build and the tests in a
    container of our image. Like Jenkins, we mount the workspace into
    the container (`-v "$GITHUB_WORKSPACE:/app"`), so the build results
    remain in the workspace after the container is removed (`--rm`).
    The `|` after `run:` starts a multi line shell script.

4. **Archive build artifacts** (`actions/upload-artifact@v4`): Upload
    the test executable as artifact `file-service-test`, like
    `archiveArtifacts` in Jenkins. `if: success()` runs the step only if
    all previous steps succeeded, like the `post { success { ... } }`
    block of a `Jenkinsfile`. (This is also the default for steps.)

If a command fails (exit code not 0), the step fails, the following
steps are skipped, and the whole run is marked as **failed**.


### Running the Workflow

After pushing the workflow file to `main`, GitHub starts it immediately.
The **Actions** tab of the repository shows all runs:

```
✔ build-docker  Run 1  Added GitHub Actions configuration.  Triggered via push       1m 8s
✔ build-docker  Run 2                                       workflow_dispatch        1m 0s
```

A click on a run shows the jobs, the log of every step, and the
artifact `file-service-test`, which we can download as a ZIP file.


## Jenkins vs. GitHub Actions

In the previous examples, we ran Jenkins **locally** on our Linux VM:
* with `agent any` on the built-in node,
* with the [Linux VM agent](../jenkins-agent-vm/),
* with a [Docker agent](../jenkins-agent-docker/) (`agent { dockerfile true }`),
* with a [Raspberry Pi agent](../jenkins-agent-raspi/) via SSH.

The workflow above corresponds to the Docker agent. The pipeline
concepts map directly:

| Jenkins (`Jenkinsfile`)             | GitHub Actions (workflow YAML)         |
|-------------------------------------|----------------------------------------|
| Pipeline                            | Workflow                               |
| Poll SCM / webhook trigger          | `on: push`                             |
| Build Now                           | `on: workflow_dispatch`                |
| `agent any` / `agent { label ... }` | `runs-on: ubuntu-latest` / self-hosted |
| `agent { dockerfile true }`         | build image + `docker run`             |
| `stage('build') { steps { sh } }`   | `- name: Build stage` with `run:`      |
| (automatic checkout)                | `uses: actions/checkout@v4`            |
| `archiveArtifacts`                  | `uses: actions/upload-artifact@v4`     |
| `post { success { ... } }`          | `if: success()`                        |
| Credentials                         | Secrets                                |
| Plugins                             | Actions (Marketplace)                  |

### Infrastructure

The main difference is **who operates the CI infrastructure**:

* **Jenkins (local):** We install, configure, update and back up the
    Jenkins server and its plugins. The jobs are created in the web UI
    (only the pipeline itself is in the repository). The builds run on
    machines we control, and the code never leaves our network.

* **GitHub Actions:** GitHub operates everything. We only write the
    workflow file, which is part of the repository, so the **complete CI
    configuration is versioned** with the code. There is no server to
    maintain.

### Triggers

Our Jenkins **polls** the repository every minute (`Poll SCM:
H/1 * * * *`): it asks GitHub regularly whether there are new commits.
A webhook (GitHub notifies Jenkins on every push) would be better, but
it requires that GitHub can reach our Jenkins server, which isn't the
case for a VM in the lab network.

GitHub Actions knows every event in the repository directly, so a push
starts the workflow **immediately**, without polling. It also supports
events like pull requests, issues or releases.

### Build Environment

|             | Jenkins           | Jenkins + Docker | GitHub Actions        |
|-------------|-------------------|------------------|-----------------------|
| Machine     | our VM or agent   | our VM or agent  | fresh VM per job      |
| Tools from  | node installation | `Dockerfile`     | runner + `Dockerfile` |
| Workspace   | persists          | persists         | new every run         |
| Environment | persists          | new container    | new VM                |
| Layer cache | -                 | local, automatic | configure `type=gha`  |
| Checkout    | automatic         | automatic        | explicit step         |

* **Jenkins without Docker** (`agent any`, `linux-vm`): The build uses
    the tools installed on the node. Fast and simple, but every project
    depends on what the administrator installed, and leftovers from
    previous builds remain in the workspace.

* **Jenkins with Docker:** The tools come from the `Dockerfile` in the
    repository. The environment is reproducible, and the local Docker
    cache makes repeated builds fast.

* **GitHub Actions:** Every job starts on a **new, clean VM**. Nothing
    survives between runs, unless we store it explicitly (artifacts,
    cache). This makes builds very reproducible, but also means that
    every run has to check out the code and restore its caches again.

### Hardware and Network

GitHub-hosted runners are machines in a data center. They **can't
access** our lab network, our Raspberry Pi or any connected hardware.

For embedded systems this means:
* Building and running unit tests in a container or for `x86_64`/`arm64`
    works well on hosted runners.
* Tests on the **target hardware** (like with our Raspberry Pi agent in
    Jenkins) require a **self-hosted runner** on a machine in the lab.
    The runner connects to GitHub (outbound), so no incoming connection
    to our network is needed.

### Costs and Limits

* GitHub-hosted runners are free for **public** repositories. For
    private repositories, a monthly quota of runner minutes is included,
    depending on the GitHub plan.

* A job can run for at most 6 hours on a hosted runner, and artifacts
    are deleted after a retention period (90 days by default).

* Our local Jenkins has no such limits, but we pay with our own
    hardware and the time to maintain it.


## References

* [GitHub Docs: Understanding GitHub Actions](https://docs.github.com/en/actions/get-started/understand-github-actions)
* [GitHub Docs: Workflow syntax](https://docs.github.com/en/actions/reference/workflows-and-actions/workflow-syntax)
* [GitHub Docs: GitHub-hosted runners](https://docs.github.com/en/actions/concepts/runners/github-hosted-runners)
* [GitHub Docs: Self-hosted runners](https://docs.github.com/en/actions/concepts/runners/self-hosted-runners)
* [Docker: Cache management with GitHub Actions](https://docs.docker.com/build/ci/github-actions/cache/)

* [Example: jenkins-pipeline-cxx](https://github.com/teiniker/jenkins-pipeline-cxx)

_Egon Teiniker, 2025-2026, GPL v3.0_
