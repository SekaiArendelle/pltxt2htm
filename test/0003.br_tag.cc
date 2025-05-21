#include <cstring>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>
#include <fast_io/fast_io.h>

int main() noexcept {
    auto html = ::pltxt2htm::pltxt2html(u8R"(
文本1<br   />文本2<bR/>文本3<br   >文本4<br    /
文本5<Br<br>
)");


    auto answer = u8R"(<br>文本1<br>文本2<br>文本3<br>文本4&lt;br&nbsp;&nbsp;&nbsp;&nbsp;/<br>文本5&lt;Br<br><br>)";
    ::exception::assert_true(::std::memcmp(html.data(), answer, html.size()) == 0);

    return 0;
}
