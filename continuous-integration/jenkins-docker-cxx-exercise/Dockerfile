FROM jenkins/jenkins:lts

RUN jenkins-plugin-cli --plugins workflow-aggregator git warnings-ng

USER root

# TODO: Install required C++ build tools.

#) TODO: Install Google Test framework.

USER jenkins
