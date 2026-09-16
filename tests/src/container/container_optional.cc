#include <concepts>
#include <cstddef>
#include <utility>

#include "precompile.hh"

#include <pltxt2htm/container/expected.hh>
#include <pltxt2htm/container/non_zero.hh>
#include <pltxt2htm/container/optional.hh>

using IntOptional = ::pltxt2htm::container::Optional<int>;

static_assert(::pltxt2htm::container::is_optional<IntOptional>);
static_assert(!::pltxt2htm::container::is_expected<IntOptional>);
static_assert(!::std::default_initializable<IntOptional>);
static_assert(::std::same_as<IntOptional::value_type, int>);
static_assert(::std::same_as<IntOptional::rebind<double>, ::pltxt2htm::container::Optional<double>>);
static_assert(::std::same_as<decltype(::std::declval<IntOptional const&>().has_value()), bool>);
static_assert(::std::same_as<decltype(::std::declval<IntOptional const&&>().has_value()), bool>);

static_assert(
    ::std::same_as<decltype(::std::declval<IntOptional&>().value<::pltxt2htm::Contracts::quick_enforce>()), int&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntOptional const&>().value<::pltxt2htm::Contracts::quick_enforce>()),
                   int const&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntOptional&&>().value<::pltxt2htm::Contracts::quick_enforce>()), int&&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntOptional const&&>().value<::pltxt2htm::Contracts::quick_enforce>()),
                   int const&&>);
static_assert(::std::same_as<decltype(::std::declval<IntOptional&>().value_or(short{})), int>);
static_assert(::std::same_as<decltype(::std::declval<IntOptional const&>().value_or(short{})), int>);
static_assert(::std::same_as<decltype(::std::declval<IntOptional&&>().value_or(short{})), int>);
static_assert(::std::same_as<decltype(::std::declval<IntOptional const&&>().value_or(short{})), int>);
static_assert(::std::same_as<decltype(::std::declval<IntOptional const&>().value_or<long>(short{})), int>);
static_assert(noexcept(::std::declval<IntOptional const&>().value_or(short{})));
static_assert(noexcept(::std::declval<IntOptional&&>().value_or(short{})));
static_assert(noexcept(::std::declval<IntOptional const&&>().value_or(short{})));
static_assert(!::std::is_assignable_v<IntOptional&&, int>);
static_assert(!::std::is_assignable_v<IntOptional&&, ::pltxt2htm::container::NulloptType>);

consteval bool optional_constexpr_operations_work() noexcept {
    IntOptional value{42};
    IntOptional empty{::pltxt2htm::container::nullopt};
    if (!value.has_value() || empty.has_value()) {
        return false;
    }
    if (value.value<::pltxt2htm::Contracts::ignore>() != 42 || value != 42 ||
        empty != ::pltxt2htm::container::nullopt) {
        return false;
    }
    empty = value;
    if (!empty.has_value() || empty.value<::pltxt2htm::Contracts::ignore>() != 42) {
        return false;
    }
    value = ::pltxt2htm::container::nullopt;
    empty.swap(value);
    return value == 42 && empty == ::pltxt2htm::container::nullopt;
}

static_assert(optional_constexpr_operations_work());

using NonZeroUsize = ::pltxt2htm::container::NonZeroUsize;
using NonZeroUsizeOptional = ::pltxt2htm::container::Optional<NonZeroUsize>;

static_assert(!::std::is_trivially_copy_assignable_v<NonZeroUsizeOptional>);

template<typename T>
consteval auto optional_non_zero_has_niche_representation() noexcept -> bool {
    using OptionalNonZero = ::pltxt2htm::container::Optional<::pltxt2htm::container::NonZero<T>>;

    return sizeof(OptionalNonZero) == sizeof(T) && alignof(OptionalNonZero) == alignof(T);
}

static_assert(optional_non_zero_has_niche_representation<unsigned char>());
static_assert(optional_non_zero_has_niche_representation<unsigned short>());
static_assert(optional_non_zero_has_niche_representation<unsigned>());
static_assert(optional_non_zero_has_niche_representation<unsigned long>());
static_assert(optional_non_zero_has_niche_representation<unsigned long long>());
static_assert(optional_non_zero_has_niche_representation<::std::size_t>());

