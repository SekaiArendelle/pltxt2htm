#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size= >text");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=&nbsp;&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
