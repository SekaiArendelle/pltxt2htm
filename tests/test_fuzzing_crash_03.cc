#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&__</_");
        auto answer = ::fast_io::u8string_view{u8"&amp;_<em>&lt;/</em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
