#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~~a~~a ");
        auto answer = ::fast_io::u8string_view{u8"<del>~a</del>a"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
