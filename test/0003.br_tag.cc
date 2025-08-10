#include "precompile.hh"

int main() {
    ::fast_io::u8string_view text{
        u8R"(
文本1<br   />文本2<bR/>文本3<br   >文本4<br    /
文本5<Br<br>
)"};

    auto html = ::pltxt2htm_test::pltxt2advanced_htmld(text);
    auto answer = ::fast_io::u8string_view{
        u8R"(<br>文本1<br>文本2<br>文本3<br>文本4&lt;br&nbsp;&nbsp;&nbsp;&nbsp;/<br>文本5&lt;Br<br><br>)"};
    ::pltxt2htm_test::assert_true(html == answer);

    auto inlined_html = ::pltxt2htm_test::pltxt2common_htmld(text);
    auto answer2 = ::fast_io::u8string_view{u8R"(文本1文本2文本3文本4&lt;br&nbsp;&nbsp;&nbsp;&nbsp;/文本5&lt;Br)"};
    ::pltxt2htm_test::assert_true(inlined_html == answer2);

    return 0;
}
