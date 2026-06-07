import fast_io;
import pltxt2htm;

int main() {
    auto html = ::pltxt2htm::pltxt2fixedadv_html(u8R"(
# Hello Quantum PhysicsLab
With Markdown supports
)",
    u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
    ::fast_io::io::println(::fast_io::u8c_stdout(), html);

    return 0;
}
