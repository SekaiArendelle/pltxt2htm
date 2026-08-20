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
        auto answer = ::fast_io::u8string_view{u8"&lt;table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<table></table>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;table&gt;&lt;/table&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <col> outside <table>/<colgroup> is treated as literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<col>");
        auto answer = ::fast_io::u8string_view{u8"&lt;col&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<table><tr><td>cell</td></tr></table>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>cell</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>table<size=20>\uff1e</size><size=20>\uff1c</size>tr<size=20>\uff1e</size><size=20>"
            u8"\uff1c</size>td<size=20>\uff1e</size>cell<size=20>\uff1c</size>/td<size=20>\uff1e</"
            u8"size><size=20>\uff1c</"
            u8"size>/tr<size=20>\uff1e</size><size=20>\uff1c</size>/table<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // ── Rejection of table-internal tags outside their valid context ──

    {
        // <tr> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<tr>");
        auto answer = ::fast_io::u8string_view{u8"&lt;tr&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<td>");
        auto answer = ::fast_io::u8string_view{u8"&lt;td&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<th>");
        auto answer = ::fast_io::u8string_view{u8"&lt;th&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <thead> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<thead>");
        auto answer = ::fast_io::u8string_view{u8"&lt;thead&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tbody> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<tbody>");
        auto answer = ::fast_io::u8string_view{u8"&lt;tbody&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tfoot> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<tfoot>");
        auto answer = ::fast_io::u8string_view{u8"&lt;tfoot&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <caption> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<caption>");
        auto answer = ::fast_io::u8string_view{u8"&lt;caption&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <colgroup> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<colgroup>");
        auto answer = ::fast_io::u8string_view{u8"&lt;colgroup&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // uppercase <TR> at top level -> literal text
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
        // <tr> directly in <table> AFTER </tbody> must NOT be merged into the section
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tbody><tr><td>b1</td></tr></tbody><tr><td>b2</td></tr></table>");
        auto answer =
            ::fast_io::u8string_view{u8"<table><tbody><tr><td>b1</td></tr></tbody><tr><td>b2</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tr> directly in <table> AFTER </thead> must NOT be merged into the section
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><thead><tr><th>h1</th></tr></thead><tr><td>b2</td></tr></table>");
        auto answer =
            ::fast_io::u8string_view{u8"<table><thead><tr><th>h1</th></tr></thead><tr><td>b2</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <tr> directly in <table> AFTER </tfoot> must NOT be merged into the section
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tfoot><tr><td>f</td></tr></tfoot><tr><td>b2</td></tr></table>");
        auto answer =
            ::fast_io::u8string_view{u8"<table><tfoot><tr><td>f</td></tr></tfoot><tr><td>b2</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // multiple bare rows after a section: none merges, none is dropped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tbody><tr><td>b1</td></tr></tbody><tr><td>b2</td></tr><tr><td>b3</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><tbody><tr><td>b1</td></tr></tbody><tr><td>b2</td></tr>"
            u8"<tr><td>b3</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // section -> bare <tr> -> re-opened section: the section after the bare row is independent
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tbody><tr><td>a</td></tr></tbody><tr><td>b</td></tr>"
            u8"<tbody><tr><td>c</td></tr></tbody></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><tbody><tr><td>a</td></tr></tbody><tr><td>b</td></tr>"
            u8"<tbody><tr><td>c</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><caption>title</caption></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><caption>title</caption></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // empty <caption> is still an authored caption node (presence != content)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><caption></caption><tr><td>x</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><caption></caption><tr><td>x</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // empty <colgroup> without any <col> is not recorded (scanner only tracks <col>)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><colgroup></colgroup><tr><td>x</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>x</td></tr></table>"};
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
        // <tr> inside <caption> -> <tr> is rejected (wrong context)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><caption><tr>x</tr></caption></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><caption>&lt;tr&gt;x&lt;/tr&gt;</caption></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ── <td style="text-align:..."> ──

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
        // <td style="text-align:left"> -> accepted (valid), default align -> no style attr
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:left\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>cell</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td style=\" ; broken ; :ignored ; text-align \t : \t center \t ; ; "
            u8"\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td style=\"text-align:center\">cell</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td> with multiple attributes — unknown attributes (class, id) -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td class=\"foo\" style=\"text-align:center\" id=\"bar\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;class=&quot;foo&quot;&nbsp;style=&quot;text-align:center&quot;&nbsp;"
            u8"id="
            u8"&quot;bar&quot;&gt;cell&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="color:red;text-align:center"> -> unknown CSS -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td style=\"color:red;text-align:center\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;color:red;text-align:center&quot;&gt;cell&lt;/td&gt;&lt;/"
            u8"tr&gt;&lt;/table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="text-align:center;color:red"> -> unknown CSS -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td style=\"text-align:center;color:red\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;text-align:center;color:red&quot;&gt;cell&lt;/td&gt;&lt;"
            u8"/tr&gt;&lt;/table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ── uppercase text-align values rejected ──

    {
        // <td style="text-align:LEFT"> -> uppercase -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:LEFT\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;text-align:LEFT&quot;&gt;cell&lt;/td&gt;&lt;/tr&gt;&lt;/"
            u8"table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="text-align:Left"> -> mixed case -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:Left\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;text-align:Left&quot;&gt;cell&lt;/td&gt;&lt;/tr&gt;&lt;/"
            u8"table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th> without style -> no style attribute
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th>header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><th>header</th></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:center\">header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><th style=\"text-align:center\">header</th></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th style="text-align:right">
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:right\">header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><th style=\"text-align:right\">header</th></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th style="text-align: center"> -> whitespace after colon accepted and normalized
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align: center\">header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><th style=\"text-align:center\">header</th></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th style="text-align:left"> -> accepted, default align -> no style attr
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:left\">header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><th>header</th></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th> with multiple attributes — unknown ones (class, id) -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><th class=\"foo\" style=\"text-align:center\" id=\"bar\">header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;class=&quot;foo&quot;&nbsp;style=&quot;text-align:center&quot;&nbsp;"
            u8"id="
            u8"&quot;bar&quot;&gt;header&lt;/th&gt;&lt;/tr&gt;&lt;/table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th style="color:red;text-align:center"> -> unknown CSS -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><th style=\"color:red;text-align:center\">header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;color:red;text-align:center&quot;&gt;header&lt;/"
            u8"th&gt;&lt;/"
            u8"tr&gt;&lt;/table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th style="text-align:center;color:red"> -> unknown CSS -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><th style=\"text-align:center;color:red\">header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;text-align:center;color:red&quot;&gt;header&lt;/"
            u8"th&gt;&lt;"
            u8"/tr&gt;&lt;/table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ── <th> uppercase text-align values rejected ──

    {
        // <th style="text-align:LEFT"> -> uppercase -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:LEFT\">header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;text-align:LEFT&quot;&gt;header&lt;/th&gt;&lt;/"
            u8"tr&gt;&lt;/"
            u8"table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th style="text-align:Left"> -> mixed case -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:Left\">header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;text-align:Left&quot;&gt;header&lt;/th&gt;&lt;/"
            u8"tr&gt;&lt;/"
            u8"table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th style="text-align:CENTER"> -> uppercase -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:CENTER\">header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;text-align:CENTER&quot;&gt;header&lt;/th&gt;&lt;/"
            u8"tr&gt;&lt;"
            u8"/table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <th style="text-align:Right"> -> mixed case -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:Right\">header</th></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;text-align:Right&quot;&gt;header&lt;/th&gt;&lt;/"
            u8"tr&gt;&lt;"
            u8"/table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ── <th> and <td> mixed styles ──

    {
        // <th> and <td> with different alignments in same row
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><th style=\"text-align:center\">h</th><td style=\"text-align:right\">d</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><tr><th style=\"text-align:center\">h</th><td style=\"text-align:right\">d</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <td style="text-align:Right"> -> uppercase -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:Right\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;text-align:Right&quot;&gt;cell&lt;/td&gt;&lt;/tr&gt;&lt;/"
            u8"table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td Style=\"text-align:center\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;Style=&quot;text-align:center&quot;&gt;cell&lt;/td&gt;&lt;/"
            u8"tr&gt;&lt;/"
            u8"table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"Text-align:center\">cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;Text-align:center&quot;&gt;cell&lt;/td&gt;&lt;/"
            u8"tr&gt;&lt;/"
            u8"table&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<table><caption>caption</caption><colgroup><col></colgroup><thead><tr><th "
            u8"style=\"text-align:center\">head</th></tr></thead><tbody><tr><td "
            u8"style=\"text-align:right\">body</td></tr></tbody><tfoot><tr><td>foot</td></tr></tfoot></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>table<size=20>\uff1e</size><size=20>\uff1c</size>caption<size=20>\uff1e</size>"
            u8"caption<size=20>\uff1c</size>/caption<size=20>\uff1e</size><size=20>\uff1c</"
            u8"size>colgroup<size=20>\uff1e</"
            u8"size><size=20>\uff1c</size>col<size=20>\uff1e</size><size=20>\uff1c</size>/colgroup<size=20>\uff1e</"
            u8"size>"
            u8"<size=20>\uff1c</size>thead<size=20>\uff1e</size><size=20>\uff1c</size>tr<size=20>\uff1e</size><size=20>"
            u8"\uff1c</size>th style=\"text-align:center\"<size=20>\uff1e</size>head<size=20>\uff1c</size>/th<size=20>"
            u8"\uff1e</size><size=20>\uff1c</size>/tr<size=20>\uff1e</size><size=20>\uff1c</size>/"
            u8"thead<size=20>\uff1e</"
            u8"size><size=20>\uff1c</size>tbody<size=20>\uff1e</size><size=20>\uff1c</size>tr<size=20>\uff1e</size>"
            u8"<size=20>\uff1c</size>td style=\"text-align:right\"<size=20>\uff1e</size>body<size=20>\uff1c</size>/td"
            u8"<size=20>\uff1e</size><size=20>\uff1c</size>/tr<size=20>\uff1e</size><size=20>\uff1c</size>/tbody"
            u8"<size=20>\uff1e</size><size=20>\uff1c</size>tfoot<size=20>\uff1e</size><size=20>\uff1c</size>tr<size=20>"
            u8"\uff1e</size><size=20>\uff1c</size>td<size=20>\uff1e</size>foot<size=20>\uff1c</size>/"
            u8"td<size=20>\uff1e</"
            u8"size><size=20>\uff1c</size>/tr<size=20>\uff1e</size><size=20>\uff1c</size>/tfoot<size=20>\uff1e</size>"
            u8"<size=20>\uff1c</size>/table<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<table><tr><th style=\"text-align:right\">head</th><td "
            u8"style=\"text-align:center\">data</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>table<size=20>\uff1e</size><size=20>\uff1c</size>tr<size=20>\uff1e</size><size=20>"
            u8"\uff1c</size>th style=\"text-align:right\"<size=20>\uff1e</size>head<size=20>\uff1c</size>/th<size=20>"
            u8"\uff1e</size><size=20>\uff1c</size>td style=\"text-align:center\"<size=20>\uff1e</size>data<size=20>"
            u8"\uff1c</size>/td<size=20>\uff1e</size><size=20>\uff1c</size>/tr<size=20>\uff1e</size><size=20>\uff1c</"
            u8"size>/table<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ── caption/cell text is parsed with the inline-only parser ──
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><caption>cap\n# **title**</caption><tr><th>head\n<h1>x</h1></th><td>body\n- "
            u8"*item*</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><caption>cap<br>#&nbsp;<strong>title</strong></caption><tr><th>head<br>&lt;h1&gt;x&lt;/"
            u8"h1&gt;</th><td>body<br>-&nbsp;<em>item</em></td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // optionally_to_html_table_ast probes subview(pltext, current_index + 2) past the end of a
        // short view after matching the opening <table> tag, which calls fast_terminate.
        auto pltext = ::fast_io::u8string_view{u8"<table>X"};
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(pltext);
        ::fast_io::u8string answer{u8"&lt;table&gt;X"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
