#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<p>text</p>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<P    >text</P  >"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<p>text</p>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p><color=red>text</color></p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<p style=\"text-align:left\"><span style=\"color:red;\">text</span></p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<p><color=red>text</color></p>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p><color=red>text</p></color>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<p style=\"text-align:left\"><span style=\"color:red;\">text&lt;/p&gt;</span></p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<p><color=red>text<size=20>\uFF1C</size>/p<size=20>\uFF1E</size></color></p>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p>text<p>text</p></p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">text&lt;p&gt;text</p>&lt;/p&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<p>text<size=20>\uFF1C</size>p<size=20>\uFF1E</size>text</p><size=20>\uFF1C</size>/p<size=20>\uFF1E</"
            u8"size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\"></p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<p></p>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"t<p></p>t"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"t&lt;p&gt;&lt;/p&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"t<size=20>\uFF1C</size>p<size=20>\uFF1E</size><size=20>\uFF1C</size>/p<size=20>\uFF1E</size>t"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"t<p></p"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"t&lt;p&gt;&lt;/p"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"t<size=20>\uFF1C</size>p<size=20>\uFF1E</size><size=20>\uFF1C</size>/p"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"text\n<p>text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"text<br><p style=\"text-align:left\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\n<p>text</p>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"text<br><p>text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"text<br><p style=\"text-align:left\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\n<p>text</p>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"text<br><p>text</p>"};
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"text<br><p style=\"text-align:left\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p>text</p>text"};
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">text</p>text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">text</p>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p style=\"text-align:right\">text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:right\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p style=\"text-align:justify\">text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:justify\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // a style with an extra CSS property is not allowed; the tag is rejected
        auto pltext = ::fast_io::u8string_view{u8"<p style=\"text-align:center;color:red\">text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"&lt;p&nbsp;style=&quot;text-align:center;color:red&quot;&gt;text&lt;/p&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an extra non-style attribute is not allowed; the tag is rejected
    {
        auto pltext = ::fast_io::u8string_view{u8"<p id=\"x\" style=\"text-align:center\">text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"&lt;p&nbsp;id=&quot;x&quot;&nbsp;style=&quot;text-align:center&quot;&gt;text&lt;/p&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a non-text-align style is rejected
    {
        auto pltext = ::fast_io::u8string_view{u8"<p style=\"color:red\">text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;p&nbsp;style=&quot;color:red&quot;&gt;text&lt;/p&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // left is the default, so no style attribute is emitted
    {
        auto pltext = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
