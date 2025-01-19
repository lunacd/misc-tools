#!/bin/env fish

if test -f ./dockerfiles/$argv[1]
    set dockerfile ./dockerfiles/$argv[1]
    set image_tag (basename $argv[1] ".Dockerfile")
else
    set dockerfile ./dockerfiles/Dockerfile
    set image_tag $argv[1]
end

docker build . -f $dockerfile -t $image_tag --build-arg COMPONENT=$image_tag; or exit 1
docker tag $image_tag haowenl/$image_tag:$argv[2]; or exit 1
docker push haowenl/$image_tag:$argv[2]; or exit 1
