#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_emphasis") {
    TEST_CASE("*test*") {
        auto const& pltext = u8"*test*";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<em>test</em>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<i>test</i>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("t*e*st") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t*e*st");
        auto const& answer = u8"t<em>e</em>st";
        CHECK(html == answer);
    }

    TEST_CASE("t**e**st") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t**e**st");
        auto const& answer = u8"t<strong>e</strong>st";
        CHECK(html == answer);
    }

    TEST_CASE("Test triple emphasis with asterisks ***text***") {
        // Test triple emphasis with asterisks ***text***
        auto const& pltext = u8"***test***";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::pltxt2htm::container::U8String{u8"<em><strong>test</strong></em>"};
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<b><i>test</i></b>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("t***e***st") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t***e***st");
        auto const& answer = u8"t<em><strong>e</strong></em>st";
        CHECK(html == answer);
    }

    TEST_CASE("t_e_st") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t_e_st");
        auto const& answer = u8"t<em>e</em>st";
        CHECK(html == answer);
    }

    TEST_CASE("t__e__st") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t__e__st");
        auto const& answer = u8"t<strong>e</strong>st";
        CHECK(html == answer);
    }

    TEST_CASE("t___e___st") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t___e___st");
        auto const& answer = u8"t<em><strong>e</strong></em>st";
        CHECK(html == answer);
    }

    TEST_CASE("t_e*st") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t_e*st");
        auto const& answer = u8"t_e*st";
        CHECK(html == answer);
    }

    TEST_CASE("*test *") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"*test\n*");
        auto const& answer = u8"*test<br>*";
        CHECK(html == answer);
    }

    TEST_CASE("**test **") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"**test\n**");
        auto const& answer = u8"**test<br>**";
        CHECK(html == answer);
    }

    TEST_CASE("te***st ***") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te***st\n***");
        auto const& answer = u8"te***st<br><hr>";
        CHECK(html == answer);
    }

    TEST_CASE("Test triple emphasis with underscores ___text___") {
        // Test triple emphasis with underscores ___text___
        auto const& pltext = u8"___test___";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::pltxt2htm::container::U8String{u8"<em><strong>test</strong></em>"};
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<b><i>test</i></b>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("t****t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t****t");
        auto answer = ::pltxt2htm::container::U8String{u8"t****t"};
        CHECK(html == answer);
    }

    TEST_CASE("t***t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t***t");
        auto answer = ::pltxt2htm::container::U8String{u8"t***t"};
        CHECK(html == answer);
    }

    TEST_CASE("t*****t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t*****t");
        auto answer = ::pltxt2htm::container::U8String{u8"t*****t"};
        CHECK(html == answer);
    }

    TEST_CASE("&__</_") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&__</_");
        auto const& answer = u8"&amp;_<em>&lt;/</em>";
        CHECK(html == answer);
    }

    // Redundant tag elimination: triple emphasis inside em-like parent → em part redundant
    TEST_CASE("Redundant tag elimination: triple emphasis inside em-lik...") {
        // <em> wrapping *** → em part of triple is redundant, convert to <strong>
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em>***text***</em>");
        auto const& answer = u8"<em><strong>text</strong></em>";
        CHECK(html == answer);
    }

    TEST_CASE("<i> wrapping *** → same as <em>; convert to <strong>") {
        // <i> wrapping *** → same as <em>, convert to <strong>
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<i>***text***</i>");
        auto const& answer = u8"<em><strong>text</strong></em>";
        CHECK(html == answer);
    }

    // Redundant tag elimination: triple emphasis inside strong-like parent → strong part redundant
    TEST_CASE("Redundant tag elimination: triple emphasis inside strong...") {
        // <strong> wrapping *** → strong part of triple is redundant, convert to <em>
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong>***text***</strong>");
        auto const& answer = u8"<strong><em>text</em></strong>";
        CHECK(html == answer);
    }

    TEST_CASE("<b> wrapping *** → same as <strong>; convert to <em>") {
        // <b> wrapping *** → same as <strong>, convert to <em>
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b>***text***</b>");
        auto const& answer = u8"<strong><em>text</em></strong>";
        CHECK(html == answer);
    }

    // Redundant tag elimination: em/strong inside triple emphasis → child redundant
    TEST_CASE("Redundant tag elimination: em/strong inside triple empha...") {
        // <em> inside *** → em redundant (triple already provides <em>)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"***<em>text</em>***");
        auto const& answer = u8"<em><strong>text</strong></em>";
        CHECK(html == answer);
    }

    TEST_CASE("<strong> inside *** → strong redundant (triple already p...") {
        // <strong> inside *** → strong redundant (triple already provides <strong>)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"***<strong>text</strong>***");
        auto const& answer = u8"<em><strong>text</strong></em>";
        CHECK(html == answer);
    }

    // Redundant tag elimination: with surrounding text content
    TEST_CASE("Redundant tag elimination: with surrounding text content") {
        // <em> wrapping text***text***text → triple converted to <strong> in em context
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em>text***text***text</em>");
        auto const& answer = u8"<em>text<strong>text</strong>text</em>";
        CHECK(html == answer);
    }

    TEST_CASE("<strong> wrapping text***text***text → triple converted...") {
        // <strong> wrapping text***text***text → triple converted to <em> in strong context
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong>text***text***text</strong>");
        auto const& answer = u8"<strong>text<em>text</em>text</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("*** surrounding text<em>text</em>text → inner em redunda...") {
        // *** surrounding text<em>text</em>text → inner em redundant, unwrapped
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text***<em>text</em>***text");
        auto const& answer = u8"text<em><strong>text</strong></em>text";
        CHECK(html == answer);
    }

    TEST_CASE("*** surrounding text<strong>text</strong>text → inner st...") {
        // *** surrounding text<strong>text</strong>text → inner strong redundant, unwrapped
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text***<strong>text</strong>***text");
        auto const& answer = u8"text<em><strong>text</strong></em>text";
        CHECK(html == answer);
    }

    // Regression test: control chars inside ***...*** used to crash
    // Control chars 0x00-0x1F/0x7F now produce InvalidUtf8 (rendered as U+FFFD)
    // instead of being silently dropped, ensuring the sub-AST is never empty.
    TEST_CASE("instead of being silently dropped; ensuring the sub-AST...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"***\a***");
        auto const& answer = u8"<em><strong>\uFFFD</strong></em>";
        CHECK(html == answer);
    }

    TEST_CASE("ab**test**cd") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab**test**cd");
        auto const& answer = u8"ab<b>test</b>cd";
        CHECK(html == answer);
    }

    TEST_CASE("ab*test*cd") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab*test*cd");
        auto const& answer = u8"ab<i>test</i>cd";
        CHECK(html == answer);
    }

    TEST_CASE("__bold__") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"__bold__");
        auto const& answer = u8"<b>bold</b>";
        CHECK(html == answer);
    }

    TEST_CASE("_italic_") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"_italic_");
        auto const& answer = u8"<i>italic</i>";
        CHECK(html == answer);
    }
}
