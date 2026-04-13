#include <exception>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../../astnode/node_type.hh"
#include "../../astnode/basic.hh"

namespace pltxt2htm::details {

template<bool ndebug>
constexpr auto convert_simple_pltxt_ast_to_string(::pltxt2htm::Ast const& ast) noexcept -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    for (auto&& node : ast) {
        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char: {
            break;
        }
        case ::pltxt2htm::NodeType::invalid_u8char: {
            break;
        }
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug>();
            }
        }
    }

    return result;
}
} // namespace pltxt2htm::details
