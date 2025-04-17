# Two-stage build
# First build the artefacts
FROM ubuntu:noble-20250404

ARG ROOT=/root

RUN apt update && apt install -y \
    git \
    curl \
    build-essential \
    zlib1g-dev \
    libcurl4-openssl-dev \
    libreadline-dev

ARG GRAALVM_VERSION="23.0.2+7.1"
ARG GRAALVM_URL=https://download.oracle.com/graalvm/23/latest/graalvm-jdk-23_linux-x64_bin.tar.gz

RUN mkdir ${ROOT}/graalvm
RUN mkdir ${ROOT}/.ssh
ARG COMPILEHOST_GRAAL=${ROOT}/graalvm/graalvm-jdk-${GRAALVM_VERSION}

RUN ssh-keyscan codeberg.org >> ${ROOT}/.ssh/known_hosts
RUN ssh-keyscan github.com >> ${ROOT}/.ssh/known_hosts
RUN cd ${ROOT} && \
    git clone https://codeberg.org/flypig/harbour-newpipe.git

RUN cd ${ROOT}/harbour-newpipe/ && \
    git submodule set-url -- java/NewPipeExtractor https://github.com/llewelld/NewPipeExtractor.git && \
    git submodule update --init

RUN cd ${ROOT}/graalvm && \
    curl ${GRAALVM_URL} -o graalvm.tar.gz && \
    tar -xf graalvm.tar.gz && \
    COMPILEHOST_GRAAL=${ROOT}/graalvm/graalvm-jdk-${GRAALVM_VERSION}

ENV GRAALVM_HOME=${COMPILEHOST_GRAAL}
ENV JAVA_HOME=${COMPILEHOST_GRAAL}

RUN cd ${ROOT}/harbour-newpipe/java/NewPipeExtractor/ && \
    ./gradlew nativeCompile

RUN cd ${ROOT}/harbour-newpipe/ && \
    make

# Second install the artefacts in a runtime container
FROM ubuntu:noble-20250404

ARG ROOT=/root

COPY --from=0 ${ROOT}/harbour-newpipe/cpp/main /root/newpipe-console
COPY --from=0 ${ROOT}/harbour-newpipe/cpp/lib/appwrapper.so /root/appwrapper.so

RUN apt update && apt install -y \
    zlib1g \
    libcurl4t64 \
    libreadline8t64

ENV LD_LIBRARY_PATH=${ROOT}
CMD ["/root/newpipe-console"]
