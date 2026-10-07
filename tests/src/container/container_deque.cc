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

using Contracts = ::pltxt2htm::Contracts;

using IntDeque = ::pltxt2htm::container::Deque<int>;

template<typename Deque>
concept has_external_subscript = requires(Deque& values) { values[0]; };

template<typename Deque>
concept has_external_empty = requires(Deque& values) { values.empty(); };

static_assert(!has_external_subscript<IntDeque>);
static_assert(!has_external_subscript<IntDeque const>);
static_assert(!has_external_empty<IntDeque>);
static_assert(!has_external_empty<IntDeque const>);

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
static_assert(!::std::is_constructible_v<IntDeque, ::std::size_t, int const&>);
static_assert(::std::is_nothrow_constructible_v<IntDeque, int const*, int const*>);
static_assert(::std::is_nothrow_copy_constructible_v<IntDeque>);
static_assert(::std::is_nothrow_copy_assignable_v<IntDeque>);
static_assert(!::std::is_copy_constructible_v<::pltxt2htm::container::Deque<ThrowingCopy>>);
static_assert(noexcept(::std::declval<IntDeque&>().emplace_back<Contracts::quick_enforce>(1)));
static_assert(noexcept(::std::declval<IntDeque&>().emplace_front<Contracts::quick_enforce>(1)));
static_assert(noexcept(::std::declval<IntDeque&>().push_back<Contracts::quick_enforce>(1)));
static_assert(noexcept(::std::declval<IntDeque&>().push_front<Contracts::quick_enforce>(1)));
static_assert(noexcept(::std::declval<IntDeque&>().resize<Contracts::quick_enforce>(1)));
static_assert(noexcept(
    ::std::declval<IntDeque&>().emplace<Contracts::quick_enforce>(::std::declval<IntDeque::const_iterator>(), 1)));
static_assert(noexcept(
    ::std::declval<IntDeque&>().insert<Contracts::quick_enforce>(::std::declval<IntDeque::const_iterator>(), 1)));
static_assert(
    noexcept(::std::declval<IntDeque&>().erase<Contracts::quick_enforce>(::std::declval<IntDeque::const_iterator>())));
static_assert(noexcept(::std::declval<IntDeque&>().assign(::std::declval<int const*>(), ::std::declval<int const*>())));

struct UnevenBlockValue {
    int value{};
    char padding[296]{};
};

constexpr auto test_offset_relocation() -> bool {
    using Queue = ::pltxt2htm::container::Deque<UnevenBlockValue>;
    static_assert(Queue::elements_per_block == 3);
    Queue queue{};
    for (int index{}; index != 40; ++index) {
        queue.emplace_back<Contracts::quick_enforce>(UnevenBlockValue{index});
    }
    queue.pop_front<Contracts::quick_enforce>();
    auto const survivor = ::std::addressof(queue.front<Contracts::quick_enforce>());
    queue.shrink_to_fit();
    for (int index{40}; index != 200; ++index) {
        queue.emplace_back<Contracts::quick_enforce>(UnevenBlockValue{index});
    }
    if (survivor != ::std::addressof(queue.front<Contracts::quick_enforce>())) {
        return false;
    }
    for (int index{1}; index != 200; ++index) {
        if (queue.front<Contracts::quick_enforce>().value != index) {
            return false;
        }
        queue.pop_front<Contracts::quick_enforce>();
        queue.emplace_back<Contracts::quick_enforce>(UnevenBlockValue{index + 199});
    }
    for (int index{199}; index != 0; --index) {
        queue.pop_back<Contracts::quick_enforce>();
        queue.emplace_front<Contracts::quick_enforce>(UnevenBlockValue{index});
    }
    for (int index{1}; index != 200; ++index) {
        if (queue.front<Contracts::quick_enforce>().value != index) {
            return false;
        }
        queue.pop_front<Contracts::quick_enforce>();
    }
    queue.shrink_to_fit();
    queue.emplace_front<Contracts::quick_enforce>(UnevenBlockValue{7});
    return queue.size() == 1 && queue.back<Contracts::quick_enforce>().value == 7;
}

static_assert(test_offset_relocation());

