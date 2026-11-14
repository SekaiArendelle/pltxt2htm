/**
 * @file inplace_vector.hh
 * @brief Fixed-capacity contiguous vector with inline storage for pltxt2htm.
 * @details A dynamically-resizable array whose capacity is fixed at compile time
 *          and whose elements are stored within the object itself. The API
 *          follows the C++26 std::inplace_vector proposal (P0843R14) adapted to
 *          the project's contract policy: operations that would exceed the
 *          capacity use pltxt2htm_assert rather than throwing bad_alloc, and
 *          try_* variants report failure through Optional instead.
 */

#pragma once

#include "impl/inplace_vector_decl.hh"
#include "impl/inplace_vector_def.inc"
