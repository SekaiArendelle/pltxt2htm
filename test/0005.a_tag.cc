#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"<a>text</a>", u8"localhost:5173");
    auto answer1 = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(
        u8R"(
<A      >text
</color   >)",
        u8"localhost:5173");
    auto answer2 = ::fast_io::u8string_view{
        u8"<br><span style=\"color:#0000AA;\">text<br>&lt;/color&nbsp;&nbsp;&nbsp;&gt;</span>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(u8"<A      >text</a   >", u8"localhost:5173");
    auto answer3 = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(u8"<a>text</a><color=red>text</color>", u8"localhost:5173");
    auto answer4 =
        ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span><span style=\"color:red;\">text</span>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2advanced_html(u8"<A><a>text</a></A>", u8"localhost:5173");
    auto answer5 = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
    ::exception::assert_true(html5 == answer5);

    // test invalid tag
    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"test<a  $", u8"localhost:5173");
    auto answer6 = ::fast_io::u8string_view{u8"test&lt;a&nbsp;&nbsp;$"};
    ::exception::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm::pltxt2advanced_html(u8"test<a>text", u8"localhost:5173");
    auto answer7 = ::fast_io::u8string_view{u8"test<span style=\"color:#0000AA;\">text</span>"};
    ::exception::assert_true(html7 == answer7);

    return 0;
}
