#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest_config.hh"

// Every src/syntax/<family>/<stem>.hh defines TEST_SUITE("<stem>") with one TEST_CASE per
// scenario. The list below is generated from the directory layout by tests/CMakeLists.txt,
// so a new case is picked up without editing this file.
#include "syntax_cases.inc"

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}