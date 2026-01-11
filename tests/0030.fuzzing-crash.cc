#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size=>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=&gt;text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<size= >text");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=&nbsp;&gt;text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
