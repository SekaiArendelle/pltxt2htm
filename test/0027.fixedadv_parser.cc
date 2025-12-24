#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<a>text</a>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
