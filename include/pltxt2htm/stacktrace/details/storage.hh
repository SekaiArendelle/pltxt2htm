/**
 * @file storage.hh
 * @brief Growable storage used by the stacktrace implementation.
 * @details Instantiates the contract-checked `container::Vector` through its
 *          declaration-only header; the checked definitions are included at the
 *          end of stacktrace.hh.
 */

#pragma once

#include <fast_io/fast_io_core.h>

#include "../../container/impl/vector_decl.hh"

namespace pltxt2htm::stacktrace::details {

template<typename T, typename Allocator>
using Buffer = ::pltxt2htm::container::Vector<T, ::fast_io::generic_allocator_adapter<Allocator>>;

} // namespace pltxt2htm::stacktrace::details
