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
static_assert(noexcept(::std::declval<IntDeque&>().emplace_back(1)));
static_assert(noexcept(::std::declval<IntDeque&>().emplace_front(1)));
static_assert(noexcept(::std::declval<IntDeque&>().push_back(1)));
static_assert(noexcept(::std::declval<IntDeque&>().push_front(1)));
static_assert(noexcept(::std::declval<IntDeque&>().resize(1)));
static_assert(noexcept(::std::declval<IntDeque&>().emplace(::std::declval<IntDeque::const_iterator>(), 1)));
static_assert(noexcept(::std::declval<IntDeque&>().insert(::std::declval<IntDeque::const_iterator>(), 1)));
static_assert(noexcept(::std::declval<IntDeque&>().erase(::std::declval<IntDeque::const_iterator>())));
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
        queue.emplace_back(UnevenBlockValue{index});
    }
    queue.pop_front();
    auto const survivor = ::std::addressof(queue.front());
    queue.shrink_to_fit();
    for (int index{40}; index != 200; ++index) {
        queue.emplace_back(UnevenBlockValue{index});
    }
    if (survivor != ::std::addressof(queue.front())) {
        return false;
    }
    for (int index{1}; index != 200; ++index) {
        if (queue.front().value != index) {
            return false;
        }
        queue.pop_front();
        queue.emplace_back(UnevenBlockValue{index + 199});
    }
    for (int index{199}; index != 0; --index) {
        queue.pop_back();
        queue.emplace_front(UnevenBlockValue{index});
    }
    for (int index{1}; index != 200; ++index) {
        if (queue.front().value != index) {
            return false;
        }
        queue.pop_front();
    }
    queue.shrink_to_fit();
    queue.emplace_front(UnevenBlockValue{7});
    return queue.size() == 1 && queue.back().value == 7;
}

static_assert(test_offset_relocation());

