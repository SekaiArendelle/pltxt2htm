#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"> [a](example.com)");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><a href=\"example.com\">a</a></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"> <color=red>test</color>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><span style=\"color:red;\">test</span></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"> <experiment=642cf37a494746375aae306a>physicsLab</experiment>");
        auto answer = ::fast_io::u8string_view{
            u8"<blockquote><a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>physicsLab</a></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"> <external=https://example.com/discussion>physicsLab</external>");
        auto answer = ::fast_io::u8string_view{
            u8"<blockquote><a href=\"https://example.com/discussion\">physicsLab</a></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
