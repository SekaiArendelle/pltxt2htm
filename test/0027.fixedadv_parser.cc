#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2fixedadv_html(u8"<a>text</a>", u8"localhost:5173");
    auto answer1 = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2fixedadv_html(u8"<a href=\"https://plweb.turtlesim.com\">text</a>", u8"localhost:5173");
    auto answer2 = ::fast_io::u8string_view{u8"<a&nbsp;href=&quot;https://plweb.turtlesim.com&quot;&gt;text</a&gt;"};
    ::exception::assert_true(html2 == answer2);

    return 0;
}