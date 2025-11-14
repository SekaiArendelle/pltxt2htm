#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"&__</_");
    auto answer1 = ::fast_io::u8string_view{u8"&amp;_<em>&lt;/</em>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    return 0;
}
