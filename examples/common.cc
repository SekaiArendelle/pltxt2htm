#include <fast_io/fast_io.h> // in include/fast_io
#include <pltxt2htm/pltxt2htm.hh> // in include/pltxt2htm

int main() {
    auto html = ::pltxt2htm::pltxt2common_html(
        u8R"(
<color=red>Hello Quantum PhysicsLab</color>
<b>Common HTML output</b>
)");
    ::fast_io::io::println(::fast_io::u8c_stdout(), html);

    return 0;
}
