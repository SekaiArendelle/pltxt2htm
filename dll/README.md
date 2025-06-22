compile pltxt2htm to dll (not recommended)

```sh
mkdir build
clang++ -shared pltxt2htm.cc -o build/libpltxt2htm.so -std=c++23 -fuse-ld=lld -flto=thin -I ../include/ -fvisibility=hidden
```