template<Contracts ndebug>
consteval auto test_constexpr_deque() -> bool {
    IntDeque values{};
    for (int value{}; value != 300; ++value) {
        values.push_back<ndebug>(value);
    }
    for (int value{1}; value != 101; ++value) {
        values.push_front<ndebug>(-value);
    }

    if (values.size() != 400 || values.front<ndebug>() != -100 || values.back<ndebug>() != 299 ||
        values.index<ndebug>(100) != 0) {
        return false;
    }

    values.erase<ndebug>(values.cbegin() + 50, values.cbegin() + 350);
    if (values.size() != 100 || values.template index<ndebug>(49) != -51 || values.template index<ndebug>(50) != 250) {
        return false;
    }

    IntDeque copy{values};
    if (copy != values) {
        return false;
    }
    copy.insert<ndebug>(copy.cbegin() + 50, {7, 8, 9});
    if (copy.size() != 103 || copy.template index<ndebug>(50) != 7 || copy.template index<ndebug>(51) != 8 ||
        copy.template index<ndebug>(52) != 9) {
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

static_assert(test_constexpr_deque<Contracts::quick_enforce>());
static_assert(test_constexpr_deque<Contracts::ignore>());

template<Contracts ndebug>
constexpr auto test_contract_operations() noexcept -> bool {
    IntDeque values{};
    values.emplace_front<ndebug>(1);
    values.emplace_back<ndebug>(2);
    if (values.front<ndebug>() != 1 || values.back<ndebug>() != 2) {
        return false;
    }
    values.clear();
    values.template insert<ndebug>(values.cbegin(), {1, 2, 3});
    values.template emplace<ndebug>(values.cbegin(), 0);
    values.template emplace<ndebug>(values.cend(), 4);
    values.template emplace<ndebug>(values.cbegin() + 2, 7);
    int const more[]{8, 9};
    values.template insert<ndebug>(values.cbegin() + 2, more, more + 2);
    values.template insert<ndebug>(values.cbegin() + 4, more[0]);
    values.template insert<ndebug>(values.cbegin() + 1, 6);
    if (values != IntDeque{0, 6, 1, 8, 9, 8, 7, 2, 3, 4}) {
        return false;
    }
    values.template pop_front<ndebug>();
    values.template pop_back<ndebug>();
    values.template erase<ndebug>(values.cbegin() + 1);
    values.template erase<ndebug>(values.cbegin() + 1, values.cbegin() + 5);
    values.template resize<ndebug>(5);
    auto const& const_values = values;
    if (const_values.template front<ndebug>() != 6 || const_values.template back<ndebug>() != 0 ||
        const_values.template index<ndebug>(1) != 2) {
        return false;
    }
    values.template resize<ndebug>(1);
    values.template pop_front<ndebug>();
    values.push_front<ndebug>(5);
    values.template pop_back<ndebug>();
    return values.is_empty();
}

static_assert(test_contract_operations<Contracts::quick_enforce>());
static_assert(test_contract_operations<Contracts::ignore>());

namespace pltxt2htm_test {

struct BlockValue {
    int value{};
    char padding[1020]{};
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
    queue.emplace_back<Contracts::quick_enforce>(BlockValue{front_to_back ? 0 : 1});
    queue.emplace_back<Contracts::quick_enforce>(BlockValue{front_to_back ? 1 : 0});
    for (int index{2}; index != 10'000; ++index) {
        if (front_to_back) {
            auto const survivor = ::std::addressof(queue.back<Contracts::quick_enforce>());
            queue.pop_front<Contracts::quick_enforce>();
            queue.emplace_back<Contracts::quick_enforce>(BlockValue{index});
            pltxt2htm_test_assert_true(survivor == ::std::addressof(queue.front<Contracts::quick_enforce>()));
            pltxt2htm_test_assert_true(queue.front<Contracts::quick_enforce>().value == index - 1 &&
                                       queue.back<Contracts::quick_enforce>().value == index);
        }
        else {
            auto const survivor = ::std::addressof(queue.front<Contracts::quick_enforce>());
            queue.pop_back<Contracts::quick_enforce>();
            queue.emplace_front<Contracts::quick_enforce>(BlockValue{index});
            pltxt2htm_test_assert_true(survivor == ::std::addressof(queue.back<Contracts::quick_enforce>()));
            pltxt2htm_test_assert_true(queue.front<Contracts::quick_enforce>().value == index &&
                                       queue.back<Contracts::quick_enforce>().value == index - 1);
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
            source.push_back<Contracts::quick_enforce>(index);
        }
        // Exercise nonzero front offsets as well as several block boundaries.
        for (int index{}; index != 17; ++index) {
            source.pop_front<Contracts::quick_enforce>();
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
    destination.pop_front<Contracts::quick_enforce>();
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
    pltxt2htm_test_assert_true(test_offset_relocation());
    pltxt2htm_test_assert_true(test_contract_operations<Contracts::quick_enforce>());
    pltxt2htm_test_assert_true(test_contract_operations<Contracts::ignore>());
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
            differential.push_front<Contracts::quick_enforce>(operation);
            reference.push_front(operation);
            break;
        case 1:
            differential.push_back<Contracts::quick_enforce>(operation);
            reference.push_back(operation);
            break;
        case 2:
            if (!reference.empty()) {
                differential.pop_front<Contracts::quick_enforce>();
                reference.pop_front();
            }
            break;
        case 3:
            if (!reference.empty()) {
                differential.pop_back<Contracts::quick_enforce>();
                reference.pop_back();
            }
            break;
        case 4:
            if (!reference.empty()) {
                ::std::size_t const index{random_value % reference.size()};
                differential.erase<Contracts::quick_enforce>(differential.cbegin() +
                                                             static_cast<::std::ptrdiff_t>(index));
                reference.erase(reference.cbegin() + static_cast<::std::ptrdiff_t>(index));
            }
            break;
        case 5: {
            ::std::size_t const index{reference.empty() ? 0 : random_value % (reference.size() + 1)};
            differential.insert<Contracts::quick_enforce>(differential.cbegin() + static_cast<::std::ptrdiff_t>(index),
                                                          operation);
            reference.insert(reference.cbegin() + static_cast<::std::ptrdiff_t>(index), operation);
            break;
        }
        }
        pltxt2htm_test_assert_true(differential.size() == reference.size());
        pltxt2htm_test_assert_true(::std::equal(differential.begin(), differential.end(), reference.begin()));
    }

    IntDeque values{};
    values.push_back<Contracts::quick_enforce>(1);
    int const* const stable_address{::std::addressof(values.front<::pltxt2htm::Contracts::quick_enforce>())};

    for (int value{2}; value != 2000; ++value) {
        if (value % 2 == 0) {
            values.push_back<Contracts::quick_enforce>(value);
        }
        else {
            values.push_front<Contracts::quick_enforce>(value);
        }
    }
    pltxt2htm_test_assert_true(*stable_address == 1);
    pltxt2htm_test_assert_true(values.size() == 1999);
    pltxt2htm_test_assert_true(values.end() - values.begin() == 1999);
    pltxt2htm_test_assert_true(::std::distance(values.cbegin(), values.cend()) == 1999);

    IntDeque copy{values};
    pltxt2htm_test_assert_true(copy == values);
    copy.pop_front<Contracts::quick_enforce>();
    copy.pop_back<Contracts::quick_enforce>();
    pltxt2htm_test_assert_true(copy < values || copy > values);

    IntDeque moved{::std::move(copy)};
    pltxt2htm_test_assert_true(copy.is_empty());
    pltxt2htm_test_assert_true(moved.size() == 1997);

    moved.assign({1, 2, 3, 4});
    moved.insert<Contracts::quick_enforce>(moved.cbegin() + 2, {9, 9});
    pltxt2htm_test_assert_true((moved == IntDeque{1, 2, 9, 9, 3, 4}));
    auto const after_erase = moved.erase<Contracts::quick_enforce>(moved.cbegin() + 1, moved.cbegin() + 5);
    pltxt2htm_test_assert_true(after_erase == moved.begin() + 1);
    pltxt2htm_test_assert_true((moved == IntDeque{1, 4}));

    moved.resize<Contracts::quick_enforce>(300);
    pltxt2htm_test_assert_true(moved.size() == 300 && moved.back<Contracts::quick_enforce>() == 0);
    moved.resize<Contracts::quick_enforce>(1);
    moved.shrink_to_fit();
    pltxt2htm_test_assert_true(moved.size() == 1 && moved.front<Contracts::quick_enforce>() == 1);
    moved.clear();
    moved.shrink_to_fit();
    pltxt2htm_test_assert_true(moved.is_empty());

    ::pltxt2htm::container::Deque<int, ::fast_io::native_thread_local_allocator> thread_local_values{};
    thread_local_values.push_front<Contracts::quick_enforce>(2);
    thread_local_values.push_front<Contracts::quick_enforce>(1);
    thread_local_values.push_back<Contracts::quick_enforce>(3);
    pltxt2htm_test_assert_true(thread_local_values.index<Contracts::quick_enforce>(0) == 1 &&
                               thread_local_values.index<Contracts::quick_enforce>(2) == 3);

    {
        ::pltxt2htm::container::Deque<::pltxt2htm_test::TrackedValue> tracked{};
        for (int value{}; value != 500; ++value) {
            tracked.emplace_back<Contracts::quick_enforce>(value);
        }
        tracked.erase<Contracts::quick_enforce>(tracked.cbegin() + 100, tracked.cbegin() + 400);
        tracked.resize<Contracts::quick_enforce>(50);
        auto tracked_copy{tracked};
        tracked_copy = tracked;
        pltxt2htm_test_assert_true(::pltxt2htm_test::TrackedValue::alive == 100);
        {
            auto move_source{tracked};
            ::pltxt2htm::container::Deque<::pltxt2htm_test::TrackedValue> move_target{};
            move_target.emplace_back<Contracts::quick_enforce>(500);
            move_target = ::std::move(move_source);
            pltxt2htm_test_assert_true(move_target == tracked);
        }
        pltxt2htm_test_assert_true(::pltxt2htm_test::TrackedValue::alive == 100);
    }
    pltxt2htm_test_assert_true(::pltxt2htm_test::TrackedValue::alive == 0);

    return 0;
}
