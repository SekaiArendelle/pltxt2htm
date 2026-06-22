#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20px\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:20px;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:blue;font-size:16px\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:blue;font-size:16px;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // uppercase STYLE attribute is rejected (only lowercase "style" is allowed)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span STYLE=\"color:red\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;STYLE=&quot;color:red&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // uppercase COLOR property is rejected (only lowercase "color" is allowed)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"COLOR:red\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;COLOR:red&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span  style=\"color:red\"  >text</span  >");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red !important\">text</span>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;span&nbsp;style=&quot;color:red&nbsp;!important&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:#FF0000\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#FF0000;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:20px;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red;\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span class=\"foo\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;class=&quot;foo&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"background:red\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;background:red&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:#GGG\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;color:#GGG&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:1em\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;font-size:1em&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red;color:blue\">text</span>");
        auto answer =
            ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;color:red;color:blue&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20px;font-size:30px\">text</span>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;span&nbsp;style=&quot;font-size:20px;font-size:30px&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<span style=\"color:red\"></span>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red\"><span style=\"color:red\">text</span></span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\">a<span style=\"color:red\">b</span>c</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">abc</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red\"><span style=\"color:blue\">text</span></span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:blue;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\">a<span style=\"color:blue\">b</span></span>");
        auto answer =
            ::fast_io::u8string_view{u8"<span style=\"color:red;\">a<span style=\"color:blue;\">b</span></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong><span style=\"color:red\">text</span></strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong><span style=\"color:red;\">text</span></strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a><span style=\"color:red\">text</span></a>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=green><span style=\"color:red\">text</span></color>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>a<span style=\"color:red\">b</span>c</color>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">abc</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:#0000AA;\">a<a>b</a>c</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">abc</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"color:#0000AA;\">b</span>c</a>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">abc</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"color:red;\">b</span>c</a>");
        auto answer =
            ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">a<span style=\"color:red;\">b</span>c</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"><a>text</a></span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"><color=blue>text</color></span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:blue;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // font-size matched nesting: should flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"font-size:20px\"><span style=\"font-size:20px\">text</span></span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:20px;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // color+font-size matched nesting: should flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red;font-size:20px\"><span style=\"color:red;font-size:20px\">text</span></span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;font-size:20px;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // color+font-size, font-size different: should NOT flatten, inner wins
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red;font-size:20px\"><span style=\"color:red;font-size:16px\">text</span></span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;font-size:16px;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // color=red wrapping font-size-only span with siblings: span preserved (no color to match)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>a<span style=\"font-size:20px\">b</span>c</color>");
        auto answer =
            ::fast_io::u8string_view{u8"<span style=\"color:red;\">a<span style=\"font-size:20px;\">b</span>c</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // color=red wrapping span with matching color AND font-size: should NOT flatten
        // (flattening would lose the font-size)
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<color=red>a<span style=\"color:red;font-size:20px\">b</span>c</color>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"color:red;\">a<span style=\"color:red;font-size:20px;\">b</span>c</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // anchor wrapping font-size-only span with siblings: span preserved (no color to match)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"font-size:20px\">b</span>c</a>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"color:#0000AA;\">a<span style=\"font-size:20px;\">b</span>c</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // non-overlapping nested attrs: merge outer font-size with inner color
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"font-size:20px\"><span style=\"color:red\">text</span></span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;font-size:20px;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // triple nested same color: should flatten to single span
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red\"><span style=\"color:red\"><span "
            u8"style=\"color:red\">text</span></span></span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // empty same-color nested span: should remove both (nothing left)
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"><span style=\"color:red\"></span></span>");
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // anchor wrapping span with matching color AND font-size: should NOT flatten
        // (flattening would lose the font-size)
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"color:#0000AA;font-size:20px\">b</span>c</a>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"color:#0000AA;\">a<span style=\"color:#0000AA;font-size:20px;\">b</span>c</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\">text");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;color:red&quot;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // overflow font-size: span tag rejected, treated as literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:99999999999999999999px\">text</span>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;span&nbsp;style=&quot;font-size:99999999999999999999px&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // single-quoted style attribute
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style='color:red'>text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // font-size:0 rejected (zero is not a valid font-size)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:0\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;font-size:0&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // empty style value rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // uppercase px unit rejected (lowercase "px" only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20PX\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;font-size:20PX&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // uppercase color name rejected (lowercase only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:Red\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;color:Red&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // uppercase color name rejected (lowercase only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:BLUE\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;color:BLUE&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // negative font-size rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:-20px\">text</span>");
        auto answer =
            ::fast_io::u8string_view{u8"&lt;span&nbsp;style=&quot;font-size:-20px&quot;&gt;text&lt;/span&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // maximum valid font-size value (boundary of overflow detection)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:18446744073709551614px\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:18446744073709551614px;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<span style=\"color:red\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<color=red>text</color>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<span style=\"font-size:20px\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<size=20>text</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<span style=\"color:blue;font-size:16px\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<color=blue><size=16>text</size></color>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
