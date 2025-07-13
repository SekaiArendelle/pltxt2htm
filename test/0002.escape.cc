#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"< >&\'\"", u8"localhost:5173");
    ::exception::assert_true(html1 == u8"&lt;&nbsp;&gt;&amp;&apos;&quot;");

    auto html2 = ::pltxt2htm::pltxt2advanced_html(u8"", u8"_");
    ::exception::assert_true(html2 == u8"");

    auto html3 = ::pltxt2htm::pltxt2common_html(u8"< >&\'\"");
    ::exception::assert_true(html3 == u8"&lt;&nbsp;&gt;&amp;&apos;&quot;");

    return 0;
}
