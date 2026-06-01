#include "precompile.hh"

int main() {
    // Basic table with header and data rows
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| Header 1 | Header 2 |\n"
            u8"|----------|----------|\n"
            u8"| Cell 1   | Cell 2   |\n"
            u8"| Cell 3   | Cell 4   |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>Header&nbsp;1</th><th>Header&nbsp;2</th></tr></thead>"
            u8"<tbody><tr><td>Cell&nbsp;1</td><td>Cell&nbsp;2</td></tr>"
            u8"<tr><td>Cell&nbsp;3</td><td>Cell&nbsp;4</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Table preceded by a line break
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"\n| A | B |\n"
            u8"|---|---|\n"
            u8"| 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<br><table><thead><tr><th>A</th><th>B</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Table with varying spaces
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"|  a  |  b  |\n"
            u8"|-----|-----|\n"
            u8"|  x  |  y  |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>a</th><th>b</th></tr></thead>"
            u8"<tbody><tr><td>x</td><td>y</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Table with empty cells
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|---|---|---|\n"
            u8"| 1 |   | 3 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>A</th><th>B</th><th>C</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td></td><td>3</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Table with only header (no data rows)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| X | Y |\n"
            u8"|---|---|");
        auto answer = ::fast_io::u8string_view{u8"<table><thead><tr><th>X</th><th>Y</th></tr></thead></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Table with special characters in cells
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| a & b | c > d |\n"
            u8"|-------|-------|\n"
            u8"| x & y | p < q |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>a&nbsp;&amp;&nbsp;b</th><th>c&nbsp;&gt;&nbsp;d</th></tr></thead>"
            u8"<tbody><tr><td>x&nbsp;&amp;&nbsp;y</td><td>p&nbsp;&lt;&nbsp;q</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Table with escape sequences
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| \\\"quote\\\" |\n"
            u8"|-----------|\n"
            u8"| \\&amp;   |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>&quot;quote&quot;</th></tr></thead>"
            u8"<tbody><tr><td>&amp;amp;</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Table followed by text (text should be separated)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| H |\n"
            u8"|---|\n"
            u8"| C |\n"
            u8"after");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>H</th></tr></thead>"
            u8"<tbody><tr><td>C</td></tr></tbody></table>"
            u8"after"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Table after a <br> tag
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<br>| A | B |\n"
            u8"    |---|---|\n"
            u8"    | 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<br><table><thead><tr><th>A</th><th>B</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Left-aligned columns (default, no style attribute)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|:---|:---|\n"
            u8"| 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>A</th><th>B</th></tr></thead>"
            u8"<tbody><tr><td>1</td><td>2</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Center-aligned columns
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|:---:|:---:|\n"
            u8"| 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th style=\"text-align:center\">A</th><th "
            u8"style=\"text-align:center\">B</th></tr></thead>"
            u8"<tbody><tr><td style=\"text-align:center\">1</td><td "
            u8"style=\"text-align:center\">2</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Right-aligned columns
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B |\n"
            u8"|---:|---:|\n"
            u8"| 1 | 2 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th style=\"text-align:right\">A</th><th "
            u8"style=\"text-align:right\">B</th></tr></thead>"
            u8"<tbody><tr><td style=\"text-align:right\">1</td><td "
            u8"style=\"text-align:right\">2</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Mixed alignment in one table
    {
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
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Header-only table with alignment
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| X | Y |\n"
            u8"|:---:|:---|");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th style=\"text-align:center\">X</th><th>Y</th></tr></thead></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Alignment with empty cells
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"| A | B | C |\n"
            u8"|:---:|:---:|---:|\n"
            u8"| 1 |   | 3 |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr>"
            u8"<th style=\"text-align:center\">A</th><th style=\"text-align:center\">B</th><th "
            u8"style=\"text-align:right\">C</th>"
            u8"</tr></thead>"
            u8"<tbody><tr>"
            u8"<td style=\"text-align:center\">1</td><td style=\"text-align:center\"></td><td "
            u8"style=\"text-align:right\">3</td>"
            u8"</tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // --- Negative tests: inputs that should NOT be parsed as tables ---

    // Single pipe-delimited line without a delimiter row
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"| A | B |\n");
        auto answer = ::fast_io::u8string_view{u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|<br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Text followed by a valid delimiter (first line is not a pipe table row)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text\n|---|---|\n");
        auto answer = ::fast_io::u8string_view{u8"text<br>|---|---|<br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Pipe character in the middle of regular text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"this | that\n");
        auto answer = ::fast_io::u8string_view{u8"this&nbsp;|&nbsp;that<br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Single pipe character on a line
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"|\n");
        auto answer = ::fast_io::u8string_view{u8"|<br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Header line without leading pipe (no table)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"A | B\n|---|---|\n");
        auto answer = ::fast_io::u8string_view{u8"A&nbsp;|&nbsp;B<br>|---|---|<br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Delimiter row without a preceding header
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"|---|---|---|\n");
        auto answer = ::fast_io::u8string_view{u8"|---|---|---|<br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Delimiter row with no dashes (all spaces) - should NOT be a table
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"| A | B |\n|   |   |\n");
        auto answer =
            ::fast_io::u8string_view{u8"|&nbsp;A&nbsp;|&nbsp;B&nbsp;|<br>|&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;|<br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
