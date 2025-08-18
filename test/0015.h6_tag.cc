#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6>text</h6>");
    auto answer1 = ::fast_io::u8string_view{u8"<h6>text</h6>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<H6    >text</H6  >");
    auto answer2 = ::fast_io::u8string_view{u8"<h6>text</h6>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6><color=red>text</color></h6>");
    auto answer3 = ::fast_io::u8string_view{u8"<h6><span style=\"color:red;\">text</span></h6>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6><color=red>text</h6></color>");
    auto answer4 = ::fast_io::u8string_view{u8"<h6><span style=\"color:red;\">text&lt;/h6&gt;</span></h6>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6>text<h6>text</h6></h6>");
    auto answer5 = ::fast_io::u8string_view{u8"<h6>text<h6>text</h6></h6>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6>");
    auto answer6 = ::fast_io::u8string_view{u8"<h6></h6>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h6>text</h5>text</h6></h6>");
    auto answer7 = ::fast_io::u8string_view{u8"<h6>text&lt;/h5&gt;text</h6>&lt;/h6&gt;"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    return 0;
}
