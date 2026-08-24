#include <concepts>
#include <utility>

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

template<typename Stack>
concept has_get_container = requires(Stack& stack) { stack.get_container(); };

template<typename Stack>
concept exposes_container = requires(Stack& stack) { stack.container; };

template<typename Stack>
concept accepts_lvalue_push = requires(Stack& stack, typename Stack::value_type const& value) { stack.push(value); };

static_assert(::std::same_as<IntStack::value_type, int>);
static_assert(!has_unchecked_operation<IntStack>);
static_assert(!has_top_without_contract<IntStack>);
static_assert(!has_pop_without_contract<IntStack>);
static_assert(!has_get_container<IntStack>);
static_assert(!exposes_container<IntStack>);
static_assert(!accepts_lvalue_push<IntStack>);

static_assert(noexcept(IntStack{}));
static_assert(noexcept(::std::declval<IntStack&>().push(1)));
static_assert(noexcept(::std::declval<IntStack&>().template top<Contracts::quick_enforce>()));
static_assert(noexcept(::std::declval<IntStack const&>().template top<Contracts::quick_enforce>()));
static_assert(noexcept(::std::declval<IntStack const&>().empty()));
static_assert(noexcept(::std::declval<IntStack const&>().size()));
static_assert(noexcept(::std::declval<IntStack&>().template pop<Contracts::quick_enforce>()));
static_assert(noexcept(::std::declval<IntStack&>().template pop_element<Contracts::quick_enforce>()));
static_assert(noexcept(::std::declval<IntStack const&>().contains_if([](int const&) noexcept { return true; })));

consteval auto test_constexpr_empty_stack() noexcept -> bool {
    IntStack stack{};
    return stack.empty() && stack.size() == 0;
}

static_assert(test_constexpr_empty_stack());

constexpr auto test_stack() noexcept -> bool {
    IntStack stack{};

    stack.push(1);
    stack.push(2);
    stack.push(3);
    if (stack.size() != 3 || stack.top<Contracts::quick_enforce>() != 3) {
        return false;
    }
    if (!stack.contains_if([](int const& value) noexcept { return value == 2; }) ||
        stack.contains_if([](int const& value) noexcept { return value == 4; })) {
        return false;
    }

    if (stack.pop_element<Contracts::quick_enforce>() != 3) {
        return false;
    }
    stack.pop<Contracts::quick_enforce>();

    auto const& const_stack{stack};
    return const_stack.size() == 1 && const_stack.top<Contracts::quick_enforce>() == 1;
}

struct NothrowMovable {
    int value{};

    constexpr explicit NothrowMovable(int value_) noexcept
        : value{value_} {
    }

    NothrowMovable(NothrowMovable const&) = delete;

    constexpr NothrowMovable(NothrowMovable&& other) noexcept
        : value{other.value} {
        other.value = 0;
    }

    constexpr auto operator=(this NothrowMovable& self, NothrowMovable&& other) noexcept -> NothrowMovable& {
        self.value = other.value;
        other.value = 0;
        return self;
    }
};

int main() {
    pltxt2htm_test_assert_true(test_stack());

    ::pltxt2htm::container::Stack<NothrowMovable> stack{};
    stack.push(NothrowMovable{42});
    auto element = stack.pop_element<Contracts::quick_enforce>();

    pltxt2htm_test_assert_true(element.value == 42);
    pltxt2htm_test_assert_true(stack.empty());
    return 0;
}
