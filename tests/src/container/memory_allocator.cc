/**
 * @file memory_allocator.cc
 * @brief Tests for the vendored allocator stack in `pltxt2htm::memory`.
 */

#include <concepts>
#include <cstddef>
#include <type_traits>

#include <pltxt2htm/container/vector.hh>
#include <pltxt2htm/memory/allocator.hh>

#include "precompile.hh"

using Contracts = ::pltxt2htm::Contracts;
using allocator_type = ::pltxt2htm::memory::native_global_allocator;
using int_allocator = ::pltxt2htm::memory::typed_generic_allocator_adapter<allocator_type, int>;

// trait surface ---------------------------------------------------------------

static_assert(!allocator_type::has_status);
static_assert(allocator_type::has_native_allocate);
static_assert(allocator_type::has_reallocate);
static_assert(!allocator_type::has_native_handle_allocate);
static_assert(::pltxt2htm::memory::Allocator<allocator_type::allocator_type>);
static_assert(::pltxt2htm::memory::StatelessAllocator<allocator_type::allocator_type>);
static_assert(!::pltxt2htm::memory::StatefulAllocator<allocator_type::allocator_type>);

[[nodiscard]]
inline auto is_aligned(void const* pointer, ::std::size_t alignment) noexcept -> bool {
    auto const raw{reinterpret_cast<::std::size_t>(pointer)};
    return (raw & (alignment - 1u)) == 0;
}

[[nodiscard]] inline consteval auto test_typed_allocation_consteval() noexcept -> bool {
    int* pointer{int_allocator::allocate(4u)};
    if (pointer == nullptr) {
        return false;
    }
    bool ok{::pltxt2htm::memory::native_global_allocator::has_reallocate};
    int_allocator::deallocate(pointer);
    return ok;
}

static_assert(test_typed_allocation_consteval());

void pltxt2htm_memory_allocator_untyped_protocol() noexcept {
    void* pointer{allocator_type::allocate(64u)};
    pltxt2htm_test_assert_true(pointer != nullptr);
    pltxt2htm_test_assert_true(is_aligned(pointer, 16u));
    allocator_type::deallocate_n(pointer, 64u);

    auto const least{allocator_type::allocate_at_least(40u)};
    pltxt2htm_test_assert_true(least.ptr != nullptr);
    pltxt2htm_test_assert_true(least.count >= 40u);
    pltxt2htm_test_assert_true(is_aligned(least.ptr, 16u));
    allocator_type::deallocate(least.ptr);

    auto const zero_pointer{static_cast<unsigned char*>(allocator_type::allocate_zero(256u))};
    pltxt2htm_test_assert_true(zero_pointer != nullptr);
    {
        bool all_zero{true};
        for (::std::size_t index{}; index != 256u; ++index) {
            all_zero = all_zero && zero_pointer[index] == 0u;
        }
        pltxt2htm_test_assert_true(all_zero);
    }
    allocator_type::deallocate_aligned_n(zero_pointer, 16u, 256u);
}

void pltxt2htm_memory_allocator_reallocation_protocol() noexcept {
    auto const src{allocator_type::reallocate_n_at_least(nullptr, 32u, 96u)};
    pltxt2htm_test_assert_true(src.ptr != nullptr);
    pltxt2htm_test_assert_true(src.count >= 64u);
    {
        auto const bytes{static_cast<unsigned char*>(src.ptr)};
        for (::std::size_t index{}; index != 32u; ++index) {
            bytes[index] = static_cast<unsigned char>(index + 1u);
        }
    }
    auto const grown{allocator_type::reallocate_n_at_least(src.ptr, 32u, 160u)};
    pltxt2htm_test_assert_true(grown.ptr != nullptr);
    pltxt2htm_test_assert_true(grown.count >= 128u);
    {
        bool preserved{true};
        auto const grown_bytes{static_cast<unsigned char* const>(grown.ptr)};
        for (::std::size_t index{}; index != 32u; ++index) {
            preserved = preserved && grown_bytes[index] == static_cast<unsigned char>(index + 1u);
        }
        pltxt2htm_test_assert_true(preserved);
    }
    auto const reset{allocator_type::reallocate_zero_at_least(grown.ptr, 160u)};
    pltxt2htm_test_assert_true(reset.ptr != nullptr);
    allocator_type::deallocate_n(reset.ptr, 96u);
}

