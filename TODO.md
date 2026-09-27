# Course Refactoring — Review Notes & TODO

Review of the proposed new course outline against the current repository content.
The stage-based structure (Pre-Commit → Commit → Acceptance → Production, per
Humble/Farley) is a good backbone and most existing material maps onto it cleanly.
Items below are gaps, moves, and things to reconsider.

## Legend

- `[ ]` open item
- `(new)` content that does not yet exist in the repo and must be written
- `(have)` content already in the repo that the outline drops or misfiles
- `(confirm)` a decision the author needs to make

---

## Cross-cutting (whole course)

- [ ] **Version control practice** `(new)` — no content on branching strategy.
      Add trunk-based development, short-lived branches, feature toggles /
      branch by abstraction, small frequent commits. This is the "integration"
      in Continuous Integration. Place in 2.1 or a new 2.2.x.
- [ ] **Build once, promote the same artifact** `(new)` — state explicitly in
      2.1 / 3.1 that one binary/image flows through every stage (no per-environment
      rebuilds). `continuous-delivery/release-into-production/README.md` already
      leans on "the pipeline is the only route to production"; pair it with this.
- [ ] **Pipeline metrics / DORA** `(new)` — lead time, deploy frequency, MTTR,
      change-fail rate. Natural capstone; currently absent.
- [ ] **Automated security in the pipeline** `(new)` — the security section is
      strong on manual pen-testing but thin on SAST/DAST/SCA, dependency
      scanning, secret scanning, container image scanning (Trivy) as pipeline
      gates. Draw on `build-process/docker/docker-container/docker-security`
      and `build-process/*/bytecode/chatgpt-decompiler`. Bridges 2.3 and 3.2.
- [ ] **Automated unit-test + coverage gate in the Commit Stage** `(new)` — 2.3
      lists build and containers but not "compile + run unit tests + measure
      coverage (gcov/lcov, gtest in CI) + package." That is the heart of the
      commit stage.
- [ ] **Agentic SE thread** `(confirm)` — 1.2 introduces it, then it only
      resurfaces once as "AI Pair Programming" (2.2.1). Decide: standalone
      lecture or a thread woven through (AI for test generation, code review,
      security analysis / decompilation, pipeline authoring). If a thread, use
      the existing `chatgpt-decompiler` examples.

---

## 1. Introduction

- [ ] Add trunk-based development / integration frequency here (see cross-cutting).
- [ ] **1.2 Agentic Software Engineering** `(confirm)` — clarify scope and how it
      reconnects to 2.2.1.

## 2. Continuous Integration

### 2.2.1 Development Practices

- [ ] **git pre-commit hooks** `(new)` — the stage is literally "Pre-Commit".
- [ ] **Code review / pull requests** `(new)`.
- [ ] **Coding standards** `(new)` — and for embedded specifically
      **MISRA C / AUTOSAR C++**. Absent from outline and repo; a real gap for
      this audience.
- [ ] Distinguish linters/formatters from static analysis / SAST.

### 2.2.2 Embedded Linux Development

- [ ] **`raspberry-pi/` directory** `(confirm)` — the working tree is currently
      mid-deletion of this whole directory (setup, GPIO/WiringPi, SSH,
      vscode-remote). This section reintroduces exactly that content. Confirm:
      coming back here, or moving to a separate repo?
- [ ] Add **SPI** and **UART/serial** `(new)` — outline lists only I2C and CAN;
      both of these are at least as common on the Pi.
- [ ] Make the **Analog Discovery 3 hardware double** pay off later in 3.2.1
      (hardware-in-the-loop acceptance testing).

### 2.2.3 Embedded Architectures — Layers

- [ ] Before link-time polymorphism and patterns, make the baseline explicit:
      **abstract interface / pure-virtual HAL** and its embedded cost (vtable,
      no inlining), plus **PIMPL** for compile-time decoupling. `(new)`
- [ ] **Facade** and **Strategy** patterns `(new)` — repo has adapter + DI only.
- [ ] Client-Server and "REST C++/Python" are TODO in the outline — realistic;
      repo is Flask-heavy with no C++ MQTT or C++ REST client.
