#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=99999999999999999999999999999999999999999>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:2435010336709935103px\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
