#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>text</a>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8R"(
<A      >text
</Discussion   >)");
        auto answer = ::fast_io::u8string_view{
            u8"<br><span style=\"color:#0000AA;\">text<br>&lt;/Discussion&nbsp;&nbsp;&nbsp;&gt;</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<A      >text</a   >");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>text</a><color=red>text</color>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"color:#0000AA;\">text</span><span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<A><a>text</a></A>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<A><color=red>text</color></A>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><a>text</a></color>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=#0000AA>t<a>ex</a>t</color>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>t<color=#0000AA>ex</color>t</a>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>t<color=red>ex</color>t</a>");
        auto answer =
            ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">t<span style=\"color:red;\">ex</span>t</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<A>t<a>ex</a>t</A>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<a  $");
        auto answer = ::fast_io::u8string_view{u8"test&lt;a&nbsp;&nbsp;$"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<a>text");
        auto answer = ::fast_io::u8string_view{u8"test<span style=\"color:#0000AA;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<a></a>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<a>es</color>t");
        auto answer = ::fast_io::u8string_view{u8"t<span style=\"color:#0000AA;\">es</span>t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a><i>test</i></a>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\"><em>test</em></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
