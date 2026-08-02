#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub>text</sub>");
        auto answer = ::fast_io::u8string_view{u8"<sub>text</sub>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SUB    >text</SUB  >");
        auto answer = ::fast_io::u8string_view{u8"<sub>text</sub>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"H<sub>2</sub>O");
        auto answer = ::fast_io::u8string_view{u8"H<sub>2</sub>O"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub><color=red>text</color></sub>");
        auto answer = ::fast_io::u8string_view{u8"<sub><span style=\"color:red;\">text</span></sub>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub><color=red>text</sub></color>");
        auto answer = ::fast_io::u8string_view{u8"<sub><span style=\"color:red;\">text&lt;/sub&gt;</span></sub>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // nested <sub> must NOT be flattened: the inner text shifts the baseline further
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub>text<sub>text</sub></sub>");
        auto answer = ::fast_io::u8string_view{u8"<sub>text<sub>text</sub></sub>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub>text<sub>text</sub>text</sub>");
        auto answer = ::fast_io::u8string_view{u8"<sub>text<sub>text</sub>text</sub>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><sub>text</sub></b>");
        auto answer = ::fast_io::u8string_view{u8"<strong><sub>text</sub></strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub>");
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub");
        auto answer = ::fast_io::u8string_view{u8"&lt;sub"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<sub></sub>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // unclosed <sub> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub>text");
        auto answer = ::fast_io::u8string_view{u8"<sub>text</sub>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <sub> can nest with other formatting tags
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>t1<sub>t2</sub>t3</u>");
        auto answer = ::fast_io::u8string_view{u8"<u>t1<sub>t2</sub>t3</u>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"H<sub>2</sub>O");
        auto answer = ::fast_io::u8string_view{u8"H<sub>2</sub>O"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<sub>t1<sub>t2</sub></sub>");
        auto answer = ::fast_io::u8string_view{u8"<sub>t1<sub>t2</sub></sub>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
