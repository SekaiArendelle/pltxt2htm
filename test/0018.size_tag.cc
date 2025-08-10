#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size=10>hello</size>");
    auto answer1 = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size=11>hello</size>");
    auto answer2 = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<SIZE=11   >hello</siZE  >");
    auto answer3 = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size=11>hello");
    auto answer4 = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"hello<size=11>");
    auto answer5 = ::fast_io::u8string_view{u8"hello"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"hello<size=9><size=11>world");
    auto answer6 = ::fast_io::u8string_view{u8"hello<span style=\"font-size:5px\">world</span>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size=9>hello<size=11>world</size></size>");
    auto answer7 = ::fast_io::u8string_view{
        u8"<span style=\"font-size:4px\">hello<span style=\"font-size:5px\">world</span></span>"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size=10>hello<size=10>world</size></size>");
    auto answer8 = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">helloworld</span>"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<size=10></size>t");
    auto answer9 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html9 == answer9);

    return 0;
}
