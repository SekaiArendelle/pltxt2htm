#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6>text</h6>");
        auto answer = ::fast_io::u8string_view{u8"<h6>text</h6>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<H6    >text</H6  >");
        auto answer = ::fast_io::u8string_view{u8"<h6>text</h6>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6><color=red>text</color></h6>");
        auto answer = ::fast_io::u8string_view{u8"<h6><span style=\"color:red;\">text</span></h6>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6><color=red>text</h6></color>");
        auto answer = ::fast_io::u8string_view{u8"<h6><span style=\"color:red;\">text&lt;/h6&gt;</span></h6>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6>text<h6>text</h6></h6>");
        auto answer = ::fast_io::u8string_view{u8"<h6>text<h6>text</h6></h6>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6>");
        auto answer = ::fast_io::u8string_view{u8"<h6></h6>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6>text</h5>text</h6></h6>");
        auto answer = ::fast_io::u8string_view{u8"<h6>text&lt;/h5&gt;text</h6>&lt;/h6&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
