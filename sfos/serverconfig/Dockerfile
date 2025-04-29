# Two-stage build
# First build the artefacts
FROM debian:buster
# Contains glibc 2.30

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt install -y \
    build-essential \
    wget \
    xz-utils \
    bzip2 \
    make \
    autoconf

WORKDIR /root

ADD https://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-10.2.0/gcc-10.2.0.tar.xz .
RUN tar xf gcc-10.2.0.tar.xz

WORKDIR /root/gcc-10.2.0

ADD https://gmplib.org/download/gmp/gmp-6.2.0.tar.xz .
RUN tar xf gmp-6.2.0.tar.xz && mv gmp-6.2.0 gmp

ADD https://ftp.gnu.org/gnu/mpfr/mpfr-4.1.0.tar.gz .
RUN tar xf mpfr-4.1.0.tar.gz && mv mpfr-4.1.0 mpfr

ADD https://ftp.gnu.org/gnu/mpc/mpc-1.2.1.tar.gz .
RUN tar xf mpc-1.2.1.tar.gz && mv mpc-1.2.1 mpc

ADD https://gcc.gnu.org/pub/gcc/infrastructure/isl-0.18.tar.bz2 .
RUN tar xf isl-0.18.tar.bz2 && mv isl-0.18 isl

RUN ./configure --prefix=/opt/gcc-10 --enable-languages=c,c++ && \
    make -j$(nproc) && \
    make install

# Second install the artefacts in a runtime container
FROM debian:buster
COPY --from=0 /opt/gcc-10 /opt/gcc-10

RUN apt update && apt install -y \
    git \
    build-essential \
    curl \
    procps \
    rsync \
    openssh-server \
    zlib1g-dev \
    libcurl4-openssl-dev \
    libreadline-dev

RUN update-rc.d ssh enable
RUN service ssh start

RUN useradd -ms /bin/bash defaultuser
USER defaultuser
WORKDIR /home/defaultuser
RUN mkdir .ssh
COPY --chown=defaultuser:defaultuser authorized_keys .ssh/

RUN echo 'export LD_LIBRARY_PATH=/opt/gcc-10/lib64/:$LD_LIBRARY_PATH' >> temp
RUN echo 'export PATH=/opt/gcc-10/bin/:$PATH' >> temp
RUN cat .bashrc >> temp
RUN mv temp .bashrc

ENV COMPILEHOST_BASE=/home/defaultuser/Documents/Development/newpipe
ENV GRAALVM_DOWNLOAD=https://download.oracle.com/graalvm/23/latest/graalvm-jdk-23_linux-aarch64_bin.tar.gz

RUN mkdir -p ${COMPILEHOST_BASE}

WORKDIR ${COMPILEHOST_BASE}

RUN mkdir graalvm
ADD --chown=defaultuser:defaultuser ${GRAALVM_DOWNLOAD} ${COMPILEHOST_BASE}/graalvm/graalvm.tar.gz

RUN cd graalvm && \
    tar -xf graalvm.tar.gz

USER root

ENTRYPOINT service ssh restart && bash
