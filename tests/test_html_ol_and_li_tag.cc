#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li>item1</li><li>item2</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol><li>item1</li><li>item2</li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<OL    ><LI>item1</LI><LI>item2</LI></OL  >");
        auto answer = ::fast_io::u8string_view{u8"<ol><li>item1</li><li>item2</li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li><color=red>item1</color></li><li>item2</li></ol>");
        auto answer =
            ::fast_io::u8string_view{u8"<ol><li><span style=\"color:red;\">item1</span></li><li>item2</li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li><color=red>item1</li></ol></color>");
        auto answer =
            ::fast_io::u8string_view{u8"<ol><li><span style=\"color:red;\">item1</span></li></ol>&lt;/color&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li>item1<ol><li>item2</li></ol></li></ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol><li>item1<ol><li>item2</li></ol></li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol>");
        auto answer = ::fast_io::u8string_view{u8"&lt;ol&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<ol></ol>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;ol&gt;&lt;/ol&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<li");
        auto answer = ::fast_io::u8string_view{u8"t&lt;li"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<olt");
        auto answer = ::fast_io::u8string_view{u8"t&lt;olt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol><li>test</li><li>text</li><li>test</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n3. test\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol><li><ol><li>xxx</li></ol></li></ol>");
        auto answer = ::fast_io::u8string_view{u8"1. \n  1. xxx\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>a<ul><li>b<ul><li>c</li></ul></li></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"\u2022 a\n  \u2218 b\n    \u25ab c\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>a<ol><li>b</li></ol></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"\u2022 a\n  1. b\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"5\"><li>item1</li><li>item2</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol start=\"5\"><li>item1</li><li>item2</li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"1\"><li>item1</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol><li>item1</li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"0\"><li>item1</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol start=\"0\"><li>item1</li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li>item1<ol start=\"3\"><li>item2</li></ol></li></ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol><li>item1<ol start=\"3\"><li>item2</li></ol></li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<OL START=\"5\" ><LI>item1</LI></OL>");
        auto answer = ::fast_io::u8string_view{u8"<ol start=\"5\"><li>item1</li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start='7'><li>item1</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol start=\"7\"><li>item1</li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol start=\"5\"><li>a</li><li>b</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"5. a\n6. b\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol><li>a<ol start=\"2\"><li>b</li></ol></li></ol>");
        auto answer = ::fast_io::u8string_view{u8"1. a\n  2. b\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<ol start=\"5\"><li>a</li><li>b</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol start=\"5\"><li>a</li><li>b</li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"abc\"><li>a</li></ol>");
        auto answer =
            ::fast_io::u8string_view{u8"&lt;ol&nbsp;start=&quot;abc&quot;&gt;&lt;li&gt;a&lt;/li&gt;&lt;/ol&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol class=\"x\"><li>a</li></ol>");
        auto answer =
            ::fast_io::u8string_view{u8"&lt;ol&nbsp;class=&quot;x&quot;&gt;&lt;li&gt;a&lt;/li&gt;&lt;/ol&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"5\">");
        auto answer = ::fast_io::u8string_view{u8"&lt;ol&nbsp;start=&quot;5&quot;&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
