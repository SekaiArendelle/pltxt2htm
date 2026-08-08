#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<P    >text</P  >"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p><color=red>text</color></p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<p style=\"text-align:left\"><span style=\"color:red;\">text</span></p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<color=red>text</color>"};
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
            ::fast_io::u8string_view{u8"<color=red>text<size=20>\uFF1C</size>/p<size=20>\uFF1E</size></color>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p>text<p>text</p></p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">text&lt;p&gt;text</p>&lt;/p&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"text<size=20>\uFF1C</size>p<size=20>\uFF1E</size>text\n<size=20>\uFF1C</size>/p<size=20>\uFF1E</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\"></p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8""};
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
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\ntext"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"text<br><p>text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"text<br><p style=\"text-align:left\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\ntext"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"text<br><p>text</p>"};
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"text<br><p style=\"text-align:left\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // newline separates two block-level <p> tags
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>a</p>\n<p>b</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<p style=\"text-align:left\">a</p><br><p style=\"text-align:left\">b</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"a\nb"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // a blank line between two <p> tags renders as two <br>
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>a</p>\n\n<p>b</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<p style=\"text-align:left\">a</p><br><br><p style=\"text-align:left\">b</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"a\n\nb"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // multiple blank lines render as multiple <br>
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>a</p>\n\n\n<p>b</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<p style=\"text-align:left\">a</p><br><br><br><p style=\"text-align:left\">b</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // trailing text after a closing </p> starts a new line
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>a</p>text\n<p>b</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<p style=\"text-align:left\">a</p>text<br><p style=\"text-align:left\">b</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"a\ntext\nb"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // a newline inside a <p> block renders as <br>
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>line1\nline2</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">line1<br>line2</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"line1\nline2"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // a blank line inside a <p> block renders as two <br>
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>line1\n\nline3</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">line1<br><br>line3</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"line1\n\nline3"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // text after a newline followed by a <p> block
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>a</p>\ntext\n<p>b</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<p style=\"text-align:left\">a</p><br>text<br><p style=\"text-align:left\">b</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a leading newline before a <p> block
    {
        auto pltext = ::fast_io::u8string_view{u8"\n<p>a</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<br><p style=\"text-align:left\">a</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a trailing newline after a closing </p>
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>a</p>\n"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">a</p><br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an unclosed <p> at a line start still forms a block containing the newline
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>a\n<p>b</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<p style=\"text-align:left\">a<br><p style=\"text-align:left\">b</p></p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"a\nb"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p>text</p>text"};
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">text</p>text"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\ntext"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">text</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<align=center>text</align>"};
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
