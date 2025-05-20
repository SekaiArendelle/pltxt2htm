#include <cstring>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>
#include <fast_io/fast_io.h>

int main() noexcept {
    auto html = ::pltxt2htm::pltxt2html(u8R"(
文本1<br   />文本2<bR/>文本3<br   >文本4<br     /
文本5<Br<br>
)");


    auto answer = u8R"(<br>
文本1<br>
文本2<br>
文本3<br>
文本4<br     /<br>
文本5<Br<br>
<br>
)";
    if (::std::memcmp(html.data(), answer, html.size()) != 0) [[unlikely]] {
        ::exception::terminate();
    }
    // ::fast_io::print(::fast_io::mnp::code_cvt(html));

    return 0;
}
