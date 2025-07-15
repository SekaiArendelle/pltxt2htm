#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    ::fast_io::u8string_view text{
        u8R"(
文本1<hR   />文本2<hr/>文本3<hr   >文本4<HR    /
文本5<hr<hr>
)"};

    auto html = ::pltxt2htm::pltxt2advanced_html(text, u8"localhost:5173");
    auto answer = ::fast_io::u8string_view{
        u8R"(<br>文本1<hr>文本2<hr>文本3<hr>文本4&lt;HR&nbsp;&nbsp;&nbsp;&nbsp;/<br>文本5&lt;hr<hr><br>)"};
    ::exception::assert_true(html == answer);

    auto inlined_html = ::pltxt2htm::pltxt2common_html(text);
    auto answer2 = ::fast_io::u8string_view{u8R"(文本1文本2文本3文本4&lt;HR&nbsp;&nbsp;&nbsp;&nbsp;/文本5&lt;hr)"};
    ::exception::assert_true(inlined_html == answer2);

    return 0;
}