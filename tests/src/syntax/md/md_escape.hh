#pragma once

#include "doctest_config.hh"

TEST_CASE("md_escape") {
    {
        auto pltext = ::fast_io::u8string_view{u8"\\\\"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"\\"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"\\"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"\\"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\!"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"!"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"!"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"!"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\\""};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"&quot;"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"&quot;"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"\""});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\#"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"#"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"#"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"#"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\$"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"$"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"$"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"$"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\%"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"%"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"%"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"%"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\&"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"&amp;"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"&amp;"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"&"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\\'"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"&apos;"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"&apos;"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"'"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\("};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"("});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"("});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"("});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\)"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8")"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8")"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8")"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\*"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"*"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"*"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"*"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\+"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"+"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"+"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"+"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\,"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8","});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8","});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8","});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\-"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"-"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"-"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"-"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\."};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"."});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"."});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"."});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\/"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"/"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"/"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"/"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\:"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8":"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8":"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8":"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\;"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8";"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8";"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8";"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\<"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"&lt;"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"&lt;"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\="};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"="});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"="});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"="});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\>"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"&gt;"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"&gt;"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\?"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"?"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"?"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"?"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\@"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"@"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"@"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"@"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\["};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"["});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"["});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"["});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\]"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"]"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"]"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"]"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\^"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"^"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"^"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"^"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\_"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"_"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"_"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"_"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\`"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"`"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"`"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"`"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\{"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"{"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"{"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"{"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\|"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"|"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"|"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"|"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\}"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"}"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"}"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"}"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\~"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"~"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"~"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"~"});
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"\\a"};
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == ::fast_io::u8string_view{u8"\\a"});
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == ::fast_io::u8string_view{u8"\\a"});
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == ::fast_io::u8string_view{u8"\\a"});
    }
}

