#include "precompile.hh"

int main() {
    // web backend renders <div style="margin-left:..."> as a block-level div with CSS margins
    {
        auto pltext = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em\">text</div>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em>text</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<div style=\"margin-right:1em\">text</div>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-right:1em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin right=1em>text</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;margin-right:3em\">text</div>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;margin-right:3em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em right=3em>text</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // px is emitted explicitly with a px suffix in the web backend
    {
        auto pltext = ::fast_io::u8string_view{u8"<div style=\"margin-left:10px\">text</div>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:10px;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=10>text</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // percent unit is preserved
    {
        auto pltext = ::fast_io::u8string_view{u8"<div style=\"margin-left:5%\">text</div>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:5%;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=5%>text</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // an empty div is kept, not erased by the optimizer
    {
        auto pltext = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em\"></div>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\"></div>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em></margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // an unclosed div still parses
    {
        auto pltext = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em\">text"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em>text</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // a newline inside a div still renders as <br>
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em\">line1\nline2</div>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">line1<br>line2</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // text after the closing </div> stays on its own content line
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em\">a</div>\nb");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">a</div><br>b"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // text before and after a block-level div
    {
        auto pltext = ::fast_io::u8string_view{u8"a\n<div style=\"margin-left:2em\">b</div>\nc"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"a<br><div style=\"margin-left:2em;\">b</div><br>c"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // mid-line <div> sequences are literal text (block-level only)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<div style=\"margin-left:2em\">x</div>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;div&nbsp;style=&quot;margin-left:2em&quot;&gt;x&lt;/div&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // non-numeric margin value renders as literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:abc\">x</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;margin-left:abc&quot;&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an unknown style declaration (e.g. color) makes the whole tag literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"color:red\">x</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;color:red&quot;&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a style mix with a non-margin declaration is rejected entirely
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em;color:red\">x</div>");
        auto answer =
            ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;margin-left:2em;color:red&quot;&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a div without the style attribute is literal text (not parsed as a margin block)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div>plain</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&gt;plain&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a script tag is not an allowed attribute and renders as literal text (XSS guard)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div onclick=\"alert(1)\">x</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&nbsp;onclick=&quot;alert(1)&quot;&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // malformed quote (unterminated style value) renders as literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em>x</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;margin-left:2em&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a missing ':' separator renders as literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left 2em\">x</div>");
        auto answer =
            ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;margin-left&nbsp;2em&quot;&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // plunity backend maps the div back to a TMP margin tag (see the blocks above)

    // nested divs are handled and map to nested margin tags
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<div style=\"margin-left:2em\"><div style=\"margin-right:1em\">x</div></div>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em><margin right=1em>x</margin>\n</margin>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // the title backend (pltxt2common_html) renders the block-level div literally
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<div style=\"margin-left:2em\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;margin-left:2em&quot;&gt;text&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // regression: roundtrip fuzzer crash (fixedadv_roundtrip, crash-6250118fd3e7f955112700997404af6e9d791b89).
    // The web backend emits a pl_margin block as <div style="margin-left:...;margin-right:...">, but the
    // experimental HTML parser (parse_pltxt_html) used by the second roundtrip pass does not understand <div>,
    // so on the second pass the emitted <div> is re-parsed as literal text and escaped. The two passes diverge:
    //     first pass : x<br><div style="margin-left:2px;margin-right:2px;"></div>
    //     second pass: x<br>&lt;div&nbsp;style=&quot;margin-left:2px;margin-right:2px;&quot;&gt;&lt;/div&gt;
    // The roundtrip is therefore not idempotent. The test below asserts the idempotency the fuzzer checks and
    // currently reproduces the crash; it should pass once the HTML parser learns <div>.
    {
        auto pltext = ::fast_io::u8string_view{u8"x\n<MARgin=2>"};
        auto once = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto twice = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::mnp::os_c_str(once));
        pltxt2htm_test_assert_equal(twice, once);
    }
    // the exact fuzzer input (span-wrapped margin block), without the libFuzzer trailing-junk bytes
    {
        auto pltext = ::fast_io::u8string_view{u8",><sIzE=4>[8t<sIzE\n<MARgin=2>"};
        auto once = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto twice = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::mnp::os_c_str(once));
        pltxt2htm_test_assert_equal(twice, once);
    }

    // the experimental HTML parser (parse_pltxt_html) understands <div style="margin-..."> too,
    // matching the main parser's behaviour (block-level only, margin-left/right only)
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<div style=\"margin-left:2em\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4htmlunittest(u8"<div style=\"margin-left:2em;margin-right:3em\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;margin-right:3em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<div style=\"margin-left:2em\">a</div>\nb");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">a</div><br>b"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"a\n<div style=\"margin-left:2em\">b</div>\nc");
        auto answer = ::fast_io::u8string_view{u8"a<br><div style=\"margin-left:2em;\">b</div><br>c"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // mid-line and unknown-style <div> stay literal text in the experimental parser
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"t<div style=\"margin-left:2em\">x</div>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;div&nbsp;style=&quot;margin-left:2em&quot;&gt;x&lt;/div&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<div style=\"color:red\">x</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;color:red&quot;&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a div nested inside a span (emitted by the roundtrip backend for an open margin scope)
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<span style=\"font-size:2px;\">a<br><div style=\"margin-left:2px;\"></div></span>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"font-size:2px;\">a<br><div style=\"margin-left:2px;\"></div></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // nested divs map to nested margin scopes in the experimental parser
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<div style=\"margin-left:2em\"><div style=\"margin-right:1em\">x</div></div>");
        auto answer = ::fast_io::u8string_view{
            u8"<div style=\"margin-left:2em;\"><div style=\"margin-right:1em;\">x</div></div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}