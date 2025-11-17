#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<p>text</p>");
        auto answer = ::fast_io::u8string_view{u8"<p>text</p>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<P    >text</P  >");
        auto answer = ::fast_io::u8string_view{u8"<p>text</p>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<p><color=red>text</color></p>");
        auto answer = ::fast_io::u8string_view{u8"<p><span style=\"color:red;\">text</span></p>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<p><color=red>text</p></color>");
        auto answer = ::fast_io::u8string_view{u8"<p><span style=\"color:red;\">text&lt;/p&gt;</span></p>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<p>text<p>text</p></p>");
        auto answer = ::fast_io::u8string_view{u8"<p>text<p>text</p></p>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<p>");
        auto answer = ::fast_io::u8string_view{u8"<p></p>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<p></p>t");
        auto answer = ::fast_io::u8string_view{u8"t<p></p>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<p></p");
        auto answer = ::fast_io::u8string_view{u8"t<p>&lt;/p</p>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
