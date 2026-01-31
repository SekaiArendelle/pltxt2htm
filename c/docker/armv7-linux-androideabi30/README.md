Build docker image
```sh
docker build -f c/docker/armv7-linux-androideabi30/Dockerfile -t armv7-linux-androideabi30-pltxt2htm-c-release .
docker run --rm armv7-linux-androideabi30-pltxt2htm-c-release cat /pltxt2htm/c/armv7-linux-androideabi30-pltxt2htm-c-release.tar.xz > armv7-linux-androideabi30-pltxt2htm-c-release.tar.xz
```

Remove docker image
```sh
docker rmi armv7-linux-androideabi30-pltxt2htm-c-release
```
