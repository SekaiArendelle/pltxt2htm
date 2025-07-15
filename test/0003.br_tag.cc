#include <cstring>
#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    ::fast_io::u8string_view text{
        u8R"(
文本1<br   />文本2<bR/>文本3<br   >文本4<br    /
文本5<Br<br>
)"};

    auto html = ::pltxt2htm::pltxt2advanced_html(text, u8"localhost:5173");
    auto answer = ::fast_io::u8string_view{
        u8R"(<br>文本1<br>文本2<br>文本3<br>文本4&lt;br&nbsp;&nbsp;&nbsp;&nbsp;/<br>文本5&lt;Br<br><br>)"};
    ::exception::assert_true(html == answer);

    auto inlined_html = ::pltxt2htm::pltxt2common_html(text);
    auto answer2 = ::fast_io::u8string_view{u8R"(文本1文本2文本3文本4&lt;br&nbsp;&nbsp;&nbsp;&nbsp;/文本5&lt;Br)"};
    ::exception::assert_true(inlined_html == answer2);

    return 0;
}