consteval auto test_constexpr_deque() -> bool {
    IntDeque values{};
    for (int value{}; value != 300; ++value) {
        values.push_back(value);
    }
    for (int value{1}; value != 101; ++value) {
        values.push_front(-value);
    }

    if (values.size() != 400 || values.front() != -100 || values.back() != 299 || values.index(100) != 0) {
        return false;
    }

    values.erase(values.cbegin() + 50, values.cbegin() + 350);
    if (values.size() != 100 || values.index(49) != -51 || values.index(50) != 250) {
        return false;
    }

    IntDeque copy{values};
    if (copy != values) {
        return false;
    }
    copy.insert(copy.cbegin() + 50, {7, 8, 9});
    if (copy.size() != 103 || copy.index(50) != 7 || copy.index(51) != 8 || copy.index(52) != 9) {
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

constexpr auto test_contract_operations() noexcept -> bool {
    IntDeque values{};
    values.emplace_front(1);
    values.emplace_back(2);
    if (values.front() != 1 || values.back() != 2) {
        return false;
    }
    values.clear();
    values.insert(values.cbegin(), {1, 2, 3});
    values.emplace(values.cbegin(), 0);
    values.emplace(values.cend(), 4);
    values.emplace(values.cbegin() + 2, 7);
    int const more[]{8, 9};
    values.insert(values.cbegin() + 2, more, more + 2);
    values.insert(values.cbegin() + 4, more[0]);
    values.insert(values.cbegin() + 1, 6);
    if (values != IntDeque{0, 6, 1, 8, 9, 8, 7, 2, 3, 4}) {
        return false;
    }
    values.pop_front();
    values.pop_back();
    values.erase(values.cbegin() + 1);
    values.erase(values.cbegin() + 1, values.cbegin() + 5);
    values.resize(5);
    auto const& const_values = values;
    if (const_values.front() != 6 || const_values.back() != 0 || const_values.index(1) != 2) {
        return false;
    }
    values.resize(1);
    values.pop_front();
    values.push_front(5);
    values.pop_back();
    return values.is_empty();
}

static_assert(test_contract_operations());

namespace pltxt2htm_test {

struct BlockValue {
    int value{};
    char padding[1020]{};
};

struct CountingAllocator {
    static inline ::std::size_t map_allocations{};
    static inline ::std::size_t block_allocations{};
    static inline ::std::size_t block_deallocations{};
    static inline ::std::size_t largest_allocation{};

    [[nodiscard]]
    static auto allocate(::std::size_t bytes) noexcept -> void* {
        if (bytes == sizeof(BlockValue)) {
            ++block_allocations;
        }
        else {
            ++map_allocations;
        }
        largest_allocation = ::std::max(largest_allocation, bytes);
        return ::fast_io::native_global_allocator::allocate(bytes);
    }

    static void deallocate(void* storage) noexcept {
        ::fast_io::native_global_allocator::deallocate(storage);
    }

    static void deallocate_n(void* storage, ::std::size_t bytes) noexcept {
        if (bytes == sizeof(BlockValue)) {
            ++block_deallocations;
        }
        ::fast_io::native_global_allocator::deallocate_n(storage, bytes);
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

void test_spare_blocks() {
    using Queue = ::pltxt2htm::container::Deque<BlockValue, ::fast_io::generic_allocator_adapter<CountingAllocator>>;
    CountingAllocator::block_allocations = 0;
    CountingAllocator::block_deallocations = 0;
    {
        Queue queue{};
        queue.emplace_back(BlockValue{1});
        for (int index{}; index != 100; ++index) {
            queue.pop_back();
            queue.emplace_back(BlockValue{index});
        }
        pltxt2htm_test_assert_true(CountingAllocator::block_allocations == 1);

        queue.emplace_back(BlockValue{2});
        for (int index{}; index != 100; ++index) {
            queue.pop_back();
            queue.emplace_back(BlockValue{index});
        }
        pltxt2htm_test_assert_true(CountingAllocator::block_allocations == 2);

        queue.pop_back();
        queue.emplace_front(BlockValue{3});
        pltxt2htm_test_assert_true(CountingAllocator::block_allocations == 2);
        queue.pop_front();
        queue.emplace_back(BlockValue{4});
        pltxt2htm_test_assert_true(CountingAllocator::block_allocations == 2);

        for (int index{}; index != 100; ++index) {
            queue.pop_front();
            queue.emplace_front(BlockValue{index});
        }
        pltxt2htm_test_assert_true(CountingAllocator::block_allocations == 2);

        for (int index{}; index != 5; ++index) {
            queue.emplace_back(BlockValue{index});
        }
        ::std::size_t const allocated_before_clear{CountingAllocator::block_allocations};
        queue.clear();
        pltxt2htm_test_assert_true(CountingAllocator::block_deallocations == allocated_before_clear - 2);
        queue.emplace_front(BlockValue{6});
        queue.emplace_back(BlockValue{5});
        pltxt2htm_test_assert_true(CountingAllocator::block_allocations == allocated_before_clear);
        queue.clear();
        queue.shrink_to_fit();
        pltxt2htm_test_assert_true(CountingAllocator::block_deallocations == CountingAllocator::block_allocations);
    }
    pltxt2htm_test_assert_true(CountingAllocator::block_deallocations == CountingAllocator::block_allocations);
}

void test_spare_transfer_at_map_edge() {
    using Queue = ::pltxt2htm::container::Deque<BlockValue, ::fast_io::generic_allocator_adapter<CountingAllocator>>;
    CountingAllocator::map_allocations = 0;
    CountingAllocator::block_allocations = 0;
    {
        Queue queue{};
        for (int index{}; index != 4; ++index) {
            queue.emplace_back(BlockValue{index});
        }
        queue.pop_front();
        queue.emplace_back(BlockValue{4});
        pltxt2htm_test_assert_true(CountingAllocator::map_allocations == 1);
        pltxt2htm_test_assert_true(CountingAllocator::block_allocations == 4);
        for (int index{}; index != 4; ++index) {
            pltxt2htm_test_assert_true(queue.index(index).value == index + 1);
        }
    }

    CountingAllocator::map_allocations = 0;
    CountingAllocator::block_allocations = 0;
    {
        Queue queue{};
        for (int index{}; index != 5; ++index) {
            queue.emplace_front(BlockValue{index});
        }
        queue.pop_back();
        queue.emplace_front(BlockValue{5});
        pltxt2htm_test_assert_true(CountingAllocator::map_allocations == 1);
        pltxt2htm_test_assert_true(CountingAllocator::block_allocations == 5);
        for (int index{}; index != 5; ++index) {
            pltxt2htm_test_assert_true(queue.index(index).value == 5 - index);
        }
    }
}

void test_spare_blocks_follow_storage() {
    using Queue = ::pltxt2htm::container::Deque<BlockValue, ::fast_io::generic_allocator_adapter<CountingAllocator>>;
    CountingAllocator::block_allocations = 0;
    CountingAllocator::block_deallocations = 0;
    {
        Queue first{};
        first.emplace_back(BlockValue{1});
        first.emplace_back(BlockValue{2});
        first.pop_front();

        Queue second{};
        second.emplace_back(BlockValue{3});
        second.emplace_back(BlockValue{4});
        second.pop_back();

        first.swap(second);
        Queue moved{::std::move(first)};
        moved.emplace_back(BlockValue{5});
        second.emplace_front(BlockValue{0});
        pltxt2htm_test_assert_true(moved.front().value == 3);
        pltxt2htm_test_assert_true(moved.back().value == 5);
        pltxt2htm_test_assert_true(second.front().value == 0);
        pltxt2htm_test_assert_true(second.back().value == 2);
        pltxt2htm_test_assert_true(CountingAllocator::block_allocations == 4);
    }
    pltxt2htm_test_assert_true(CountingAllocator::block_deallocations == CountingAllocator::block_allocations);
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
    static inline ::std::size_t moves{};

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
        ++moves;
    }

    auto operator=(this TrackedValue& self, TrackedValue const& other) noexcept -> TrackedValue& {
        self.value = other.value;
        return self;
    }

    auto operator=(this TrackedValue& self, TrackedValue&& other) noexcept -> TrackedValue& {
        self.value = other.value;
        other.value = -1;
        ++moves;
        return self;
    }

    ~TrackedValue() noexcept {
        --alive;
    }

    auto operator==(this TrackedValue const&, TrackedValue const&) noexcept -> bool = default;
    friend void swap(TrackedValue&, TrackedValue&) = delete;
};

void test_range_insertion() {
    using Queue = ::pltxt2htm::container::Deque<TrackedValue>;
    for (int scenario{}; scenario != 2; ++scenario) {
        Queue queue{};
        for (int index{}; index != 700; ++index) {
            queue.emplace_back(index);
        }
        Queue inserted{};
        for (int index{}; index != 200; ++index) {
            inserted.emplace_back(1000 + index);
        }
        int const insertion_index{scenario == 0 ? 100 : 600};
        TrackedValue::moves = 0;
        auto const result = queue.insert(queue.cbegin() + insertion_index, inserted.cbegin(), inserted.cend());
        pltxt2htm_test_assert_true(result == queue.begin() + insertion_index);
        pltxt2htm_test_assert_true(TrackedValue::moves < 4500);
        for (int index{}; index != 900; ++index) {
            int const expected{index < insertion_index         ? index
                               : index < insertion_index + 200 ? 1000 + index - insertion_index
                                                               : index - 200};
            pltxt2htm_test_assert_true(queue.index(index).value == expected);
        }
    }

    Queue endpoints{};
    endpoints.emplace_back(1);
    Queue source{};
    source.emplace_back(2);
    source.emplace_back(3);
    TrackedValue::moves = 0;
    auto const empty_result = endpoints.insert(endpoints.cbegin(), source.cbegin(), source.cbegin());
    pltxt2htm_test_assert_true(empty_result == endpoints.begin() && TrackedValue::moves == 0);
    endpoints.insert(endpoints.cbegin(), source.cbegin(), source.cend());
    pltxt2htm_test_assert_true(TrackedValue::moves == source.size());
    TrackedValue::moves = 0;
    endpoints.insert(endpoints.cend(), source.cbegin(), source.cend());
    pltxt2htm_test_assert_true(TrackedValue::moves == source.size());

    IntDeque self{1, 2, 3, 4};
    auto const result = self.insert(self.cbegin() + 2, self.cbegin(), self.cbegin() + 3);
    pltxt2htm_test_assert_true(result == self.begin() + 2);
    pltxt2htm_test_assert_true((self == IntDeque{1, 2, 1, 2, 3, 3, 4}));

    IntDeque front_self{0, 1, 2, 3, 4, 5};
    auto const front_result =
        front_self.insert(front_self.cbegin() + 1, front_self.cbegin() + 2, front_self.cbegin() + 5);
    pltxt2htm_test_assert_true(front_result == front_self.begin() + 1);
    pltxt2htm_test_assert_true((front_self == IntDeque{0, 2, 3, 4, 1, 2, 3, 4, 5}));
}

} // namespace pltxt2htm_test

int main() {
    pltxt2htm_test_assert_true(test_offset_relocation());
    pltxt2htm_test_assert_true(test_contract_operations());
    ::pltxt2htm_test::test_rolling_queue(true);
    ::pltxt2htm_test::test_rolling_queue(false);
    ::pltxt2htm_test::test_spare_blocks();
    ::pltxt2htm_test::test_spare_transfer_at_map_edge();
    ::pltxt2htm_test::test_spare_blocks_follow_storage();
    ::pltxt2htm_test::test_iterator_storage();
    ::pltxt2htm_test::test_range_insertion();
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
    int const* const stable_address{::std::addressof(values.front())};

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
    pltxt2htm_test_assert_true(copy.is_empty());
    pltxt2htm_test_assert_true(moved.size() == 1997);

    moved.assign({1, 2, 3, 4});
    moved.insert(moved.cbegin() + 2, {9, 9});
    pltxt2htm_test_assert_true((moved == IntDeque{1, 2, 9, 9, 3, 4}));
    auto const after_erase = moved.erase(moved.cbegin() + 1, moved.cbegin() + 5);
    pltxt2htm_test_assert_true(after_erase == moved.begin() + 1);
    pltxt2htm_test_assert_true((moved == IntDeque{1, 4}));

    moved.resize(300);
    pltxt2htm_test_assert_true(moved.size() == 300 && moved.back() == 0);
    moved.resize(1);
    moved.shrink_to_fit();
    pltxt2htm_test_assert_true(moved.size() == 1 && moved.front() == 1);
    moved.clear();
    moved.shrink_to_fit();
    pltxt2htm_test_assert_true(moved.is_empty());

    ::pltxt2htm::container::Deque<int, ::fast_io::native_thread_local_allocator> thread_local_values{};
    thread_local_values.push_front(2);
    thread_local_values.push_front(1);
    thread_local_values.push_back(3);
    pltxt2htm_test_assert_true(thread_local_values.index(0) == 1 && thread_local_values.index(2) == 3);

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
