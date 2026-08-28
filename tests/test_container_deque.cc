#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <iterator>
#include <type_traits>
#include <utility>

#include <fast_io/fast_io_core.h>

#include <pltxt2htm/container/deque.hh>

#include "precompile.hh"

using IntDeque = ::pltxt2htm::container::Deque<int>;

static_assert(::std::same_as<IntDeque::allocator_type, ::fast_io::native_global_allocator>);
static_assert(::std::random_access_iterator<IntDeque::iterator>);
static_assert(::std::random_access_iterator<IntDeque::const_iterator>);
static_assert(::std::same_as<::std::iter_reference_t<IntDeque::iterator>, int&>);
static_assert(::std::same_as<::std::iter_reference_t<IntDeque::const_iterator>, int const&>);
static_assert(::std::is_nothrow_move_constructible_v<IntDeque>);
static_assert(::std::is_nothrow_move_assignable_v<IntDeque>);

consteval auto test_constexpr_deque() -> bool {
    IntDeque values{};
    for (int value{}; value != 300; ++value) {
        values.push_back(value);
    }
    for (int value{1}; value != 101; ++value) {
        values.push_front(-value);
    }

    if (values.size() != 400 || values.front<::pltxt2htm::Contracts::quick_enforce>() != -100 ||
        values.back<::pltxt2htm::Contracts::quick_enforce>() != 299 ||
        values.index<::pltxt2htm::Contracts::quick_enforce>(100) != 0) {
        return false;
    }

    values.erase(values.cbegin() + 50, values.cbegin() + 350);
    if (values.size() != 100 || values[49] != -51 || values[50] != 250) {
        return false;
    }

    IntDeque copy{values};
    if (copy != values) {
        return false;
    }
    copy.insert(copy.cbegin() + 50, {7, 8, 9});
    return copy.size() == 103 && copy[50] == 7 && copy[51] == 8 && copy[52] == 9;
}

static_assert(test_constexpr_deque());

