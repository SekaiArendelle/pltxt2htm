// Counterpart of test_deep_copy.cc: this TU is built with PLTXT2HTM_INTERNAL_USE, so it guards the
// project's own view of the AST instead of the external user's view (see tests/CMakeLists.txt).
// If a build forgets to define the macro, the assertions below fail and the mistake surfaces here
// rather than silently leaving the implementation free to deep-copy nodes.

#include <type_traits>
#include <pltxt2htm/ast/ast.hh>

namespace {

using nd = ::pltxt2htm::Contracts;

static_assert(!::std::is_copy_constructible_v<::pltxt2htm::PlTxtNode<nd::quick_enforce>>,
              "PlTxtNode must not be copy constructible while building pltxt2htm itself");

static_assert(!::std::is_copy_assignable_v<::pltxt2htm::PlTxtNode<nd::quick_enforce>>,
              "PlTxtNode must not be copy assignable while building pltxt2htm itself");

static_assert(::std::is_move_constructible_v<::pltxt2htm::PlTxtNode<nd::quick_enforce>>,
              "PlTxtNode must stay move constructible");

static_assert(::std::is_move_assignable_v<::pltxt2htm::PlTxtNode<nd::quick_enforce>>,
              "PlTxtNode must stay move assignable");

} // namespace

int main() {
    return 0;
}