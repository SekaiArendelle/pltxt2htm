#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest_config.hh"

// CMake generates this list from AST case headers in the same directory.
#include "doctest_ast_cases.inc"

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
