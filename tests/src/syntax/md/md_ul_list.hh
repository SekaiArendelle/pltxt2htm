#pragma once

#include "doctest_config.hh"

TEST_CASE("md_ul_list") {
    {
        auto const& pltext = u8"- test";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<ul><li>test</li></ul>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"• test\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test");
        auto const& answer = u8"<ul><li>test</li><li>test</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   - text");
        auto const& answer = u8"<ul><li>test</li><li>test<ul><li>text</li></ul></li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   - text\n     - test");
        auto const& answer = u8"<ul><li>test</li><li>test<ul><li>text<ul><li>test</li></ul></li></ul></li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n  - test\n   - test\n    - test");
        auto const& answer = u8"<ul><li>test</li><li>test</li><li>test</li><li>test</li><li>test</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   - t**ex**t");
        auto const& answer = u8"<ul><li>test</li><li>test<ul><li>t<strong>ex</strong>t</li></ul></li></ul>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"-\ttest");
        auto const& answer = u8"<ul><li>test</li></ul>";
        CHECK(html == answer);
    }

    // ---- + marker tests ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"+ test");
        auto const& answer = u8"<ul><li>test</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"+ test\n + test");
        auto const& answer = u8"<ul><li>test</li><li>test</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"+ test\n   + test");
        auto const& answer = u8"<ul><li>test<ul><li>test</li></ul></li></ul>";
        CHECK(html == answer);
    }

    // ---- * marker tests ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* test");
        auto const& answer = u8"<ul><li>test</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* test\n * test");
        auto const& answer = u8"<ul><li>test</li><li>test</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* test\n   * test");
        auto const& answer = u8"<ul><li>test<ul><li>test</li></ul></li></ul>";
        CHECK(html == answer);
    }

    // ---- empty list item ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - ");
        auto const& answer = u8"<ul><li></li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - test\n - ");
        auto const& answer = u8"<ul><li>test</li><li></li></ul>";
        CHECK(html == answer);
    }

    // ---- different markers ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - text\n   - text\n   * text");
        auto const& answer = u8"<ul><li>text<ul><li>text</li><li>text</li></ul></li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - text\n   - text\n * text");
        auto const& answer = u8"<ul><li>text<ul><li>text</li></ul></li></ul><ul><li>text</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - text\n - text\n * text");
        auto const& answer = u8"<ul><li>text</li><li>text</li></ul><ul><li>text</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - text\n + test");
        auto const& answer = u8"<ul><li>text</li></ul><ul><li>test</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   + text");
        auto const& answer = u8"<ul><li>test</li><li>test<ul><li>text</li></ul></li></ul>";
        CHECK(html == answer);
    }

    // ---- list mixed with text ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello\n- foo\n- bar\nworld");
        auto const& answer = u8"hello<br><ul><li>foo</li><li>bar</li></ul>world";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"before\n- item\nbetween\n+ another\n- last\nafter");
        auto const& answer =
            u8"before<br><ul><li>item</li></ul>between<br><ul><li>another</li></ul><ul><li>last</li></ul>after";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"first\n- a\n- b\n- c\nlast");
        auto const& answer = u8"first<br><ul><li>a</li><li>b</li><li>c</li></ul>last";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- a\n+ b\n* c");
        auto const& answer = u8"<ul><li>a</li></ul><ul><li>b</li></ul><ul><li>c</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text\n- item");
        auto const& answer = u8"text<br><ul><li>item</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- item\ntext");
        auto const& answer = u8"<ul><li>item</li></ul>text";
        CHECK(html == answer);
    }

    // ---- nested different markers in child ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- a\n  - b\n  + c");
        auto const& answer = u8"<ul><li>a<ul><li>b</li><li>c</li></ul></li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- a\n  - b\n+ c");
        auto const& answer = u8"<ul><li>a<ul><li>b</li></ul></li></ul><ul><li>c</li></ul>";
        CHECK(html == answer);
    }

    // ---- alternating text and single-item lists ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"a\n- 1\nb\n- 2\nc\n- 3");
        auto const& answer = u8"a<br><ul><li>1</li></ul>b<br><ul><li>2</li></ul>c<br><ul><li>3</li></ul>";
        CHECK(html == answer);
    }

    // ---- emphasis inside list ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- **bold**\n- *italic*");
        auto const& answer = u8"<ul><li><strong>bold</strong></li><li><em>italic</em></li></ul>";
        CHECK(html == answer);
    }

    // ---- varying indentation ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"    - a\n   - b\n      - c");
        auto const& answer = u8"<ul><li>a</li><li>b<ul><li>c</li></ul></li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"     - a\n   - b\n    - c");
        auto const& answer = u8"<ul><li>a</li><li>b</li><li>c</li></ul>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"- test\n- text");
        auto const& answer = u8"• test\n• text\n";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"* test\n  - text\n    + test");
        auto const& answer = u8"• test\n  ∘ text\n    ▫ test\n";
        CHECK(html == answer);
    }

    // regression: roundtrip fuzzer finding. Consecutive sibling-nested lists
    // ([li, ul, ul]) must keep the parent item's </li> open across the whole run and
    // emit it only once, otherwise the emitted HTML (<ul> directly inside <ul> plus a
    // stray </li>) is rejected by the HTML list scanner and the roundtrip diverges.
    {
        auto const& pltext = u8"*\t&\n\t\t\t\t*\t&*\t\t&*\t&\n\t\t*\t\n\t\t*\t\t\t~~~\t \"  & \n";
        auto once = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto const& once_answer =
            u8"<ul><li>&amp;<ul><li>&amp;<em>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&amp;</"
            u8"em>&nbsp;&nbsp;&nbsp;&nbsp;"
            u8"&amp;</li></ul><ul><li></li><li>~~~&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&quot;&nbsp;&nbsp;&amp;</li></ul></"
            u8"li></ul>";
        CHECK(once == once_answer);
        auto twice =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{once.data(), once.size()});
        CHECK(twice == once);
    }
}
