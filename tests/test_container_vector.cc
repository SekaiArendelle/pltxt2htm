#include <concepts>
#include <type_traits>

#include <fast_io/fast_io_core.h>

#include <pltxt2htm/container/vector.hh>

#include "precompile.hh"

using IntVector = ::pltxt2htm::container::Vector<int>;

static_assert(::std::same_as<IntVector::allocator_type, ::fast_io::native_global_allocator>);
static_assert(::std::same_as<IntVector::value_type, int>);
static_assert(::std::same_as<IntVector::iterator, int*>);
static_assert(::std::ranges::contiguous_range<IntVector>);
static_assert(::std::ranges::contiguous_range<IntVector const>);

consteval auto test_constexpr_vector() -> bool {
    IntVector values{};
    if (!values.empty() || values.size() != 0 || values.capacity() != 0 || values.data() != nullptr) {
        return false;
    }
    if (values.erase(values.begin(), values.end()) != values.end()) {
        return false;
    }

    values.push_back(1);
    values.emplace_back(2);
    values.reserve(8);
    if (values.size() != 2 || values.capacity() < 8 ||
        values.template front<::pltxt2htm::Contracts::quick_enforce>() != 1 ||
        values.template index<::pltxt2htm::Contracts::quick_enforce>(1) != 2) {
        return false;
    }

    IntVector suffix{3, 4};
    values.append_range(::std::move(suffix));
    if (values != IntVector{1, 2, 3, 4}) {
        return false;
    }

    values.erase(values.begin() + 1);
    if (values != IntVector{1, 3, 4}) {
        return false;
    }

    values.erase(values.begin() + 1, values.end());
    if (values.erase(values.begin(), values.begin()) != values.begin()) {
        return false;
    }
    IntVector copy{values};
    copy.template index<::pltxt2htm::Contracts::ignore>(0) = 9;
    return values == IntVector{1} && copy == IntVector{9};
}

static_assert(test_constexpr_vector());

int main() {
    IntVector original{1, 2, 3};
    IntVector copy{};
    copy = original;
    copy.template index<::pltxt2htm::Contracts::quick_enforce>(0) = 4;
    pltxt2htm_test_assert_true(original == IntVector{1, 2, 3});
    pltxt2htm_test_assert_true(copy == IntVector{4, 2, 3});

    IntVector moved{};
    moved = ::std::move(copy);
    pltxt2htm_test_assert_true(copy.empty());
    pltxt2htm_test_assert_true(moved == IntVector{4, 2, 3});
    moved.pop_back();
    moved.clear();
    pltxt2htm_test_assert_true(moved.empty());

    return 0;
}
