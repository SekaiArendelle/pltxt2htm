#include <fast_io/fast_io_dsal/array.h>
#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text");
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text \ntext ");
        auto answer = ::fast_io::u8string_view{u8"text\ntext"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
