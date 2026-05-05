#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"`test`"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<code>test</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8" test "};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`t t`");
        auto answer = ::fast_io::u8string_view{u8"<code>t&nbsp;t</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t`e`st");
        auto answer = ::fast_io::u8string_view{u8"t<code>e</code>st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"``test``");
        auto answer = ::fast_io::u8string_view{u8"<code>test</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```test```");
        auto answer = ::fast_io::u8string_view{u8"<code>test</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`t\\`t`");
        auto answer = ::fast_io::u8string_view{u8"<code>t`t</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t``e``st");
        auto answer = ::fast_io::u8string_view{u8"t<code>e</code>st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t```e```st");
        auto answer = ::fast_io::u8string_view{u8"t<code>e</code>st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    // Note: this does not follow commonmark
    // This is an implement defined behavior
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`t");
        auto answer = ::fast_io::u8string_view{u8"<code>t</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```<br>```");
        auto answer = ::fast_io::u8string_view{u8"<code>&lt;br&gt;</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`<em>t</em>`");
        auto answer = ::fast_io::u8string_view{u8"<code>&lt;em&gt;t&lt;/em&gt;</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
