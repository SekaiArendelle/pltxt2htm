import fast_io;
import pltxt2htm;

int main() {
    auto html = ::pltxt2htm::pltxt2advanced_html(u8R"(
# Hello Quantum PhysicsLab
With Markdown supports
)");
    ::fast_io::io::println(::fast_io::u8c_stdout(), html);

    return 0;
}
