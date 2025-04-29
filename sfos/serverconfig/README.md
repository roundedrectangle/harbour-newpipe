# Remote build server for harbour-newpipe

The NewPipe Extractor Java code needs building using GraalVM native-build, which can either be done on-phone or on some other aarch64 device.
As the memory requirements for the build are quite large, running on a phone may not be viable, even with significant quantities of swap space allocated.

Running the build on a remote aarch64 server is therefore an alterantive with a more suitable level of resources.

## Cloud configuration

I'm using a remote server with the following configuration:
1. An aarch64 `r8g.8xlarge` [instance](https://aws.amazon.com/ec2/instance-types/r8g/) from AWS.
2. Running Ubuntu Noble: 'ubuntu/images/hvm-ssd-gp3/ubuntu-noble-24.04-arm64-server-20250305'.
3. 32 vCPUs.
4. 256 GiB memory.
5. 128 GiB SSD volume.
6. Security group with ports 22 and 23 open for TCP access for SSH.

## Server configuration

The server needs to be running Docker.

```sh
$ sudo useradd -ms /bin/bash newpipe
$ sudo apt update
$ sudo apt install -y docker.io
```

Copy the `Dockerfile` and the user's `authorized_keys` file into the same directory.

```sh
$ curl -O https://codeberg.org/flypig/harbour-newpipe/raw/branch/main/sfos/serverconfig/Dockerfile
$ cp ~/.ssh/authorized_keys .
```

Build the Docker image.

```sh
$ docker build -t newpipe-devenv .
```

Run the Docker image in a container.
This will run it in the background and open port 23 for SSH access into the container.

```sh
docker run -p 23:22 -dit --restart unless-stopped newpipe-devenv
```

## Buidling on the server

With this arrangement a build can be run on the server using the following command, where `<IP-ADDRESS>` is replaced with the IP address of the running cloud server.

```sh
PORT=23 COMPILEHOST=defaultuser@<IP-ADDRESS> ./compile.sh 
```


