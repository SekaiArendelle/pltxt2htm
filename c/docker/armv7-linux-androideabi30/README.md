
```sh
docker build -f c/docker/armv7-linux-androideabi30/Dockerfile -t armv7-linux-androideabi30-pltxt2htm-c-release .
docker run -d --name armv7-linux-androideabi30-pltxt2htm-c-release armv7-linux-androideabi30-pltxt2htm-c-release
docker cp armv7-linux-androideabi30-pltxt2htm-c-release:/pltxt2htm/c/armv7-linux-androideabi30-pltxt2htm-c-release.tar.xz .
```

Remove docker container and image
```sh
docker rm -f armv7-linux-androideabi30-pltxt2htm-c-release
docker rmi armv7-linux-androideabi30-pltxt2htm-c-release
```
