#include <cstring>
#include <fast_io/fast_io.h>
#include <pltxt2htm/pltxt2htm.hh>
#include "exception/exception.hh"

int main() noexcept {
    auto html = pltxt2htm::pltxt2html(u8"< >");

    ::exception::assert_true(::std::memcmp(html.data(), u8"&lt;&nbsp;&gt;", html.size()) == 0);

    return 0;
}
