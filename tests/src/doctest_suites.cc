#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest_config.hh"

// Syntax and parser case headers share this translation unit. CMake generates the include
// list from their directory layout, so adding or removing a case needs no edit here.
#include "doctest_suites_cases.inc"

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
