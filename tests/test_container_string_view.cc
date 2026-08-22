#include <concepts>
#include <type_traits>

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

#include <pltxt2htm/container/string_view.hh>
#include <pltxt2htm/container/string.hh>
#include <pltxt2htm/details/literal_string.hh>

#include "precompile.hh"

using U8StringView = ::pltxt2htm::container::U8StringView;

template<typename CharType>
concept can_form_basic_string_view = requires { typename ::pltxt2htm::container::BasicStringView<CharType>; };

static_assert(::std::is_trivially_copyable_v<U8StringView>);
static_assert(::std::is_standard_layout_v<U8StringView>);
static_assert(::std::same_as<U8StringView::value_type, char8_t>);
static_assert(::std::same_as<U8StringView::const_iterator, char8_t const*>);
static_assert(::std::is_constructible_v<U8StringView, ::fast_io::u8string_view>);
static_assert(::std::is_constructible_v<U8StringView, ::fast_io::u8string const&>);
static_assert(!::std::is_constructible_v<U8StringView, ::fast_io::u8string&&>);
static_assert(::std::is_constructible_v<U8StringView, ::pltxt2htm::container::u8string const&>);
static_assert(!::std::is_constructible_v<U8StringView, ::pltxt2htm::container::u8string&&>);
static_assert(can_form_basic_string_view<char>);
static_assert(can_form_basic_string_view<wchar_t>);
static_assert(can_form_basic_string_view<char8_t>);
static_assert(can_form_basic_string_view<char16_t>);
static_assert(can_form_basic_string_view<char32_t>);
static_assert(!can_form_basic_string_view<bool>);
static_assert(!can_form_basic_string_view<int>);

consteval auto test_constexpr_string_view() noexcept -> bool {
    U8StringView const empty{};
    if (!empty.empty() || empty.size() != 0 || empty.data() != nullptr || empty.begin() != empty.end()) {
        return false;
    }

    U8StringView const text{u8"abcdef"};
    if (text.empty() || text.size() != 6 || text.template index<::pltxt2htm::Contracts::quick_enforce>(2) != u8'c') {
        return false;
    }

    auto const tail = text.template subview<::pltxt2htm::Contracts::quick_enforce>(3);
    auto const middle = text.template subview<::pltxt2htm::Contracts::quick_enforce>(1, 3);
    auto const ignored_contract = text.template subview<::pltxt2htm::Contracts::ignore>(2, 2);
    if (tail != u8"def" || middle != u8"bcd" || !text.starts_with(u8"abc") || !text.ends_with(u8"def") ||
        text.ends_with(u8"abcdefg") || ignored_contract.template index<::pltxt2htm::Contracts::ignore>(0) != u8'c') {
        return false;
    }

    constexpr auto literal = ::pltxt2htm::details::U8LiteralString{u8"literal"};
    auto const literal_view = ::pltxt2htm::container::BasicStringView{literal};
    static_assert(::std::same_as<::std::remove_cvref_t<decltype(literal_view)>, U8StringView>);
    return literal_view == u8"literal";
}

static_assert(test_constexpr_string_view());

int main() {
    ::pltxt2htm::container::u8string own_string{u8"pltxt2htm"};
    U8StringView const own_string_view{own_string};
    auto const deduced_own_string_view = ::pltxt2htm::container::BasicStringView{own_string};
    static_assert(::std::same_as<::std::remove_cvref_t<decltype(deduced_own_string_view)>, U8StringView>);
    pltxt2htm_test_assert_true(own_string_view == u8"pltxt2htm");
    pltxt2htm_test_assert_true(deduced_own_string_view == own_string_view);

    ::fast_io::u8string string{u8"fast_io"};
    U8StringView const string_view{string};
    auto const deduced_string_view = ::pltxt2htm::container::BasicStringView{string};
    static_assert(::std::same_as<::std::remove_cvref_t<decltype(deduced_string_view)>, U8StringView>);
    pltxt2htm_test_assert_true(string_view == u8"fast_io");
    pltxt2htm_test_assert_true(deduced_string_view == string_view);
    pltxt2htm_test_assert_true(string == string_view);
    pltxt2htm_test_assert_true(string_view == string);

    ::fast_io::u8string_view const fast_io_view{u8"view"};
    U8StringView const compatible_view{fast_io_view};
    pltxt2htm_test_assert_true(fast_io_view == compatible_view);
    pltxt2htm_test_assert_true(compatible_view == fast_io_view);

    ::fast_io::u8string appended{};
    appended.append(string_view);
    pltxt2htm_test_assert_true(appended == string_view);

    auto const converted = static_cast<::fast_io::u8string_view>(compatible_view);
    pltxt2htm_test_assert_true(converted == fast_io_view);

    U8StringView const manipulator_view{::fast_io::mnp::os_c_str(string)};
    pltxt2htm_test_assert_true(manipulator_view == string_view);

    return 0;
}
