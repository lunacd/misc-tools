#!/bin/env fish

docker build . -f ./dockerfiles/Dockerfile -t $argv[1] --build-arg COMPONENT=$argv[1]; or exit 1
docker tag $argv[1] haowenl/$argv[1]:$argv[2]; or exit 1
docker push haowenl/$argv[1]:$argv[2]; or exit 1
