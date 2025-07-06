#include <cstring>
#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"<a>text</a>", u8"localhost:5173");
    auto answer1 = u8"<span style=\"color:#0000AA;\">text</span>";
    ::exception::assert_true(::std::memcmp(html1.data(), answer1, html1.size()) == 0);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(
        u8R"(
<A      >text
</color   >)",
        u8"localhost:5173");
    auto answer2 = u8"<br><span style=\"color:#0000AA;\">text<br>&lt;/color&nbsp;&nbsp;&nbsp;&gt;</span>";
    ::exception::assert_true(::std::memcmp(html2.data(), answer2, html2.size()) == 0);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(u8"<A      >text</a   >", u8"localhost:5173");
    auto answer3 = u8"<span style=\"color:#0000AA;\">text</span>";
    ::exception::assert_true(::std::memcmp(html3.data(), answer3, html3.size()) == 0);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(u8"<a>text</a><color=red>text</color>", u8"localhost:5173");
    auto answer4 = u8"<span style=\"color:#0000AA;\">text</span><span style=\"color:red;\">text</span>";
    ::exception::assert_true(::std::memcmp(html4.data(), answer4, html4.size()) == 0);

    auto html5 = ::pltxt2htm::pltxt2advanced_html(u8"<A><a>text</a></A>", u8"localhost:5173");
    auto answer5 = u8"<span style=\"color:#0000AA;\">text</span>";
    ::exception::assert_true(::std::memcmp(html5.data(), answer5, html5.size()) == 0);

    // test invalid tag
    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"test<a  $", u8"localhost:5173");
    auto answer6 = u8"test&lt;a&nbsp;&nbsp;$";
    ::exception::assert_true(::std::memcmp(html6.data(), answer6, html6.size()) == 0);

    auto html7 = ::pltxt2htm::pltxt2advanced_html(u8"test<a>text", u8"localhost:5173");
    auto answer7 = u8"test<span style=\"color:#0000AA;\">text</span>";
    ::exception::assert_true(::std::memcmp(html7.data(), answer7, html7.size()) == 0);

    return 0;
}
