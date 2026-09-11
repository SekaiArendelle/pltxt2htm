#include <concepts>
#include <iterator>
#include <span>
#include <type_traits>
#include <utility>

#include <fast_io/fast_io_core.h>

#include <pltxt2htm/container/vector.hh>

#include "precompile.hh"

using IntVector = ::pltxt2htm::container::Vector<int>;

class TrackingRawAllocator {
    static inline constexpr ::std::size_t slot_count{4};
    static inline constexpr ::std::size_t additional_bytes{3 * sizeof(int)};
    static inline void* allocation_pointers[slot_count]{};
    static inline ::std::size_t allocation_sizes[slot_count]{};
    static inline ::std::size_t active_allocations{};
    static inline ::std::size_t deallocation_count{};
    static inline bool deallocation_mismatch{};

public:
    [[nodiscard]]
    static auto allocate_at_least(::std::size_t requested_bytes) noexcept -> ::fast_io::allocation_least_result {
        ::std::size_t const allocated_bytes{requested_bytes + additional_bytes};
        void* const pointer{::fast_io::native_global_allocator::allocate(allocated_bytes)};
        for (::std::size_t index{}; index != slot_count; ++index) {
            if (allocation_pointers[index] == nullptr) {
                allocation_pointers[index] = pointer;
                allocation_sizes[index] = allocated_bytes;
                ++active_allocations;
                return {pointer, allocated_bytes};
            }
        }
        ::fast_io::fast_terminate();
    }

    static void deallocate_n(void* pointer, ::std::size_t allocated_bytes) noexcept {
        for (::std::size_t index{}; index != slot_count; ++index) {
            if (allocation_pointers[index] != pointer) {
                continue;
            }
            deallocation_mismatch = deallocation_mismatch || allocation_sizes[index] != allocated_bytes;
            ::fast_io::native_global_allocator::deallocate_n(pointer, allocation_sizes[index]);
            allocation_pointers[index] = nullptr;
            allocation_sizes[index] = 0;
            --active_allocations;
            ++deallocation_count;
            return;
        }
        deallocation_mismatch = true;
        ::fast_io::native_global_allocator::deallocate_n(pointer, allocated_bytes);
    }

    static void reset() noexcept {
        active_allocations = 0;
        deallocation_count = 0;
        deallocation_mismatch = false;
        for (::std::size_t index{}; index != slot_count; ++index) {
            allocation_pointers[index] = nullptr;
            allocation_sizes[index] = 0;
        }
    }

    [[nodiscard]]
    static auto get_active_allocations() noexcept -> ::std::size_t {
        return active_allocations;
    }

    [[nodiscard]]
    static auto get_deallocation_count() noexcept -> ::std::size_t {
        return deallocation_count;
    }

    [[nodiscard]]
    static auto has_deallocation_mismatch() noexcept -> bool {
        return deallocation_mismatch;
    }
};

using TrackingAllocator = ::fast_io::generic_allocator_adapter<TrackingRawAllocator>;
using TrackingIntVector = ::pltxt2htm::container::Vector<int, TrackingAllocator>;

class ConstructionTrace {
    int payload{};
    int construction_kind{};

public:
    constexpr explicit ConstructionTrace(int payload_) noexcept
        : payload{payload_},
          construction_kind{1} {
    }

    constexpr ConstructionTrace(ConstructionTrace const& other) noexcept
        : payload{other.payload},
          construction_kind{2} {
    }

    constexpr ConstructionTrace(ConstructionTrace&& other) noexcept
        : payload{::std::exchange(other.payload, -1)},
          construction_kind{3} {
    }

    constexpr auto operator=(this ConstructionTrace& self, ConstructionTrace const& other) noexcept
        -> ConstructionTrace& {
        self.payload = other.payload;
        self.construction_kind = 2;
        return self;
    }

    constexpr auto operator=(this ConstructionTrace& self, ConstructionTrace&& other) noexcept -> ConstructionTrace& {
        self.payload = ::std::exchange(other.payload, -1);
        self.construction_kind = 3;
        return self;
    }

    [[nodiscard]]
    constexpr auto get_payload(this ConstructionTrace const& self) noexcept -> int {
        return self.payload;
    }

    [[nodiscard]]
    constexpr auto get_construction_kind(this ConstructionTrace const& self) noexcept -> int {
        return self.construction_kind;
    }

    [[nodiscard]]
    friend constexpr auto operator==(ConstructionTrace const&, ConstructionTrace const&) noexcept -> bool = default;
};

class SinglePassIntRange {
public:
    class Iterator {
        int const* current{};

    public:
        using value_type = int;
        using difference_type = ::std::ptrdiff_t;
        using iterator_concept = ::std::input_iterator_tag;

