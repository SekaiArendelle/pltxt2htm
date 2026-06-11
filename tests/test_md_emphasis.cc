#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"*test*"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<em>test</em>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<i>test</i>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t*e*st");
        auto answer = ::fast_io::u8string_view{u8"t<em>e</em>st"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t**e**st");
        auto answer = ::fast_io::u8string_view{u8"t<strong>e</strong>st"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Test triple emphasis with asterisks ***text***
        auto pltext = ::fast_io::u8string_view{u8"***test***"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string{u8"<em><strong>test</strong></em>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<b><i>test</i></b>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t***e***st");
        auto answer = ::fast_io::u8string_view{u8"t<em><strong>e</strong></em>st"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t_e_st");
        auto answer = ::fast_io::u8string_view{u8"t<em>e</em>st"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t__e__st");
        auto answer = ::fast_io::u8string_view{u8"t<strong>e</strong>st"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t___e___st");
        auto answer = ::fast_io::u8string_view{u8"t<em><strong>e</strong></em>st"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t_e*st");
        auto answer = ::fast_io::u8string_view{u8"t_e*st"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"*test\n*");
        auto answer = ::fast_io::u8string_view{u8"*test<br>*"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"**test\n**");
        auto answer = ::fast_io::u8string_view{u8"**test<br>**"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te***st\n***");
        auto answer = ::fast_io::u8string_view{u8"te***st<br><hr>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Test triple emphasis with underscores ___text___
        auto pltext = ::fast_io::u8string_view{u8"___test___"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string{u8"<em><strong>test</strong></em>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<b><i>test</i></b>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t****t");
        auto answer = ::fast_io::u8string{u8"t****t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t***t");
        auto answer = ::fast_io::u8string{u8"t***t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t*****t");
        auto answer = ::fast_io::u8string{u8"t*****t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"&__</_");
        auto answer = ::fast_io::u8string_view{u8"&amp;_<em>&lt;/</em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Redundant tag elimination: triple emphasis inside em-like parent → em part redundant
    {
        // <em> wrapping *** → em part of triple is redundant, convert to <strong>
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em>***text***</em>");
        auto answer = ::fast_io::u8string_view{u8"<em><strong>text</strong></em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <i> wrapping *** → same as <em>, convert to <strong>
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<i>***text***</i>");
        auto answer = ::fast_io::u8string_view{u8"<em><strong>text</strong></em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Redundant tag elimination: triple emphasis inside strong-like parent → strong part redundant
    {
        // <strong> wrapping *** → strong part of triple is redundant, convert to <em>
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong>***text***</strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong><em>text</em></strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <b> wrapping *** → same as <strong>, convert to <em>
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b>***text***</b>");
        auto answer = ::fast_io::u8string_view{u8"<strong><em>text</em></strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Redundant tag elimination: em/strong inside triple emphasis → child redundant
    {
        // <em> inside *** → em redundant (triple already provides <em>)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"***<em>text</em>***");
        auto answer = ::fast_io::u8string_view{u8"<em><strong>text</strong></em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <strong> inside *** → strong redundant (triple already provides <strong>)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"***<strong>text</strong>***");
        auto answer = ::fast_io::u8string_view{u8"<em><strong>text</strong></em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Redundant tag elimination: with surrounding text content
    {
        // <em> wrapping text***text***text → triple converted to <strong> in em context
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em>text***text***text</em>");
        auto answer = ::fast_io::u8string_view{u8"<em>text<strong>text</strong>text</em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <strong> wrapping text***text***text → triple converted to <em> in strong context
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong>text***text***text</strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong>text<em>text</em>text</strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // *** surrounding text<em>text</em>text → inner em redundant, unwrapped
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text***<em>text</em>***text");
        auto answer = ::fast_io::u8string_view{u8"text<em><strong>text</strong></em>text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // *** surrounding text<strong>text</strong>text → inner strong redundant, unwrapped
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text***<strong>text</strong>***text");
        auto answer = ::fast_io::u8string_view{u8"text<em><strong>text</strong></em>text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Regression test: control chars inside ***...*** used to crash
    // Control chars 0x00-0x1F/0x7F now produce InvalidU8Char (rendered as U+FFFD)
    // instead of being silently dropped, ensuring the sub-AST is never empty.
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"***\a***");
        auto answer = ::fast_io::u8string_view{u8"<em><strong>\uFFFD</strong></em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
