#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"> [a](b)");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><a href=\"b\">a</a></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }
}
