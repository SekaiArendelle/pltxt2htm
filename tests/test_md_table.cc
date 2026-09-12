#include <boost/ut.hpp>

#include <string_view>

#include "precompile.hh"

namespace pltxt2htm_test {

void register_md_table_tests() {
    using namespace boost::ut;
    // Basic table with header and data rows
    "markdown/table/basic"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| Header 1 | Header 2 |\n"
            u8"|----------|----------|\n"
            u8"| Cell 1   | Cell 2   |\n"
            u8"| Cell 3   | Cell 4   |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>Header&nbsp;1</th><th>Header&nbsp;2</th></tr></thead>"
            u8"<tbody><tr><td>Cell&nbsp;1</td><td>Cell&nbsp;2</td></tr>"
            u8"<tr><td>Cell&nbsp;3</td><td>Cell&nbsp;4</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Table preceded by a line break
    "markdown/table/preceded-by-line-break"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"\n| A | B |\n"
            u8"|---|---|\n"
            u8"| 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<br><table><thead><tr><th>A</th><th>B</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Table with varying spaces
    "markdown/table/trims-cell-spacing"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"|  a  |  b  |\n"
            u8"|-----|-----|\n"
            u8"|  x  |  y  |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>a</th><th>b</th></tr></thead>"
            u8"<tbody><tr><td>x</td><td>y</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Table with empty cells
    "markdown/table/empty-cells"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|---|---|---|\n"
            u8"| 1 |   | 3 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>A</th><th>B</th><th>C</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td></td><td>3</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Table with only header (no data rows)
    "markdown/table/header-only"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| X | Y |\n"
            u8"|---|---|");
        auto answer = ::fast_io::u8string_view{u8"<table><thead><tr><th>X</th><th>Y</th></tr></thead></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Table with special characters in cells
    "markdown/table/escapes-special-characters"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| a & b | c > d |\n"
            u8"|-------|-------|\n"
            u8"| x & y | p < q |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>a&nbsp;&amp;&nbsp;b</th><th>c&nbsp;&gt;&nbsp;d</th></tr></thead>"
            u8"<tbody><tr><td>x&nbsp;&amp;&nbsp;y</td><td>p&nbsp;&lt;&nbsp;q</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Table with escape sequences
    "markdown/table/escape-sequences"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| \\\"quote\\\" |\n"
            u8"|-----------|\n"
            u8"| \\&amp;   |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>&quot;quote&quot;</th></tr></thead>"
            u8"<tbody><tr><td>&amp;amp;</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Table followed by text (text should be separated)
    "markdown/table/followed-by-text"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| H |\n"
            u8"|---|\n"
            u8"| C |\n"
            u8"after");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>H</th></tr></thead>"
            u8"<tbody><tr><td>C</td></tr></tbody></table>"
            u8"after"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    "markdown/table/surrounded-by-text"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"before\n"
            u8"| H |\n"
            u8"|---|\n"
            u8"| C |\n"
            u8"after");
        auto answer = ::fast_io::u8string_view{
            u8"before<br><table><thead><tr><th>H</th></tr></thead>"
            u8"<tbody><tr><td>C</td></tr></tbody></table>"
            u8"after"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Table after a <br> tag
    "markdown/table/after-br-tag"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<br>| A | B |\n"
            u8"    |---|---|\n"
            u8"    | 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<br><table><thead><tr><th>A</th><th>B</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Left-aligned columns (default, no style attribute)
    "markdown/table/left-alignment-is-default"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|:---|:---|\n"
            u8"| 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>A</th><th>B</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Center-aligned columns
    "markdown/table/center-alignment"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|:---:|:---:|\n"
            u8"| 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th style=\"text-align:center\">A</th><th "
            u8"style=\"text-align:center\">B</th></tr></thead>"
            u8"<tbody><tr><td style=\"text-align:center\">1</td><td "
            u8"style=\"text-align:center\">2</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Right-aligned columns
    "markdown/table/right-alignment"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|---:|---:|\n"
            u8"| 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th style=\"text-align:right\">A</th><th "
            u8"style=\"text-align:right\">B</th></tr></thead>"
            u8"<tbody><tr><td style=\"text-align:right\">1</td><td "
            u8"style=\"text-align:right\">2</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Mixed alignment in one table
    "markdown/table/mixed-alignment"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| Left | Center | Right |\n"
            u8"|:-----|:------:|------:|\n"
            u8"| a    |   b    |   c   |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr>"
            u8"<th>Left</th><th style=\"text-align:center\">Center</th><th style=\"text-align:right\">Right</th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td>a</td><td style=\"text-align:center\">b</td><td style=\"text-align:right\">c</td>"
            u8"</tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Header-only table with alignment
    "markdown/table/header-only-with-alignment"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| X | Y |\n"
            u8"|:---:|:---|");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th style=\"text-align:center\">X</th><th>Y</th></tr></thead></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Alignment with empty cells
    "markdown/table/alignment-with-empty-cells"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|:---:|:---:|---:|\n"
            u8"| 1 |   | 3 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr>"
            u8"<th style=\"text-align:center\">A</th>"
            u8"<th style=\"text-align:center\">B</th>"
            u8"<th style=\"text-align:right\">C</th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td style=\"text-align:center\">1</td>"
            u8"<td style=\"text-align:center\"></td>"
            u8"<td style=\"text-align:right\">3</td>"
            u8"</tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // --- Negative tests: inputs that should NOT be parsed as tables ---

    // Single pipe-delimited line without a delimiter row
    "markdown/table/rejects-single-row"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"| A | B |\n");
        auto answer = ::fast_io::u8string_view{u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|<br>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Text followed by a valid delimiter (first line is not a pipe table row)
    "markdown/table/rejects-delimiter-after-plain-text"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text\n|---|---|\n");
        auto answer = ::fast_io::u8string_view{u8"text<br>|---|---|<br>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Pipe character in the middle of regular text
    "markdown/table/plain-text-with-middle-pipe"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"this | that\n");
        auto answer = ::fast_io::u8string_view{u8"this&nbsp;|&nbsp;that<br>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Single pipe character on a line
    "markdown/table/plain-text-with-single-pipe"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"|\n");
        auto answer = ::fast_io::u8string_view{u8"|<br>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Header line without leading pipe (no table)
    "markdown/table/rejects-header-without-leading-pipe"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"A | B\n|---|---|\n");
        auto answer = ::fast_io::u8string_view{u8"A&nbsp;|&nbsp;B<br>|---|---|<br>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Delimiter row without a preceding header
    "markdown/table/rejects-delimiter-without-header"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"|---|---|---|\n");
        auto answer = ::fast_io::u8string_view{u8"|---|---|---|<br>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Delimiter row with no dashes (all spaces) - should NOT be a table
    "markdown/table/rejects-empty-delimiter"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"| A | B |\n|   |   |\n");
        auto answer = ::fast_io::u8string_view{
            u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|<br>"
            u8"|&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;|<br>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // --- Additional positive tests for delimiter patterns ---

    // Single dash in delimiter (minimum valid: -+)
    "markdown/table/accepts-single-dash-delimiter"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|-|\n"
            u8"| B |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>A</th></tr></thead>"
            u8"<tbody><tr><td>B</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Two dashes left-aligned
    "markdown/table/accepts-two-dash-left-alignment"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|:--|\n"
            u8"| B |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>A</th></tr></thead>"
            u8"<tbody><tr><td>B</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Three dashes right-aligned
    "markdown/table/accepts-three-dash-right-alignment"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|---:|\n"
            u8"| B |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th style=\"text-align:right\">A</th></tr></thead>"
            u8"<tbody><tr><td style=\"text-align:right\">B</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Trailing whitespace in delimiter row
    "markdown/table/delimiter-trailing-whitespace"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| H |\n"
            u8"|---|  \n"
            u8"| C |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>H</th></tr></thead>"
            u8"<tbody><tr><td>C</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Spaces around dashes within delimiter cells
    "markdown/table/delimiter-cell-spacing"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|---| --- |\n"
            u8"| 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>A</th><th>B</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Very long dashes with center alignment
    "markdown/table/long-center-aligned-delimiters"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|:-----------------:|:-----------------:|\n"
            u8"| 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr>"
            u8"<th style=\"text-align:center\">A</th><th style=\"text-align:center\">B</th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td style=\"text-align:center\">1</td><td style=\"text-align:center\">2</td>"
            u8"</tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // --- Negative tests: delimiter patterns that violate the spec ---

    // Extra dash after trailing colon (|:-:-|)
    "markdown/table/rejects-extra-dash-after-trailing-colon"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|:-:-|\n"
            u8"| B |");
        auto answer = ::fast_io::u8string_view{u8"|&nbsp;A&nbsp;|<br>|:-:-|<br>|&nbsp;B&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Space between dashes and trailing colon within cell
    "markdown/table/rejects-space-before-trailing-colon"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|:-- :|\n"
            u8"| B |");
        auto answer = ::fast_io::u8string_view{u8"|&nbsp;A&nbsp;|<br>|:--&nbsp;:|<br>|&nbsp;B&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Invalid character inside delimiter cell
    "markdown/table/rejects-invalid-delimiter-character"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|--x--|\n"
            u8"| B |");
        auto answer = ::fast_io::u8string_view{u8"|&nbsp;A&nbsp;|<br>|--x--|<br>|&nbsp;B&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Double leading colon
    "markdown/table/rejects-double-leading-colon"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|::---|\n"
            u8"| B |");
        auto answer = ::fast_io::u8string_view{u8"|&nbsp;A&nbsp;|<br>|::---|<br>|&nbsp;B&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Double trailing colon
    "markdown/table/rejects-double-trailing-colon"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|---::|\n"
            u8"| B |");
        auto answer = ::fast_io::u8string_view{u8"|&nbsp;A&nbsp;|<br>|---::|<br>|&nbsp;B&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Colon in middle of dashes (not at edge)
    "markdown/table/rejects-middle-colon"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|-:-|\n"
            u8"| B |");
        auto answer = ::fast_io::u8string_view{u8"|&nbsp;A&nbsp;|<br>|-:-|<br>|&nbsp;B&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Extraneous text after valid delimiter cell
    "markdown/table/rejects-text-after-delimiter-cell"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A |\n"
            u8"|---|extra|\n"
            u8"| B |");
        auto answer = ::fast_io::u8string_view{u8"|&nbsp;A&nbsp;|<br>|---|extra|<br>|&nbsp;B&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Second column has invalid delimiter, first is valid
    "markdown/table/rejects-invalid-second-delimiter"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|---|:-:-|\n"
            u8"| 1 | 2 |");
        auto answer =
            ::fast_io::u8string_view{u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|<br>|---|:-:-|<br>|&nbsp;1&nbsp;|&nbsp;2&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // --- Inline Markdown inside table cells ---

    // Bold in header cell
    "markdown/table/bold-in-header"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **bold** | normal |\n"
            u8"|----------|--------|\n"
            u8"| 1        | 2      |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th><strong>bold</strong></th><th>normal</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Italic in data cell
    "markdown/table/italic-in-body"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| H | I |\n"
            u8"|---|---|\n"
            u8"| *italic* | normal |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>H</th><th>I</th></tr></thead>"
            u8"<tbody><tr><td><em>italic</em></td><td>normal</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Code span in cell
    "markdown/table/code-span-in-cell"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| `code` | text |\n"
            u8"|--------|------|\n"
            u8"| a      | b    |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th><code>code</code></th><th>text</th></tr></thead>"
            u8"<tbody><tr><td>a</td><td>b</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Strikethrough in cell
    "markdown/table/strikethrough-in-cell"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| H |\n"
            u8"|---|\n"
            u8"| ~~strike~~ |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>H</th></tr></thead>"
            u8"<tbody><tr><td><del>strike</del></td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Bold and italic in separate cells
    "markdown/table/bold-and-italic-in-separate-cells"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **bold** | *italic* |\n"
            u8"|----------|----------|\n"
            u8"| a        | b        |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr>"
            u8"<th><strong>bold</strong></th><th><em>italic</em></th>"
            u8"</tr></thead>"
            u8"<tbody><tr><td>a</td><td>b</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Inline markdown in same cell
    "markdown/table/bold-and-italic-in-one-cell"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **bold** *italic* | plain |\n"
            u8"|-------------------|-------|\n"
            u8"| 1                 | 2     |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr>"
            u8"<th><strong>bold</strong>&nbsp;<em>italic</em></th><th>plain</th>"
            u8"</tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Inline markdown with center alignment
    "markdown/table/inline-markdown-with-center-alignment"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **bold** |\n"
            u8"|:--------:|\n"
            u8"| *italic* |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr>"
            u8"<th style=\"text-align:center\"><strong>bold</strong></th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td style=\"text-align:center\"><em>italic</em></td>"
            u8"</tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Inline markdown with right alignment
    "markdown/table/inline-markdown-with-right-alignment"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| *italic* |\n"
            u8"|---------:|\n"
            u8"| **bold** |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr>"
            u8"<th style=\"text-align:right\"><em>italic</em></th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td style=\"text-align:right\"><strong>bold</strong></td>"
            u8"</tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    "markdown/table/nested-inline-markdown"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| *~~italic~~* |\n"
            u8"|---------:|\n"
            u8"| **bold** |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr>"
            u8"<th style=\"text-align:right\"><em><del>italic</del></em></th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td style=\"text-align:right\"><strong>bold</strong></td>"
            u8"</tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Escaped asterisks render as literal *, not italic
    "markdown/table/escaped-asterisks"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| \\*not italic\\* |\n"
            u8"|-----------------|\n"
            u8"| x               |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>*not&nbsp;italic*</th></tr></thead>"
            u8"<tbody><tr><td>x</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Code span with HTML entities
    "markdown/table/code-span-with-html-entities"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| `<html>` | `a & b` |\n"
            u8"|----------|--------|\n"
            u8"| 1        | 2      |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr>"
            u8"<th><code>&lt;html&gt;</code></th><th><code>a&nbsp;&amp;&nbsp;b</code></th>"
            u8"</tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Bold inside empty cell should still work
    "markdown/table/bold-with-empty-cell"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | **B** |\n"
            u8"|---|---|\n"
            u8"|   | **2** |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>A</th><th><strong>B</strong></th></tr></thead>"
            u8"<tbody><tr><td></td><td><strong>2</strong></td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Uneven column counts: body row with fewer cells than header → not a table
    "markdown/table/rejects-fewer-body-cells"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|---|---|---|\n"
            u8"| 1 | 2   |\n"
            u8"| 3 | 4 | 5 |");
        auto answer = ::fast_io::u8string_view{
            u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|&nbsp;C&nbsp;|<br>"
            u8"|---|---|---|<br>"
            u8"|&nbsp;1&nbsp;|&nbsp;2&nbsp;&nbsp;&nbsp;|<br>"
            u8"|&nbsp;3&nbsp;|&nbsp;4&nbsp;|&nbsp;5&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Uneven column counts with inline markdown → not a table
    "markdown/table/rejects-fewer-body-cells-with-inline-markdown"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **A** | B | *C* |\n"
            u8"|-------|---|-----|\n"
            u8"| **x** | y |\n"
            u8"| 1     | 2 | *3* |");
        auto answer = ::fast_io::u8string_view{
            u8"|&nbsp;<strong>A</strong>&nbsp;|&nbsp;B&nbsp;|&nbsp;<em>C</em>&nbsp;|<br>"
            u8"|-------|---|-----|<br>"
            u8"|&nbsp;<strong>x</strong>&nbsp;|&nbsp;y&nbsp;|<br>"
            u8"|&nbsp;1&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;2&nbsp;|&nbsp;<em>3</em>&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // More cells in body row than header → not a table
    "markdown/table/rejects-extra-body-cells"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|---|---|---|\n"
            u8"| 1 | 2 | 3 | 4 |");
        auto answer = ::fast_io::u8string_view{
            u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|&nbsp;C&nbsp;|<br>"
            u8"|---|---|---|<br>"
            u8"|&nbsp;1&nbsp;|&nbsp;2&nbsp;|&nbsp;3&nbsp;|&nbsp;4&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // More cells in body row than header with inline markdown → not a table
    "markdown/table/rejects-extra-body-cells-with-inline-markdown"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **A** | B | *C* |\n"
            u8"|---|---|---|\n"
            u8"| **x** | y | *z* | extra |");
        auto answer = ::fast_io::u8string_view{
            u8"|&nbsp;<strong>A</strong>&nbsp;|&nbsp;B&nbsp;|&nbsp;<em>C</em>&nbsp;|<br>"
            u8"|---|---|---|<br>"
            u8"|&nbsp;<strong>x</strong>&nbsp;|&nbsp;y&nbsp;|&nbsp;<em>z</em>&nbsp;|&nbsp;extra&nbsp;|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Delimiter row with fewer cells than header row → not a table
    "markdown/table/rejects-fewer-delimiter-cells"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|---|---|");
        auto answer = ::fast_io::u8string_view{
            u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|&nbsp;C&nbsp;|<br>"
            u8"|---|---|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Delimiter row with more cells than header row → not a table
    "markdown/table/rejects-extra-delimiter-cells"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|---|---|---|");
        auto answer = ::fast_io::u8string_view{
            u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|<br>"
            u8"|---|---|---|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Escaped pipe \| in cell content → literal |, not column separator
    "markdown/table/escaped-pipe-in-cell"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| a \\| b | c |\n"
            u8"|---------|---|\n"
            u8"| 1       | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>a&nbsp;|&nbsp;b</th><th>c</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Double backslash \\| → literal \, pipe is separator (3-col table)
    "markdown/table/double-backslash-before-separator"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| a | b | c |\n"
            u8"|---|---|---|\n"
            u8"| \\\\| 2 | 3 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>a</th><th>b</th><th>c</th></tr></thead>"
            u8"<tbody><tr><td>\\</td><td>2</td><td>3</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Escaped pipe with inline markdown
    "markdown/table/escaped-pipe-with-inline-markdown"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| **bold** \\| text | plain |\n"
            u8"|--------------------|-------|\n"
            u8"| 1                  | 2     |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr>"
            u8"<th><strong>bold</strong>&nbsp;|&nbsp;text</th><th>plain</th>"
            u8"</tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // --- Regression tests: `</` inside table cells (previously crashed) ---

    // Minimal table with empty header and `</` in body cell
    "markdown/table/regression-closing-tag-prefix-in-cell"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"||\n"
            u8"|-|\n"
            u8"|</|");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th></th></tr></thead>"
            u8"<tbody><tr><td>&lt;/</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Table with indented delimiter and `</` in body cell
    // (rows without trailing | are NOT parsed as a table — see md_table.hh)
    "markdown/table/regression-indented-incomplete-table"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"|cell\n"
            u8" |--\n"
            u8" |</|");
        auto answer = ::fast_io::u8string_view{u8"|cell<br>&nbsp;|--<br>&nbsp;|&lt;/|"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    // Table with `</x` (tag-like content) in body cell
    "markdown/table/regression-tag-like-cell-content"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"|ok|\n"
            u8"|---|\n"
            u8"|</x|");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>ok</th></tr></thead>"
            u8"<tbody><tr><td>&lt;/x</td></tr></tbody></table>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    "markdown/table/plunity-backend"_test = [] {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"| left | center | right |\n"
            u8"|:-----|:------:|------:|\n"
            u8"| a | b | c |");
        auto answer = ::fast_io::u8string_view{
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
            u8"<size=20>\uff1c</size>/tbody<size=20>\uff1e</size><size=20>\uff1c</size>/table<size=20>\uff1e</size>"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };
}

} // namespace pltxt2htm_test