consteval bool optional_non_zero_constexpr_operations_work() noexcept {
    auto const seven = NonZeroUsize::from<::pltxt2htm::Contracts::quick_enforce>(7);
    auto const eleven = NonZeroUsize::from<::pltxt2htm::Contracts::quick_enforce>(11);
    NonZeroUsizeOptional value{seven};
    NonZeroUsizeOptional empty{::pltxt2htm::container::nullopt};
    if (!value.has_value() || empty.has_value()) {
        return false;
    }
    if (value.value<::pltxt2htm::Contracts::ignore>().get<::pltxt2htm::Contracts::ignore>() != 7) {
        return false;
    }
    empty = value;
    if (!empty.has_value() ||
        empty.value<::pltxt2htm::Contracts::ignore>().get<::pltxt2htm::Contracts::ignore>() != 7) {
        return false;
    }
    value = eleven;
    empty.swap(value);
    if (empty.value<::pltxt2htm::Contracts::ignore>().get<::pltxt2htm::Contracts::ignore>() != 11 ||
        value.value<::pltxt2htm::Contracts::ignore>().get<::pltxt2htm::Contracts::ignore>() != 7) {
        return false;
    }
    empty = ::pltxt2htm::container::nullopt;
    value.swap(empty);
    return !value.has_value() && empty.has_value() &&
           empty.value<::pltxt2htm::Contracts::ignore>().get<::pltxt2htm::Contracts::ignore>() == 7;
}

static_assert(optional_non_zero_constexpr_operations_work());

namespace pltxt2htm_test {

struct ImplicitReferenceProxy {
    int* pointer;

    [[nodiscard]]
    constexpr operator int&() const noexcept {
        return *pointer;
    }
};

struct ExplicitReferenceProxy {
    int* pointer;

    [[nodiscard]]
    constexpr explicit operator int&() const noexcept {
        return *pointer;
    }
};

using OptionalReferenceArray = int[2];
using OptionalReferenceFunction = void();

void optional_reference_function();

} // namespace pltxt2htm_test

using IntReferenceOptional = ::pltxt2htm::container::Optional<int&>;
using ConstIntReferenceOptional = ::pltxt2htm::container::Optional<int const&>;
using ArrayReferenceOptional = ::pltxt2htm::container::Optional<::pltxt2htm_test::OptionalReferenceArray&>;
using FunctionReferenceOptional = ::pltxt2htm::container::Optional<::pltxt2htm_test::OptionalReferenceFunction&>;
using IntReferenceValueFunction = int&(IntReferenceOptional const&) noexcept;

static_assert(::pltxt2htm::container::is_optional<IntReferenceOptional>);
static_assert(!::std::default_initializable<IntReferenceOptional>);
static_assert(::std::same_as<IntReferenceOptional::value_type, int>);
static_assert(::std::same_as<ConstIntReferenceOptional::value_type, int const>);
static_assert(::std::same_as<IntReferenceOptional::rebind<double&>, ::pltxt2htm::container::Optional<double&>>);
static_assert(sizeof(IntReferenceOptional) == sizeof(int*));
static_assert(alignof(IntReferenceOptional) == alignof(int*));
static_assert(::std::is_trivially_copyable_v<IntReferenceOptional>);
static_assert(::std::is_trivially_copy_assignable_v<IntReferenceOptional>);
static_assert(::std::is_trivially_move_assignable_v<IntReferenceOptional>);
static_assert(::std::is_trivially_copyable_v<ConstIntReferenceOptional>);
static_assert(::std::is_copy_assignable_v<ConstIntReferenceOptional>);
static_assert(::std::is_move_assignable_v<ConstIntReferenceOptional>);
static_assert(::std::swappable<ConstIntReferenceOptional>);
static_assert(::std::is_constructible_v<IntReferenceOptional, int&>);
static_assert(!::std::is_constructible_v<IntReferenceOptional, int&&>);
static_assert(::std::is_constructible_v<ConstIntReferenceOptional, int&>);
static_assert(::std::is_constructible_v<ConstIntReferenceOptional, int const&>);
static_assert(!::std::is_constructible_v<ConstIntReferenceOptional, int&&>);
static_assert(!::std::is_constructible_v<ConstIntReferenceOptional, int const&&>);
static_assert(::std::is_assignable_v<IntReferenceOptional&, int&>);
static_assert(!::std::is_assignable_v<IntReferenceOptional&, int&&>);
static_assert(!::std::is_assignable_v<IntReferenceOptional&&, int&>);
static_assert(::std::is_convertible_v<::pltxt2htm_test::ImplicitReferenceProxy&, int&>);
static_assert(::std::is_constructible_v<IntReferenceOptional, ::pltxt2htm_test::ImplicitReferenceProxy&>);
static_assert(::std::is_convertible_v<::pltxt2htm_test::ImplicitReferenceProxy&, IntReferenceOptional>);
static_assert(::std::is_assignable_v<IntReferenceOptional&, ::pltxt2htm_test::ImplicitReferenceProxy&>);
static_assert(::std::is_constructible_v<int&, ::pltxt2htm_test::ExplicitReferenceProxy&>);
static_assert(!::std::is_convertible_v<::pltxt2htm_test::ExplicitReferenceProxy&, int&>);
static_assert(::std::is_constructible_v<IntReferenceOptional, ::pltxt2htm_test::ExplicitReferenceProxy&>);
static_assert(!::std::is_convertible_v<::pltxt2htm_test::ExplicitReferenceProxy&, IntReferenceOptional>);
static_assert(!::std::is_assignable_v<IntReferenceOptional&, ::pltxt2htm_test::ExplicitReferenceProxy&>);
static_assert(::std::is_constructible_v<ArrayReferenceOptional, ::pltxt2htm_test::OptionalReferenceArray&>);
static_assert(::std::same_as<
              decltype(::std::declval<ArrayReferenceOptional const&>().value<::pltxt2htm::Contracts::quick_enforce>()),
              ::pltxt2htm_test::OptionalReferenceArray&>);
