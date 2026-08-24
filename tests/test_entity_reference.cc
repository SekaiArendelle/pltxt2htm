#include <pltxt2htm/details/backend/for_plweb_text.hh>
#include "precompile.hh"

constexpr void assert_decoded(::fast_io::u8string_view text, ::std::size_t consumed_size, char32_t first_code_point,
                              char32_t second_code_point = char32_t{}) {
    auto const result =
        ::pltxt2htm::details::try_decode_character_reference<::pltxt2htm::Contracts::quick_enforce>(text);
    pltxt2htm_test_assert_true(result.has_value());
    auto const& decoded = result.value<::pltxt2htm::Contracts::quick_enforce>();
    pltxt2htm_test_assert_true(decoded.consumed_size == consumed_size);
    pltxt2htm_test_assert_true(decoded.first_code_point == first_code_point);
    pltxt2htm_test_assert_true(decoded.second_code_point == second_code_point);
    pltxt2htm_test_assert_true(decoded.code_point_count == (second_code_point == 0 ? 1 : 2));
}

constexpr void assert_not_decoded(::fast_io::u8string_view text) {
    pltxt2htm_test_assert_true(
        ::pltxt2htm::details::try_decode_character_reference<::pltxt2htm::Contracts::quick_enforce>(text).has_value() ==
        false);
}

constexpr auto entity_decoder_is_constexpr() -> bool {
    auto const result =
        ::pltxt2htm::details::try_decode_character_reference<::pltxt2htm::Contracts::quick_enforce>(u8"&amp;");
    return result.has_value() && result.value<::pltxt2htm::Contracts::quick_enforce>().first_code_point == U'&';
}

static_assert(entity_decoder_is_constexpr());

int main() {
    assert_decoded(u8"&amp;", 5, U'&');
    assert_decoded(u8"&AMP;", 5, U'&');
    assert_decoded(u8"&#38;", 5, U'&');
    assert_decoded(u8"&#x26;", 6, U'&');
    assert_decoded(u8"&#X2A;", 6, U'*');
    assert_decoded(u8"&amp;rest", 5, U'&');
    assert_decoded(u8"&NotEqualTilde;", 15, char32_t{0x2242}, char32_t{0x338});

    assert_not_decoded(u8"amp;");
    assert_not_decoded(u8"&amp");
    assert_not_decoded(u8"&;");
    assert_not_decoded(u8"&#;");
    assert_not_decoded(u8"&#x;");
    assert_not_decoded(u8"&#xGG;");
    assert_not_decoded(u8"&#12a;");
    assert_not_decoded(u8"&a-b;");
    assert_not_decoded(u8"&bogus;");
    assert_not_decoded(u8"&notit;");

    // Numeric references use HTML's replacement and Windows-1252 recovery rules.
    assert_decoded(u8"&#0;", 4, char32_t{0xFFFD});
    assert_decoded(u8"&#xD800;", 8, char32_t{0xFFFD});
    assert_decoded(u8"&#1114112;", 10, char32_t{0xFFFD});
    assert_decoded(u8"&#999999999999999999999999;", 27, char32_t{0xFFFD});
    assert_decoded(u8"&#128;", 6, char32_t{0x20AC});

    // Every generated named reference round-trips through the decoder.
    for (auto const& entity : ::pltxt2htm::details::html_named_character_references) {
        ::fast_io::u8string spelling{u8"&"};
        spelling.append(::pltxt2htm::container::U8StringView{
            ::pltxt2htm::details::html_named_character_reference_names + entity.name_offset, entity.name_size});
        spelling.push_back(u8';');
        auto const decoded =
            ::pltxt2htm::details::try_decode_character_reference<::pltxt2htm::Contracts::quick_enforce>(
                ::pltxt2htm::container::U8StringView{spelling});
        pltxt2htm_test_assert_true(decoded.has_value());
        auto const& value = decoded.value<::pltxt2htm::Contracts::quick_enforce>();
        pltxt2htm_test_assert_true(value.consumed_size == spelling.size());
        pltxt2htm_test_assert_true(value.first_code_point == entity.first_code_point);
        pltxt2htm_test_assert_true(value.second_code_point == entity.second_code_point);
    }

    // Backend attribute escaping never treats its input as pre-escaped HTML.
    {
        ::fast_io::u8string escaped{};
        ::pltxt2htm::details::append_html_attr_escaped<::pltxt2htm::Contracts::quick_enforce>(
            escaped, ::pltxt2htm::container::U8StringView{u8"&quot;&bogus;"});
        pltxt2htm_test_assert_equal(escaped, u8"&amp;quot;&amp;bogus;");
    }

    // Web output is normalized from text semantics rather than preserving source spelling.
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&quot;"), u8"&quot;");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&amp;"), u8"&amp;");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&#38;"), u8"&amp;");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&#x26;"), u8"&amp;");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&QUOT;"), u8"&quot;");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&copy;"), u8"©");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&NotEqualTilde;"), u8"≂̸");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&#160;"), u8"&nbsp;");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8" "), u8"&nbsp;");

    // Unknown and unterminated names remain literal text and cannot be reinterpreted by HTML.
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&bogus;"), u8"&amp;bogus;");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&notit;"), u8"&amp;notit;");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&amp"), u8"&amp;amp");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&;"), u8"&amp;;");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&"), u8"&amp;");

    // Rendering HTML and parsing it again preserves text semantics.
    {
        auto const first_pass = ::pltxt2htm_test::pltxt4unittest(u8"\"");
        pltxt2htm_test_assert_equal(first_pass, u8"&quot;");
        auto const first_pass_view = ::fast_io::u8string_view{first_pass.data(), first_pass.size()};
        auto const second_pass = ::pltxt2htm_test::pltxt4unittest(first_pass_view);
        pltxt2htm_test_assert_equal(second_pass, u8"&quot;");
    }
    {
        auto const first_pass = ::pltxt2htm_test::pltxt4unittest(u8"<");
        pltxt2htm_test_assert_equal(first_pass, u8"&lt;");
        auto const first_pass_view = ::fast_io::u8string_view{first_pass.data(), first_pass.size()};
        auto const second_pass = ::pltxt2htm_test::pltxt4unittest(first_pass_view);
        pltxt2htm_test_assert_equal(second_pass, u8"&lt;");
    }

    // Unity receives the same decoded text semantics, with its own delimiter escaping.
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&amp;"), u8"&");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&quot;"), u8"\"");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&copy;"), u8"©");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&#34;"), u8"\"");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&#39;"), u8"'");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&#60;"), u8"<size=20>＜</size>");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&#62;"), u8"<size=20>＞</size>");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&#160;"), u8" ");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"a&#33;b"), u8"a!b");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&#x1F600;"), u8"😀");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&#1114112;"), u8"�");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&#xD800;"), u8"�");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&#128;"), u8"€");
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&bogus;"), u8"&bogus;");

    return 0;
}
