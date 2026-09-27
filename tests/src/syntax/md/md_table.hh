#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_table") {
    // Basic table with header and data rows
    TEST_CASE("basic-with-data-rows") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| Header 1 | Header 2 |\n"
            u8"|----------|----------|\n"
            u8"| Cell 1   | Cell 2   |\n"
            u8"| Cell 3   | Cell 4   |");
        auto const& answer =
            u8"<table><thead><tr><th>Header&nbsp;1</th><th>Header&nbsp;2</th></tr></thead>"
            u8"<tbody><tr><td>Cell&nbsp;1</td><td>Cell&nbsp;2</td></tr>"
            u8"<tr><td>Cell&nbsp;3</td><td>Cell&nbsp;4</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Table preceded by a line break
    TEST_CASE("preceded-by-line-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"\n| A | B |\n"
            u8"|---|---|\n"
            u8"| 1 | 2 |");
        auto const& answer =
            u8"<br><table><thead><tr><th>A</th><th>B</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Table with varying spaces
    TEST_CASE("varying-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"|  a  |  b  |\n"
            u8"|-----|-----|\n"
            u8"|  x  |  y  |");
        auto const& answer =
            u8"<table><thead><tr><th>a</th><th>b</th></tr></thead>"
            u8"<tbody><tr><td>x</td><td>y</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Table with empty cells
    TEST_CASE("empty-cells") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|---|---|---|\n"
            u8"| 1 |   | 3 |");
        auto const& answer =
            u8"<table><thead><tr><th>A</th><th>B</th><th>C</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td></td><td>3</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Table with only header (no data rows)
    TEST_CASE("header-only") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| X | Y |\n"
            u8"|---|---|");
        auto const& answer = u8"<table><thead><tr><th>X</th><th>Y</th></tr></thead></table>";
        CHECK(html == answer);
    }

    // Table with special characters in cells
    TEST_CASE("special-characters") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| a & b | c > d |\n"
            u8"|-------|-------|\n"
            u8"| x & y | p < q |");
        auto const& answer =
            u8"<table><thead><tr><th>a&nbsp;&amp;&nbsp;b</th><th>c&nbsp;&gt;&nbsp;d</th></tr></thead>"
            u8"<tbody><tr><td>x&nbsp;&amp;&nbsp;y</td><td>p&nbsp;&lt;&nbsp;q</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Table with escape sequences
    TEST_CASE("escape-sequences") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| \\\"quote\\\" |\n"
            u8"|-----------|\n"
            u8"| \\&amp;   |");
        auto const& answer =
            u8"<table><thead><tr><th>&quot;quote&quot;</th></tr></thead>"
            u8"<tbody><tr><td>&amp;amp;</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Table followed by text (text should be separated)
    TEST_CASE("followed-by-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| H |\n"
            u8"|---|\n"
            u8"| C |\n"
            u8"after");
        auto const& answer =
            u8"<table><thead><tr><th>H</th></tr></thead>"
            u8"<tbody><tr><td>C</td></tr></tbody></table>"
            u8"after";
        CHECK(html == answer);
    }

    TEST_CASE("text-before-and-after") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"before\n"
            u8"| H |\n"
            u8"|---|\n"
            u8"| C |\n"
            u8"after");
        auto const& answer =
            u8"before<br><table><thead><tr><th>H</th></tr></thead>"
            u8"<tbody><tr><td>C</td></tr></tbody></table>"
            u8"after";
        CHECK(html == answer);
    }

    // Table after a <br> tag
    TEST_CASE("after-br-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<br>| A | B |\n"
            u8"    |---|---|\n"
            u8"    | 1 | 2 |");
        auto const& answer =
            u8"<br><table><thead><tr><th>A</th><th>B</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Left-aligned columns (default, no style attribute)
    TEST_CASE("left-aligned-default") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|:---|:---|\n"
            u8"| 1 | 2 |");
        auto const& answer =
            u8"<table><thead><tr><th>A</th><th>B</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Center-aligned columns
    TEST_CASE("center-aligned") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|:---:|:---:|\n"
            u8"| 1 | 2 |");
        auto const& answer =
            u8"<table><thead><tr><th style=\"text-align:center\">A</th><th "
            u8"style=\"text-align:center\">B</th></tr></thead>"
            u8"<tbody><tr><td style=\"text-align:center\">1</td><td "
            u8"style=\"text-align:center\">2</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Right-aligned columns
    TEST_CASE("right-aligned") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|---:|---:|\n"
            u8"| 1 | 2 |");
        auto const& answer =
            u8"<table><thead><tr><th style=\"text-align:right\">A</th><th "
            u8"style=\"text-align:right\">B</th></tr></thead>"
            u8"<tbody><tr><td style=\"text-align:right\">1</td><td "
            u8"style=\"text-align:right\">2</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Mixed alignment in one table
    TEST_CASE("mixed-alignment") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| Left | Center | Right |\n"
            u8"|:-----|:------:|------:|\n"
            u8"| a    |   b    |   c   |");
        auto const& answer =
            u8"<table><thead><tr>"
            u8"<th>Left</th><th style=\"text-align:center\">Center</th><th style=\"text-align:right\">Right</th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td>a</td><td style=\"text-align:center\">b</td><td style=\"text-align:right\">c</td>"
            u8"</tr></tbody></table>";
        CHECK(html == answer);
    }

    // Header-only table with alignment
    TEST_CASE("header-only-with-alignment") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| X | Y |\n"
            u8"|:---:|:---|");
        auto const& answer =
            u8"<table><thead><tr><th style=\"text-align:center\">X</th><th>Y</th></tr></thead></table>";
        CHECK(html == answer);
    }

    // Alignment with empty cells
    TEST_CASE("alignment-with-empty-cells") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|:---:|:---:|---:|\n"
            u8"| 1 |   | 3 |");
        auto const& answer =
            u8"<table><thead><tr>"
            u8"<th style=\"text-align:center\">A</th>"
            u8"<th style=\"text-align:center\">B</th>"
            u8"<th style=\"text-align:right\">C</th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td style=\"text-align:center\">1</td>"
            u8"<td style=\"text-align:center\"></td>"
            u8"<td style=\"text-align:right\">3</td>"
            u8"</tr></tbody></table>";
        CHECK(html == answer);
    }

    // --- Negative tests: inputs that should NOT be parsed as tables ---

    // Single pipe-delimited line without a delimiter row
    TEST_CASE("single-line-no-delimiter-not-table") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"| A | B |\n");
        auto const& answer = u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|<br>";
        CHECK(html == answer);
    }

    // Text followed by a valid delimiter (first line is not a pipe table row)
    TEST_CASE("text-then-delimiter-not-table") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text\n|---|---|\n");
        auto const& answer = u8"text<br>|---|---|<br>";
        CHECK(html == answer);
    }

    // Pipe character in the middle of regular text
    TEST_CASE("pipe-in-text-not-table") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"this | that\n");
        auto const& answer = u8"this&nbsp;|&nbsp;that<br>";
        CHECK(html == answer);
    }

    // Single pipe character on a line
    TEST_CASE("lone-pipe-not-table") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"|\n");
        auto const& answer = u8"|<br>";
        CHECK(html == answer);
    }

    // Header line without leading pipe (no table)
    TEST_CASE("no-leading-pipe-not-table") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"A | B\n|---|---|\n");
        auto const& answer = u8"A&nbsp;|&nbsp;B<br>|---|---|<br>";
        CHECK(html == answer);
    }

    // Delimiter row without a preceding header
    TEST_CASE("delimiter-without-header-not-table") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"|---|---|---|\n");
        auto const& answer = u8"|---|---|---|<br>";
        CHECK(html == answer);
    }

    // Delimiter row with no dashes (all spaces) - should NOT be a table
    TEST_CASE("delimiter-all-spaces-not-table") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"| A | B |\n|   |   |\n");
        auto const& answer =
            u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|<br>"
            u8"|&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;|<br>";
        CHECK(html == answer);
    }

    // --- Additional positive tests for delimiter patterns ---

    // Single dash in delimiter (minimum valid: -+)
    TEST_CASE("single-dash-delimiter") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|-|\n"
            u8"| B |");
        auto const& answer =
            u8"<table><thead><tr><th>A</th></tr></thead>"
            u8"<tbody><tr><td>B</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Two dashes left-aligned
    TEST_CASE("two-dashes-left") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|:--|\n"
            u8"| B |");
        auto const& answer =
            u8"<table><thead><tr><th>A</th></tr></thead>"
            u8"<tbody><tr><td>B</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Three dashes right-aligned
    TEST_CASE("three-dashes-right") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|---:|\n"
            u8"| B |");
        auto const& answer =
            u8"<table><thead><tr><th style=\"text-align:right\">A</th></tr></thead>"
            u8"<tbody><tr><td style=\"text-align:right\">B</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Trailing whitespace in delimiter row
    TEST_CASE("trailing-whitespace-in-delimiter") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| H |\n"
            u8"|---|  \n"
            u8"| C |");
        auto const& answer =
            u8"<table><thead><tr><th>H</th></tr></thead>"
            u8"<tbody><tr><td>C</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Spaces around dashes within delimiter cells
    TEST_CASE("spaces-around-dashes") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|---| --- |\n"
            u8"| 1 | 2 |");
        auto const& answer =
            u8"<table><thead><tr><th>A</th><th>B</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Very long dashes with center alignment
    TEST_CASE("long-dashes-center") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|:-----------------:|:-----------------:|\n"
            u8"| 1 | 2 |");
        auto const& answer =
            u8"<table><thead><tr>"
            u8"<th style=\"text-align:center\">A</th><th style=\"text-align:center\">B</th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td style=\"text-align:center\">1</td><td style=\"text-align:center\">2</td>"
            u8"</tr></tbody></table>";
        CHECK(html == answer);
    }

    // --- Negative tests: delimiter patterns that violate the spec ---

    // Extra dash after trailing colon (|:-:-|)
    TEST_CASE("extra-dash-after-colon") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|:-:-|\n"
            u8"| B |");
        auto const& answer = u8"|&nbsp;A&nbsp;|<br>|:-:-|<br>|&nbsp;B&nbsp;|";
        CHECK(html == answer);
    }

    // Space between dashes and trailing colon within cell
    TEST_CASE("space-before-trailing-colon") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|:-- :|\n"
            u8"| B |");
        auto const& answer = u8"|&nbsp;A&nbsp;|<br>|:--&nbsp;:|<br>|&nbsp;B&nbsp;|";
        CHECK(html == answer);
    }

    // Invalid character inside delimiter cell
    TEST_CASE("invalid-char-in-delimiter") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|--x--|\n"
            u8"| B |");
        auto const& answer = u8"|&nbsp;A&nbsp;|<br>|--x--|<br>|&nbsp;B&nbsp;|";
        CHECK(html == answer);
    }

    // Double leading colon
    TEST_CASE("double-leading-colon") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|::---|\n"
            u8"| B |");
        auto const& answer = u8"|&nbsp;A&nbsp;|<br>|::---|<br>|&nbsp;B&nbsp;|";
        CHECK(html == answer);
    }

    // Double trailing colon
    TEST_CASE("double-trailing-colon") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|---::|\n"
            u8"| B |");
        auto const& answer = u8"|&nbsp;A&nbsp;|<br>|---::|<br>|&nbsp;B&nbsp;|";
        CHECK(html == answer);
    }

    // Colon in middle of dashes (not at edge)
    TEST_CASE("colon-in-middle") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|-:-|\n"
            u8"| B |");
        auto const& answer = u8"|&nbsp;A&nbsp;|<br>|-:-|<br>|&nbsp;B&nbsp;|";
        CHECK(html == answer);
    }

    // Extraneous text after valid delimiter cell
    TEST_CASE("extraneous-text-after-cell") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|---|extra|\n"
            u8"| B |");
        auto const& answer = u8"|&nbsp;A&nbsp;|<br>|---|extra|<br>|&nbsp;B&nbsp;|";
        CHECK(html == answer);
    }

    // Second column has invalid delimiter, first is valid
    TEST_CASE("second-column-invalid-delimiter") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|---|:-:-|\n"
            u8"| 1 | 2 |");
        auto const& answer = u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|<br>|---|:-:-|<br>|&nbsp;1&nbsp;|&nbsp;2&nbsp;|";
        CHECK(html == answer);
    }

    // --- Inline Markdown inside table cells ---

    // Bold in header cell
    TEST_CASE("bold-in-header") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **bold** | normal |\n"
            u8"|----------|--------|\n"
            u8"| 1        | 2      |");
        auto const& answer =
            u8"<table><thead><tr><th><strong>bold</strong></th><th>normal</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Italic in data cell
    TEST_CASE("italic-in-data-cell") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| H | I |\n"
            u8"|---|---|\n"
            u8"| *italic* | normal |");
        auto const& answer =
            u8"<table><thead><tr><th>H</th><th>I</th></tr></thead>"
            u8"<tbody><tr><td><em>italic</em></td><td>normal</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Code span in cell
    TEST_CASE("code-span-in-cell") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| `code` | text |\n"
            u8"|--------|------|\n"
            u8"| a      | b    |");
        auto const& answer =
            u8"<table><thead><tr><th><code>code</code></th><th>text</th></tr></thead>"
            u8"<tbody><tr><td>a</td><td>b</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Strikethrough in cell
    TEST_CASE("strikethrough-in-cell") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| H |\n"
            u8"|---|\n"
            u8"| ~~strike~~ |");
        auto const& answer =
            u8"<table><thead><tr><th>H</th></tr></thead>"
            u8"<tbody><tr><td><del>strike</del></td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Bold and italic in separate cells
    TEST_CASE("bold-and-italic-separate-cells") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **bold** | *italic* |\n"
            u8"|----------|----------|\n"
            u8"| a        | b        |");
        auto const& answer =
            u8"<table><thead><tr>"
            u8"<th><strong>bold</strong></th><th><em>italic</em></th>"
            u8"</tr></thead>"
            u8"<tbody><tr><td>a</td><td>b</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Inline markdown in same cell
    TEST_CASE("inline-markdown-same-cell") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **bold** *italic* | plain |\n"
            u8"|-------------------|-------|\n"
            u8"| 1                 | 2     |");
        auto const& answer =
            u8"<table><thead><tr>"
            u8"<th><strong>bold</strong>&nbsp;<em>italic</em></th><th>plain</th>"
            u8"</tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Inline markdown with center alignment
    TEST_CASE("inline-markdown-center") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **bold** |\n"
            u8"|:--------:|\n"
            u8"| *italic* |");
        auto const& answer =
            u8"<table><thead><tr>"
            u8"<th style=\"text-align:center\"><strong>bold</strong></th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td style=\"text-align:center\"><em>italic</em></td>"
            u8"</tr></tbody></table>";
        CHECK(html == answer);
    }

    // Inline markdown with right alignment
    TEST_CASE("inline-markdown-right") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| *italic* |\n"
            u8"|---------:|\n"
            u8"| **bold** |");
        auto const& answer =
            u8"<table><thead><tr>"
            u8"<th style=\"text-align:right\"><em>italic</em></th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td style=\"text-align:right\"><strong>bold</strong></td>"
            u8"</tr></tbody></table>";
        CHECK(html == answer);
    }

    TEST_CASE("italic-and-strikethrough-header") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| *~~italic~~* |\n"
            u8"|---------:|\n"
            u8"| **bold** |");
        auto const& answer =
            u8"<table><thead><tr>"
            u8"<th style=\"text-align:right\"><em><del>italic</del></em></th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td style=\"text-align:right\"><strong>bold</strong></td>"
            u8"</tr></tbody></table>";
        CHECK(html == answer);
    }

    // Escaped asterisks render as literal *, not italic
    TEST_CASE("escaped-asterisks-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| \\*not italic\\* |\n"
            u8"|-----------------|\n"
            u8"| x               |");
        auto const& answer =
            u8"<table><thead><tr><th>*not&nbsp;italic*</th></tr></thead>"
            u8"<tbody><tr><td>x</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Code span with HTML entities
    TEST_CASE("code-span-with-entities") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| `<html>` | `a & b` |\n"
            u8"|----------|--------|\n"
            u8"| 1        | 2      |");
        auto const& answer =
            u8"<table><thead><tr>"
            u8"<th><code>&lt;html&gt;</code></th><th><code>a&nbsp;&amp;&nbsp;b</code></th>"
            u8"</tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Bold inside empty cell should still work
    TEST_CASE("bold-inside-empty-cell") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | **B** |\n"
            u8"|---|---|\n"
            u8"|   | **2** |");
        auto const& answer =
            u8"<table><thead><tr><th>A</th><th><strong>B</strong></th></tr></thead>"
            u8"<tbody><tr><td></td><td><strong>2</strong></td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Uneven column counts: body row with fewer cells than header → not a table
    TEST_CASE("fewer-body-cells") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|---|---|---|\n"
            u8"| 1 | 2   |\n"
            u8"| 3 | 4 | 5 |");
        auto const& answer =
            u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|&nbsp;C&nbsp;|<br>"
            u8"|---|---|---|<br>"
            u8"|&nbsp;1&nbsp;|&nbsp;2&nbsp;&nbsp;&nbsp;|<br>"
            u8"|&nbsp;3&nbsp;|&nbsp;4&nbsp;|&nbsp;5&nbsp;|";
        CHECK(html == answer);
    }

    // Uneven column counts with inline markdown → not a table
    TEST_CASE("fewer-body-cells-with-markdown") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **A** | B | *C* |\n"
            u8"|-------|---|-----|\n"
            u8"| **x** | y |\n"
            u8"| 1     | 2 | *3* |");
        auto const& answer =
            u8"|&nbsp;<strong>A</strong>&nbsp;|&nbsp;B&nbsp;|&nbsp;<em>C</em>&nbsp;|<br>"
            u8"|-------|---|-----|<br>"
            u8"|&nbsp;<strong>x</strong>&nbsp;|&nbsp;y&nbsp;|<br>"
            u8"|&nbsp;1&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;2&nbsp;|&nbsp;<em>3</em>&nbsp;|";
        CHECK(html == answer);
    }

    // More cells in body row than header → not a table
    TEST_CASE("more-body-cells") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|---|---|---|\n"
            u8"| 1 | 2 | 3 | 4 |");
        auto const& answer =
            u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|&nbsp;C&nbsp;|<br>"
            u8"|---|---|---|<br>"
            u8"|&nbsp;1&nbsp;|&nbsp;2&nbsp;|&nbsp;3&nbsp;|&nbsp;4&nbsp;|";
        CHECK(html == answer);
    }

    // More cells in body row than header with inline markdown → not a table
    TEST_CASE("more-body-cells-with-markdown") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **A** | B | *C* |\n"
            u8"|---|---|---|\n"
            u8"| **x** | y | *z* | extra |");
        auto const& answer =
            u8"|&nbsp;<strong>A</strong>&nbsp;|&nbsp;B&nbsp;|&nbsp;<em>C</em>&nbsp;|<br>"
            u8"|---|---|---|<br>"
            u8"|&nbsp;<strong>x</strong>&nbsp;|&nbsp;y&nbsp;|&nbsp;<em>z</em>&nbsp;|&nbsp;extra&nbsp;|";
        CHECK(html == answer);
    }

    // Delimiter row with fewer cells than header row → not a table
    TEST_CASE("fewer-delimiter-cells") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|---|---|");
        auto const& answer =
            u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|&nbsp;C&nbsp;|<br>"
            u8"|---|---|";
        CHECK(html == answer);
    }

    // Delimiter row with more cells than header row → not a table
    TEST_CASE("more-delimiter-cells") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|---|---|---|");
        auto const& answer =
            u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|<br>"
            u8"|---|---|---|";
        CHECK(html == answer);
    }

    // Escaped pipe \| in cell content → literal |, not column separator
    TEST_CASE("escaped-pipe-in-cell") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| a \\| b | c |\n"
            u8"|---------|---|\n"
            u8"| 1       | 2 |");
        auto const& answer =
            u8"<table><thead><tr><th>a&nbsp;|&nbsp;b</th><th>c</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Double backslash \\| → literal \, pipe is separator (3-col table)
    TEST_CASE("double-backslash-pipe") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| a | b | c |\n"
            u8"|---|---|---|\n"
            u8"| \\\\| 2 | 3 |");
        auto const& answer =
            u8"<table><thead><tr><th>a</th><th>b</th><th>c</th></tr></thead>"
            u8"<tbody><tr><td>\\</td><td>2</td><td>3</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Escaped pipe with inline markdown
    TEST_CASE("escaped-pipe-with-markdown") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **bold** \\| text | plain |\n"
            u8"|--------------------|-------|\n"
            u8"| 1                  | 2     |");
        auto const& answer =
            u8"<table><thead><tr>"
            u8"<th><strong>bold</strong>&nbsp;|&nbsp;text</th><th>plain</th>"
            u8"</tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // --- Regression tests: `</` inside table cells (previously crashed) ---

    // Minimal table with empty header and `</` in body cell
    TEST_CASE("escaped-close-tag-in-cell") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"||\n"
            u8"|-|\n"
            u8"|</|");
        auto const& answer =
            u8"<table><thead><tr><th></th></tr></thead>"
            u8"<tbody><tr><td>&lt;/</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    // Table with indented delimiter and `</` in body cell
    // (rows without trailing | are NOT parsed as a table — see md_table.hh)
    TEST_CASE("no-trailing-pipe-not-table") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"|cell\n"
            u8" |--\n"
            u8" |</|");
        auto const& answer = u8"|cell<br>&nbsp;|--<br>&nbsp;|&lt;/|";
        CHECK(html == answer);
    }

    // Table with `</x` (tag-like content) in body cell
    TEST_CASE("tag-like-close-in-cell") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"|ok|\n"
            u8"|---|\n"
            u8"|</x|");
        auto const& answer =
            u8"<table><thead><tr><th>ok</th></tr></thead>"
            u8"<tbody><tr><td>&lt;/x</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-escapes-table-markup") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"| left | center | right |\n"
            u8"|:-----|:------:|------:|\n"
            u8"| a | b | c |");
        auto const& answer =
            u8"<size=20>\uff1c</size>table<size=20>\uff1e</size><size=20>\uff1c</size>thead<size=20>\uff1e</size>"
            u8"<size=20>\uff1c</size>tr<size=20>\uff1e</size><size=20>\uff1c</size>th<size=20>\uff1e</size>left"
            u8"<size=20>\uff1c</size>/th<size=20>\uff1e</size><size=20>\uff1c</size>th style=\"text-align:center\""
            u8"<size=20>\uff1e</size>center<size=20>\uff1c</size>/th<size=20>\uff1e</size><size=20>\uff1c</size>th "
            u8"style=\"text-align:right\"<size=20>\uff1e</size>right<size=20>\uff1c</size>/th<size=20>\uff1e</size>"
            u8"<size=20>\uff1c</size>/tr<size=20>\uff1e</size><size=20>\uff1c</size>/thead<size=20>\uff1e</size>"
            u8"<size=20>\uff1c</size>tbody<size=20>\uff1e</size><size=20>\uff1c</size>tr<size=20>\uff1e</size>"
            u8"<size=20>\uff1c</size>td<size=20>\uff1e</size>a<size=20>\uff1c</size>/td<size=20>\uff1e</size>"
            u8"<size=20>\uff1c</size>td style=\"text-align:center\"<size=20>\uff1e</size>b<size=20>\uff1c</size>/td"
            u8"<size=20>\uff1e</size><size=20>\uff1c</size>td style=\"text-align:right\"<size=20>\uff1e</size>c"
            u8"<size=20>\uff1c</size>/td<size=20>\uff1e</size><size=20>\uff1c</size>/tr<size=20>\uff1e</size>"
            u8"<size=20>\uff1c</size>/tbody<size=20>\uff1e</size><size=20>\uff1c</size>/table<size=20>\uff1e</size>";
        CHECK(html == answer);
    }
}