static_assert(::std::is_constructible_v<FunctionReferenceOptional, ::pltxt2htm_test::OptionalReferenceFunction&>);
static_assert(::std::same_as<decltype(::std::declval<FunctionReferenceOptional const&>()
                                          .value<::pltxt2htm::Contracts::quick_enforce>()),
                             ::pltxt2htm_test::OptionalReferenceFunction&>);

static_assert(::std::same_as<
              decltype(::std::declval<IntReferenceOptional&>().value<::pltxt2htm::Contracts::quick_enforce>()), int&>);
static_assert(
    ::std::same_as<
        decltype(::std::declval<IntReferenceOptional const&>().value<::pltxt2htm::Contracts::quick_enforce>()), int&>);
static_assert(::std::same_as<
              decltype(::std::declval<IntReferenceOptional&&>().value<::pltxt2htm::Contracts::quick_enforce>()), int&>);
static_assert(
    ::std::same_as<
        decltype(::std::declval<IntReferenceOptional const&&>().value<::pltxt2htm::Contracts::quick_enforce>()), int&>);
static_assert(::std::same_as<decltype(::std::declval<IntReferenceOptional const&>().value_or(0)), int>);
static_assert(noexcept(::std::declval<IntReferenceOptional const&>().value_or(0)));
static_assert(noexcept(::std::declval<ConstIntReferenceOptional const&>().value_or(0)));
static_assert(::std::same_as<decltype(&IntReferenceOptional::template value<::pltxt2htm::Contracts::ignore>),
                             IntReferenceValueFunction*>);