        constexpr Iterator() noexcept = default;

        constexpr explicit Iterator(int const* current_) noexcept
            : current{current_} {
        }

        [[nodiscard]]
        constexpr auto operator*(this Iterator const& self) noexcept -> int const& {
            return *self.current;
        }

        constexpr auto operator++(this Iterator& self) noexcept -> Iterator& {
            ++self.current;
            return self;
        }

        constexpr void operator++(this Iterator& self, int) noexcept {
            ++self.current;
        }

        [[nodiscard]]
        friend constexpr auto operator==(Iterator const&, Iterator const&) noexcept -> bool = default;
    };

private:
    int const* first{};
    int const* last{};

public:
    constexpr SinglePassIntRange(int const* first_, int const* last_) noexcept
        : first{first_},
          last{last_} {
    }

    [[nodiscard]]
    constexpr auto begin(this SinglePassIntRange const& self) noexcept -> Iterator {
        return Iterator{self.first};
    }

    [[nodiscard]]
    constexpr auto end(this SinglePassIntRange const& self) noexcept -> Iterator {
        return Iterator{self.last};
    }
};

class ThrowingIncrementRange {
public:
    class Iterator {
        int const* current{};

    public:
        using value_type = int;
        using difference_type = ::std::ptrdiff_t;
        using iterator_concept = ::std::input_iterator_tag;

        constexpr Iterator() noexcept = default;

        constexpr explicit Iterator(int const* current_) noexcept
            : current{current_} {
        }

        [[nodiscard]]
        constexpr auto operator*(this Iterator const& self) noexcept -> int const& {
            return *self.current;
        }

        constexpr auto operator++(this Iterator& self) -> Iterator& {
            ++self.current;
            return self;
        }

        constexpr void operator++(this Iterator& self, int) {
            ++self.current;
        }

        [[nodiscard]]
        friend constexpr auto operator==(Iterator const&, Iterator const&) noexcept -> bool = default;
    };

    [[nodiscard]]
    constexpr auto begin(this ThrowingIncrementRange const&) noexcept -> Iterator {
        return Iterator{};
    }

    [[nodiscard]]
    constexpr auto end(this ThrowingIncrementRange const&) noexcept -> Iterator {
        return Iterator{};
    }
};

class CvSensitiveForwardRange {
public:
    class Iterator {
        int const* current{};

    public:
        using value_type = int;
        using difference_type = ::std::ptrdiff_t;
        using iterator_concept = ::std::forward_iterator_tag;

        constexpr Iterator() noexcept = default;

        constexpr explicit Iterator(int const* current_) noexcept
            : current{current_} {
        }

        [[nodiscard]]
        constexpr auto operator*(this Iterator const& self) noexcept -> int const& {
            return *self.current;
        }

        constexpr auto operator++(this Iterator& self) noexcept -> Iterator& {
            ++self.current;
            return self;
        }

        constexpr auto operator++(this Iterator& self, int) noexcept -> Iterator {
            Iterator const previous{self};
            ++self.current;
            return previous;
        }

        [[nodiscard]]
        friend constexpr auto operator==(Iterator& left, Iterator& right) noexcept -> bool {
            return left.current == right.current;
        }

        [[nodiscard]]
        friend constexpr auto operator==(Iterator const& left, Iterator const& right) -> bool {
            return left.current == right.current;
        }
    };

private:
    int const* first{};
    int const* last{};

public:
    constexpr CvSensitiveForwardRange(int const* first_, int const* last_) noexcept
        : first{first_},
          last{last_} {
    }

    [[nodiscard]]
    constexpr auto begin(this CvSensitiveForwardRange const& self) noexcept -> Iterator {
        return Iterator{self.first};
    }

    [[nodiscard]]
    constexpr auto end(this CvSensitiveForwardRange const& self) noexcept -> Iterator {
        return Iterator{self.last};
    }
};

struct ThrowingMove {
    ThrowingMove() noexcept = default;
    ThrowingMove(ThrowingMove&&);
};

struct ThrowingEquality {
    friend auto operator==(ThrowingEquality const&, ThrowingEquality const&) -> bool;
};

struct ThrowingBoolean {
    explicit operator bool() const;
};

struct ThrowingBooleanEquality {
    friend constexpr auto operator==(ThrowingBooleanEquality const&, ThrowingBooleanEquality const&) noexcept
        -> ThrowingBoolean {
        return {};
    }
};

struct ThrowingConstruction {
    ThrowingConstruction() noexcept = default;
    explicit ThrowingConstruction(int);
    ThrowingConstruction(ThrowingConstruction&&) noexcept = default;
};