namespace pltxt2htm_test {

constexpr auto next_random(::std::uint_least32_t& state) noexcept -> ::std::uint_least32_t {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

struct TrackedValue {
    static inline ::std::size_t alive{};

    int value{};

    explicit TrackedValue(int value_ = 0) noexcept
        : value{value_} {
        ++alive;
    }

    TrackedValue(TrackedValue const& other) noexcept
        : value{other.value} {
        ++alive;
    }

    TrackedValue(TrackedValue&& other) noexcept
        : value{other.value} {
        other.value = -1;
        ++alive;
    }

    auto operator=(this TrackedValue& self, TrackedValue const& other) noexcept -> TrackedValue& {
        self.value = other.value;
        return self;
    }

    auto operator=(this TrackedValue& self, TrackedValue&& other) noexcept -> TrackedValue& {
        self.value = other.value;
        other.value = -1;
        return self;
    }

    ~TrackedValue() noexcept {
        --alive;
    }

    auto operator==(this TrackedValue const&, TrackedValue const&) noexcept -> bool = default;
};

} // namespace pltxt2htm_test

int main() {
    IntDeque differential{};
    ::std::deque<int> reference{};
    ::std::uint_least32_t random_state{0xC0FFEEu};
    for (int operation{}; operation != 10'000; ++operation) {
        ::std::uint_least32_t const random_value{::pltxt2htm_test::next_random(random_state)};
        switch (random_value % 6) {
        case 0:
            differential.push_front(operation);
            reference.push_front(operation);
            break;
        case 1:
            differential.push_back(operation);
            reference.push_back(operation);
            break;
        case 2:
            if (!reference.empty()) {
                differential.pop_front();
                reference.pop_front();
            }
            break;
        case 3:
            if (!reference.empty()) {
                differential.pop_back();
                reference.pop_back();
            }
            break;
        case 4:
            if (!reference.empty()) {
                ::std::size_t const index{random_value % reference.size()};
                differential.erase(differential.cbegin() + static_cast<::std::ptrdiff_t>(index));
                reference.erase(reference.cbegin() + static_cast<::std::ptrdiff_t>(index));
            }
            break;
        case 5: {
            ::std::size_t const index{reference.empty() ? 0 : random_value % (reference.size() + 1)};
            differential.insert(differential.cbegin() + static_cast<::std::ptrdiff_t>(index), operation);
            reference.insert(reference.cbegin() + static_cast<::std::ptrdiff_t>(index), operation);
            break;
        }
        }
        pltxt2htm_test_assert_true(differential.size() == reference.size());
        pltxt2htm_test_assert_true(::std::equal(differential.begin(), differential.end(), reference.begin()));
    }

    IntDeque values{};
    values.push_back(1);
    int const* const stable_address{::std::addressof(values.front<::pltxt2htm::Contracts::quick_enforce>())};

    for (int value{2}; value != 2000; ++value) {
        if (value % 2 == 0) {
            values.push_back(value);
        }
        else {
            values.push_front(value);
        }
    }
    pltxt2htm_test_assert_true(*stable_address == 1);
    pltxt2htm_test_assert_true(values.size() == 1999);
    pltxt2htm_test_assert_true(values.end() - values.begin() == 1999);
    pltxt2htm_test_assert_true(::std::distance(values.cbegin(), values.cend()) == 1999);

    IntDeque copy{values};
    pltxt2htm_test_assert_true(copy == values);
    copy.pop_front();
    copy.pop_back();
    pltxt2htm_test_assert_true(copy < values || copy > values);

    IntDeque moved{::std::move(copy)};
    pltxt2htm_test_assert_true(copy.empty());
    pltxt2htm_test_assert_true(moved.size() == 1997);

    moved.assign({1, 2, 3, 4});
    moved.insert(moved.cbegin() + 2, 2, 9);
    pltxt2htm_test_assert_true((moved == IntDeque{1, 2, 9, 9, 3, 4}));
    auto const after_erase = moved.erase(moved.cbegin() + 1, moved.cbegin() + 5);
    pltxt2htm_test_assert_true(after_erase == moved.begin() + 1);
    pltxt2htm_test_assert_true((moved == IntDeque{1, 4}));

    moved.resize(300, 5);
    pltxt2htm_test_assert_true(moved.size() == 300 && moved.back_unchecked() == 5);
    moved.resize(1);
    moved.shrink_to_fit();
    pltxt2htm_test_assert_true(moved.size() == 1 && moved.front_unchecked() == 1);
    moved.clear();
    moved.shrink_to_fit();
    pltxt2htm_test_assert_true(moved.empty());

    ::pltxt2htm::container::Deque<int, ::fast_io::native_thread_local_allocator> thread_local_values{};
    thread_local_values.push_front(2);
    thread_local_values.push_front(1);
    thread_local_values.push_back(3);
    pltxt2htm_test_assert_true(thread_local_values[0] == 1 && thread_local_values[2] == 3);

    {
        ::pltxt2htm::container::Deque<::pltxt2htm_test::TrackedValue> tracked{};
        for (int value{}; value != 500; ++value) {
            tracked.emplace_back(value);
        }
        tracked.erase(tracked.cbegin() + 100, tracked.cbegin() + 400);
        tracked.resize(50);
        auto tracked_copy{tracked};
        tracked_copy = tracked;
        pltxt2htm_test_assert_true(::pltxt2htm_test::TrackedValue::alive == 100);
    }
    pltxt2htm_test_assert_true(::pltxt2htm_test::TrackedValue::alive == 0);

    IntDeque erasable{1, 2, 1, 3, 1, 4};
    pltxt2htm_test_assert_true(::pltxt2htm::container::erase(erasable, 1) == 3);
    pltxt2htm_test_assert_true((erasable == IntDeque{2, 3, 4}));
    pltxt2htm_test_assert_true(
        ::pltxt2htm::container::erase_if(erasable, [](int value) noexcept { return value % 2 == 0; }) == 2);
    pltxt2htm_test_assert_true((erasable == IntDeque{3}));

    return 0;
}