void pltxt2htm_memory_allocator_aligned_protocol() noexcept {
    constexpr ::std::size_t alignment{64u};

    void* const aligned{allocator_type::allocate_aligned(alignment, 128u)};
    pltxt2htm_test_assert_true(aligned != nullptr);
    pltxt2htm_test_assert_true(is_aligned(aligned, alignment));

    auto const aligned_least{allocator_type::allocate_aligned_at_least(alignment, 96u)};
    pltxt2htm_test_assert_true(aligned_least.ptr != nullptr);
    pltxt2htm_test_assert_true(aligned_least.count >= 96u);
    pltxt2htm_test_assert_true(is_aligned(aligned_least.ptr, alignment));

    void* const grown_aligned{allocator_type::allocate_aligned(alignment, 256u)};
    pltxt2htm_test_assert_true(grown_aligned != nullptr);
    pltxt2htm_test_assert_true(is_aligned(grown_aligned, alignment));

    allocator_type::deallocate_aligned(aligned_least.ptr, alignment);
    allocator_type::deallocate_aligned_n(aligned, alignment, 128u);
    allocator_type::deallocate_aligned(grown_aligned, alignment);
}

// tracking through containers -------------------------------------------------

namespace {

class CountingRawAllocator {
public:
    static inline ::std::size_t allocations{};
    static inline ::std::size_t deallocations{};
    static inline ::std::size_t last_bytes{};

private:
    static inline void* slot{};

public:
    [[nodiscard]]
    static auto allocate_at_least(::std::size_t requested_bytes) noexcept
        -> ::pltxt2htm::memory::allocation_least_result {
        ++allocations;
        last_bytes = requested_bytes;
        slot = allocator_type::allocate(requested_bytes);
        return {slot, requested_bytes};
    }

    static void deallocate_n(void* pointer, ::std::size_t requested_bytes) noexcept {
        ++deallocations;
        last_bytes = requested_bytes;
        allocator_type::deallocate_n(pointer, requested_bytes);
        if (pointer == slot) {
            slot = nullptr;
        }
    }
};

using count_allocator = ::pltxt2htm::memory::generic_allocator_adapter<CountingRawAllocator>;

static_assert(count_allocator::has_status == false);
static_assert(count_allocator::has_native_allocate_at_least);

} // namespace

void pltxt2htm_memory_allocator_container_tracking() noexcept {
    CountingRawAllocator::allocations = 0u;
    CountingRawAllocator::deallocations = 0u;

    {
        ::pltxt2htm::container::Vector<int, count_allocator> tracked{};
        tracked.template push_back<Contracts::quick_enforce>(7);
        tracked.template push_back<Contracts::quick_enforce>(9);
        pltxt2htm_test_assert_true(tracked.size() == 2u);
    }

    pltxt2htm_test_assert_true(CountingRawAllocator::allocations >= 1u);
    pltxt2htm_test_assert_true(CountingRawAllocator::deallocations == CountingRawAllocator::allocations);
    pltxt2htm_test_assert_true(CountingRawAllocator::last_bytes >= sizeof(int) * 2u);
}

// consteval container strings -------------------------------------------------

[[nodiscard]]
inline consteval bool consteval_container_allocation_works() noexcept {
    ::pltxt2htm::container::U8String text{};
    text.assign<Contracts::quick_enforce>(u8"defg");
    return !text.is_empty() && text.size() == 4u;
}

static_assert(consteval_container_allocation_works(),
              "constant-evaluation container allocation must work through the vendored adapter");

int main() {
    pltxt2htm_test_assert_true(test_typed_allocation_consteval());
    pltxt2htm_memory_allocator_untyped_protocol();
    pltxt2htm_memory_allocator_reallocation_protocol();
    pltxt2htm_memory_allocator_aligned_protocol();
    pltxt2htm_memory_allocator_container_tracking();
    pltxt2htm_test_assert_true(consteval_container_allocation_works());
}