struct ThrowingCopy {
    ThrowingCopy() noexcept = default;
    ThrowingCopy(ThrowingCopy const&);
    ThrowingCopy(ThrowingCopy&&) noexcept = default;
};

struct ThrowingMoveAssignment {
    ThrowingMoveAssignment() noexcept = default;
    ThrowingMoveAssignment(ThrowingMoveAssignment&&) noexcept = default;
    auto operator=(ThrowingMoveAssignment&&) -> ThrowingMoveAssignment&;
};

template<typename T>
concept ReservableVectorElement = requires(::pltxt2htm::container::Vector<T>& values) { values.reserve(1); };

template<typename T>
concept IntEmplaceableVectorElement = requires(::pltxt2htm::container::Vector<T>& values) { values.emplace_back(1); };

template<typename T>
concept CopyPushableVectorElement =
    requires(::pltxt2htm::container::Vector<T>& values, T const& value) { values.push_back(value); };

template<typename T>
concept ErasableVectorElement = requires(::pltxt2htm::container::Vector<T>& values) { values.erase(values.begin()); };

template<typename T>
concept EqualityComparableVectorElement = requires(::pltxt2htm::container::Vector<T> const& left,
                                                   ::pltxt2htm::container::Vector<T> const& right) { left == right; };

template<typename R>
concept AppendableIntRange = requires(IntVector& values, R&& range) { values.append_range(::std::forward<R>(range)); };

static_assert(::std::same_as<IntVector::allocator_type, ::fast_io::native_global_allocator>);
static_assert(::std::same_as<IntVector::value_type, int>);
static_assert(::std::same_as<IntVector::iterator, int*>);
static_assert(::std::ranges::contiguous_range<IntVector>);
static_assert(::std::ranges::contiguous_range<IntVector const>);
static_assert(::std::ranges::input_range<SinglePassIntRange>);
static_assert(!::std::ranges::forward_range<SinglePassIntRange>);
static_assert(::std::ranges::forward_range<CvSensitiveForwardRange>);
static_assert(!ReservableVectorElement<ThrowingMove>);
static_assert(!IntEmplaceableVectorElement<ThrowingConstruction>);
static_assert(!CopyPushableVectorElement<ThrowingCopy>);
static_assert(!ErasableVectorElement<ThrowingMoveAssignment>);
static_assert(!EqualityComparableVectorElement<ThrowingEquality>);
static_assert(!EqualityComparableVectorElement<ThrowingBooleanEquality>);
static_assert(!AppendableIntRange<ThrowingIncrementRange&>);
static_assert(noexcept(::std::declval<IntVector&>().reserve(1)));
static_assert(noexcept(::std::declval<IntVector&>().emplace_back(1)));
static_assert(noexcept(::std::declval<IntVector&>().erase(::std::declval<IntVector::const_iterator>())));
static_assert(noexcept(::std::declval<IntVector&>().append_range(::std::declval<SinglePassIntRange&>())));
static_assert(noexcept(::std::declval<IntVector&>().append_range(::std::declval<CvSensitiveForwardRange&>())));
static_assert(noexcept(::std::declval<IntVector const&>() == ::std::declval<IntVector const&>()));

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
    if (values != IntVector{1} || copy != IntVector{9}) {
        return false;
    }

    IntVector self_reference{};
    self_reference.reserve(1);
    self_reference.push_back(7);
    while (self_reference.size() != self_reference.capacity()) {
        self_reference.push_back(0);
    }
    auto const previous_size = self_reference.size();
    self_reference.emplace_back(self_reference.template front<::pltxt2htm::Contracts::quick_enforce>());
    if (self_reference.size() != previous_size + 1 ||
        self_reference.template index<::pltxt2htm::Contracts::quick_enforce>(previous_size) != 7) {
        return false;
    }

    IntVector self_append{};
    self_append.reserve(1);
    self_append.push_back(1);
    while (self_append.size() != self_append.capacity()) {
        self_append.push_back(static_cast<int>(self_append.size() + 1));
    }
    auto const self_append_size = self_append.size();
    self_append.append_range(self_append);
    if (self_append.size() != self_append_size * 2) {
        return false;
    }
    for (::std::size_t index{}; index < self_append_size; ++index) {
        if (self_append.template index<::pltxt2htm::Contracts::quick_enforce>(index) !=
            self_append.template index<::pltxt2htm::Contracts::quick_enforce>(self_append_size + index)) {
            return false;
        }
    }
    return true;
}

static_assert(test_constexpr_vector());

