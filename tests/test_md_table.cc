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

    return 0;
}
