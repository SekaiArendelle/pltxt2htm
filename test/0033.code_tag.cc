#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code>text</code>");
    auto answer1 = ::fast_io::u8string_view{u8"<code>text</code>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<CODE    >text</CODE  >");
    auto answer2 = ::fast_io::u8string_view{u8"<code>text</code>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code><color=red>text</color></code>");
    auto answer3 = ::fast_io::u8string_view{u8"<code><span style=\"color:red;\">text</span></code>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code><color=red>text</code></color>");
    auto answer4 = ::fast_io::u8string_view{u8"<code><span style=\"color:red;\">text&lt;/code&gt;</span></code>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code>text<code>text</code></code>");
    auto answer5 = ::fast_io::u8string_view{u8"<code>texttext</code>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code>");
    auto answer6 = ::fast_io::u8string_view{u8"<code></code>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code");
    auto answer7 = ::fast_io::u8string_view{u8"&lt;code"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<code></code>t");
    auto answer8 = ::fast_io::u8string_view{u8"t<code></code>t"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    return 0;
}