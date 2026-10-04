#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest_config.hh"

// Every case header under src/ (src/syntax/<family>/<stem>.hh, src/parser/<stem>.hh, ...) is
// header-only: it defines TEST_SUITE("<stem>") with one TEST_CASE per scenario. They all share
// this translation unit, pulled in by the list generated from the directory layout by
// tests/CMakeLists.txt, so adding or removing a case never requires editing this file. Cases
// that need their own translation unit stay as .cc and keep their own test.
#include "cases.inc"

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}