- [ ] One sentence on microservices as a style (= REST + independent
      deployability + per-service data); repo README treats it as its own style.

### 2.3.1 Build Process

- [ ] **CMake / build systems** `(have)` — dropped by the outline, but repo has
      `hello-cmake`, `test-cmake`, `gtest`, `cross-platform`. crosstool-NG is the
      toolchain; CMake is the orchestration. Both need a bullet.
- [ ] **Python build & packaging** `(have)` — dropped entirely, but repo has
      `build-process/python` (venv, packaging, wheels, `pyproject.toml`,
      `analyze-numpy`). Needed if Flask services are deployed.
- [ ] Consider adding: static vs. dynamic linking, stripping, semantic
      versioning of artifacts, reproducible builds.

### 2.3.2 Containers

- [ ] **Multi-stage builds** `(have)` — make explicit; repo has
      `docker-cxx-multi-stage` (relevant to small embedded images).
- [ ] **Image minimization / scanning** `(have/new)` — distroless, Trivy; repo
      has `docker-security`.
- [ ] **Yocto** `(move)` — "Yocto ??" is misfiled under containers. It is a
      distro/build-system topic. Move to 2.3.1, or a dedicated "Embedded Linux
      build systems: Yocto / Buildroot", or cut. Repo has only
      `yocto/firmware-analysis`.

### 2.3.3 CI Infrastructure

- [ ] Nexus and GitHub Actions are good additions (Nexus fills the otherwise
      missing "artifact repository" concept).
- [ ] **Build/test on real hardware** `(have)` — add as an explicit topic; repo
      has `jenkins-agent-raspi`. HW-in-the-loop agents are a distinctive
      embedded-CI concern; ties back to the Analog Discovery 3.

## 3. Continuous Delivery

### 3.2.1 Acceptance Testing

- [ ] Add **BDD / given-when-then / executable specification** `(have)` — repo
      has `executable-specification`.
- [ ] Add **production-like environments & test data management** — already
      described in `acceptance-stage/README.md`.
- [ ] Add **hardware-in-the-loop acceptance tests** `(new)` — where the hardware
      double from 2.2.2 returns.

### 3.2.2 Security Testing

- [ ] Intro: add **STRIDE**, **CVSS**; point at the OWASP/CWE lists the repo
      already ships (`OWASP-IoT-Top10.md`, `OWASP-Top10.md`, `CWE-TOP25.md`).
- [ ] Data Stores: call out **XXE** and **insecure deserialization** (Python
      pickle / Java) explicitly; consider **command injection** and **SSRF**.
- [ ] Add **credential storage / password hashing (bcrypt)** `(have)` — repo
      `docker-python-bcrypt`.
- [ ] MQTT Security: add **TLS** explicitly `(have)` — repo `mosquitto-docker-tls`;
      and topic-level ACLs.
- [ ] API Security: add **JWT** `(have)` (`article-service-auth-jwt`),
      **TLS / mTLS** `(have)` (`microservices/api-security/tls`),
      **rate limiting**, **input validation / mass assignment**.
- [ ] API discovery & monitoring `(have)` — repo `api-monitoring` (gobuster,
      Actuator, strings) is not reflected in the outline.
- [ ] Decide where `embedded-architectures/penetration-testing/`
      (interception-proxy, wireshark, password-john) lands — fits 3.2.2 tooling.

### 3.3 Production Stage

The outline has only TODOs here, but `release-into-production/README.md` already
covers CD vs. Continuous Deployment and release strategies. Flesh out:

- [ ] **Deployment strategies**: blue-green, canary, rolling, feature toggles.
- [ ] **Release management**: versioning, changelog, rollback.
- [ ] **Config & secrets management** across environments (12-factor).
- [ ] **Monitoring / observability**: health checks, logging, metrics, alerting,
      device telemetry.
- [ ] **OTA for embedded**: A/B partitions, signed images, delta updates,
      rollback-on-failed-boot, RAUC / SWUpdate / Mender, secure boot / chain of
      trust, fleet management.
- [ ] Optionally **IaC** (Ansible / Terraform) as a mention.

## Minor / tidy

- [ ] Numbering is inconsistent: everything is deeply numbered except 3.3, which
      has none. Fix once content lands.
