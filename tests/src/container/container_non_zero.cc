#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

#include <pltxt2htm/container/non_zero.hh>

#include "precompile.hh"

template<typename T>
concept can_form_non_zero = requires { typename ::pltxt2htm::container::NonZero<T>; };

template<typename T>
concept can_call_get_without_contract = requires(T const& value) { value.get(); };

template<typename T>
    requires can_form_non_zero<T>
consteval auto has_zero_overhead_representation() noexcept -> bool {
    using NonZero = ::pltxt2htm::container::NonZero<T>;

    return ::std::is_trivially_copyable_v<NonZero> && ::std::is_standard_layout_v<NonZero> &&
           sizeof(NonZero) == sizeof(T) && alignof(NonZero) == alignof(T);
}

using NonZeroUsize = ::pltxt2htm::container::NonZeroUsize;

template<typename T>
concept can_compare_non_zero_with = requires(NonZeroUsize const& value, T other) {
    { value == other } -> ::std::same_as<bool>;
    { other == value } -> ::std::same_as<bool>;
};

static_assert(::std::same_as<::pltxt2htm::container::NonZeroU8, ::pltxt2htm::container::NonZero<::std::uint8_t>>);
static_assert(::std::same_as<::pltxt2htm::container::NonZeroU16, ::pltxt2htm::container::NonZero<::std::uint16_t>>);
static_assert(::std::same_as<::pltxt2htm::container::NonZeroU32, ::pltxt2htm::container::NonZero<::std::uint32_t>>);
static_assert(::std::same_as<::pltxt2htm::container::NonZeroU64, ::pltxt2htm::container::NonZero<::std::uint64_t>>);
static_assert(::std::same_as<::pltxt2htm::container::NonZeroUsize, ::pltxt2htm::container::NonZero<::std::size_t>>);
static_assert(can_form_non_zero<unsigned char>);
static_assert(can_form_non_zero<unsigned short>);
static_assert(can_form_non_zero<unsigned>);
static_assert(can_form_non_zero<unsigned long>);
static_assert(can_form_non_zero<unsigned long long>);
static_assert(!can_form_non_zero<bool>);
static_assert(!can_form_non_zero<int>);
static_assert(!can_form_non_zero<unsigned const>);

static_assert(has_zero_overhead_representation<unsigned char>());
static_assert(has_zero_overhead_representation<unsigned short>());
static_assert(has_zero_overhead_representation<unsigned>());
static_assert(has_zero_overhead_representation<unsigned long>());
static_assert(has_zero_overhead_representation<unsigned long long>());
static_assert(has_zero_overhead_representation<::std::size_t>());

static_assert(!::std::default_initializable<NonZeroUsize>);
static_assert(!::std::is_aggregate_v<NonZeroUsize>);
static_assert(!::std::is_constructible_v<NonZeroUsize, ::std::size_t>);
static_assert(!::std::is_convertible_v<NonZeroUsize, ::std::size_t>);
static_assert(!can_call_get_without_contract<NonZeroUsize>);
static_assert(can_compare_non_zero_with<unsigned char>);
static_assert(can_compare_non_zero_with<unsigned short>);
static_assert(can_compare_non_zero_with<unsigned>);
static_assert(can_compare_non_zero_with<unsigned long>);
static_assert(can_compare_non_zero_with<unsigned long long>);
static_assert(can_compare_non_zero_with<signed char>);
static_assert(can_compare_non_zero_with<short>);
static_assert(can_compare_non_zero_with<int>);
static_assert(can_compare_non_zero_with<long>);
static_assert(can_compare_non_zero_with<long long>);
static_assert(!can_compare_non_zero_with<bool>);
static_assert(!can_compare_non_zero_with<float>);
static_assert(::std::same_as<
              decltype(::std::declval<NonZeroUsize const&>().template get<::pltxt2htm::Contracts::quick_enforce>()),
              ::std::size_t>);

consteval auto test_constexpr_non_zero() noexcept -> bool {
    auto const value = NonZeroUsize::from<::pltxt2htm::Contracts::quick_enforce>(42);
    auto const equal_value = NonZeroUsize::from<::pltxt2htm::Contracts::quick_enforce>(42);
    auto const maximum =
        NonZeroUsize::from<::pltxt2htm::Contracts::quick_enforce>((::std::numeric_limits<::std::size_t>::max)());
    auto const u8_maximum = ::pltxt2htm::container::NonZeroU8::from<::pltxt2htm::Contracts::quick_enforce>(
        (::std::numeric_limits<::std::uint8_t>::max)());

    return value.get<::pltxt2htm::Contracts::quick_enforce>() == 42 &&
           value.get<::pltxt2htm::Contracts::ignore>() == 42 && value == equal_value && value == 42 && 42 == value &&
           value == 42U && 42U == value && value != 41 && 41 != value && value != -1 && -1 != value && maximum != -1 &&
           -1 != maximum && u8_maximum == 255LL && 255LL == u8_maximum && u8_maximum == 255ULL &&
           255ULL == u8_maximum && u8_maximum != 257 && 257 != u8_maximum && u8_maximum != 511U && 511U != u8_maximum;
}

static_assert(test_constexpr_non_zero());

int main() {
    return 0;
}
