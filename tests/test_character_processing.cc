#include <fast_io/fast_io_dsal/array.h>
#include <pltxt2htm/details/backend/for_plweb_text.hh>
#include <pltxt2htm/details/parser/character_processing.hh>
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

constexpr auto utf8_helpers_are_constexpr() noexcept -> bool {
    auto const decoded =
        ::pltxt2htm::details::decode_utf8_code_point<::pltxt2htm::Contracts::quick_enforce>(u8"\U0001F600");
    auto const encoded = ::pltxt2htm::details::encode_utf8_code_point(char32_t{0x1F600});
    return decoded.valid && decoded.consumed_size == 4 && decoded.code_point == char32_t{0x1F600} &&
           encoded.size == 4 && encoded.code_units[0] == char8_t{0xF0} && encoded.code_units[1] == char8_t{0x9F} &&
           encoded.code_units[2] == char8_t{0x98} && encoded.code_units[3] == char8_t{0x80};
}

static_assert(entity_decoder_is_constexpr());
static_assert(utf8_helpers_are_constexpr());
static_assert(::pltxt2htm::details::is_unicode_scalar_value(char32_t{0x10FFFF}));
static_assert(::pltxt2htm::details::is_unicode_scalar_value(char32_t{0xD800}) == false);
static_assert(::pltxt2htm::details::is_unicode_scalar_value(char32_t{0x110000}) == false);
static_assert(::pltxt2htm::details::is_ascii_control_code_point(char32_t{0x1F}));
static_assert(::pltxt2htm::details::is_ascii_control_code_point(char32_t{0x7F}));
static_assert(::pltxt2htm::details::is_ascii_control_code_point(U' ') == false);

int main() {
    {
        auto const decoded = ::pltxt2htm::details::decode_utf8_code_point<::pltxt2htm::Contracts::quick_enforce>(u8"A");
        pltxt2htm_test_assert_true(decoded.valid);
        pltxt2htm_test_assert_true(decoded.consumed_size == 1);
        pltxt2htm_test_assert_true(decoded.code_point == U'A');
    }
    {
        auto const decoded =
            ::pltxt2htm::details::decode_utf8_code_point<::pltxt2htm::Contracts::quick_enforce>(u8"\u20AC");
        pltxt2htm_test_assert_true(decoded.valid);
        pltxt2htm_test_assert_true(decoded.consumed_size == 3);
        pltxt2htm_test_assert_true(decoded.code_point == char32_t{0x20AC});
    }
    {
        constexpr auto bytes = ::fast_io::array{char8_t{0xE2}, char8_t{0x82}};
        auto const decoded = ::pltxt2htm::details::decode_utf8_code_point<::pltxt2htm::Contracts::quick_enforce>(
            ::fast_io::u8string_view{bytes.data(), bytes.size()});
        pltxt2htm_test_assert_true(decoded.valid == false);
        pltxt2htm_test_assert_true(decoded.consumed_size == 2);
    }
    {
        constexpr auto bytes = ::fast_io::array{char8_t{0xF0}, char8_t{0x90}, char8_t{'A'}};
        auto const decoded = ::pltxt2htm::details::decode_utf8_code_point<::pltxt2htm::Contracts::quick_enforce>(
            ::fast_io::u8string_view{bytes.data(), bytes.size()});
        pltxt2htm_test_assert_true(decoded.valid == false);
        pltxt2htm_test_assert_true(decoded.consumed_size == 2);
    }
    {
        constexpr auto bytes = ::fast_io::array{char8_t{0xED}, char8_t{0xA0}, char8_t{0x80}};
        auto const decoded = ::pltxt2htm::details::decode_utf8_code_point<::pltxt2htm::Contracts::quick_enforce>(
            ::fast_io::u8string_view{bytes.data(), bytes.size()});
        pltxt2htm_test_assert_true(decoded.valid == false);
        pltxt2htm_test_assert_true(decoded.consumed_size == 3);
    }
    {
        auto const encoded = ::pltxt2htm::details::encode_utf8_code_point(char32_t{0xD800});
        pltxt2htm_test_assert_true(encoded.size == 0);
    }
    {
        ::fast_io::u8string encoded{};
        ::pltxt2htm::details::append_utf8_code_point(encoded, U'A');
        ::pltxt2htm::details::append_utf8_code_point(encoded, char32_t{0x20AC});
        ::pltxt2htm::details::append_utf8_code_point(encoded, char32_t{0x1F600});
        pltxt2htm_test_assert_equal(encoded, u8"A\u20AC\U0001F600");
    }

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
    assert_decoded(u8"&#1;", 4, char32_t{0x01});
    assert_decoded(u8"&#13;", 5, char32_t{0x0D});
    assert_decoded(u8"&#127;", 6, char32_t{0x7F});

    // Parser-produced ASTs never store raw ASCII control bytes in U8Char nodes.
    for (auto const code_point : ::fast_io::array{char32_t{0x01}, char32_t{0x0D}, char32_t{0x7F}}) {
        ::pltxt2htm::Ast<::pltxt2htm::Contracts::quick_enforce> ast{};
        ::pltxt2htm::details::append_code_point_to_ast<::pltxt2htm::Contracts::quick_enforce>(code_point, ast);
        pltxt2htm_test_assert_true(ast.size() == 1);
        pltxt2htm_test_assert_true(ast[0].get_node_kind() == ::pltxt2htm::NodeKind::invalid_u8char);
    }

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
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"&#1;&#13;&#127;"), u8"���");

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
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(u8"&#1;&#13;&#127;"), u8"���");

    return 0;
}
