#include <compare>
#include <concepts>
#include <cstddef>
#include <type_traits>

#include <pltxt2htm/contracts.hh>
#include <pltxt2htm/details/inplace_string.hh>

#include "precompile.hh"

using CheckedU8String4 = ::pltxt2htm::details::U8InplaceString<4, ::pltxt2htm::Contracts::quick_enforce>;
using IgnoredU8String8 = ::pltxt2htm::details::U8InplaceString<8, ::pltxt2htm::Contracts::ignore>;
using CountOperation = void (*)(CheckedU8String4&, ::std::size_t, char8_t) noexcept;

struct PotentiallyThrowingIterator {
    using value_type = char8_t;
    using difference_type = ::std::ptrdiff_t;

    char8_t const* pointer{};

    [[nodiscard]]
    constexpr auto operator*() const noexcept -> char8_t {
        return *this->pointer;
    }

    constexpr auto operator++() -> PotentiallyThrowingIterator& {
        ++this->pointer;
        return *this;
    }

    constexpr void operator++(int) {
        ++*this;
    }

    [[nodiscard]]
    constexpr auto operator==(PotentiallyThrowingIterator const&) const noexcept -> bool = default;
};

template<typename CharType, ::std::size_t extent = 4>
concept can_form_basic_inplace_string = requires {
    typename ::pltxt2htm::details::BasicInplaceString<CharType, extent, ::pltxt2htm::Contracts::quick_enforce>;
};

static_assert(can_form_basic_inplace_string<char>);
static_assert(can_form_basic_inplace_string<wchar_t>);
static_assert(can_form_basic_inplace_string<char8_t>);
static_assert(can_form_basic_inplace_string<char16_t>);
static_assert(can_form_basic_inplace_string<char32_t>);
static_assert(!can_form_basic_inplace_string<bool>);
static_assert(!can_form_basic_inplace_string<int>);
static_assert(!can_form_basic_inplace_string<char8_t, 0>);

static_assert(::std::is_trivially_copyable_v<CheckedU8String4>);
static_assert(::std::is_standard_layout_v<CheckedU8String4>);
static_assert(::std::same_as<CheckedU8String4::value_type, char8_t>);
static_assert(::std::same_as<CheckedU8String4::iterator, char8_t*>);
static_assert(requires {
    static_cast<CountOperation>(&CheckedU8String4::assign);
    static_cast<CountOperation>(&CheckedU8String4::append);
    static_cast<CountOperation>(&CheckedU8String4::resize);
});
static_assert(::std::input_iterator<PotentiallyThrowingIterator>);
static_assert(::std::is_nothrow_constructible_v<CheckedU8String4, char8_t const*, char8_t const*>);
static_assert(
    !::std::is_nothrow_constructible_v<CheckedU8String4, PotentiallyThrowingIterator, PotentiallyThrowingIterator>);
static_assert(noexcept(::std::declval<CheckedU8String4&>().append(::std::declval<char8_t const*>(),
                                                                  ::std::declval<char8_t const*>())));
static_assert(!noexcept(::std::declval<CheckedU8String4&>().append(::std::declval<PotentiallyThrowingIterator>(),
                                                                   ::std::declval<PotentiallyThrowingIterator>())));
static_assert(sizeof(CheckedU8String4) == 5);
static_assert(CheckedU8String4::capacity() == 4);
static_assert(CheckedU8String4::max_size() == 4);
static_assert(CheckedU8String4::max_size_bytes() == 4);

consteval auto test_constexpr_inplace_string() -> bool {
    CheckedU8String4 const empty{};
    if (!empty.empty() || empty.size() != 0 || empty.data() == nullptr || empty.begin() != empty.end()) {
        return false;
    }

    constexpr char8_t initial[]{u8'a', u8'\0', u8'b'};
    auto value = CheckedU8String4{initial, initial + 3};
    if (value.empty() || value.size() != 3 || value.size_bytes() != 3 || value.index(0) != u8'a' ||
        value.index(1) != u8'\0' || value.front() != u8'a' || value.back() != u8'b') {
        return false;
    }

    value.index(2) = u8'c';
    if (!value.try_push_back(u8'd') || value.try_push_back(u8'e') || value.back() != u8'd') {
        return false;
    }

    value.pop_back();
    value.resize(4, u8'x');
    if (value.size() != 4 || value.index(2) != u8'c' || value.index(3) != u8'x') {
        return false;
    }

    value.resize(2);
    value.append(2, u8'z');
    if (value.index(2) != u8'z' || value.index(3) != u8'z' || *value.rbegin() != u8'z') {
        return false;
    }

    value.assign(3, u8'q');
    if (value.size() != 3 || value.front() != u8'q' || value.back() != u8'q') {
        return false;
    }

    auto copy = value;
    copy.clear();
    copy.append(initial, initial + 3);
    auto const larger = IgnoredU8String8{initial, initial + 3};
    if (!(copy == larger) || (copy <=> larger) != ::std::strong_ordering::equal) {
        return false;
    }

    copy.index(2) = u8'd';
    if ((copy <=> larger) != ::std::strong_ordering::greater || value == copy) {
        return false;
    }

    copy.assign(copy.begin(), copy.end());
    if (copy.size() != 3 || copy.index(0) != u8'a' || copy.index(1) != u8'\0' || copy.index(2) != u8'd') {
        return false;
    }

    copy.assign(copy.begin() + 1, copy.end());
    return copy.size() == 2 && copy.index(0) == u8'\0' && copy.index(1) == u8'd';
}

static_assert(test_constexpr_inplace_string());

int main() {
    constexpr char8_t source[]{u8't', u8'e', u8's', u8't'};
    auto value = CheckedU8String4{source, source + 4};

    pltxt2htm_test_assert_true(value.size() == 4);
    pltxt2htm_test_assert_true(value.data() == value.begin());
    pltxt2htm_test_assert_true(value.end() - value.begin() == 4);
    pltxt2htm_test_assert_true(value.front() == u8't');
    pltxt2htm_test_assert_true(value.back() == u8't');

    return 0;
}
