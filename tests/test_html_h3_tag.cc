#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"<h3>text</h3>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<h3>text</h3>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<size=36><b>text</b></size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<H3    >text</h3  >"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<h3>text</h3>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<size=36><b>text</b></size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<h3><color=red>text</color></h3>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<h3><span style=\"color:red;\">text</span></h3>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<size=36><b><color=red>text</color></b></size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<h3><color=red>text</h3></color>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<h3><span style=\"color:red;\">text&lt;/h3&gt;</span></h3>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<size=36><b><color=red>text<size=20>\uFF1C</size>/h3<size=20>\uFF1E</size></color></b></"
            u8"size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<h3>text<h3>text</h3></h3>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<h3>text&lt;h3&gt;text</h3>&lt;/h3&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<size=36><b>text<size=20>\uFF1C</size>h3<size=20>\uFF1E</size>text</b></size>\n"
            u8"<size=20>\uFF1C</size>/h3<size=20>\uFF1E</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<h3>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<h3></h3>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<size=36><b></b></size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<h3></h3"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<h3>&lt;/h3</h3>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<size=36><b><size=20>\uFF1C</size>/h3</b></size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"# <h3>text"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<h1>&lt;h3&gt;text</h1>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<size=38><b><size=20>\uFF1C</size>h3<size=20>\uFF1E</size>text</b></size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"t<h3></h3>t"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"t&lt;h3&gt;&lt;/h3&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"t<size=20>\uFF1C</size>h3<size=20>\uFF1E</size><size=20>\uFF1C</size>/h3<size=20>\uFF1E</size>t"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"t<h3></h3"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"t&lt;h3&gt;&lt;/h3"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"t<size=20>\uFF1C</size>h3<size=20>\uFF1E</size><size=20>\uFF1C</size>/h3"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"text\n<h3>text</h3>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"text<br><h3>text</h3>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\n<size=36><b>text</b></size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"text<br><h3>text</h3>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"text<br><h3>text</h3>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\n<size=36><b>text</b></size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"text<br><h3>text</h3>"};
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"text<br><h3>text</h3>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