consteval bool optional_reference_constexpr_operations_work() noexcept {
    int first{7};
    int second{11};
    int equal_to_first{7};
    IntReferenceOptional value{first};
    IntReferenceOptional empty{::pltxt2htm::container::nullopt};
    IntReferenceOptional equal_value{equal_to_first};
    ::pltxt2htm_test::ImplicitReferenceProxy implicit_proxy{::std::addressof(first)};
    ::pltxt2htm_test::ExplicitReferenceProxy explicit_proxy{::std::addressof(second)};
    IntReferenceOptional implicit_proxy_value = implicit_proxy;
    IntReferenceOptional explicit_proxy_value{explicit_proxy};
    if (!value.has_value() || empty.has_value() || value != equal_value) {
        return false;
    }
    if (empty.value_or(17) != 17) {
        return false;
    }
    if (::std::addressof(implicit_proxy_value.value<::pltxt2htm::Contracts::ignore>()) != ::std::addressof(first) ||
        ::std::addressof(explicit_proxy_value.value<::pltxt2htm::Contracts::ignore>()) != ::std::addressof(second)) {
        return false;
    }
    value.value<::pltxt2htm::Contracts::ignore>() = 9;
    if (first != 9 || value.value_or(0) != 9) {
        return false;
    }
    empty = value;
    if (::std::addressof(empty.value<::pltxt2htm::Contracts::ignore>()) != ::std::addressof(first)) {
        return false;
    }
    value = second;
    if (::std::addressof(value.value<::pltxt2htm::Contracts::ignore>()) != ::std::addressof(second) || first != 9) {
        return false;
    }
    static_cast<IntReferenceOptional const&>(value).value<::pltxt2htm::Contracts::ignore>() = 13;
    if (second != 13) {
        return false;
    }
    value.swap(empty);
    if (::std::addressof(value.value<::pltxt2htm::Contracts::ignore>()) != ::std::addressof(first) ||
        ::std::addressof(empty.value<::pltxt2htm::Contracts::ignore>()) != ::std::addressof(second)) {
        return false;
    }
    value = ::pltxt2htm::container::nullopt;
    return value == ::pltxt2htm::container::nullopt && empty == 13;
}

static_assert(optional_reference_constexpr_operations_work());

namespace pltxt2htm_test {

struct OptionalTrackedValue {
    int value{};

    constexpr OptionalTrackedValue() noexcept = default;

    constexpr explicit OptionalTrackedValue(int value_) noexcept
        : value{value_} {
    }

    constexpr OptionalTrackedValue(OptionalTrackedValue const&) noexcept = default;

    constexpr OptionalTrackedValue(OptionalTrackedValue&& other) noexcept
        : value{other.value} {
        other.value = -1;
    }

    constexpr auto operator=(this OptionalTrackedValue& self, OptionalTrackedValue const&) noexcept
        -> OptionalTrackedValue& = default;

    constexpr auto operator=(this OptionalTrackedValue& self, OptionalTrackedValue&& other) noexcept
        -> OptionalTrackedValue& {
        self.value = other.value;
        other.value = -1;
        return self;
    }

    constexpr bool operator==(this OptionalTrackedValue const&, OptionalTrackedValue const&) noexcept = default;

    constexpr ~OptionalTrackedValue() noexcept {
    }
};

struct OptionalMoveOnlyValue {
    int value{};

    constexpr explicit OptionalMoveOnlyValue(int value_) noexcept
        : value{value_} {
    }

    constexpr OptionalMoveOnlyValue(OptionalMoveOnlyValue const&) = delete;

    constexpr OptionalMoveOnlyValue(OptionalMoveOnlyValue&& other) noexcept
        : value{other.value} {
        other.value = -1;
    }

    constexpr auto operator=(this OptionalMoveOnlyValue& self, OptionalMoveOnlyValue const&)
        -> OptionalMoveOnlyValue& = delete;

    constexpr auto operator=(this OptionalMoveOnlyValue& self, OptionalMoveOnlyValue&& other) noexcept
        -> OptionalMoveOnlyValue& {
        self.value = other.value;
        other.value = -1;
        return self;
    }

    constexpr ~OptionalMoveOnlyValue() noexcept {
    }
};

struct OptionalCopyOnlyValue {
    int value{};

    constexpr explicit OptionalCopyOnlyValue(int value_) noexcept
        : value{value_} {
    }

    constexpr OptionalCopyOnlyValue(OptionalCopyOnlyValue const&) noexcept = default;

    constexpr OptionalCopyOnlyValue(OptionalCopyOnlyValue&&) = delete;

    constexpr auto operator=(this OptionalCopyOnlyValue& self, OptionalCopyOnlyValue const& other) noexcept
        -> OptionalCopyOnlyValue& {
        self.value = other.value;
        return self;
    }

    constexpr auto operator=(this OptionalCopyOnlyValue&, OptionalCopyOnlyValue&&) -> OptionalCopyOnlyValue& = delete;

    constexpr ~OptionalCopyOnlyValue() noexcept {
    }
};

struct OptionalThrowingValue {
    int value{};

    constexpr explicit OptionalThrowingValue(int value_) noexcept
        : value{value_} {
    }

    constexpr OptionalThrowingValue(OptionalThrowingValue const& other) noexcept(false)
        : value{other.value} {
    }

