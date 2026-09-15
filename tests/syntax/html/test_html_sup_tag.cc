#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup>text</sup>");
        auto answer = ::fast_io::u8string_view{u8"<sup>text</sup>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SUP    >text</SUP  >");
        auto answer = ::fast_io::u8string_view{u8"<sup>text</sup>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"x<sup>2</sup>+y<sup>2</sup>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"x<sup>2</sup>+y<sup>2</sup>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"x<sup>2</sup>+y<sup>2</sup>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup><color=red>text</color></sup>");
        auto answer = ::fast_io::u8string_view{u8"<sup><span style=\"color:red;\">text</span></sup>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup><color=red>text</sup></color>");
        auto answer = ::fast_io::u8string_view{u8"<sup><span style=\"color:red;\">text&lt;/sup&gt;</span></sup>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // nested <sup> must NOT be flattened: the inner text shifts the baseline further
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup>text<sup>text</sup></sup>");
        auto answer = ::fast_io::u8string_view{u8"<sup>text<sup>text</sup></sup>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup>text<sup>text</sup>text</sup>");
        auto answer = ::fast_io::u8string_view{u8"<sup>text<sup>text</sup>text</sup>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><sup>text</sup></b>");
        auto answer = ::fast_io::u8string_view{u8"<strong><sup>text</sup></strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup>");
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup");
        auto answer = ::fast_io::u8string_view{u8"&lt;sup"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<sup></sup>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // unclosed <sup> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup>text");
        auto answer = ::fast_io::u8string_view{u8"<sup>text</sup>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <sup> can nest with other formatting tags
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>t1<sup>t2</sup>t3</u>");
        auto answer = ::fast_io::u8string_view{u8"<u>t1<sup>t2</sup>t3</u>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<sup>t1<sup>t2</sup></sup>");
        auto answer = ::fast_io::u8string_view{u8"<sup>t1<sup>t2</sup></sup>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
