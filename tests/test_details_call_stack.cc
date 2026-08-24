#include <concepts>
#include <utility>

#include <pltxt2htm/details/call_stack.hh>

#include "precompile.hh"

using Contracts = ::pltxt2htm::Contracts;
using IntCallStack = ::pltxt2htm::details::CallStack<int>;

template<typename CallStack>
concept has_unchecked_operation = requires(CallStack& call_stack) {
    call_stack.current_frame_unchecked();
    call_stack.discard_current_frame_unchecked();
    call_stack.pop_frame_unchecked();
};

template<typename CallStack>
concept has_current_frame_without_contract = requires(CallStack& call_stack) { call_stack.current_frame(); };

template<typename CallStack>
concept has_discard_without_contract = requires(CallStack& call_stack) { call_stack.discard_current_frame(); };

template<typename CallStack>
concept has_get_frames = requires(CallStack& call_stack) { call_stack.get_frames(); };

template<typename CallStack>
concept exposes_frames = requires(CallStack& call_stack) { call_stack.frames; };

template<typename CallStack>
concept accepts_lvalue_push_frame =
    requires(CallStack& call_stack, typename CallStack::frame_type const& frame) { call_stack.push_frame(frame); };

static_assert(::std::same_as<IntCallStack::frame_type, int>);
static_assert(!has_unchecked_operation<IntCallStack>);
static_assert(!has_current_frame_without_contract<IntCallStack>);
static_assert(!has_discard_without_contract<IntCallStack>);
static_assert(!has_get_frames<IntCallStack>);
static_assert(!exposes_frames<IntCallStack>);
static_assert(!accepts_lvalue_push_frame<IntCallStack>);

static_assert(noexcept(IntCallStack{}));
static_assert(noexcept(::std::declval<IntCallStack&>().push_frame(1)));
static_assert(noexcept(::std::declval<IntCallStack&>().template current_frame<Contracts::quick_enforce>()));
static_assert(noexcept(::std::declval<IntCallStack const&>().template current_frame<Contracts::quick_enforce>()));
static_assert(noexcept(::std::declval<IntCallStack const&>().empty()));
static_assert(noexcept(::std::declval<IntCallStack const&>().frame_count()));
static_assert(noexcept(::std::declval<IntCallStack const&>().is_root()));
static_assert(noexcept(::std::declval<IntCallStack const&>().has_parent()));
static_assert(noexcept(::std::declval<IntCallStack&>().template discard_current_frame<Contracts::quick_enforce>()));
static_assert(noexcept(::std::declval<IntCallStack&>().template pop_frame<Contracts::quick_enforce>()));
static_assert(noexcept(::std::declval<IntCallStack const&>().contains_frame_if([](int const&) noexcept {
    return true;
})));

consteval auto test_constexpr_empty_stack() noexcept -> bool {
    IntCallStack call_stack{};
    return call_stack.empty() && call_stack.frame_count() == 0 && !call_stack.is_root() && !call_stack.has_parent();
}

static_assert(test_constexpr_empty_stack());

constexpr auto test_call_stack() noexcept -> bool {
    IntCallStack call_stack{};

    call_stack.push_frame(1);
    if (!call_stack.is_root() || call_stack.has_parent()) {
        return false;
    }
    call_stack.push_frame(2);
    call_stack.push_frame(3);
    if (call_stack.frame_count() != 3 || !call_stack.has_parent() ||
        call_stack.current_frame<Contracts::quick_enforce>() != 3) {
        return false;
    }
    if (!call_stack.contains_frame_if([](int const& value) noexcept { return value == 2; }) ||
        call_stack.contains_frame_if([](int const& value) noexcept { return value == 4; })) {
        return false;
    }

    if (call_stack.pop_frame<Contracts::quick_enforce>() != 3) {
        return false;
    }
    call_stack.discard_current_frame<Contracts::quick_enforce>();

    auto const& const_call_stack{call_stack};
    return const_call_stack.is_root() && !const_call_stack.has_parent() &&
           const_call_stack.current_frame<Contracts::quick_enforce>() == 1;
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
    pltxt2htm_test_assert_true(test_call_stack());

    ::pltxt2htm::details::CallStack<NothrowMovable> call_stack{};
    call_stack.push_frame(NothrowMovable{42});
    auto frame = call_stack.pop_frame<Contracts::quick_enforce>();

    pltxt2htm_test_assert_true(frame.value == 42);
    pltxt2htm_test_assert_true(call_stack.empty());
    return 0;
}
