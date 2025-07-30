#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2common_html(u8"\t");
    auto answer1 = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(u8"\t", u8"_");
    auto answer2 = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;"};
    ::exception::assert_true(html2 == answer2);

    return 0;
}
