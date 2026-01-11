#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ul><li>item1</li><li>item2</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>item1</li><li>item2</li></ul>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<UL    ><LI>item1</LI><LI>item2</LI></UL  >");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>item1</li><li>item2</li></ul>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ul><li><color=red>item1</color></li><li>item2</li></ul>");
        auto answer =
            ::fast_io::u8string_view{u8"<ul><li><span style=\"color:red;\">item1</span></li><li>item2</li></ul>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ul><li><color=red>item1</li></ul></color>");
        auto answer = ::fast_io::u8string_view{
            u8"<ul><li><span style=\"color:red;\">item1&lt;/li&gt;&lt;/ul&gt;</span></li></ul>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ul><li>item1<ul><li>item2</li></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>item1<ul><li>item2</li></ul></li></ul>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul></ul>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<ul></ul>t");
        auto answer = ::fast_io::u8string_view{u8"t<ul></ul>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<li");
        auto answer = ::fast_io::u8string_view{u8"t&lt;li"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
