/**
 * @file stacktrace_def.inc
 * @brief Out-of-line contract-checked definitions for BasicStacktrace.
 * @details Declarations live in `stacktrace.hh`, which includes this file after
 *          the class definition. The assert machinery included here reaches the
 *          panic definition, which names `Stacktrace`, so this file must not be
 *          included before the class is declared.
 */

#pragma once

#include "../../details/push_macro.hh"

namespace pltxt2htm::stacktrace {

template<typename Allocator>
constexpr auto BasicStacktrace<Allocator>::operator=(this BasicStacktrace& self, BasicStacktrace const& other) noexcept
    -> BasicStacktrace& {
    pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"Self-assignment is not allowed");
    self.entries = other.entries;
    self.truncated = other.truncated;
    return self;
}

template<typename Allocator>
constexpr auto BasicStacktrace<Allocator>::operator=(this BasicStacktrace& self, BasicStacktrace&& other) noexcept
    -> BasicStacktrace& {
    pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"Self-move-assignment is not allowed");
    self.entries = ::std::move(other.entries);
    self.truncated = other.truncated;
    other.truncated = false;
    return self;
}

template<typename Allocator>
constexpr auto BasicStacktrace<Allocator>::operator[](this BasicStacktrace const& self, size_type index) noexcept
    -> const_reference {
    pltxt2htm_assert(index < self.size(), u8"Index of Stacktrace out of bound");
    return self.entries[index];
}

} // namespace pltxt2htm::stacktrace

#include "../../details/pop_macro.hh"
