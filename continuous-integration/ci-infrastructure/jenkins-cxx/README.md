# Example: Jenkins C++ Pipeline

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

## Configure Jenkins Job

We use the `jenkins-pipeline-cxx` with `Jenkinsfile`:

* New Item
    - Enter an item Name "**C++ Pipeline**"
	- Pipeline
    - Description (optional)
    - Build Triggers 
	    - Poll SCM: `H/2 * * * *`
    - Pipeline / Definition: 
      - Pipeline: Pipeline Script from SCM: 
        - SCM: Git 
        - Repository URL: `https://github.com/teiniker/jenkins-pipeline-cxx.git` 
        - Branches to build: `*/main` 
      - Script Path: **Jenkinsfile**


## Build Trigger: Poll SCM

With **Poll SCM**, Jenkins regularly asks the Git repository whether
there is a new commit. Only if there is one, a new build is started.

### The Schedule

The schedule uses the cron syntax with five fields:

```
MINUTE  HOUR  DAY-OF-MONTH  MONTH  DAY-OF-WEEK
  H/2    *         *          *         *
```

`H/2 * * * *` means: **every 2 minutes**, every hour, every day.

* `*/2` would mean every 2 minutes, starting at minute 0
    (`:00, :02, :04, ...`).

* `H/2` also means every 2 minutes, but starting at an **offset** which
    Jenkins computes from a **hash** of the job name. Depending on the job,
    this is either `:00, :02, :04, ...` or `:01, :03, :05, ...`.
    So not all jobs poll the Git server at the same time.

Below the schedule field, Jenkins shows when the trigger would have run
last and when it will run next.

Note that with `H/1 * * * *`, the offset can only be 0, so it is the same
as `* * * * *` (every minute).

### How Polling Works

At every scheduled time, Jenkins does the following:

1. It looks up the commit (revision) which the **last build** has
    checked out.

2. It asks the remote repository for the latest commit of the branch,
    with `git ls-remote`. This command only transfers the commit IDs of
    the branches, not the code, so polling is fast and needs no
    workspace.

3. If the latest commit differs from the last built commit, Jenkins
    schedules a new build. Otherwise, nothing happens.

Jenkins writes the result of the last poll into the **Git Polling Log**
(link on the job page):

```
Started on Nov 26, 2026, 5:29:00 PM
Using strategy: Default
[poll] Last Built Revision: Revision e05435e03d8fed550b59250cc52db268d7f2c02e (refs/remotes/origin/main)
 > git ls-remote -h -- https://github.com/teiniker/jenkins-pipeline-cxx.git # timeout=10
Found 1 remote heads on https://github.com/teiniker/jenkins-pipeline-cxx.git
[poll] Latest remote head revision on refs/heads/main is: e05435e03d8fed550b59250cc52db268d7f2c02e - already built by 36
...
Done. Took 0.95 sec
No changes
```

The latest commit `e05435e` has already been built by build #36, so no
new build is started.

The check appears **twice** in the log because the last build has checked
out the repository twice: once to read the `Jenkinsfile` (Pipeline Script
from SCM) and once for the build itself (`checkout scm`).

### Poll SCM vs. Build Periodically

Jenkins also offers the trigger **Build periodically**, which uses the
same cron syntax. The difference:

* **Poll SCM**: Start a build only if there is a **new commit**.
* **Build periodically**: Start a build at the given times, **whether or
    not** something has changed (for example, a nightly build).


### The First Build

Jenkins compares with the commit which the **last build** checked out:

* For a **new job** without any build, the first poll finds no previous
    build and starts a build immediately (`no previous build to compare
    to`).

* If we change the **repository URL** of an existing job, Jenkins still
    polls the repository of the last build. Commits to the new repository
    are not detected. So after changing the repository, we start one
    build manually (`Build Now`).


## Test the Trigger

1. **No change, no build:** Wait a few minutes without pushing a commit.
    The Git Polling Log shows `No changes`, and no new build appears in
    the build history.

2. **Push a commit:** Change a file in the repository (for example the
    `README.md` of `jenkins-pipeline-cxx`) and push the commit:

    ```bash
    $ git commit -am "Test Poll SCM trigger"
    $ git push
    ```

3. **Wait for the next poll:** Within 2 minutes, a new build appears in
    the build history. Its console output starts with:

    ```
    Started by an SCM change
    ```

    and shows the new commit:

    ```
    Checking out Revision e05435e03d8fed550b59250cc52db268d7f2c02e (refs/remotes/origin/main)
    Commit message: "Comment out InvalidFileExtension test case"
    ```

    The **Changes** page of the build lists the commits which are new
    since the last build.

4. **Break the build:** Push a commit with a failing test. The triggered
    build fails, and Jenkins shows it in red. This is exactly the fast
    feedback which Continuous Integration is about: the developer knows
    within minutes that the last commit broke something.

### Polling vs. Webhooks

Polling causes a request to the Git server every 2 minutes, even if
nothing has changed, and a commit is detected with a delay of up to
2 minutes.

The alternative is a **webhook**: GitHub sends a request to Jenkins
immediately after every push. This requires that GitHub can reach our
Jenkins server over the network, which is usually not the case for a
Jenkins running in a lab VM. So we use polling.

_Egon Teiniker, 2025-2026, GPL v3.0_    



