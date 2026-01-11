#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size=10>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size=11>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<SIZE=11   >hello</siZE  >");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size=11>hello");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"hello<size=11>");
        auto answer = ::fast_io::u8string_view{u8"hello"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"hello<size=9><size=11>world");
        auto answer = ::fast_io::u8string_view{u8"hello<span style=\"font-size:5px\">world</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size=9>hello<size=11>world</size></size>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"font-size:4px\">hello<span style=\"font-size:5px\">world</span></span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size=10>hello<size=10>world</size></size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">helloworld</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<size=10></size>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
