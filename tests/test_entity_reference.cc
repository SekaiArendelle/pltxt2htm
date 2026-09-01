#include <pltxt2htm/details/backend/for_plweb_text.hh>
#include "precompile.hh"

int main() {
    {
        auto text = ::fast_io::u8string_view{u8"&amp;"};
        auto result = ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text);
        pltxt2htm_test_assert_true(
            result.has_value() &&
            result.value<::pltxt2htm::Contracts::quick_enforce>().get<::pltxt2htm::Contracts::quick_enforce>() == 5);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#38;"};
        auto result = ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text);
        pltxt2htm_test_assert_true(
            result.has_value() &&
            result.value<::pltxt2htm::Contracts::quick_enforce>().get<::pltxt2htm::Contracts::quick_enforce>() == 5);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#x26;"};
        auto result = ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text);
        pltxt2htm_test_assert_true(
            result.has_value() &&
            result.value<::pltxt2htm::Contracts::quick_enforce>().get<::pltxt2htm::Contracts::quick_enforce>() == 6);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#X2A;"};
        auto result = ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text);
        pltxt2htm_test_assert_true(
            result.has_value() &&
            result.value<::pltxt2htm::Contracts::quick_enforce>().get<::pltxt2htm::Contracts::quick_enforce>() == 6);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&amp;rest"};
        auto result = ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text);
        pltxt2htm_test_assert_true(
            result.has_value() &&
            result.value<::pltxt2htm::Contracts::quick_enforce>().get<::pltxt2htm::Contracts::quick_enforce>() == 5);
    }
    {
        auto text = ::fast_io::u8string_view{u8"foo&amp;bar"};
        auto result =
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text.subview(3));
        pltxt2htm_test_assert_true(
            result.has_value() &&
            result.value<::pltxt2htm::Contracts::quick_enforce>().get<::pltxt2htm::Contracts::quick_enforce>() == 5);
    }

    {
        auto text = ::fast_io::u8string_view{u8"amp;"};
        pltxt2htm_test_assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&amp"};
        pltxt2htm_test_assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&;"};
        pltxt2htm_test_assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#;"};
        pltxt2htm_test_assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#x;"};
        pltxt2htm_test_assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#xGG;"};
        pltxt2htm_test_assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#12a;"};
        pltxt2htm_test_assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&a-b;"};
        pltxt2htm_test_assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }

    // Full roundtrip tests via pltxt4unittest
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&quot;");
        pltxt2htm_test_assert_equal(html, u8"&quot;");
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&amp;");
        pltxt2htm_test_assert_equal(html, u8"&amp;");
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&lt;");
        pltxt2htm_test_assert_equal(html, u8"&lt;");
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&gt;");
        pltxt2htm_test_assert_equal(html, u8"&gt;");
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&apos;");
        pltxt2htm_test_assert_equal(html, u8"&apos;");
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"&#38;"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        pltxt2htm_test_assert_equal(html, u8"&#38;");
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        pltxt2htm_test_assert_equal(plunity_richtext, u8"&");
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"&#x26;"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        pltxt2htm_test_assert_equal(html, u8"&#x26;");
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        pltxt2htm_test_assert_equal(plunity_richtext, u8"&");
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"&#X2A;"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        pltxt2htm_test_assert_equal(html, u8"&#X2A;");
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        pltxt2htm_test_assert_equal(plunity_richtext, u8"*");
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&QUOT;");
        pltxt2htm_test_assert_equal(html, u8"&QUOT;");
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"a&quot;b");
        pltxt2htm_test_assert_equal(html, u8"a&quot;b");
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&quot;&amp;&lt;");
        pltxt2htm_test_assert_equal(html, u8"&quot;&amp;&lt;");
    }
    // Invalid entity &; falls back to bare ampersand
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&;");
        pltxt2htm_test_assert_equal(html, u8"&amp;;");
    }
    // Bare & without entity
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&");
        pltxt2htm_test_assert_equal(html, u8"&amp;");
    }
    // Roundtrip: bare " produces &quot;, which parses back to entity reference
    {
        auto const first_pass = ::pltxt2htm_test::pltxt4unittest(u8"\"");
        pltxt2htm_test_assert_equal(first_pass, u8"&quot;");
        auto const first_pass_view = ::fast_io::u8string_view{first_pass.data(), first_pass.size()};
        auto const second_pass = ::pltxt2htm_test::pltxt4unittest(first_pass_view);
        pltxt2htm_test_assert_equal(second_pass, u8"&quot;");
    }
    // Roundtrip: bare < produces &lt; which parses back to entity reference
    {
        auto const first_pass = ::pltxt2htm_test::pltxt4unittest(u8"<");
        pltxt2htm_test_assert_equal(first_pass, u8"&lt;");
        auto const first_pass_view = ::fast_io::u8string_view{first_pass.data(), first_pass.size()};
        auto const second_pass = ::pltxt2htm_test::pltxt4unittest(first_pass_view);
        pltxt2htm_test_assert_equal(second_pass, u8"&lt;");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"&amp;\t'\"");
        auto answer = ::fast_io::u8string_view{u8"&amp;\t'\""};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // plunity backend decodes numeric character references to their characters (see above)
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"&#34;");
        pltxt2htm_test_assert_equal(html, u8"\"");
    }
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"&#39;");
        pltxt2htm_test_assert_equal(html, u8"'");
    }
    {
        // '<' and '>' are emitted in TMP-escaped full-width form
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"&#60;");
        pltxt2htm_test_assert_equal(html, u8"<size=20>\uff1c</size>");
    }
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"&#62;");
        pltxt2htm_test_assert_equal(html, u8"<size=20>\uff1e</size>");
    }
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"&#160;");
        pltxt2htm_test_assert_equal(html, u8"\u00A0");
    }
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"a&#33;b");
        pltxt2htm_test_assert_equal(html, u8"a!b");
    }
    {
        // 4-byte UTF-8 code point
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"&#x1F600;");
        pltxt2htm_test_assert_equal(html, u8"\U0001F600");
    }
    {
        // Out-of-range code points fall back to the verbatim reference
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"&#1114112;");
        pltxt2htm_test_assert_equal(html, u8"&#1114112;");
    }
    {
        // Surrogate code points fall back to the verbatim reference
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"&#xD800;");
        pltxt2htm_test_assert_equal(html, u8"&#xD800;");
    }

    return 0;
}
