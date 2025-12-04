FROM jenkins/jenkins:lts

RUN jenkins-plugin-cli --plugins workflow-aggregator git warnings-ng

USER root

# Install required packages: g++, make, cmake, and build tools
RUN apt update && apt install -y git g++ make cmake

# Install Google Test framework
RUN git clone -q https://github.com/google/googletest.git /googletest \
  && mkdir -p /googletest/build \
  && cd /googletest/build \
  && cmake .. && make && make install \
  && cd / && rm -rf /googletest

USER jenkins
