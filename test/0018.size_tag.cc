#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"<size=10>hello</size>", u8"localhost");
    auto answer1 = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(u8"<size=11>hello</size>", u8"localhost");
    auto answer2 = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(u8"<SIZE=11   >hello</siZE  >", u8"localhost");
    auto answer3 = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(u8"<size=11>hello", u8"localhost");
    auto answer4 = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">hello</span>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2advanced_html(u8"hello<size=11>", u8"localhost");
    auto answer5 = ::fast_io::u8string_view{u8"hello"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"hello<size=9><size=11>world", u8"localhost");
    auto answer6 = ::fast_io::u8string_view{u8"hello<span style=\"font-size:5px\">world</span>"};
    ::exception::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm::pltxt2advanced_html(u8"<size=9>hello<size=11>world</size></size>", u8"localhost");
    auto answer7 = ::fast_io::u8string_view{
        u8"<span style=\"font-size:4px\">hello<span style=\"font-size:5px\">world</span></span>"};
    ::exception::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm::pltxt2advanced_html(u8"<size=10>hello<size=10>world</size></size>", u8"localhost");
    auto answer8 = ::fast_io::u8string_view{u8"<span style=\"font-size:5px\">helloworld</span>"};
    ::exception::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm::pltxt2advanced_html(u8"t<size=10></size>t", u8"localhost:5173");
    auto answer9 = ::fast_io::u8string_view{u8"tt"};
    ::exception::assert_true(html9 == answer9);

    return 0;
}
