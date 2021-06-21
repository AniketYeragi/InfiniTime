#!/bin/bash
# This script should be located on each Jenkins slave, and the jenkins user should have permission to run it with sudo

# Attempts to cleanly stop and remove all containers, volumes and images.
docker ps -q | xargs --no-run-if-empty docker stop
docker ps -q -a | xargs --no-run-if-empty docker rm --force --volumes
docker volume ls -q | xargs --no-run-if-empty docker volume rm
docker images -a -q | xargs --no-run-if-empty docker rmi -f

# Stops the docker service, unmounts all docker-related mounts, removes the entire docker directory, and starts docker again.

service docker stop
while [[ -n $(mount | grep -E "^none") ]]; do
  echo "Unmounting none"
  umount none
done
umount /var/lib/docker/aufs
echo "Deleting content of /var/lib/docker"
rm -rf /var/lib/docker/*
service docker start