int main() {
    TrackingRawAllocator::reset();
    {
        TrackingIntVector tracked{};
        tracked.reserve(2);
        pltxt2htm_test_assert_true(tracked.capacity() == 5);
        tracked.push_back(1);
        tracked.reserve(8);
        pltxt2htm_test_assert_true(tracked.size() == 1);
        pltxt2htm_test_assert_true(tracked.capacity() == 11);
        pltxt2htm_test_assert_true(TrackingRawAllocator::get_active_allocations() == 1);
        pltxt2htm_test_assert_true(TrackingRawAllocator::get_deallocation_count() == 1);
        pltxt2htm_test_assert_true(!TrackingRawAllocator::has_deallocation_mismatch());

        TrackingIntVector moved{::std::move(tracked)};
        TrackingIntVector swapped{};
        swapped.swap(moved);
        pltxt2htm_test_assert_true(tracked.capacity() == 0);
        pltxt2htm_test_assert_true(moved.capacity() == 0);
        pltxt2htm_test_assert_true(swapped.capacity() == 11);
    }
    pltxt2htm_test_assert_true(TrackingRawAllocator::get_active_allocations() == 0);
    pltxt2htm_test_assert_true(TrackingRawAllocator::get_deallocation_count() == 2);
    pltxt2htm_test_assert_true(!TrackingRawAllocator::has_deallocation_mismatch());

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

    int const single_pass_values[]{6, 7};
    auto single_pass_range = SinglePassIntRange{single_pass_values, single_pass_values + 2};
    IntVector single_pass_result{};
    single_pass_result.append_range(single_pass_range);
    pltxt2htm_test_assert_true(single_pass_result == IntVector{6, 7});

    int const cv_sensitive_values[]{8, 9};
    auto cv_sensitive_range =
        CvSensitiveForwardRange{cv_sensitive_values, cv_sensitive_values + ::std::size(cv_sensitive_values)};
    IntVector cv_sensitive_result{};
    cv_sensitive_result.append_range(cv_sensitive_range);
    pltxt2htm_test_assert_true(cv_sensitive_result == IntVector{8, 9});

    ::pltxt2htm::container::Vector<ConstructionTrace> direct_construction{};
    direct_construction.reserve(1);
    while (direct_construction.size() != direct_construction.capacity()) {
        direct_construction.emplace_back(1);
    }
    auto const direct_construction_size = direct_construction.size();
    direct_construction.emplace_back(2);
    pltxt2htm_test_assert_true(
        direct_construction.template index<::pltxt2htm::Contracts::quick_enforce>(direct_construction_size)
            .get_construction_kind() == 1);

    ::pltxt2htm::container::Vector<ConstructionTrace> copy_source{};
    copy_source.emplace_back(3);
    ::pltxt2htm::container::Vector<ConstructionTrace> copied_range{};
    copied_range.append_range(copy_source);
    pltxt2htm_test_assert_true(
        copied_range.template front<::pltxt2htm::Contracts::quick_enforce>().get_construction_kind() == 2);

    ::pltxt2htm::container::Vector<ConstructionTrace> move_source{};
    move_source.emplace_back(4);
    ::pltxt2htm::container::Vector<ConstructionTrace> moved_range{};
    moved_range.append_range(::std::span<ConstructionTrace>{move_source.data(), move_source.size()});
    pltxt2htm_test_assert_true(
        moved_range.template front<::pltxt2htm::Contracts::quick_enforce>().get_construction_kind() == 3);

    ::pltxt2htm::container::Vector<ConstructionTrace> self_move{};
    self_move.reserve(1);
    self_move.emplace_back(5);
    while (self_move.size() != self_move.capacity()) {
        self_move.emplace_back(0);
    }
    auto const self_move_size = self_move.size();
    self_move.push_back(::std::move(self_move.template front<::pltxt2htm::Contracts::quick_enforce>()));
    pltxt2htm_test_assert_true(
        self_move.template index<::pltxt2htm::Contracts::quick_enforce>(self_move_size).get_payload() == 5);

    IntVector overlap{};
    overlap.reserve(1);
    overlap.push_back(1);
    while (overlap.size() != overlap.capacity()) {
        overlap.push_back(static_cast<int>(overlap.size() + 1));
    }
    auto const overlap_size = overlap.size();
    auto const overlap_source = ::std::span<int const>{overlap.data(), overlap_size};
    overlap.append_range(overlap_source);
    pltxt2htm_test_assert_true(overlap.size() == overlap_size * 2);
    for (::std::size_t index{}; index < overlap_size; ++index) {
        pltxt2htm_test_assert_true(overlap.template index<::pltxt2htm::Contracts::quick_enforce>(index) ==
                                   overlap.template index<::pltxt2htm::Contracts::quick_enforce>(overlap_size + index));
    }

    return 0;
}
