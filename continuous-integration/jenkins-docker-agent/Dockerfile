FROM jenkins/jenkins:lts

RUN jenkins-plugin-cli --plugins workflow-aggregator git warnings-ng

USER jenkins
