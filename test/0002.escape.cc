#include <cstring>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html = ::pltxt2htm::pltxt2advanced_html(u8"< >&\'\"", u8"localhost:5173");

    ::exception::assert_true(::std::memcmp(html.data(), u8"&lt;&nbsp;&gt;&amp;&apos;&quot;", html.size()) == 0);

    return 0;
}
