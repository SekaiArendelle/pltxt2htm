#include "precompile.hh"
#include "pltxt2htm/details/parser/md_list.hh"

namespace pltxt2htm_test {

constexpr auto reverse_md_list_ast_to_md(::pltxt2htm::details::MdListAst const& ast) -> ::fast_io::u8string {}

}

int main() {
    {
        auto data = ::fast_io::u8string_view{u8"- test"};
    }

    return 0;
}
