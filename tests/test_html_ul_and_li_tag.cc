#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>item1</li><li>item2</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>item1</li><li>item2</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<UL    ><LI>item1</LI><LI>item2</LI></UL  >");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>item1</li><li>item2</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li><color=red>item1</color></li><li>item2</li></ul>");
        auto answer =
            ::fast_io::u8string_view{u8"<ul><li><span style=\"color:red;\">item1</span></li><li>item2</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li><color=red>item1</li></ul></color>");
        auto answer = ::fast_io::u8string_view{
            u8"<ul><li><span style=\"color:red;\">item1&lt;/li&gt;&lt;/ul&gt;</span></li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>item1<ul><li>item2</li></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{
            u8"<ul><li>item1&lt;ul&gt;&lt;li&gt;item2</li></ul>&lt;/li&gt;&lt;/ul&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>item1<ul><li>item2</li></ul></li></ul>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"\u2022 item1<size=20>\uFF1C</size>ul<size=20>\uFF1E</size><size=20>\uFF1C</size>li<size=20>\uFF1E</size>item2\n"
            u8"<size=20>\uFF1C</size>/li<size=20>\uFF1E</size><size=20>\uFF1C</size>/ul<size=20>\uFF1E</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<ul></ul>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;ul&gt;&lt;/ul&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"t<ul></ul>t");
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"t<size=20>\uFF1C</size>ul<size=20>\uFF1E</size><size=20>\uFF1C</size>/ul<size=20>\uFF1E</size>t"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<li");
        auto answer = ::fast_io::u8string_view{u8"t&lt;li"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>item1</li><del>test</del></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>item1</li><del>test</del></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto plrichtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<li>test</li>");
        auto answer = ::fast_io::u8string_view{
            u8"<size=20>\uFF1C</size>li<size=20>\uFF1E</size>test<size=20>\uFF1C</size>/li<size=20>\uFF1E</size>"};
        pltxt2htm_test_assert_equal(plrichtext, answer);
    }

    {
        auto plrichtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li><ul><li>xxx</li></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{
            u8"\u2022 <size=20>\uFF1C</size>ul<size=20>\uFF1E</size><size=20>\uFF1C</size>li<size=20>\uFF1E</size>xxx\n"
            u8"<size=20>\uFF1C</size>/li<size=20>\uFF1E</size><size=20>\uFF1C</size>/ul<size=20>\uFF1E</size>"};
        pltxt2htm_test_assert_equal(plrichtext, answer);
    }

    {
        auto plrichtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>text<ul><li>sub</li></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{
            u8"\u2022 text<size=20>\uFF1C</size>ul<size=20>\uFF1E</size><size=20>\uFF1C</size>li<size=20>\uFF1E</size>sub\n"
            u8"<size=20>\uFF1C</size>/li<size=20>\uFF1E</size><size=20>\uFF1C</size>/ul<size=20>\uFF1E</size>"};
        pltxt2htm_test_assert_equal(plrichtext, answer);
    }

    {
        // <ul> without <li> must not be parsed as the <u> underline tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul>t</ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul>t</ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text\n<ul><li>item</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"text<br><ul><li>item</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text\n<ul><li>item</li></ul>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\n\u2022 item\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text<br><ul><li>item</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"text<br><ul><li>item</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text<br><ul><li>item</li></ul>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\n\u2022 item\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"text<br><ul><li>item</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"text<br><ul><li>item</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}