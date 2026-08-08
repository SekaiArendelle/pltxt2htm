#include "precompile.hh"

int main() {
    ::fast_io::u8string_view const text{
        u8R"(
文本1<hR   />文本2<hr/>文本3<hr   >文本4<HR    /
文本5<hr<hr>
)"};

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(text);
        auto answer = ::fast_io::u8string_view{
            u8R"(<br>文本1&lt;hR&nbsp;&nbsp;&nbsp;/&gt;文本2&lt;hr/&gt;文本3&lt;hr&nbsp;&nbsp;&nbsp;&gt;文本4&lt;HR&nbsp;&nbsp;&nbsp;&nbsp;/<br>文本5&lt;hr&lt;hr&gt;<br>)"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(text);
        auto answer = ::fast_io::u8string_view{
            u8R"(文本1&lt;hR&nbsp;&nbsp;&nbsp;/&gt;文本2&lt;hr/&gt;文本3&lt;hr&nbsp;&nbsp;&nbsp;&gt;文本4&lt;HR&nbsp;&nbsp;&nbsp;&nbsp;/文本5&lt;hr&lt;hr&gt;)"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<hr>"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"---\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<hr>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<hr>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<hr><h1>text</h1>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<hr><h1>text</h1>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}