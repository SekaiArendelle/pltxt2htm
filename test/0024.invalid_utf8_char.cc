#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2common_html(u8"\xE2\x82");
    auto answer1 = ::fast_io::u8string_view{u8"�"};
    ::exception::assert_true(html1 == answer1);

    return 0;
}
