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

    return 0;
}
