#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest_config.hh"

// Every src/parser/<stem>.hh defines TEST_SUITE("<stem>") with one TEST_CASE per scenario.
// The list below is generated from the directory layout by tests/CMakeLists.txt, so adding
// or removing a case never requires editing this file.
#include "parser_cases.inc"

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}