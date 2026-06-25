#include <pltxt2htm/details/backend/for_plweb_text.hh>
#include "precompile.hh"

int main() {
    {
        auto text = ::fast_io::u8string_view{u8"&amp;"};
        auto result = ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text);
        ::pltxt2htm_test::assert_true(result.has_value() && result.value() == 5);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#38;"};
        auto result = ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text);
        ::pltxt2htm_test::assert_true(result.has_value() && result.value() == 5);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#x26;"};
        auto result = ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text);
        ::pltxt2htm_test::assert_true(result.has_value() && result.value() == 6);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#X2A;"};
        auto result = ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text);
        ::pltxt2htm_test::assert_true(result.has_value() && result.value() == 6);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&amp;rest"};
        auto result = ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text);
        ::pltxt2htm_test::assert_true(result.has_value() && result.value() == 5);
    }
    {
        auto text = ::fast_io::u8string_view{u8"foo&amp;bar"};
        auto result =
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text.subview(3));
        ::pltxt2htm_test::assert_true(result.has_value() && result.value() == 5);
    }

    {
        auto text = ::fast_io::u8string_view{u8"amp;"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&amp"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&;"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#;"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#x;"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#xGG;"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&#12a;"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::try_parse_entity_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
            false);
    }
    {
        auto text = ::fast_io::u8string_view{u8"&a-b;"};
        ::pltxt2htm_test::assert_true(
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
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&#38;");
        pltxt2htm_test_assert_equal(html, u8"&#38;");
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&#x26;");
        pltxt2htm_test_assert_equal(html, u8"&#x26;");
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&#X2A;");
        pltxt2htm_test_assert_equal(html, u8"&#X2A;");
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

    return 0;
}
