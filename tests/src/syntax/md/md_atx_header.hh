#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_atx_header") {
    TEST_CASE("after-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n# test<br>text");
        auto const& answer = u8"<br><h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("br-terminates-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# test<br>text");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("text-after-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# test\ntext");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("br-then-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br># test\ntext");
        auto const& answer = u8"<br><h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("empty-input") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-bold-inside") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# test<b>\ntext");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("bold-inside-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# te<b>st\ntext");
        auto const& answer = u8"<h1>te<strong>st</strong></h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("h2") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"## test\ntext");
        auto const& answer = u8"<h2>test</h2>text";
        CHECK(html == answer);
    }

    TEST_CASE("h3") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"### test");
        auto const& answer = u8"<h3>test</h3>";
        CHECK(html == answer);
    }

    TEST_CASE("h4") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#### test");
        auto const& answer = u8"<h4>test</h4>";
        CHECK(html == answer);
    }

    TEST_CASE("h5") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"##### test");
        auto const& answer = u8"<h5>test</h5>";
        CHECK(html == answer);
    }

    TEST_CASE("h6") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"###### test\ntext");
        auto const& answer = u8"<h6>test</h6>text";
        CHECK(html == answer);
    }

    TEST_CASE("seven-hashes-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"####### test\ntext");
        auto const& answer = u8"#######&nbsp;test<br>text";
        CHECK(html == answer);
    }

    TEST_CASE("no-space-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#test\ntext");
        auto const& answer = u8"#test<br>text";
        CHECK(html == answer);
    }

    TEST_CASE("many-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#            test\ntext");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("leading-spaces-indented") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"          # test\ntext");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("tab-surrounded") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\t#\ttest\ntext");
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-size-38") {
        auto const& pltext = u8"# test";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h1>test</h1>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=38><b>test</b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("plunity-text-after") {
        auto const& pltext = u8"# test\ntext";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h1>test</h1>text";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=38><b>test</b></size>\ntext";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("two-headings") {
        auto const& pltext = u8"# test\n# more";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h1>test</h1><h1>more</h1>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=38><b>test</b></size>\n<size=38><b>more</b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("empty-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# ");
        auto const& answer = u8"<h1></h1>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-heading-after-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n# ");
        auto const& answer = u8"<br><h1></h1>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-heading-after-br") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br># ");
        auto const& answer = u8"<br><h1></h1>";
        CHECK(html == answer);
    }

    TEST_CASE("lone-hash") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#");
        auto const& answer = u8"<h1></h1>";
        CHECK(html == answer);
    }

    TEST_CASE("lone-double-hash") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n##");
        auto const& answer = u8"<br><h2></h2>";
        CHECK(html == answer);
    }

    TEST_CASE("br-then-double-hash") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Br/>##");
        auto const& answer = u8"<br><h2></h2>";
        CHECK(html == answer);
    }

    TEST_CASE("escaped-hash") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\\# test<br>text");
        auto const& answer = u8"#&nbsp;test<br>text";
        CHECK(html == answer);
    }

    TEST_CASE("italic-inside") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# <i>test<br>text");
        auto const& answer = u8"<h1><em>test</em></h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("italic-inside-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# <i>test\ntext");
        auto const& answer = u8"<h1><em>test</em></h1>text";
        CHECK(html == answer);
    }

    TEST_CASE("hash-as-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" ## #");
        auto const& answer = u8"<h2>#</h2>";
        CHECK(html == answer);
    }

    // Note that this test does not match standard markdown
    // If this issues, I will fix it
    TEST_CASE("space-then-hash-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" #\n");
        auto const& answer = u8"&nbsp;#<br>";
        CHECK(html == answer);
    }

    TEST_CASE("lone-triple-hash") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"###");
        auto const& answer = u8"<h3></h3>";
        CHECK(html == answer);
    }

    TEST_CASE("lone-quadruple-hash") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"####");
        auto const& answer = u8"<h4></h4>";
        CHECK(html == answer);
    }

    TEST_CASE("lone-quintuple-hash") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#####");
        auto const& answer = u8"<h5></h5>";
        CHECK(html == answer);
    }

    TEST_CASE("lone-sextuple-hash") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"######");
        auto const& answer = u8"<h6></h6>";
        CHECK(html == answer);
    }

    TEST_CASE("lone-septuple-hash") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"#######");
        auto const& answer = u8"#######";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-h2-size-37") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"## heading");
        auto const& answer = u8"<size=37><b>heading</b></size>\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-h3-size-36") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"### heading");
        auto const& answer = u8"<size=36><b>heading</b></size>\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-h4-size-35") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"#### heading");
        auto const& answer = u8"<size=35><b>heading</b></size>\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-h5-bold") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"##### heading");
        auto const& answer = u8"<b>heading</b>\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-h6-bold") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"###### heading");
        auto const& answer = u8"<b>heading</b>\n";
        CHECK(html == answer);
    }
}
