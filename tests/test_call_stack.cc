#include "precompile.hh"
#include <pltxt2htm/details/call_stack.hh>

namespace {

class Tracked {
public:
    int value{};
    int* alive{};

    constexpr Tracked(int const value_, int& alive_) noexcept
        : value{value_},
          alive{::std::addressof(alive_)} {
        ++(*alive);
    }

    constexpr Tracked(Tracked const&) noexcept = delete;
    constexpr auto operator=(Tracked const&) noexcept -> Tracked& = delete;

    constexpr Tracked(Tracked&& other) noexcept
        : value{other.value},
          alive{other.alive} {
        other.alive = nullptr;
    }

    constexpr auto operator=(Tracked&& other) noexcept -> Tracked& = delete;

    constexpr ~Tracked() noexcept {
        if (alive != nullptr) {
            --(*alive);
        }
    }
};

} // namespace

int main() {
    int alive{};
    {
        ::pltxt2htm::details::CallStack<Tracked, 2> stack{};
        pltxt2htm_test_assert_true(stack.empty());
        pltxt2htm_test_assert_true(stack.size() == 0);
        pltxt2htm_test_assert_true(stack.capacity() == 2);

        pltxt2htm_test_assert_true(stack.try_emplace(1, alive));
        pltxt2htm_test_assert_true(stack.template top<::pltxt2htm::Contracts::quick_enforce>().value == 1);
        pltxt2htm_test_assert_true(alive == 1);

        pltxt2htm_test_assert_true(stack.try_emplace(2, alive));
        pltxt2htm_test_assert_true(stack.is_full());
        pltxt2htm_test_assert_true(stack.template top<::pltxt2htm::Contracts::quick_enforce>().value == 2);
        pltxt2htm_test_assert_true(alive == 2);

        pltxt2htm_test_assert_true(stack.try_emplace(3, alive) == false);
        pltxt2htm_test_assert_true(stack.size() == 2);
        pltxt2htm_test_assert_true(alive == 2);

        stack.template pop<::pltxt2htm::Contracts::quick_enforce>();
        pltxt2htm_test_assert_true(stack.template top<::pltxt2htm::Contracts::quick_enforce>().value == 1);
        pltxt2htm_test_assert_true(alive == 1);
    }
    pltxt2htm_test_assert_true(alive == 0);

    return 0;
}
