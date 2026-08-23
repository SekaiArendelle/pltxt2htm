#include <array>
#include <concepts>
#include <vector>

#include <fast_io/fast_io_dsal/deque.h>
#include <fast_io/fast_io_dsal/vector.h>

#include <pltxt2htm/container/stack.hh>

#include "precompile.hh"

using Contracts = ::pltxt2htm::Contracts;
using IntStack = ::pltxt2htm::container::Stack<int>;

template<typename Stack>
concept has_unchecked_operation = requires(Stack& stack) {
    stack.top_unchecked();
    stack.pop_unchecked();
    stack.pop_element_unchecked();
};

template<typename Stack>
concept has_top_without_contract = requires(Stack& stack) { stack.top(); };

template<typename Stack>
concept has_pop_without_contract = requires(Stack& stack) { stack.pop(); };

static_assert(::std::same_as<IntStack::container_type, ::fast_io::deque<int>>);
static_assert(::std::same_as<IntStack::value_type, int>);
static_assert(!has_unchecked_operation<IntStack>);
static_assert(!has_top_without_contract<IntStack>);
static_assert(!has_pop_without_contract<IntStack>);

consteval auto test_constexpr_stack() noexcept -> bool {
    ::pltxt2htm::container::Stack<int, ::std::vector<int>> stack{};
    if (!stack.empty() || !stack.is_empty() || stack.size() != 0) {
        return false;
    }

    stack.push(1);
    stack.emplace(2);
    if (stack.size() != 2 || stack.top<Contracts::quick_enforce>() != 2) {
        return false;
    }

    stack.pop<Contracts::quick_enforce>();
    stack.push(3);
    stack.push(4);
    if (stack.top<Contracts::quick_enforce>() != 4 || stack.pop_element<Contracts::quick_enforce>() != 4) {
        return false;
    }

    auto const& const_stack{stack};
    if (const_stack.top<Contracts::quick_enforce>() != 3) {
        return false;
    }

    stack.clear();
    return stack.empty();
}

static_assert(test_constexpr_stack());

int main() {
    constexpr ::std::array values{1, 2, 3};
    IntStack from_range{::fast_io::freestanding::from_range, values};
    pltxt2htm_test_assert_true(from_range.size() == values.size());
    pltxt2htm_test_assert_true(from_range.top<Contracts::quick_enforce>() == 3);

    ::pltxt2htm::container::Stack<int, ::fast_io::vector<int>> reserved{};
    reserved.reserve(64);
    reserved.push(42);
    pltxt2htm_test_assert_true(reserved.get_container().capacity() >= 64);
    pltxt2htm_test_assert_true(reserved.top<Contracts::quick_enforce>() == 42);

    IntStack pushed_range{};
    pushed_range.push_range(values);
    pltxt2htm_test_assert_true(pushed_range.pop_element<Contracts::quick_enforce>() == 3);
    pushed_range.clear_destroy();
    pltxt2htm_test_assert_true(pushed_range.empty());

    IntStack other{};
    other.push(4);
    from_range.swap(other);
    pltxt2htm_test_assert_true(from_range.top<Contracts::quick_enforce>() == 4);
    pltxt2htm_test_assert_true(other.top<Contracts::quick_enforce>() == 3);

    return 0;
}
