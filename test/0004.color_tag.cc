#include <cstring>
#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2html(u8"<cOLOr=red>text</color>", u8"localhost:5173");
    auto answer1 = u8"<span style=\"color:red;\">text</span>";
    ::exception::assert_true(::std::memcmp(html1.data(), answer1, html1.size()) == 0);

    auto html2 = ::pltxt2htm::pltxt2html(u8"<color=#66CcFf      >text</color   >", u8"localhost:5173");
    auto answer2 = u8"<span style=\"color:#66CcFf;\">text</span>";
    ::exception::assert_true(::std::memcmp(html2.data(), answer2, html2.size()) == 0);

    auto html3 = ::pltxt2htm::pltxt2html(
        u8R"(
<color=#66CcFf>te
 xt</color>
)",
        u8"localhost:5173");
    auto answer3 = u8"<br><span style=\"color:#66CcFf;\">te<br>&nbsp;xt</span><br>";
    ::exception::assert_true(::std::memcmp(html3.data(), answer3, html3.size()) == 0);

    auto html4 = ::pltxt2htm::pltxt2html(u8"<color=red><Color=red>text</color></color>", u8"localhost:5173");
    auto answer4 = u8"<span style=\"color:red;\">text</span>";
    ::exception::assert_true(::std::memcmp(html4.data(), answer4, html4.size()) == 0);

    auto html5 = ::pltxt2htm::pltxt2html(u8"<color=red><Color=#66CcFf>text</color></color>", u8"localhost:5173");
    auto answer5 = u8"<span style=\"color:red;\"><span style=\"color:#66CcFf;\">text</span></span>";
    ::exception::assert_true(::std::memcmp(html5.data(), answer5, html5.size()) == 0);

    auto html6 = ::pltxt2htm::pltxt2html(u8"test<Color=#66CcFf>", u8"localhost:5173");
    auto answer6 = u8"test";
    ::exception::assert_true(::std::memcmp(html6.data(), answer6, html6.size()) == 0);

    auto html7 = ::pltxt2htm::pltxt2html(u8"test<Color=#66CcFf>text", u8"localhost:5173");
    auto answer7 = u8"test<span style=\"color:#66CcFf;\">text</span>";
    // ::fast_io::println(::fast_io::u8c_stdout(), html7);
    ::exception::assert_true(::std::memcmp(html7.data(), answer7, html7.size()) == 0);

    // test invalid tag
    auto html8 = ::pltxt2htm::pltxt2html(u8"test<Color=#66CcFf  $", u8"localhost:5173");
    auto answer8 = u8"test&lt;Color=#66CcFf&nbsp;&nbsp;$";
    ::exception::assert_true(::std::memcmp(html8.data(), answer8, html8.size()) == 0);

    auto html9 = ::pltxt2htm::pltxt2html(u8"<color=red>text<Color=red>text</color></color>", u8"localhost:5173");
    auto answer9 = u8"<span style=\"color:red;\">texttext</span>";
    ::exception::assert_true(::std::memcmp(html9.data(), answer9, html4.size()) == 0);

    auto html10 = ::pltxt2htm::pltxt2html(u8"<color=red>text<Color=#66ccff>text</color></color>", u8"localhost:5173");
    auto answer10 = u8"<span style=\"color:red;\">text<span style=\"color:#66ccff;\">text</span></span>";
    ::exception::assert_true(::std::memcmp(html10.data(), answer10, html4.size()) == 0);

    return 0;
}
