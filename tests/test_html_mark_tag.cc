#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"<mark>text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:#FFFF00;\">text</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=#FFFF00>text</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<MARK    >text</Mark  >"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:#FFFF00;\">text</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=#FFFF00>text</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<mark><color=red>text</color></mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<mark style=\"background-color:#FFFF00;\"><span style=\"color:red;\">text</span></mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=#FFFF00><color=red>text</color></mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<mark><color=red>text</mark></color>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<mark style=\"background-color:#FFFF00;\"><span style=\"color:red;\">text&lt;/mark&gt;</span></mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<mark=#FFFF00><color=red>text<size=20>\uFF1C</size>/mark<size=20>\uFF1E</size></color></mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<Mark>text<mark>text</mark></Mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:#FFFF00;\">texttext</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=#FFFF00>texttext</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"text<mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"t<mark></mark>t"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<mark></mark"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:#FFFF00;\">&lt;/mark</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=#FFFF00><size=20>\uFF1C</size>/mark</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<mark style=\"background-color:red\">text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:red;\">text</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=red>text</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<mark style=\"background-color:#FF0000\">text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:#FF0000;\">text</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=#FF0000>text</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<mark  style=\"background-color:red\"  >text</mark  >"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:red;\">text</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=red>text</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext =
            ::fast_io::u8string_view{u8"<mark style=\"background-color:yellow\">a<mark style=\"background-color:yellow\">b</mark>c</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:yellow;\">abc</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=yellow>abc</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext =
            ::fast_io::u8string_view{u8"<mark style=\"background-color:red\">a<mark style=\"background-color:blue\">b</mark>c</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<mark style=\"background-color:red;\">a<mark style=\"background-color:blue;\">b</mark>c</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=red>a<mark=blue>b</mark>c</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        // XSS: other CSS properties are rejected and the tag degrades to literal text
        auto pltext = ::fast_io::u8string_view{u8"<mark style=\"color:red\">text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;mark&nbsp;style=&quot;color:red&quot;&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: uppercase STYLE attribute is rejected
        auto pltext = ::fast_io::u8string_view{u8"<mark STYLE=\"background-color:red\">text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;mark&nbsp;STYLE=&quot;background-color:red&quot;&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: uppercase CSS property is rejected
        auto pltext = ::fast_io::u8string_view{u8"<mark style=\"BACKGROUND-COLOR:red\">text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"&lt;mark&nbsp;style=&quot;BACKGROUND-COLOR:red&quot;&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: extra CSS property after background-color is rejected
        auto pltext = ::fast_io::u8string_view{u8"<mark style=\"background-color:red;color:blue\">text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"&lt;mark&nbsp;style=&quot;background-color:red;color:blue&quot;&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: duplicate background-color property is rejected
        auto pltext =
            ::fast_io::u8string_view{u8"<mark style=\"background-color:red;background-color:blue\">text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"&lt;mark&nbsp;style=&quot;background-color:red;background-color:blue&quot;&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: url(...) color value is rejected
        auto pltext = ::fast_io::u8string_view{u8"<mark style=\"background-color:url(javascript:alert(1))\">text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"&lt;mark&nbsp;style=&quot;background-color:url(javascript:alert(1))&quot;&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: invalid hex color value is rejected
        auto pltext = ::fast_io::u8string_view{u8"<mark style=\"background-color:#GGG\">text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;mark&nbsp;style=&quot;background-color:#GGG&quot;&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: unknown attribute is rejected
        auto pltext = ::fast_io::u8string_view{u8"<mark class=\"foo\">text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;mark&nbsp;class=&quot;foo&quot;&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"t<mark style=\"background-color:red\"></mark>t"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<mark=red>text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:red;\">text</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=red>text</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<mark=#FF0000>text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:#FF0000;\">text</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=#FF0000>text</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<MARK=red>text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:red;\">text</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=red>text</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<mark=red >text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:red;\">text</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=red>text</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext =
            ::fast_io::u8string_view{u8"<mark=yellow>a<mark=yellow>b</mark>c</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<mark style=\"background-color:yellow;\">abc</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=yellow>abc</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<mark=red>a<mark=blue>b</mark>c</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<mark style=\"background-color:red;\">a<mark style=\"background-color:blue;\">b</mark>c</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<mark=red>a<mark=blue>b</mark>c</mark>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"t<mark=red></mark>t"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        // title backend mirrors html_mark title behavior
        auto pltext = ::fast_io::u8string_view{u8"<mark=red>text</mark>"};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: empty value is rejected and the tag degrades to escaped literal text
        auto pltext = ::fast_io::u8string_view{u8"<mark=>text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;mark=&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: numeric value is rejected
        auto pltext = ::fast_io::u8string_view{u8"<mark=123>text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;mark=123&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: extra CSS after the color value is rejected
        auto pltext = ::fast_io::u8string_view{u8"<mark=red;color:blue>text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;mark=red;color:blue&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: url(...) color value is rejected
        auto pltext = ::fast_io::u8string_view{u8"<mark=url(javascript:alert(1))>text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;mark=url(javascript:alert(1))&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: invalid hex color value is rejected
        auto pltext = ::fast_io::u8string_view{u8"<mark=#GGG>text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;mark=#GGG&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // XSS: extra attribute after the color value is rejected
        auto pltext = ::fast_io::u8string_view{u8"<mark=red class=\"x\">text</mark>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;mark=red&nbsp;class=&quot;x&quot;&gt;text&lt;/mark&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
