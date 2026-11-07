/**
 * @file symbol_info_storage.hh
 * @brief Owning buffer for DbgHelp SYMBOL_INFO records.
 * @note Internal header. Include `stacktrace.hh` for the public API.
 */

#pragma once

#include <cstddef>
#include <cstdlib>
#include <limits>

#include <fast_io/fast_io_dsal/string_view.h>

#include "../../../sys/win32/dbghelp_definitions.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this header.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace pltxt2htm::stacktrace::details {

/**
 * @brief Owns a zero-initialized SymbolInfo record and its trailing name.
 * @details DbgHelp declares the record's name as a one-character array at the
 *          end of the structure, so a usable record is a single allocation of
 *          `offsetof(SymbolInfo, name) + capacity` bytes. Zero initialization
 *          is part of the contract: DbgHelp may fill the buffer without writing
 *          a terminator, and the untouched zero byte at
 *          `name()[name_length()]` supplies it.
 *
 *          The record's MaxNameLen field is the capacity, so the allocation size
 *          is not duplicated in the object. DbgHelp treats SizeOfStruct and
 *          MaxNameLen as inputs and leaves them unchanged (verified against the
 *          local DbgHelp for successful and failed lookups), so they are seeded
 *          once when the record is allocated. Except for is_valid(), the members
 *          require a successfully allocated record.
 */
class SymbolInfoStorage {
public:
    /// Initial name capacity, matching DbgHelp's MAX_SYM_NAME.
    static constexpr ::std::size_t default_name_capacity{2000};

private:
    ::pltxt2htm::sys_win32::SymbolInfo* record{};

    /**
     * @brief Allocates a zero-filled record with room for `name_capacity` bytes.
     * @param name_capacity Bytes reserved for the name, including its terminator.
     * @return The record, or nullptr when the capacity does not fit DbgHelp's
     *         length field or the allocation failed.
     */
    [[nodiscard]]
    static auto allocate(::std::size_t name_capacity) noexcept -> ::pltxt2htm::sys_win32::SymbolInfo* {
        constexpr auto max_size = ::std::numeric_limits<::std::size_t>::max();
        constexpr auto name_offset = offsetof(::pltxt2htm::sys_win32::SymbolInfo, name);
        if (name_capacity > ::std::numeric_limits<::pltxt2htm::sys_win32::Ulong>::max() ||
            name_capacity > max_size - name_offset) {
            return nullptr;
        }
        auto* const bytes = static_cast<unsigned char*>(::std::calloc(name_offset + name_capacity, 1));
        if (bytes == nullptr) {
            return nullptr;
        }
        auto* const record = reinterpret_cast<::pltxt2htm::sys_win32::SymbolInfo*>(bytes);
        record->size_of_struct = sizeof(::pltxt2htm::sys_win32::SymbolInfo);
        record->max_name_len = static_cast<::pltxt2htm::sys_win32::Ulong>(name_capacity);
        return record;
    }

public:
    /**
     * @brief Allocates a record with the given name capacity.
     * @param name_capacity Bytes reserved for the name, including its terminator;
     *        the default matches DbgHelp's MAX_SYM_NAME.
     * @details Allocation failure leaves the storage invalid; callers report the
     *          frame as truncated instead of resolving it.
     */
    explicit SymbolInfoStorage(::std::size_t name_capacity = default_name_capacity) noexcept
        : record{SymbolInfoStorage::allocate(name_capacity)} {
    }

    ~SymbolInfoStorage() noexcept {
        ::std::free(record);
    }

    SymbolInfoStorage(SymbolInfoStorage const&) noexcept = delete;
    SymbolInfoStorage(SymbolInfoStorage&&) noexcept = delete;
    auto operator=(SymbolInfoStorage const&) noexcept -> SymbolInfoStorage& = delete;
    auto operator=(SymbolInfoStorage&&) noexcept -> SymbolInfoStorage& = delete;

    /// Whether the record buffer was allocated.
    [[nodiscard]]
    constexpr auto is_valid(this SymbolInfoStorage const& self) noexcept -> bool {
        return self.record != nullptr;
    }

    /// The record to pass to DbgHelp lookup functions.
    [[nodiscard]]
    constexpr auto symbol_info(this SymbolInfoStorage& self) noexcept -> ::pltxt2htm::sys_win32::SymbolInfo* {
        return self.record;
    }

    /**
     * @brief Clears the reported name length before a lookup.
     * @details The allocation starts zero-filled, but a failed lookup leaves the
     *          previous NameLen in place (verified on the local DbgHelp), so the
     *          caller clears it before every attempt. SizeOfStruct and MaxNameLen
     *          need no reseeding: DbgHelp leaves its input fields unchanged.
     */
    constexpr void reset_for_lookup(this SymbolInfoStorage& self) noexcept {
        self.record->name_len = 0;
    }

    /// Name capacity in bytes, including the terminator, as stored in MaxNameLen.
    [[nodiscard]]
    constexpr auto name_capacity(this SymbolInfoStorage const& self) noexcept -> ::std::size_t {
        return static_cast<::std::size_t>(self.record->max_name_len);
    }

    /// Name length DbgHelp reported, excluding the terminator.
    [[nodiscard]]
    constexpr auto name_length(this SymbolInfoStorage const& self) noexcept -> ::std::size_t {
        return static_cast<::std::size_t>(self.record->name_len);
    }

    /// Whether DbgHelp filled the whole name buffer, so a bigger one is needed.
    [[nodiscard]]
    constexpr auto is_name_truncated(this SymbolInfoStorage const& self) noexcept -> bool {
        return self.name_length() >= self.name_capacity();
    }

    /**
     * @brief The name DbgHelp wrote, as a NUL-terminated view.
     * @pre The storage is valid and the name is not truncated.
     * @return View over the reported characters; the byte after them is the
     *         terminator supplied by the zero-filled allocation.
     */
    [[nodiscard]]
    constexpr auto name(this SymbolInfoStorage const& self) noexcept -> ::fast_io::cstring_view {
        return ::fast_io::cstring_view{::fast_io::null_terminated, self.record->name, self.name_length()};
    }

    /**
     * @brief Replaces the record with a larger zero-filled one.
     * @return False when the growth would exceed DbgHelp's length field or the
     *         allocation failed; the current record is kept unchanged.
     * @details Grows to the truncated length plus its terminator, or doubles the
     *          capacity, whichever is larger, so repeated growth stays linear in
     *          the final size.
     */
    [[nodiscard]]
    auto grow(this SymbolInfoStorage& self) noexcept -> bool {
        if (self.record == nullptr) {
            return false;
        }
        constexpr auto max_size = ::std::numeric_limits<::std::size_t>::max();
        auto const length = self.name_length();
        auto const required = length == max_size ? max_size : length + 1;
        auto const capacity = self.name_capacity();
        auto const doubled = capacity > max_size / 2 ? max_size : capacity * 2;
        auto const new_capacity = required > doubled ? required : doubled;
        if (new_capacity <= capacity) {
            return false;
        }
        auto* const replacement = SymbolInfoStorage::allocate(new_capacity);
        if (replacement == nullptr) {
            return false;
        }
        ::std::free(self.record);
        self.record = replacement;
        return true;
    }
};

} // namespace pltxt2htm::stacktrace::details

#pragma pop_macro("max")
#pragma pop_macro("min")
