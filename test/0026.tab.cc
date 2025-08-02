#include "precompile.hh"

int main() noexcept {
    auto html1 = ::pltxt2htm_test::pltxt2common_htmld(u8"\t");
    auto answer1 = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\t");
    auto answer2 = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    return 0;
}
