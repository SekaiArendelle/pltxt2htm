#include <boost/ut.hpp>

#include <cstdlib>

namespace pltxt2htm_test {

void register_fixedadv_parser_tests();
void register_md_table_tests();
void register_tab_tests();

} // namespace pltxt2htm_test

int main(int argc, char const** argv) {
    auto& runner = boost::ut::cfg<>;
    runner.on(boost::ut::events::suite<decltype(&::pltxt2htm_test::register_fixedadv_parser_tests)>{
        .run = &::pltxt2htm_test::register_fixedadv_parser_tests, .name = "fixedadv"});
    runner.on(boost::ut::events::suite<decltype(&::pltxt2htm_test::register_md_table_tests)>{
        .run = &::pltxt2htm_test::register_md_table_tests, .name = "markdown/table"});
    runner.on(boost::ut::events::suite<decltype(&::pltxt2htm_test::register_tab_tests)>{
        .run = &::pltxt2htm_test::register_tab_tests, .name = "tab"});

    auto const failed = runner.run({.report_errors = true, .argc = argc, .argv = argv});
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
