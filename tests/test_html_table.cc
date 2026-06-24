#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td>cell1</td><td>cell2</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>cell1</td><td>cell2</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><thead><tr><th>h1</th><th>h2</th></tr></thead></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><thead><tr><th>h1</th><th>h2</th></tr></thead></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td>a</td><td>b</td></tr><tr><td>c</td><td>d</td></tr></table>");
        auto answer =
            ::fast_io::u8string_view{u8"<table><tr><td>a</td><td>b</td></tr><tr><td>c</td><td>d</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<TABLE><TR><TD>CELL</TD></TR></TABLE>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>CELL</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><caption>caption</caption><tr><th>header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><caption>caption</caption><tr><th>header</th></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><colgroup><col></colgroup><tr><td>text</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><colgroup><col></colgroup><tr><td>text</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td><color=red>red</color></td></tr></table>");
        auto answer =
            ::fast_io::u8string_view{u8"<table><tr><td><span style=\"color:red;\">red</span></td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tbody><tr><td>body</td></tr></tbody></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tbody><tr><td>body</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tfoot><tr><td>foot</td></tr></tfoot></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tfoot><tr><td>foot</td></tr></tfoot></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table>");
        auto answer = ::fast_io::u8string_view{u8"<table></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<table></table>t");
        auto answer = ::fast_io::u8string_view{u8"t<table></table>t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <col> outside <table>/<colgroup> is treated as literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<col>");
        auto answer = ::fast_io::u8string_view{u8"&lt;col&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<table><tr><td>cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>cell</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ── Rejection of table-internal tags outside their valid context ──

    {
        // <tr> at top level → literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<tr>");
        auto answer = ::fast_io::u8string_view{u8"&lt;tr&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td> at top level → literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<td>");
        auto answer = ::fast_io::u8string_view{u8"&lt;td&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th> at top level → literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<th>");
        auto answer = ::fast_io::u8string_view{u8"&lt;th&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <thead> at top level → literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<thead>");
        auto answer = ::fast_io::u8string_view{u8"&lt;thead&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tbody> at top level → literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<tbody>");
        auto answer = ::fast_io::u8string_view{u8"&lt;tbody&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tfoot> at top level → literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<tfoot>");
        auto answer = ::fast_io::u8string_view{u8"&lt;tfoot&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <caption> at top level → literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<caption>");
        auto answer = ::fast_io::u8string_view{u8"&lt;caption&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <colgroup> at top level → literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<colgroup>");
        auto answer = ::fast_io::u8string_view{u8"&lt;colgroup&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // uppercase <TR> at top level → literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<TR>");
        auto answer = ::fast_io::u8string_view{u8"&lt;TR&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <thead> inside <table> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><thead><tr><th>x</th></tr></thead></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><thead><tr><th>x</th></tr></thead></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tbody> inside <table> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tbody><tr><td>x</td></tr></tbody></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tbody><tr><td>x</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tfoot> inside <table> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tfoot><tr><td>x</td></tr></tfoot></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tfoot><tr><td>x</td></tr></tfoot></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <caption> inside <table> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><caption>title</caption></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><caption>title</caption></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <colgroup> inside <table> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><colgroup><col></colgroup></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><colgroup><col></colgroup></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <col> inside <colgroup> inside <table> is valid (multiple cols)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><colgroup><col><col></colgroup></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><colgroup><col><col></colgroup></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tr> directly in <table> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td>x</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>x</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tr> in <thead> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><thead><tr><th>x</th></tr></thead></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><thead><tr><th>x</th></tr></thead></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tr> in <tbody> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tbody><tr><td>x</td></tr></tbody></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tbody><tr><td>x</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td> and <th> in <tr> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th>h</th><td>b</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><th>h</th><td>b</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tr> inside <caption> → <tr> is rejected (wrong context)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><caption><tr>x</tr></caption></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><caption>&lt;tr&gt;x&lt;/tr&gt;</caption></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ── <td style="text-align:..."> ──

    {
        // <td> without style → no style attribute
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td>cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>cell</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="text-align:center">
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:center\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td style=\"text-align:center\">cell</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="text-align:right">
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:right\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td style=\"text-align:right\">cell</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="text-align:left"> → accepted (valid), default align → no style attr
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:left\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>cell</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td> with multiple attributes — unknown attributes (class, id) → tag rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td class=\"foo\" style=\"text-align:center\" id=\"bar\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><tr>&lt;td&nbsp;class=&quot;foo&quot;&nbsp;style=&quot;text-align:center&quot;&nbsp;id=&quot;bar&"
            u8"quot;&gt;cell&lt;/td&gt;</tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="color:red;text-align:center"> → unknown CSS → tag rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td style=\"color:red;text-align:center\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><tr>&lt;td&nbsp;style=&quot;color:red;text-align:center&quot;&gt;cell&lt;/td&gt;</tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="text-align:center;color:red"> → unknown CSS → tag rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td style=\"text-align:center;color:red\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><tr>&lt;td&nbsp;style=&quot;text-align:center;color:red&quot;&gt;cell&lt;/td&gt;</tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ── uppercase text-align values rejected ──

    {
        // <td style="text-align:LEFT"> → uppercase → rejected
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:LEFT\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><tr>&lt;td&nbsp;style=&quot;text-align:LEFT&quot;&gt;cell&lt;/td&gt;</tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="text-align:Left"> → mixed case → rejected
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:Left\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><tr>&lt;td&nbsp;style=&quot;text-align:Left&quot;&gt;cell&lt;/td&gt;</tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="text-align:CENTER"> → uppercase → rejected
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:CENTER\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><tr>&lt;td&nbsp;style=&quot;text-align:CENTER&quot;&gt;cell&lt;/td&gt;</tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="text-align:Right"> → mixed case → rejected
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:Right\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><tr>&lt;td&nbsp;style=&quot;text-align:Right&quot;&gt;cell&lt;/td&gt;</tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
