#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_atx_header") {
    TEST_CASE("# test<br>text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n# test<br>text");
        auto const& answer = u8"<br><h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("# test<br>text (2)") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# test<br>text");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("# test text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# test\ntext");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("<br># test text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br># test\ntext");
        auto const& answer = u8"<br><h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("case 5") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("# test<b> text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# test<b>\ntext");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("# te<b>st text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# te<b>st\ntext");
        auto const& answer = u8"<h1>te<strong>st</strong></h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("## test text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"## test\ntext");
        auto const& answer = u8"<h2>test</h2>text";
        CHECK(html == answer);
    }

    TEST_CASE("### test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"### test");
        auto const& answer = u8"<h3>test</h3>";
        CHECK(html == answer);
    }

    TEST_CASE("#### test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#### test");
        auto const& answer = u8"<h4>test</h4>";
        CHECK(html == answer);
    }

    TEST_CASE("##### test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"##### test");
        auto const& answer = u8"<h5>test</h5>";
        CHECK(html == answer);
    }

    TEST_CASE("###### test text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"###### test\ntext");
        auto const& answer = u8"<h6>test</h6>text";
        CHECK(html == answer);
    }

    TEST_CASE("####### test text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"####### test\ntext");
        auto const& answer = u8"#######&nbsp;test<br>text";
        CHECK(html == answer);
    }

    TEST_CASE("#test text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#test\ntext");
        auto const& answer = u8"#test<br>text";
        CHECK(html == answer);
    }

    TEST_CASE("# test text (15)") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#            test\ntext");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("# test text (16)") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"          # test\ntext");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("# test text (17)") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\t#\ttest\ntext");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("# test") {
        auto const& pltext = u8"# test";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h1>test</h1>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=38><b>test</b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("# test text (19)") {
        auto const& pltext = u8"# test\ntext";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=38><b>test</b></size>\ntext";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("# test # more") {
        auto const& pltext = u8"# test\n# more";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h1>test</h1><h1>more</h1>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=38><b>test</b></size>\n<size=38><b>more</b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("case 21") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# ");
        auto const& answer = u8"<h1></h1>";
        CHECK(html == answer);
    }

    TEST_CASE("case 22") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n# ");
        auto const& answer = u8"<br><h1></h1>";
        CHECK(html == answer);
    }

    TEST_CASE("<br>#") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br># ");
        auto const& answer = u8"<br><h1></h1>";
        CHECK(html == answer);
    }

    TEST_CASE("case 24") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#");
        auto const& answer = u8"<h1></h1>";
        CHECK(html == answer);
    }

    TEST_CASE("##") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n##");
        auto const& answer = u8"<br><h2></h2>";
        CHECK(html == answer);
    }

    TEST_CASE("<Br/>##") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Br/>##");
        auto const& answer = u8"<br><h2></h2>";
        CHECK(html == answer);
    }

    TEST_CASE("/# test<br>text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\\# test<br>text");
        auto const& answer = u8"#&nbsp;test<br>text";
        CHECK(html == answer);
    }

    TEST_CASE("# <i>test<br>text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# <i>test<br>text");
        auto const& answer = u8"<h1><em>test</em></h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("# <i>test text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# <i>test\ntext");
        auto const& answer = u8"<h1><em>test</em></h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("## #") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" ## #");
        auto const& answer = u8"<h2>#</h2>";
        CHECK(html == answer);
    }

    // Note that this test does not match standard markdown
    // If this issues, I will fix it
    TEST_CASE("If this issues; I will fix it") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" #\n");
        auto const& answer = u8"&nbsp;#<br>";
        CHECK(html == answer);
    }

    TEST_CASE("###") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"###");
        auto const& answer = u8"<h3></h3>";
        CHECK(html == answer);
    }

    TEST_CASE("####") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"####");
        auto const& answer = u8"<h4></h4>";
        CHECK(html == answer);
    }

    TEST_CASE("#####") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#####");
        auto const& answer = u8"<h5></h5>";
        CHECK(html == answer);
    }

    TEST_CASE("######") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"######");
        auto const& answer = u8"<h6></h6>";
        CHECK(html == answer);
    }

    TEST_CASE("#######") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#######");
        auto const& answer = u8"#######";
        CHECK(html == answer);
    }

    TEST_CASE("## heading") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"## heading");
        auto const& answer = u8"<size=37><b>heading</b></size>\n";
        CHECK(html == answer);
    }

    TEST_CASE("### heading") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"### heading");
        auto const& answer = u8"<size=36><b>heading</b></size>\n";
        CHECK(html == answer);
    }

    TEST_CASE("#### heading") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"#### heading");
        auto const& answer = u8"<size=35><b>heading</b></size>\n";
        CHECK(html == answer);
    }

    TEST_CASE("##### heading") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"##### heading");
        auto const& answer = u8"<b>heading</b>\n";
        CHECK(html == answer);
    }

    TEST_CASE("###### heading") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"###### heading");
        auto const& answer = u8"<b>heading</b>\n";
        CHECK(html == answer);
    }
}
