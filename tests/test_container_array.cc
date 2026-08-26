#include <compare>
#include <concepts>
#include <cstddef>
#include <string_view>
#include <type_traits>

#if !defined(_WIN32) && __has_include(<sys/wait.h>) && __has_include(<unistd.h>)
    #include <sys/wait.h>
    #include <unistd.h>
#endif

#include <pltxt2htm/container/array.hh>

#include "precompile.hh"

template<::std::size_t extent>
concept can_form_array = requires { typename ::pltxt2htm::container::Array<int, extent>; };

template<typename T>
concept can_subscript = requires(T& value) { value[0]; };

template<typename T>
concept can_call_empty = requires(T const& value) { value.empty(); };

template<typename T>
concept can_call_is_empty = requires(T const& value) { value.is_empty(); };

template<typename T>
concept can_call_front_without_contract = requires(T& value) { value.front(); };

template<typename T>
concept can_call_back_without_contract = requires(T& value) { value.back(); };

using IntArray = ::pltxt2htm::container::Array<int, 3>;

static_assert(can_form_array<1>);
static_assert(!can_form_array<0>);
static_assert(::std::is_aggregate_v<IntArray>);
static_assert(::std::is_standard_layout_v<IntArray>);
static_assert(::std::is_trivially_copyable_v<IntArray>);
static_assert(!can_subscript<IntArray>);
static_assert(!can_call_empty<IntArray>);
static_assert(!can_call_is_empty<IntArray>);
static_assert(!can_call_front_without_contract<IntArray>);
static_assert(!can_call_back_without_contract<IntArray>);

consteval auto test_constexpr_array() noexcept -> bool {
    auto values = ::pltxt2htm::container::Array{1, 2, 3};
    static_assert(::std::same_as<decltype(values), IntArray>);
    if (values.size() != 3 || values.size_bytes() != 3 * sizeof(int) || values.max_size() != 3 ||
        values.max_size_bytes() != 3 * sizeof(int)) {
        return false;
    }
    if (values.data() != values.begin() || values.end() - values.begin() != 3 || values.cend() - values.cbegin() != 3 ||
        values.template front<::pltxt2htm::Contracts::quick_enforce>() != 1 ||
        values.template back<::pltxt2htm::Contracts::ignore>() != 3 || *values.rbegin() != 3 ||
        *values.crbegin() != 3) {
        return false;
    }

    values.template index<::pltxt2htm::Contracts::quick_enforce>(1) = 4;
    if (values.template index<::pltxt2htm::Contracts::ignore>(1) != 4 ||
        values.template index<::pltxt2htm::Contracts::quick_enforce>(2) != 3) {
        return false;
    }

    auto other = ::pltxt2htm::container::Array{7, 8, 9};
    values.swap(other);
    if (values != ::pltxt2htm::container::Array{7, 8, 9} || other != ::pltxt2htm::container::Array{1, 4, 3}) {
        return false;
    }
    values.fill(5);
    if (values != ::pltxt2htm::container::Array{5, 5, 5}) {
        return false;
    }

    int source[]{10, 11, 12};
    auto copied = ::pltxt2htm::container::to_array(source);
    if (copied != ::pltxt2htm::container::Array{10, 11, 12}) {
        return false;
    }
    ::pltxt2htm::container::swap(copied, other);
    return copied == ::pltxt2htm::container::Array{1, 4, 3} && other == ::pltxt2htm::container::Array{10, 11, 12} &&
           (::pltxt2htm::container::Array{1, 2} <=> ::pltxt2htm::container::Array{1, 3}) ==
               ::std::strong_ordering::less &&
           ::pltxt2htm::container::Array{1, 2} != ::pltxt2htm::container::Array{1, 2, 3};
}

static_assert(test_constexpr_array());

int main() {
    auto const strings = ::pltxt2htm::container::Array{"first", "second", nullptr};
    static_assert(
        ::std::same_as<::std::remove_cvref_t<decltype(strings)>, ::pltxt2htm::container::Array<char const*, 3>>);
    pltxt2htm_test_assert_true(::std::string_view{strings.template index<::pltxt2htm::Contracts::quick_enforce>(1)} ==
                               "second");

#if !defined(_WIN32) && __has_include(<sys/wait.h>) && __has_include(<unistd.h>)
    auto const process_id = ::fork();
    if (process_id == -1) [[unlikely]] {
        ::pltxt2htm::details::trap();
    }
    if (process_id == 0) {
        static_cast<void>(strings.template index<::pltxt2htm::Contracts::quick_enforce>(strings.size()));
    }

    int wait_status{};
    ::waitpid(process_id, ::std::addressof(wait_status), 0);
    pltxt2htm_test_assert_true(WIFSIGNALED(wait_status));
#endif

    return 0;
}
