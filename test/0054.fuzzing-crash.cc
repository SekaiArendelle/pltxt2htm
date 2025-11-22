#include <fast_io/fast_io_dsal/array.h>
#include "precompile.hh"

int main() {
    {
        ::fast_io::array data{char8_t(0x30), 0x33, 0x35, 0x35};
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(::fast_io::u8string_view(data.data(), data.size()));
    }
}
