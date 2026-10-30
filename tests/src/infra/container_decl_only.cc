#include <pltxt2htm/container/impl/array_decl.hh>
#include <pltxt2htm/container/impl/deque_decl.hh>
#include <pltxt2htm/container/impl/non_zero_decl.hh>
#include <pltxt2htm/container/impl/optional_decl.hh>
#include <pltxt2htm/container/impl/string_decl.hh>
#include <pltxt2htm/container/impl/string_view_decl.hh>
#include <pltxt2htm/container/impl/vector_decl.hh>

#if defined(pltxt2htm_assert) || defined(pltxt2htm_assume) || defined(pltxt2htm_unreachable)
    #error "declaration-only container headers must not define assertion macros"
#endif

auto main() -> int {
    return 0;
}
