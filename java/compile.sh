#!/bin/bash

set -e

if [[ -z "${PORT}" ]]; then
    PORT=22
fi

# Check that the COMPILEHOST environment variable is set
if [[ -z "$COMPILEHOST" ]]; then
    echo "Ensure COMPILEHOST is set to the SSH connection string of your phone." 1>&2
    echo "For example: export COMPILEHOST=defaultuser@192.168.2.15" 1>&2
    exit 1
fi

# Set up the base directory
COMPILEHOST_BASE=/home/defaultuser/Documents/Development/newpipe

# Other directories are derived from the base directory
COMPILEHOST_WORKSPACE=${COMPILEHOST_BASE}/NewPipeExtractor
COMPILEHOST_GRAAL=${COMPILEHOST_BASE}/graalvm/graalvm-jdk-23.0.2+7.1
COMPILEHOST_MAVENLOCAL=${COMPILEHOST_BASE}/graalvm/m2

# Where to download the GraalVM development kit from
GRAALVM_DOWNLOAD=https://download.oracle.com/graalvm/23/latest/graalvm-jdk-23_linux-aarch64_bin.tar.gz

MEMCHECK=$(ssh -p ${PORT} ${COMPILEHOST} 'free -g | tail -n 2 | tr -s " " | cut -d " " -f 2 | echo $(awk "{sum+=\$1} END {print sum < 16}")')
if [[ "${MEMCHECK}" -eq "1" ]]; then
    echo "Ensure the compile host has at least 16 GiB of RAM." 1>&2
    echo "If necessary, swap can be used for this." 1>&2
    exit 1
fi

# To install GraalVM on your phone
# cd ${COMPILEHOST_BASE}
# mkdir graalvm
# cd graalvm
# curl ${GRAALVM_DOWNLOAD} -o graalvm.tar.gz
# tar -xf graalvm.tar.gz
echo "Installing toolchain"
ssh -p ${PORT} ${COMPILEHOST} "mkdir -p ${COMPILEHOST_BASE}"
ssh -p ${PORT} ${COMPILEHOST} "if [ ! -d \"${COMPILEHOST_GRAAL}\" ]; then cd ${COMPILEHOST_BASE}; mkdir graalvm; cd graalvm; curl ${GRAALVM_DOWNLOAD} -o graalvm.tar.gz; tar -xf graalvm.tar.gz; else echo Already installed; fi"

# Trqnsfer using rsync
echo "Transferring code to phone"
pushd NewPipeExtractor
rsync -ah --info=progress2 --update -e "ssh -p ${PORT}" . ${COMPILEHOST}:${COMPILEHOST_WORKSPACE}
popd

# Build using GraalVM
# cd ${COMPILEHOST_WORKSPACE}
# GRAALVM_HOME=${COMPILEHOST_GRAAL} JAVA_HOME=${COMPILEHOST_GRAAL} ./gradlew nativeCompile
echo "Starting build"
ssh -p ${PORT} ${COMPILEHOST} "cd ${COMPILEHOST_WORKSPACE}; GRAALVM_HOME=${COMPILEHOST_GRAAL} JAVA_HOME=${COMPILEHOST_GRAAL} ./gradlew nativeCompile" --console verbose

# Copy the build artefacts back
echo "Transferring code from phone"
mkdir -p include
scp -P ${PORT} ${COMPILEHOST}:"${COMPILEHOST_WORKSPACE}/extractor/build/native/nativeCompile/*.h" ./include/
scp -P ${PORT} ${COMPILEHOST}:"${COMPILEHOST_WORKSPACE}/appwrapper/build/native/nativeCompile/*.h" ./include/
mkdir -p lib
scp -P ${PORT} ${COMPILEHOST}:"${COMPILEHOST_WORKSPACE}/extractor/build/native/nativeCompile/extractor.so" ./lib/
scp -P ${PORT} ${COMPILEHOST}:"${COMPILEHOST_WORKSPACE}/appwrapper/build/native/nativeCompile/appwrapper.so" ./lib/



