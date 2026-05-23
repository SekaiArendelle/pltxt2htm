#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"\\\\"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"\\"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"\\"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"\\"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\!"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"!"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"!"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"!"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\\""};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"&quot;"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"&quot;"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"\""});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\#"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"#"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"#"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"#"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\$"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"$"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"$"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"$"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\%"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"%"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"%"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"%"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\&"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"&amp;"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"&amp;"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"&"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\\'"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"&apos;"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"&apos;"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"'"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\("};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"("});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"("});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"("});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\)"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8")"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8")"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8")"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\*"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"*"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"*"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"*"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\+"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"+"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"+"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"+"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\,"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8","});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8","});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8","});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\-"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"-"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"-"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"-"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\."};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"."});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"."});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"."});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\/"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"/"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"/"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"/"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\:"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8":"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8":"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8":"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\;"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8";"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8";"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8";"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\<"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"&lt;"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"&lt;"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\="};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"="});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"="});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"="});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\>"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"&gt;"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"&gt;"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\?"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"?"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"?"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"?"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\@"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"@"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"@"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"@"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\["};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"["});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"["});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"["});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\]"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"]"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"]"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"]"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\^"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"^"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"^"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"^"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\_"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"_"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"_"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"_"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\`"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"`"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"`"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"`"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\{"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"{"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"{"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"{"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\|"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"|"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"|"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"|"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\}"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"}"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"}"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"}"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\~"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"~"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"~"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"~"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\a"};
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(pltext), ::fast_io::u8string_view{u8"\\a"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(pltext), ::fast_io::u8string_view{u8"\\a"});
        pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2plunity_introduction(pltext),
                                    ::fast_io::u8string_view{u8"\\a"});
    }

    return 0;
}
