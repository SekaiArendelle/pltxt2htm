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

struct ThrowingCopy {
    ThrowingCopy() noexcept = default;

    ThrowingCopy(ThrowingCopy const&) noexcept(false) {
    }

    ThrowingCopy(ThrowingCopy&&) noexcept = default;
    auto operator=(this ThrowingCopy&, ThrowingCopy const&) noexcept -> ThrowingCopy& = default;
    auto operator=(this ThrowingCopy&, ThrowingCopy&&) noexcept -> ThrowingCopy& = default;
    ~ThrowingCopy() noexcept = default;
};

static_assert(::std::same_as<IntDeque::allocator_type, ::fast_io::native_global_allocator>);
static_assert(::std::random_access_iterator<IntDeque::iterator>);
static_assert(::std::random_access_iterator<IntDeque::const_iterator>);
static_assert(::std::same_as<::std::iter_reference_t<IntDeque::iterator>, int&>);
static_assert(::std::same_as<::std::iter_reference_t<IntDeque::const_iterator>, int const&>);
static_assert(::std::is_nothrow_move_constructible_v<IntDeque>);
static_assert(::std::is_nothrow_move_assignable_v<IntDeque>);
static_assert(::std::is_nothrow_constructible_v<IntDeque, ::std::size_t>);
static_assert(::std::is_nothrow_constructible_v<IntDeque, ::std::size_t, int const&>);
static_assert(::std::is_nothrow_constructible_v<IntDeque, int const*, int const*>);
static_assert(::std::is_nothrow_copy_constructible_v<IntDeque>);
static_assert(::std::is_nothrow_copy_assignable_v<IntDeque>);
static_assert(!::std::is_copy_constructible_v<::pltxt2htm::container::Deque<ThrowingCopy>>);
static_assert(noexcept(::std::declval<IntDeque&>().emplace_back(1)));
static_assert(noexcept(::std::declval<IntDeque&>().emplace_front(1)));
static_assert(noexcept(::std::declval<IntDeque&>().push_back(1)));
static_assert(noexcept(::std::declval<IntDeque&>().push_front(1)));
static_assert(noexcept(::std::declval<IntDeque&>().resize(1)));
static_assert(noexcept(::std::declval<IntDeque&>().resize(1, 2)));
static_assert(noexcept(::std::declval<IntDeque&>().emplace(::std::declval<IntDeque::const_iterator>(), 1)));
static_assert(noexcept(::std::declval<IntDeque&>().insert(::std::declval<IntDeque::const_iterator>(), 1)));
static_assert(noexcept(::std::declval<IntDeque&>().erase(::std::declval<IntDeque::const_iterator>())));
static_assert(noexcept(::std::declval<IntDeque&>().assign(::std::declval<int const*>(), ::std::declval<int const*>())));

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
    if (copy.size() != 103 || copy[50] != 7 || copy[51] != 8 || copy[52] != 9) {
        return false;
    }

    auto const saved = values.begin() + 50;
    auto const saved_const = values.cbegin() + 50;
    values.swap(copy);
    if (*saved != 250 || saved != copy.begin() + 50 || saved_const - copy.cbegin() != 50) {
        return false;
    }
    IntDeque moved{::std::move(copy)};
    values = ::std::move(moved);
    return *saved == 250 && saved == values.begin() + 50 && saved_const == saved;
}

static_assert(test_constexpr_deque());

namespace pltxt2htm_test {

struct BlockValue {
    int value{};
    char padding[508]{};
};

struct CountingAllocator {
    static inline ::std::size_t map_allocations{};
    static inline ::std::size_t largest_allocation{};

    [[nodiscard]]
    static auto allocate(::std::size_t bytes) noexcept -> void* {
        if (bytes != sizeof(BlockValue)) {
            ++map_allocations;
        }
        largest_allocation = ::std::max(largest_allocation, bytes);
        return ::fast_io::native_global_allocator::allocate(bytes);
    }

