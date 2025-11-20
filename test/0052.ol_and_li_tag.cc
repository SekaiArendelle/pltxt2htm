#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ol><li>item1</li><li>item2</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol><li>item1</li><li>item2</li></ol>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<OL    ><LI>item1</LI><LI>item2</LI></OL  >");
        auto answer = ::fast_io::u8string_view{u8"<ol><li>item1</li><li>item2</li></ol>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ol><li><color=red>item1</color></li><li>item2</li></ol>");
        auto answer =
            ::fast_io::u8string_view{u8"<ol><li><span style=\"color:red;\">item1</span></li><li>item2</li></ol>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ol><li><color=red>item1</li></ol></color>");
        auto answer = ::fast_io::u8string_view{
            u8"<ol><li><span style=\"color:red;\">item1&lt;/li&gt;&lt;/ol&gt;</span></li></ol>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ol><li>item1<ol><li>item2</li></ol></li></ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol><li>item1<ol><li>item2</li></ol></li></ol>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol></ol>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<ol></ol>t");
        auto answer = ::fast_io::u8string_view{u8"t<ol></ol>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<li");
        auto answer = ::fast_io::u8string_view{u8"t&lt;li"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
