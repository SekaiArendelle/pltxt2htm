Build docker image
```sh
docker build -f c/docker/aarch64-linux-android30/Dockerfile -t aarch64-linux-android30-pltxt2htm-c-release .
docker run --rm aarch64-linux-android30-pltxt2htm-c-release cat /pltxt2htm/c/aarch64-linux-android30-pltxt2htm-c-release.tar.xz > aarch64-linux-android30-pltxt2htm-c-release.tar.xz
```

Remove docker image
```sh
docker rmi aarch64-linux-android30-pltxt2htm-c-release
```
