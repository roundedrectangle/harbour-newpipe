# Two-stage build
# First build the artefacts
FROM ubuntu:noble-20250404

ARG ROOT=/root

WORKDIR $ROOT

RUN apt update && apt install -y \
    git \
    build-essential \
    zlib1g-dev \
    libcurl4-openssl-dev \
    libreadline-dev

ARG GRAALVM_VERSION="23.0.2+7.1"
ARG GRAALVM_URL=https://download.oracle.com/graalvm/23/latest/graalvm-jdk-23_linux-x64_bin.tar.gz

RUN mkdir graalvm
ARG COMPILEHOST_GRAAL=$ROOT/graalvm/graalvm-jdk-${GRAALVM_VERSION}
ENV GRAALVM_HOME=${COMPILEHOST_GRAAL}
ENV JAVA_HOME=${COMPILEHOST_GRAAL}

WORKDIR $ROOT/graalvm

ADD ${GRAALVM_URL} $ROOT/graalvm/graalvm.tar.gz

RUN tar -xf graalvm.tar.gz

WORKDIR $ROOT/harbour-newpipe

COPY . $ROOT/harbour-newpipe/

WORKDIR $ROOT/harbour-newpipe/java/NewPipeExtractor

RUN ./gradlew nativeCompile

WORKDIR $ROOT/harbour-newpipe

RUN make

# Second install the artefacts in a runtime container
FROM ubuntu:noble-20250404

ARG ROOT=/root

COPY --from=0 $ROOT/harbour-newpipe/cpp/main $ROOT/newpipe-console
COPY --from=0 $ROOT/harbour-newpipe/cpp/lib/appwrapper.so $ROOT/appwrapper.so

RUN apt update && apt install -y \
    zlib1g \
    libcurl4t64 \
    libreadline8t64

ENV PATH=$PATH:$ROOT
ENV LD_LIBRARY_PATH=$ROOT
ENTRYPOINT ["newpipe-console"]
