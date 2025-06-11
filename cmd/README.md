# Use `pltxt2htm` in command line

## Usage:
```sh
pltxt2htm -i $your_input_file --host localhost
```
pltxt2htm will print output html in stdout

```sh
pltxt2htm -i $your_input_file --host localhost -o $your_output_file
```
pltxt2htm will write output html to $your_output_file

## Build
I love cross compiling, therefore, I will always use `target` and `sysroot` to build `pltxt2htm`.

Prepare:
* I use [xmake](https://github.com/xmake-io/xmake) to build `pltxt2htm`
* I downloaded clang from [their](https://github.com/trcrsired/llvm-releases/releases), which not only supply $host.tar.xz, but also supply host's sysroot in tar.
* I downloaded gcc from [their](https://github.com/24bit-xjkp/toolchains/releases/tag/gcc16.0.0-rc1).

You can also use your own toolchains.

explain:
* like cmake, xmake also use `config, build, install` to configure, build and install a project.
* for clang, `x86_64-windows-gnu-clang` means `clang --target=x86_64-windows-gnu` to compile
* for gcc, `x86_64-w64-mingw32-gcc` means using executable $target-gcc to compile
* for msvc ... which still not full support C++23

You can use
```sh
xmake config --help
```
to show all config options.

> Cause I use header-only to supply api, therefore, it's easy to build `pltxt2htm` without build tools:
> ```sh
> clang++ ./pltxt2htm.cc -o pltxt2htm -std=c++23 -I ../include
> ```
> If you are building `pltxt2htm` to windows, don't forget to add `-lntdll`

### x86_64-windows-gnu (aka. x86_64-w64-mingw32)

#### clang
```sh
xmake config --plat=mingw --arch=x64 --toolchain=x86_64-windows-gnu-clang
# Note: --sysroot=$YOUR_SYSROOT_PATH is optional
# you can specify the sysroot path by yourself (all triplets are supported):
# xmake config --plat=mingw --arch=x64 --toolchain=x86_64-w64-mingw32-clang --sysroot=$YOUR_SYSROOT_PATH
xmake build
xmake install --output=$YOUR_INSTALL_PATH
```

You can also add `--sysroot=$YOUR_SYSROOT_PATH` to `xmake config`, otherwise, clang will detect sysroot automatically if you have installed x86_64-w64-mingw32-native-gcc.

You can also use `libunwind`, `libc++` and `compiler-rt` to build `pltxt2htm`
```sh
xmake config --plat=mingw --arch=x64 --toolchain=x86_64-windows-gnu-clang --unwindlib=libunwind --runtimes=c++_shared --rtlib=compiler-rt --sysroot=$YOUR_SYSROOT_PATH
xmake build
```

Download [sysroot](https://github.com/24bit-xjkp/toolchains/releases/download/llvm20.0.0-v1/sysroot.tar.xz) at their.


#### gcc
```sh
xmake config --plat=mingw --arch=x64 --toolchain=x86_64-w64-mingw32-gcc
xmake build
xmake install --output=$YOUR_INSTALL_PATH
```

### x86_64-windows-msvc
Only clang supports this triplet.

```sh
xmake config --plat=windows --arch=x64 --toolchain=x86_64-windows-msvc-clang
# or, specify --sysroot
# xmake config --plat=windows --arch=x64 --toolchain=x86_64-windows-msvc-clang --sysroot=$YOUR_SYSROOT_PATH
xmake build
xmake install --output=$YOUR_INSTALL_PATH
```

Download [windows-msvc-sysroot](https://github.com/trcrsired/windows-msvc-sysroot) at their.

### aarch64-windows-msvc
Only clang supports this triplet.

```sh
xmake config --plat=windows --arch=aarch64 --toolchain=aarch64-windows-msvc-clang --sysroot=$YOUR_SYSROOT_PATH
xmake build
xmake install --output=$YOUR_INSTALL_PATH
```

Download [windows-msvc-sysroot](https://github.com/trcrsired/windows-msvc-sysroot) at their.

### i686-windows-msvc
Only clang supports this triplet.

```sh
xmake config --plat=windows --arch=i686 --toolchain=i686-windows-msvc-clang --sysroot=$YOUR_SYSROOT_PATH
xmake build
xmake install --output=$YOUR_INSTALL_PATH
```

Download [windows-msvc-sysroot](https://github.com/trcrsired/windows-msvc-sysroot) at their.

### x86_64-linux-gnu
Take ubuntu2004 for example (glibc v2.31):

```sh
xmake config --plat=linux --arch=x64 --toolchain=x86_64-linux-gnu-clang --sysroot=$YOUR_SYSROOT_PATH
xmake build
xmake install --output=$YOUR_INSTALL_PATH
```

You can also use `libunwind`, `libc++` and `compiler-rt` to build `pltxt2htm`
```sh
xmake config --plat=linux --arch=x64 --toolchain=x86_64-linux-gnu-clang --unwindlib=libunwind --runtimes=c++_shared --rtlib=compiler-rt --sysroot=$YOUR_SYSROOT_PATH
xmake build
xmake install --output=$YOUR_INSTALL_PATH
```

Download x86_64-ubuntu2004-linux-gnu-sysroot at [their](https://github.com/GoodenoughPhysicsLab/releases-for-ubuntu2004/releases/download/only-for-release/x86_64-ubuntu2004-gnu-sysroot.tar.xz)

### aarch64-linux-android30
This triplet is only supported by clang

Take aarch64-linux-android30 for example:
```sh
xmake config --plat=android --arch=aarch64 --toolchain=aarch64-linux-android30-clang --sysroot=$YOUR_SYSROOT_PATH
xmake build
xmake install --output=$YOUR_INSTALL_PATH
```

I downloaded aarch64-linux-android30-sysroot from [their](https://github.com/trcrsired/llvm-releases/releases/download/llvm21-20250518/aarch64-linux-android30.tar.xz) (you should extract aarch64-linux-android30.tar.xz, the sysroot is in aarch64-linux-android30/aarch64-linux-android30)

### aarch64-apple-darwin24
```sh
xmake config --plat=macosx --arch=aarch64 --toolchain=aarch64-apple-darwin24-clang --sysroot=$YOUR_SYSROOT_PATH
xmake build
xmake install --output=$YOUR_INSTALL_PATH
```

Download aarch64-apple-darwin24-sysroot at [their](https://github.com/trcrsired/apple-darwin-sysroot/releases/download/20250207/aarch64-apple-darwin24.tar.xz)

### wasm32-wasip1
```sh
xmake config --plat=wasm --arch=wasm32 --toolchain=wasm32-wasip1-clang --sysroot=$YOUR_SYSROOT_PATH
xmake build
xmake install --output=$YOUR_INSTALL_PATH
```

Download wasm-sysroot at [their](https://github.com/trcrsired/llvm-releases/releases/download/llvm21-20250518/wasm-sysroots.tar.xz)

Q: why I still use emcc in js/xmake.lua?
A: emcc is a wrapper of clang, but it will generate a glue js file for wasm
