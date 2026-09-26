#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_table") {
    TEST_CASE("<table><tr><td>cell1</td><td>cell2</td></tr></table>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td>cell1</td><td>cell2</td></tr></table>");
        auto const& answer = u8"<table><tr><td>cell1</td><td>cell2</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<table><thead><tr><th>h1</th><th>h2</th></tr></thead></t...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><thead><tr><th>h1</th><th>h2</th></tr></thead></table>");
        auto const& answer = u8"<table><thead><tr><th>h1</th><th>h2</th></tr></thead></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<table><tr><td>a</td><td>b</td></tr><tr><td>c</td><td>d<...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td>a</td><td>b</td></tr><tr><td>c</td><td>d</td></tr></table>");
        auto const& answer = u8"<table><tr><td>a</td><td>b</td></tr><tr><td>c</td><td>d</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<TABLE><TR><TD>CELL</TD></TR></TABLE>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<TABLE><TR><TD>CELL</TD></TR></TABLE>");
        auto const& answer = u8"<table><tr><td>CELL</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<table><caption>caption</caption><tr><th>header</th></tr...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><caption>caption</caption><tr><th>header</th></tr></table>");
        auto const& answer = u8"<table><caption>caption</caption><tr><th>header</th></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<table><colgroup><col></colgroup><tr><td>text</td></tr><...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><colgroup><col></colgroup><tr><td>text</td></tr></table>");
        auto const& answer = u8"<table><colgroup><col></colgroup><tr><td>text</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<table><tr><td><color=red>red</color></td></tr></table>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td><color=red>red</color></td></tr></table>");
        auto const& answer = u8"<table><tr><td><span style=\"color:red;\">red</span></td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<table><tbody><tr><td>body</td></tr></tbody></table>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tbody><tr><td>body</td></tr></tbody></table>");
        auto const& answer = u8"<table><tbody><tr><td>body</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<table><tfoot><tr><td>foot</td></tr></tfoot></table>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tfoot><tr><td>foot</td></tr></tfoot></table>");
        auto const& answer = u8"<table><tfoot><tr><td>foot</td></tr></tfoot></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<table>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table>");
        auto const& answer = u8"&lt;table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("t<table></table>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<table></table>t");
        auto const& answer = u8"t&lt;table&gt;&lt;/table&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("<col> outside <table>/<colgroup> is treated as literal t...") {
        // <col> outside <table>/<colgroup> is treated as literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<col>");
        auto const& answer = u8"&lt;col&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<table><tr><td>cell</td></tr></table>") {
        auto const& pltext = u8"<table><tr><td>cell</td></tr></table>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<table><tr><td>cell</td></tr></table>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=20>\uff1c</size>table<size=20>\uff1e</size><size=20>\uff1c</size>tr<size=20>\uff1e</size><size=20>"
            u8"\uff1c</size>td<size=20>\uff1e</size>cell<size=20>\uff1c</size>/td<size=20>\uff1e</"
            u8"size><size=20>\uff1c</"
            u8"size>/tr<size=20>\uff1e</size><size=20>\uff1c</size>/table<size=20>\uff1e</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // ── Rejection of table-internal tags outside their valid context ──

    TEST_CASE("── Rejection of table-internal tags outside their valid...") {
        // <tr> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<tr>");
        auto const& answer = u8"&lt;tr&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<td> at top level -> literal text") {
        // <td> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<td>");
        auto const& answer = u8"&lt;td&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<th> at top level -> literal text") {
        // <th> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<th>");
        auto const& answer = u8"&lt;th&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<thead> at top level -> literal text") {
        // <thead> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<thead>");
        auto const& answer = u8"&lt;thead&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<tbody> at top level -> literal text") {
        // <tbody> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<tbody>");
        auto const& answer = u8"&lt;tbody&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<tfoot> at top level -> literal text") {
        // <tfoot> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<tfoot>");
        auto const& answer = u8"&lt;tfoot&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<caption> at top level -> literal text") {
        // <caption> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<caption>");
        auto const& answer = u8"&lt;caption&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<colgroup> at top level -> literal text") {
        // <colgroup> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<colgroup>");
        auto const& answer = u8"&lt;colgroup&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase <TR> at top level -> literal text") {
        // uppercase <TR> at top level -> literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<TR>");
        auto const& answer = u8"&lt;TR&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<thead> inside <table> is valid") {
        // <thead> inside <table> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><thead><tr><th>x</th></tr></thead></table>");
        auto const& answer = u8"<table><thead><tr><th>x</th></tr></thead></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<tbody> inside <table> is valid") {
        // <tbody> inside <table> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tbody><tr><td>x</td></tr></tbody></table>");
        auto const& answer = u8"<table><tbody><tr><td>x</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<tfoot> inside <table> is valid") {
        // <tfoot> inside <table> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tfoot><tr><td>x</td></tr></tfoot></table>");
        auto const& answer = u8"<table><tfoot><tr><td>x</td></tr></tfoot></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<tr> directly in <table> AFTER </tbody> must NOT be merg...") {
        // <tr> directly in <table> AFTER </tbody> must NOT be merged into the section
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tbody><tr><td>b1</td></tr></tbody><tr><td>b2</td></tr></table>");
        auto const& answer = u8"<table><tbody><tr><td>b1</td></tr></tbody><tr><td>b2</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<tr> directly in <table> AFTER </thead> must NOT be merg...") {
        // <tr> directly in <table> AFTER </thead> must NOT be merged into the section
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><thead><tr><th>h1</th></tr></thead><tr><td>b2</td></tr></table>");
        auto const& answer = u8"<table><thead><tr><th>h1</th></tr></thead><tr><td>b2</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<tr> directly in <table> AFTER </tfoot> must NOT be merg...") {
        // <tr> directly in <table> AFTER </tfoot> must NOT be merged into the section
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tfoot><tr><td>f</td></tr></tfoot><tr><td>b2</td></tr></table>");
        auto const& answer = u8"<table><tfoot><tr><td>f</td></tr></tfoot><tr><td>b2</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("multiple bare rows after a section: none merges; none is...") {
        // multiple bare rows after a section: none merges, none is dropped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tbody><tr><td>b1</td></tr></tbody><tr><td>b2</td></tr><tr><td>b3</td></tr></table>");
        auto const& answer =
            u8"<table><tbody><tr><td>b1</td></tr></tbody><tr><td>b2</td></tr>"
            u8"<tr><td>b3</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("section -> bare <tr> -> re-opened section: the section a...") {
        // section -> bare <tr> -> re-opened section: the section after the bare row is independent
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tbody><tr><td>a</td></tr></tbody><tr><td>b</td></tr>"
            u8"<tbody><tr><td>c</td></tr></tbody></table>");
        auto const& answer =
            u8"<table><tbody><tr><td>a</td></tr></tbody><tr><td>b</td></tr>"
            u8"<tbody><tr><td>c</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<table><caption>title</caption></table>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><caption>title</caption></table>");
        auto const& answer = u8"<table><caption>title</caption></table>";
        CHECK(html == answer);
    }

    TEST_CASE("empty <caption> is still an authored caption node (prese...") {
        // empty <caption> is still an authored caption node (presence != content)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><caption></caption><tr><td>x</td></tr></table>");
        auto const& answer = u8"<table><caption></caption><tr><td>x</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("empty <colgroup> without any <col> is not recorded (scan...") {
        // empty <colgroup> without any <col> is not recorded (scanner only tracks <col>)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><colgroup></colgroup><tr><td>x</td></tr></table>");
        auto const& answer = u8"<table><tr><td>x</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<colgroup> inside <table> is valid") {
        // <colgroup> inside <table> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><colgroup><col></colgroup></table>");
        auto const& answer = u8"<table><colgroup><col></colgroup></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<col> inside <colgroup> inside <table> is valid (multipl...") {
        // <col> inside <colgroup> inside <table> is valid (multiple cols)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><colgroup><col><col></colgroup></table>");
        auto const& answer = u8"<table><colgroup><col><col></colgroup></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<tr> directly in <table> is valid") {
        // <tr> directly in <table> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td>x</td></tr></table>");
        auto const& answer = u8"<table><tr><td>x</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<tr> in <thead> is valid") {
        // <tr> in <thead> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><thead><tr><th>x</th></tr></thead></table>");
        auto const& answer = u8"<table><thead><tr><th>x</th></tr></thead></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<tr> in <tbody> is valid") {
        // <tr> in <tbody> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tbody><tr><td>x</td></tr></tbody></table>");
        auto const& answer = u8"<table><tbody><tr><td>x</td></tr></tbody></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<td> and <th> in <tr> is valid") {
        // <td> and <th> in <tr> is valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th>h</th><td>b</td></tr></table>");
        auto const& answer = u8"<table><tr><th>h</th><td>b</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<tr> inside <caption> -> <tr> is rejected (wrong context...") {
        // <tr> inside <caption> -> <tr> is rejected (wrong context)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><caption><tr>x</tr></caption></table>");
        auto const& answer = u8"<table><caption>&lt;tr&gt;x&lt;/tr&gt;</caption></table>";
        CHECK(html == answer);
    }

    // ── <td style="text-align:..."> ──

    TEST_CASE("── <td style='text-align:...'> ──") {
        // <td style="text-align:center">
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:center\">cell</td></tr></table>");
        auto const& answer = u8"<table><tr><td style=\"text-align:center\">cell</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<td style='text-align:right'>") {
        // <td style="text-align:right">
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:right\">cell</td></tr></table>");
        auto const& answer = u8"<table><tr><td style=\"text-align:right\">cell</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<td style='text-align:left'> -> accepted (valid); defaul...") {
        // <td style="text-align:left"> -> accepted (valid), default align -> no style attr
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:left\">cell</td></tr></table>");
        auto const& answer = u8"<table><tr><td>cell</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<table><tr><td style=/' ; broken ; :ignored ; text-align...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td style=\" ; broken ; :ignored ; text-align \t : \t center \t ; ; "
            u8"\">cell</td></tr></table>");
        auto const& answer = u8"<table><tr><td style=\"text-align:center\">cell</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<td> with multiple attributes — unknown attributes (clas...") {
        // <td> with multiple attributes — unknown attributes (class, id) -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td class=\"foo\" style=\"text-align:center\" id=\"bar\">cell</td></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;class=&quot;foo&quot;&nbsp;style=&quot;text-align:center&quot;&nbsp;"
            u8"id="
            u8"&quot;bar&quot;&gt;cell&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<td style='color:red;text-align:center'> -> unknown CSS...") {
        // <td style="color:red;text-align:center"> -> unknown CSS -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td style=\"color:red;text-align:center\">cell</td></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;color:red;text-align:center&quot;&gt;cell&lt;/td&gt;&lt;/"
            u8"tr&gt;&lt;/table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<td style='text-align:center;color:red'> -> unknown CSS...") {
        // <td style="text-align:center;color:red"> -> unknown CSS -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><td style=\"text-align:center;color:red\">cell</td></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;text-align:center;color:red&quot;&gt;cell&lt;/td&gt;&lt;"
            u8"/tr&gt;&lt;/table&gt;";
        CHECK(html == answer);
    }

    // ── uppercase text-align values rejected ──

    TEST_CASE("── uppercase text-align values rejected ──") {
        // <td style="text-align:LEFT"> -> uppercase -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:LEFT\">cell</td></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;text-align:LEFT&quot;&gt;cell&lt;/td&gt;&lt;/tr&gt;&lt;/"
            u8"table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<td style='text-align:Left'> -> mixed case -> whole tabl...") {
        // <td style="text-align:Left"> -> mixed case -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:Left\">cell</td></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;text-align:Left&quot;&gt;cell&lt;/td&gt;&lt;/tr&gt;&lt;/"
            u8"table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<th> without style -> no style attribute") {
        // <th> without style -> no style attribute
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th>header</th></tr></table>");
        auto const& answer = u8"<table><tr><th>header</th></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<table><tr><th style=/'text-align:center/'>header</th></...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:center\">header</th></tr></table>");
        auto const& answer = u8"<table><tr><th style=\"text-align:center\">header</th></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<th style='text-align:right'>") {
        // <th style="text-align:right">
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:right\">header</th></tr></table>");
        auto const& answer = u8"<table><tr><th style=\"text-align:right\">header</th></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<th style='text-align: center'> -> whitespace after colo...") {
        // <th style="text-align: center"> -> whitespace after colon accepted and normalized
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align: center\">header</th></tr></table>");
        auto const& answer = u8"<table><tr><th style=\"text-align:center\">header</th></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<th style='text-align:left'> -> accepted; default align...") {
        // <th style="text-align:left"> -> accepted, default align -> no style attr
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:left\">header</th></tr></table>");
        auto const& answer = u8"<table><tr><th>header</th></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<th> with multiple attributes — unknown ones (class; id)...") {
        // <th> with multiple attributes — unknown ones (class, id) -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><th class=\"foo\" style=\"text-align:center\" id=\"bar\">header</th></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;class=&quot;foo&quot;&nbsp;style=&quot;text-align:center&quot;&nbsp;"
            u8"id="
            u8"&quot;bar&quot;&gt;header&lt;/th&gt;&lt;/tr&gt;&lt;/table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<th style='color:red;text-align:center'> -> unknown CSS...") {
        // <th style="color:red;text-align:center"> -> unknown CSS -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><th style=\"color:red;text-align:center\">header</th></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;color:red;text-align:center&quot;&gt;header&lt;/"
            u8"th&gt;&lt;/"
            u8"tr&gt;&lt;/table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<th style='text-align:center;color:red'> -> unknown CSS...") {
        // <th style="text-align:center;color:red"> -> unknown CSS -> whole table rejected, escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><th style=\"text-align:center;color:red\">header</th></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;text-align:center;color:red&quot;&gt;header&lt;/"
            u8"th&gt;&lt;"
            u8"/tr&gt;&lt;/table&gt;";
        CHECK(html == answer);
    }

    // ── <th> uppercase text-align values rejected ──

    TEST_CASE("── <th> uppercase text-align values rejected ──") {
        // <th style="text-align:LEFT"> -> uppercase -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:LEFT\">header</th></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;text-align:LEFT&quot;&gt;header&lt;/th&gt;&lt;/"
            u8"tr&gt;&lt;/"
            u8"table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<th style='text-align:Left'> -> mixed case -> whole tabl...") {
        // <th style="text-align:Left"> -> mixed case -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:Left\">header</th></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;text-align:Left&quot;&gt;header&lt;/th&gt;&lt;/"
            u8"tr&gt;&lt;/"
            u8"table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<th style='text-align:CENTER'> -> uppercase -> whole tab...") {
        // <th style="text-align:CENTER"> -> uppercase -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:CENTER\">header</th></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;text-align:CENTER&quot;&gt;header&lt;/th&gt;&lt;/"
            u8"tr&gt;&lt;"
            u8"/table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<th style='text-align:Right'> -> mixed case -> whole tab...") {
        // <th style="text-align:Right"> -> mixed case -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><th style=\"text-align:Right\">header</th></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&nbsp;style=&quot;text-align:Right&quot;&gt;header&lt;/th&gt;&lt;/"
            u8"tr&gt;&lt;"
            u8"/table&gt;";
        CHECK(html == answer);
    }

    // ── <th> and <td> mixed styles ──

    TEST_CASE("── <th> and <td> mixed styles ──") {
        // <th> and <td> with different alignments in same row
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><tr><th style=\"text-align:center\">h</th><td style=\"text-align:right\">d</td></tr></table>");
        auto const& answer =
            u8"<table><tr><th style=\"text-align:center\">h</th><td style=\"text-align:right\">d</td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("<td style='text-align:Right'> -> uppercase -> whole tabl...") {
        // <td style="text-align:Right"> -> uppercase -> whole table rejected, escaped
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"text-align:Right\">cell</td></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;text-align:Right&quot;&gt;cell&lt;/td&gt;&lt;/tr&gt;&lt;/"
            u8"table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<table><tr><td Style=/'text-align:center/'>cell</td></tr...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td Style=\"text-align:center\">cell</td></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;Style=&quot;text-align:center&quot;&gt;cell&lt;/td&gt;&lt;/"
            u8"tr&gt;&lt;/"
            u8"table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<table><tr><td style=/'Text-align:center/'>cell</td></tr...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td style=\"Text-align:center\">cell</td></tr></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&nbsp;style=&quot;Text-align:center&quot;&gt;cell&lt;/td&gt;&lt;/"
            u8"tr&gt;&lt;/"
            u8"table&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<table><caption>caption</caption><colgroup><col></colgro...") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<table><caption>caption</caption><colgroup><col></colgroup><thead><tr><th "
            u8"style=\"text-align:center\">head</th></tr></thead><tbody><tr><td "
            u8"style=\"text-align:right\">body</td></tr></tbody><tfoot><tr><td>foot</td></tr></tfoot></table>");
        auto const& answer =
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
            u8"<size=20>\uff1c</size>/table<size=20>\uff1e</size>";
        CHECK(html == answer);
    }

    TEST_CASE("<table><tr><th style=/'text-align:right/'>head</th><td") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<table><tr><th style=\"text-align:right\">head</th><td "
            u8"style=\"text-align:center\">data</td></tr></table>");
        auto const& answer =
            u8"<size=20>\uff1c</size>table<size=20>\uff1e</size><size=20>\uff1c</size>tr<size=20>\uff1e</size><size=20>"
            u8"\uff1c</size>th style=\"text-align:right\"<size=20>\uff1e</size>head<size=20>\uff1c</size>/th<size=20>"
            u8"\uff1e</size><size=20>\uff1c</size>td style=\"text-align:center\"<size=20>\uff1e</size>data<size=20>"
            u8"\uff1c</size>/td<size=20>\uff1e</size><size=20>\uff1c</size>/tr<size=20>\uff1e</size><size=20>\uff1c</"
            u8"size>/table<size=20>\uff1e</size>";
        CHECK(html == answer);
    }

    // ── caption/cell text is parsed with the inline-only parser ──
    TEST_CASE("── caption/cell text is parsed with the inline-only pars...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<table><caption>cap\n# **title**</caption><tr><th>head\n<h1>x</h1></th><td>body\n- "
            u8"*item*</td></tr></table>");
        auto const& answer =
            u8"<table><caption>cap<br>#&nbsp;<strong>title</strong></caption><tr><th>head<br>&lt;h1&gt;x&lt;/"
            u8"h1&gt;</th><td>body<br>-&nbsp;<em>item</em></td></tr></table>";
        CHECK(html == answer);
    }

    TEST_CASE("optionally_to_html_table_ast probes subview(pltext; curr...") {
        // optionally_to_html_table_ast probes subview(pltext, current_index + 2) past the end of a
        // short view after matching the opening <table> tag, which calls fast_terminate.
        auto const& pltext = u8"<table>X";
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(pltext);
        ::pltxt2htm::container::U8String answer{u8"&lt;table&gt;X"};
        CHECK(html == answer);
    }
}
