#include "fast_io/fast_io_dsal/string_view.h"
#include "precompile.hh"

int main() {
    {
        auto text = ::pltxt2htm_test::to_plunity_introduction(u8"text");
        auto answer = ::fast_io::u8string_view{u8"text"};
        ::pltxt2htm_test::assert_true(text == answer);
    }

    return 0;
}
