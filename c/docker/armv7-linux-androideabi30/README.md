Build docker image

> [!NOTE]
> This image temporarily uses Android NDK r30 beta 2. The clang-r563880c
> compiler shipped with NDK r29 crashes while parsing valid C++23
> explicit-object member calls in `container::Expected`. NDK r30 beta 2 ships
> clang-r574158b, which has been verified with both the shared and static
> Release builds. Switch back to a stable NDK after r30 is released.

```sh
docker build -f c/docker/armv7-linux-androideabi30/Dockerfile -t armv7-linux-androideabi30-pltxt2htm-c-release .
docker run --rm armv7-linux-androideabi30-pltxt2htm-c-release cat /pltxt2htm/c/armv7-linux-androideabi30-pltxt2htm-c-release.tar.xz > armv7-linux-androideabi30-pltxt2htm-c-release.tar.xz
```

Remove docker image
```sh
docker rmi armv7-linux-androideabi30-pltxt2htm-c-release
```