    constexpr OptionalThrowingValue(OptionalThrowingValue&& other) noexcept(false)
        : value{other.value} {
        other.value = -1;
    }

    constexpr ~OptionalThrowingValue() noexcept {
    }
};

struct OptionalNothrowImplicitFallback;

struct OptionalExplicitThrowingValue {
    int value{};

    constexpr OptionalExplicitThrowingValue() noexcept = default;

    constexpr explicit OptionalExplicitThrowingValue(OptionalNothrowImplicitFallback const&) noexcept(false)
        : value{1} {
    }

    constexpr OptionalExplicitThrowingValue(OptionalExplicitThrowingValue const&) noexcept = default;

    constexpr OptionalExplicitThrowingValue(OptionalExplicitThrowingValue&&) noexcept = default;

    constexpr ~OptionalExplicitThrowingValue() noexcept {
    }
};

struct OptionalNothrowImplicitFallback {
    [[nodiscard]]
    constexpr operator OptionalExplicitThrowingValue() const noexcept {
        return OptionalExplicitThrowingValue{};
    }
};

struct OptionalThrowingEqualityResult {
    [[nodiscard]]
    constexpr operator bool() const noexcept(false) {
        return true;
    }
};

struct OptionalThrowingEqualityValue {
    [[nodiscard]]
    constexpr auto operator==(this OptionalThrowingEqualityValue const&, OptionalThrowingEqualityValue const&) noexcept
        -> OptionalThrowingEqualityResult {
        return {};
    }

    [[nodiscard]]
    constexpr auto operator!=(this OptionalThrowingEqualityValue const&, OptionalThrowingEqualityValue const&) noexcept
        -> OptionalThrowingEqualityResult {
        return {};
    }
};

template<typename OptionalType, typename Fallback>
concept can_call_value_or = requires(OptionalType&& optional, Fallback&& fallback) {
    ::std::forward<OptionalType>(optional).value_or(::std::forward<Fallback>(fallback));
};

} // namespace pltxt2htm_test

using MoveOnlyOptional = ::pltxt2htm::container::Optional<::pltxt2htm_test::OptionalMoveOnlyValue>;
using CopyOnlyOptional = ::pltxt2htm::container::Optional<::pltxt2htm_test::OptionalCopyOnlyValue>;
using ThrowingOptional = ::pltxt2htm::container::Optional<::pltxt2htm_test::OptionalThrowingValue>;
using ExplicitThrowingOptional = ::pltxt2htm::container::Optional<::pltxt2htm_test::OptionalExplicitThrowingValue>;
using ThrowingReferenceOptional = ::pltxt2htm::container::Optional<::pltxt2htm_test::OptionalThrowingValue&>;
using ExplicitThrowingReferenceOptional =
    ::pltxt2htm::container::Optional<::pltxt2htm_test::OptionalExplicitThrowingValue&>;
using ThrowingEqualityOptional = ::pltxt2htm::container::Optional<::pltxt2htm_test::OptionalThrowingEqualityValue>;

static_assert(!::pltxt2htm_test::can_call_value_or<MoveOnlyOptional&, ::pltxt2htm_test::OptionalMoveOnlyValue>);
static_assert(::pltxt2htm_test::can_call_value_or<MoveOnlyOptional&&, ::pltxt2htm_test::OptionalMoveOnlyValue>);
static_assert(::std::is_copy_constructible_v<::pltxt2htm_test::OptionalCopyOnlyValue>);
static_assert(!::std::is_move_constructible_v<::pltxt2htm_test::OptionalCopyOnlyValue>);
static_assert(::std::is_copy_assignable_v<CopyOnlyOptional>);
static_assert(::pltxt2htm_test::can_call_value_or<CopyOnlyOptional&, ::pltxt2htm_test::OptionalCopyOnlyValue&>);
static_assert(!::pltxt2htm_test::can_call_value_or<CopyOnlyOptional&&, ::pltxt2htm_test::OptionalCopyOnlyValue&>);

