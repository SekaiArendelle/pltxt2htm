#include <cstddef>
#include <pltxt2htm/ast/ast.hh>

#if __cpp_impl_reflection >= 202506L

    #include <meta>

namespace {

constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
using Storage = ::pltxt2htm::details::PlTxtNodeStorage<ndebug>;
using Text = ::pltxt2htm::Text<ndebug>;

// Reflecting the node union cannot miss a node type: every union member is one
// concrete AST node type, and the members are what the union layout is built from.
[[nodiscard]]
consteval auto largest_other_node_size() noexcept -> ::std::size_t {
    ::std::size_t result{};
    template for (constexpr auto member : ::std::define_static_array(
                      ::std::meta::nonstatic_data_members_of(^^Storage, ::std::meta::access_context::unprivileged()))) {
        constexpr auto member_type = ::std::meta::type_of(member);
        if constexpr (member_type == ^^Text) {
            continue;
        }
        auto const member_size = ::std::meta::size_of(member_type);
        if (result < member_size) {
            result = member_size;
        }
    }
    return result;
}

static_assert(sizeof(Text) == largest_other_node_size(),
              "Text must use the full storage footprint of the largest other AST node");

} // namespace

#else

    #warning "node_layout reflection test skipped: requires GCC 16 or newer with C++26"

#endif

int main() {
    return 0;
}
