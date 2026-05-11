/**
 * @file ast_decl.hh
 * @brief AST type declarations for pltxt2htm
 * @details Defines the core AST container type used throughout the ast2 namespace.
 */

#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include "../../contracts.hh"

namespace pltxt2htm::ast2 {

template<::pltxt2htm::Contracts>
class PlTxtNode;

/**
 * @brief AST container type
 * @details A vector of ::pltxt2htm::ast2::PlTxtNode<ndebug> elements representing the Abstract Syntax Tree.
 *          This is the primary storage type for all AST nodes.
 */
template<::pltxt2htm::Contracts ndebug>
using Ast = ::fast_io::vector<::pltxt2htm::ast2::PlTxtNode<ndebug>>;

} // namespace pltxt2htm::ast2