consteval bool optional_copy_only_assignment_works() noexcept {
    ::pltxt2htm_test::OptionalCopyOnlyValue source_value{17};
    ::pltxt2htm_test::OptionalCopyOnlyValue target_value{23};
    CopyOnlyOptional source{source_value};
    CopyOnlyOptional target{::pltxt2htm::container::nullopt};
    target = source;
    if (!target.has_value() || target.value<::pltxt2htm::Contracts::ignore>().value != 17) {
        return false;
    }
    CopyOnlyOptional other{target_value};
    target = other;
    if (target.value<::pltxt2htm::Contracts::ignore>().value != 23) {
        return false;
    }
    CopyOnlyOptional empty{::pltxt2htm::container::nullopt};
    target = empty;
    return !target.has_value();
}

static_assert(optional_copy_only_assignment_works());
static_assert(!noexcept(::std::declval<ThrowingOptional const&>().value_or(
    ::std::declval<::pltxt2htm_test::OptionalThrowingValue const&>())));
static_assert(!noexcept(
    ::std::declval<ThrowingOptional&&>().value_or(::std::declval<::pltxt2htm_test::OptionalThrowingValue&&>())));
static_assert(!noexcept(
    ::std::declval<ThrowingOptional const&&>().value_or(::std::declval<::pltxt2htm_test::OptionalThrowingValue&&>())));
static_assert(::std::is_nothrow_convertible_v<::pltxt2htm_test::OptionalNothrowImplicitFallback&,
                                              ::pltxt2htm_test::OptionalExplicitThrowingValue>);
static_assert(!noexcept(static_cast<::pltxt2htm_test::OptionalExplicitThrowingValue>(
    ::std::declval<::pltxt2htm_test::OptionalNothrowImplicitFallback&>())));
static_assert(!noexcept(::std::declval<ExplicitThrowingOptional const&>().value_or(
    ::std::declval<::pltxt2htm_test::OptionalNothrowImplicitFallback&>())));
static_assert(!noexcept(::std::declval<ThrowingReferenceOptional const&>().value_or(
    ::std::declval<::pltxt2htm_test::OptionalThrowingValue const&>())));
static_assert(!noexcept(::std::declval<ExplicitThrowingReferenceOptional const&>().value_or(
    ::std::declval<::pltxt2htm_test::OptionalNothrowImplicitFallback&>())));
static_assert(::std::equality_comparable<::pltxt2htm_test::OptionalThrowingEqualityValue>);
static_assert(noexcept(::std::declval<::pltxt2htm_test::OptionalThrowingEqualityValue const&>() ==
                       ::std::declval<::pltxt2htm_test::OptionalThrowingEqualityValue const&>()));
static_assert(!noexcept(static_cast<bool>(::std::declval<::pltxt2htm_test::OptionalThrowingEqualityValue const&>() ==
                                          ::std::declval<::pltxt2htm_test::OptionalThrowingEqualityValue const&>())));
static_assert(noexcept(::std::declval<IntOptional const&>() == ::std::declval<IntOptional const&>()));
static_assert(noexcept(::std::declval<IntOptional const&>() == ::std::declval<int const&>()));
static_assert(noexcept(::std::declval<IntOptional const&>() == ::pltxt2htm::container::nullopt));
static_assert(!noexcept(::std::declval<ThrowingEqualityOptional const&>() ==
                        ::std::declval<ThrowingEqualityOptional const&>()));
static_assert(!noexcept(::std::declval<ThrowingEqualityOptional const&>() ==
                        ::std::declval<::pltxt2htm_test::OptionalThrowingEqualityValue const&>()));

