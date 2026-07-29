#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text</s>");
        auto answer = ::fast_io::u8string_view{u8"<s>text</s>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<S    >text</S  >");
        auto answer = ::fast_io::u8string_view{u8"<s>text</s>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s><color=red>text</color></s>");
        auto answer = ::fast_io::u8string_view{u8"<s><span style=\"color:red;\">text</span></s>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s><color=red>text</s></color>");
        auto answer = ::fast_io::u8string_view{u8"<s><span style=\"color:red;\">text&lt;/s&gt;</span></s>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text<s>text</s></s>");
        auto answer = ::fast_io::u8string_view{u8"<s>texttext</s>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><s>text</s></b>");
        auto answer = ::fast_io::u8string_view{u8"<strong><s>text</s></strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>");
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s");
        auto answer = ::fast_io::u8string_view{u8"&lt;s"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<s></s>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // unclosed <s> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text");
        auto answer = ::fast_io::u8string_view{u8"<s>text</s>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <strong>, <span>, <size> must not be parsed as <s>
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong>t</strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong>t</strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=1>t</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:1px;\">t</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // strikethrough tags from different syntaxes can nest
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>t1<del>t2</del>t3</s>");
        auto answer = ::fast_io::u8string_view{u8"<s>t1<del>t2</del>t3</s>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~t1<s>t2</s>t3~~");
        auto answer = ::fast_io::u8string_view{u8"<del>t1<s>t2</s>t3</del>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
