#include <concepts>
#include <cstddef>
#include <type_traits>

#include <pltxt2htm/container/non_zero.hh>

#include "precompile.hh"

template<typename T>
concept can_form_non_zero = requires { typename ::pltxt2htm::container::NonZero<T>; };

template<typename T>
    requires can_form_non_zero<T>
consteval auto has_zero_overhead_representation() noexcept -> bool {
    using NonZero = ::pltxt2htm::container::NonZero<T>;

    return ::std::is_trivially_copyable_v<NonZero> && ::std::is_standard_layout_v<NonZero> &&
           sizeof(NonZero) == sizeof(T) && alignof(NonZero) == alignof(T);
}

using NonZeroSize = ::pltxt2htm::container::NonZeroSize;

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

static_assert(!::std::default_initializable<NonZeroSize>);
static_assert(!::std::is_aggregate_v<NonZeroSize>);
static_assert(!::std::is_constructible_v<NonZeroSize, ::std::size_t>);
static_assert(!::std::is_convertible_v<NonZeroSize, ::std::size_t>);

consteval auto test_constexpr_non_zero() noexcept -> bool {
    auto const value = NonZeroSize::from<::pltxt2htm::Contracts::quick_enforce>(42);
    auto const equal_value = NonZeroSize::from<::pltxt2htm::Contracts::quick_enforce>(42);

    return value.get() == 42 && value == equal_value;
}

static_assert(test_constexpr_non_zero());

int main() {
    return 0;
}