int main() {
    IntOptional empty{::pltxt2htm::container::nullopt};
    pltxt2htm_test_assert_false(empty.has_value());
    pltxt2htm_test_assert_true(empty == ::pltxt2htm::container::nullopt);
    pltxt2htm_test_assert_true(::pltxt2htm::container::nullopt == empty);

    IntOptional value{42};
    pltxt2htm_test_assert_true(value.has_value());
    pltxt2htm_test_assert_true(value.value<::pltxt2htm::Contracts::quick_enforce>() == 42);
    pltxt2htm_test_assert_true(value == 42);

    int fallback{7};
    int const const_fallback{9};
    int empty_result{empty.value_or(fallback)};
    int value_result{value.value_or(fallback)};
    pltxt2htm_test_assert_true(empty_result == 7 && value_result == 42);
    empty_result = 8;
    value_result = 43;
    pltxt2htm_test_assert_true(empty_result == 8 && value_result == 43 && fallback == 7);
    pltxt2htm_test_assert_true(value.value<::pltxt2htm::Contracts::quick_enforce>() == 42);
    pltxt2htm_test_assert_true(static_cast<IntOptional const&>(empty).value_or(const_fallback) == const_fallback);
    pltxt2htm_test_assert_true(IntOptional{::pltxt2htm::container::nullopt}.value_or(int{11}) == 11);

    int first_reference_value{21};
    int second_reference_value{34};
    IntReferenceOptional reference{first_reference_value};
    reference.value<::pltxt2htm::Contracts::quick_enforce>() = 22;
    pltxt2htm_test_assert_true(first_reference_value == 22);
    reference = second_reference_value;
    pltxt2htm_test_assert_true(::std::addressof(reference.value<::pltxt2htm::Contracts::quick_enforce>()) ==
                               ::std::addressof(second_reference_value));
    pltxt2htm_test_assert_true(first_reference_value == 22);

    using TrackedOptional = ::pltxt2htm::container::Optional<::pltxt2htm_test::OptionalTrackedValue>;

    TrackedOptional state{::pltxt2htm_test::OptionalTrackedValue{42}};
    TrackedOptional other{::pltxt2htm::container::nullopt};
    state.swap(other);
    pltxt2htm_test_assert_false(state.has_value());
    pltxt2htm_test_assert_true(other.has_value());
    pltxt2htm_test_assert_true(other.value<::pltxt2htm::Contracts::quick_enforce>().value == 42);

    state.swap(other);
    pltxt2htm_test_assert_true(state.has_value());
    pltxt2htm_test_assert_false(other.has_value());

    state.swap(state);
    pltxt2htm_test_assert_true(state.has_value());
    pltxt2htm_test_assert_true(state.value<::pltxt2htm::Contracts::quick_enforce>().value == 42);

    other = state;
    pltxt2htm_test_assert_true(other.has_value());
    pltxt2htm_test_assert_true(other.value<::pltxt2htm::Contracts::quick_enforce>().value == 42);

    other = ::pltxt2htm::container::nullopt;
    pltxt2htm_test_assert_false(other.has_value());
    other = ::pltxt2htm_test::OptionalTrackedValue{9};
    pltxt2htm_test_assert_true(other.has_value());
    pltxt2htm_test_assert_true(other.value<::pltxt2htm::Contracts::quick_enforce>().value == 9);

    TrackedOptional moved{::std::move(other)};
    pltxt2htm_test_assert_true(moved.has_value());
    pltxt2htm_test_assert_true(moved.value<::pltxt2htm::Contracts::quick_enforce>().value == 9);

    TrackedOptional moved_assignment{::pltxt2htm_test::OptionalTrackedValue{13}};
    moved_assignment = ::std::move(moved);
    pltxt2htm_test_assert_true(moved_assignment.has_value());
    pltxt2htm_test_assert_true(moved_assignment.value<::pltxt2htm::Contracts::quick_enforce>().value == 9);

    ::pltxt2htm_test::OptionalTrackedValue tracked_fallback{7};
    auto copied_value = moved_assignment.value_or(tracked_fallback);
    copied_value.value = 14;
    pltxt2htm_test_assert_true(copied_value.value == 14 &&
                               moved_assignment.value<::pltxt2htm::Contracts::quick_enforce>().value == 9 &&
                               tracked_fallback.value == 7);

    TrackedOptional empty_tracked{::pltxt2htm::container::nullopt};
    auto copied_fallback = empty_tracked.value_or(tracked_fallback);
    copied_fallback.value = 15;
    pltxt2htm_test_assert_true(copied_fallback.value == 15 && tracked_fallback.value == 7);

    auto moved_fallback = ::std::move(empty_tracked).value_or(::std::move(tracked_fallback));
    pltxt2htm_test_assert_true(moved_fallback.value == 7 && tracked_fallback.value == -1);

    MoveOnlyOptional move_only{::pltxt2htm_test::OptionalMoveOnlyValue{21}};
    auto moved_value = ::std::move(move_only).value_or(::pltxt2htm_test::OptionalMoveOnlyValue{34});
    pltxt2htm_test_assert_true(moved_value.value == 21);
    pltxt2htm_test_assert_true(move_only.value<::pltxt2htm::Contracts::quick_enforce>().value == -1);

    return 0;
}
