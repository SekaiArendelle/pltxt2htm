#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\t");
        auto answer = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\t");
        auto answer = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
