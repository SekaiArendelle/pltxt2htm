#include <fast_io/fast_io.h> // in include/fast_io
#include <pltxt2htm/pltxt2htm.hh> // in include/pltxt2htm

int main() {
    auto html = ::pltxt2htm::pltxt2fixedadv_html(
        u8R"(
# Hello Quantum PhysicsLab
[Physics-Lab-Web2](https://github.com/NetLogo-Mobile/plweb2)
)",
        u8"localhost:5173", u8"project", u8"visitor", u8"author", u8"coauthors");
    ::fast_io::io::println(::fast_io::u8c_stdout(), html);

    return 0;
}
