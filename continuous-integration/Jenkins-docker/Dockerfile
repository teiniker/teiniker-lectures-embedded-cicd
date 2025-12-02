FROM jenkins/jenkins:lts

RUN jenkins-plugin-cli --plugins workflow-aggregator git warnings-ng

USER root
RUN apt-get update && \
    apt-get install -y \
        build-essential \
        gcc \
        g++ \
        cppcheck \
        make \
        cmake \
        git \
        gdb \
        valgrind \
        python3 \
        python3-venv \
        python3-pip \
        pylint \
        python3-pandas \
    && rm -rf /var/lib/apt/lists/*

USER jenkins
