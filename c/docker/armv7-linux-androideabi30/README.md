## Debug

```sh
docker build -f c/docker/armv7-linux-androideabi30/Dockerfile --build-arg mode=debug -t armv7-linux-androideabi30-pltxt2htm-c-debug .
docker run -d --name armv7-linux-androideabi30-pltxt2htm-cmd-debug armv7-linux-androideabi30-pltxt2htm-cmd-debug
docker cp armv7-linux-androideabi30-pltxt2htm-cmd-debug:/pltxt2htm/cmd/armv7-linux-androideabi30-pltxt2htm-cmd-debug.tar.xz .
```

Remove docker container and image
```sh
docker rm -f armv7-linux-androideabi30-pltxt2htm-cmd-debug
docker rmi armv7-linux-androideabi30-pltxt2htm-cmd-debug
```

## Release
