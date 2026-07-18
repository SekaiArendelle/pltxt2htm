#include "fast_io/fast_io_dsal/string_view.h"
#include "precompile.hh"

int main() {
    {
        ::fast_io::u8string input;
        for (::std::size_t i{}; i < 500; ++i) {
            input.append(u8"<color=red>");
        }
        input.append(u8"hello");
        for (::std::size_t i{}; i < 500; ++i) {
            input.append(u8"</color>");
        }
        auto html = ::pltxt2htm_test::pltxt4unittest(::fast_io::mnp::os_c_str(input));
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