    static void deallocate(void* storage) noexcept {
        ::fast_io::native_global_allocator::deallocate(storage);
    }
};

void test_rolling_queue(bool front_to_back) {
    using Queue = ::pltxt2htm::container::Deque<BlockValue, ::fast_io::generic_allocator_adapter<CountingAllocator>>;
    static_assert(Queue::elements_per_block == 1);
    CountingAllocator::map_allocations = 0;
    CountingAllocator::largest_allocation = 0;
    Queue queue{};
    queue.emplace_back(BlockValue{front_to_back ? 0 : 1});
    queue.emplace_back(BlockValue{front_to_back ? 1 : 0});
    for (int index{2}; index != 10'000; ++index) {
        if (front_to_back) {
            auto const survivor = ::std::addressof(queue.back());
            queue.pop_front();
            queue.emplace_back(BlockValue{index});
            pltxt2htm_test_assert_true(survivor == ::std::addressof(queue.front()));
            pltxt2htm_test_assert_true(queue.front().value == index - 1 && queue.back().value == index);
        }
        else {
            auto const survivor = ::std::addressof(queue.front());
            queue.pop_back();
            queue.emplace_front(BlockValue{index});
            pltxt2htm_test_assert_true(survivor == ::std::addressof(queue.back()));
            pltxt2htm_test_assert_true(queue.front().value == index && queue.back().value == index - 1);
        }
        pltxt2htm_test_assert_true(queue.size() == 2);
    }
    pltxt2htm_test_assert_true(CountingAllocator::map_allocations == 1);
    pltxt2htm_test_assert_true(CountingAllocator::largest_allocation == sizeof(BlockValue));
}

void test_iterator_storage() {
    IntDeque destination{9};
    IntDeque::iterator saved{};
    IntDeque::const_iterator saved_const{};
    IntDeque::reverse_iterator saved_reverse{};
    {
        IntDeque source{};
        for (int index{}; index != 400; ++index) {
            source.push_back(index);
        }
        // Exercise nonzero front offsets as well as several block boundaries.
        for (int index{}; index != 17; ++index) {
            source.pop_front();
        }
        saved = source.begin() + 150;
        saved_const = saved;
        saved_reverse = source.rbegin();
        auto const address = ::std::addressof(*saved);
        auto const old_destination = destination.begin();
        source.swap(destination);
        pltxt2htm_test_assert_true(*old_destination == 9 && old_destination == source.begin());
        pltxt2htm_test_assert_true(*saved == 167 && ::std::addressof(*saved) == address);
        pltxt2htm_test_assert_true(saved == destination.begin() + 150 && saved_const - destination.cbegin() == 150);
        IntDeque moved{::std::move(destination)};
        pltxt2htm_test_assert_true(saved == moved.begin() + 150 && saved_reverse == moved.rbegin());
        destination = ::std::move(moved);
    }
    pltxt2htm_test_assert_true(*saved == 167 && saved_const == destination.cbegin() + 150);
    pltxt2htm_test_assert_true(saved[-150] == 17 && saved[200] == 367);
    pltxt2htm_test_assert_true(saved - 150 == destination.begin() && saved + 233 == destination.end());
    pltxt2htm_test_assert_true(destination.end() - saved == 233 && saved - destination.end() == -233);
    pltxt2htm_test_assert_true(*saved_reverse == 399);
    auto const survivor = destination.begin() + 1;
    destination.pop_front();
    pltxt2htm_test_assert_true(survivor == destination.begin() && *survivor == 18);
    IntDeque empty{};
    pltxt2htm_test_assert_true(empty.end() - empty.begin() == 0 && empty.begin() + 0 == empty.end());
}

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
    ::pltxt2htm_test::test_rolling_queue(true);
    ::pltxt2htm_test::test_rolling_queue(false);
    ::pltxt2htm_test::test_iterator_storage();
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
        {
            auto move_source{tracked};
            ::pltxt2htm::container::Deque<::pltxt2htm_test::TrackedValue> move_target{};
            move_target.emplace_back(500);
            move_target = ::std::move(move_source);
            pltxt2htm_test_assert_true(move_target == tracked);
        }
        pltxt2htm_test_assert_true(::pltxt2htm_test::TrackedValue::alive == 100);
    }
    pltxt2htm_test_assert_true(::pltxt2htm_test::TrackedValue::alive == 0);

    return 0;
}
