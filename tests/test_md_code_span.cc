#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"`test`"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<code>test</code>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<font=\"PhysicsLab-SarasaMonoSC SDF\"> test </font>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`t t`");
        auto answer = ::fast_io::u8string_view{u8"<code>t&nbsp;t</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t`e`st");
        auto answer = ::fast_io::u8string_view{u8"t<code>e</code>st"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"``test``");
        auto answer = ::fast_io::u8string_view{u8"<code>test</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```test```");
        auto answer = ::fast_io::u8string_view{u8"<code>test</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`t\\`t`");
        auto answer = ::fast_io::u8string_view{u8"<code>t`t</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t``e``st");
        auto answer = ::fast_io::u8string_view{u8"t<code>e</code>st"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t```e```st");
        auto answer = ::fast_io::u8string_view{u8"t<code>e</code>st"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // An opening backtick without a matching closing backtick is literal text,
    // not an unterminated code span.
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`t");
        auto answer = ::fast_io::u8string_view{u8"`t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```<br>```");
        auto answer = ::fast_io::u8string_view{u8"<code>&lt;br&gt;</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`<em>t</em>`");
        auto answer = ::fast_io::u8string_view{u8"<code>&lt;em&gt;t&lt;/em&gt;</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab`test`cd");
        auto answer = ::fast_io::u8string_view{u8"ab<font=\"PhysicsLab-SarasaMonoSC SDF\"> test </font>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab``test``cd");
        auto answer = ::fast_io::u8string_view{u8"ab<font=\"PhysicsLab-SarasaMonoSC SDF\"> test </font>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab```test```cd");
        auto answer = ::fast_io::u8string_view{u8"ab<font=\"PhysicsLab-SarasaMonoSC SDF\"> test </font>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Regression: an opening delimiter without a matching closing delimiter must not be
    // accepted as a code span. Before the fix, the 2-backtick branch in the inline parser
    // subtracted the delimiters from the consumed count and underflowed to a huge size_t,
    // terminating (quick_enforce) or forming an out-of-bounds subview (ignore). See
    // fixedadv_fuzzer_crash_analysis.md. Delimiter lengths 1, 2, 3 at top level.
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`");
        auto answer = ::fast_io::u8string_view{u8"`"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // A delimiter run with no content is NOT a code span and stays literal text.
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"``");
        auto answer = ::fast_io::u8string_view{u8"``"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```");
        auto answer = ::fast_io::u8string_view{u8"```"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // Content long enough to fill an unclosed span still must not be consumed by it.
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`t");
        auto answer = ::fast_io::u8string_view{u8"`t"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"``ab");
        auto answer = ::fast_io::u8string_view{u8"``ab"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```x");
        auto answer = ::fast_io::u8string_view{u8"```x"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // A backslash-escaped backtick at the end is consumed as content, not a closing delimiter.
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`a\\`");
        auto answer = ::fast_io::u8string_view{u8"`a`"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // Even balanced delimiter runs with no content stay literal.
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"````");
        auto answer = ::fast_io::u8string_view{u8"````"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"``````");
        auto answer = ::fast_io::u8string_view{u8"``````"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // The fuzzer crash input: an unclosed code span inside a Markdown list item.
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* `");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>`</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* ``");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>``</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* ```");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>```</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // Unclosed code span inside a table cell.
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"| a |\n|---|\n| `` |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>a</th></tr></thead><tbody><tr><td>``</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
