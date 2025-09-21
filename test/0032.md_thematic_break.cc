#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n---");
    auto answer1 = ::fast_io::u8string_view{u8"<br><hr>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n--- \nt");
    auto answer2 = ::fast_io::u8string_view{u8"<br><hr>t"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n--- <Br />t");
    auto answer3 = ::fast_io::u8string_view{u8"<br><hr>t"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Br />---");
    auto answer4 = ::fast_io::u8string_view{u8"<br><hr>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Br />--- \nt");
    auto answer5 = ::fast_io::u8string_view{u8"<br><hr>t"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Br />--- <Br />t");
    auto answer6 = ::fast_io::u8string_view{u8"<br><hr>t"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"---");
    auto answer7 = ::fast_io::u8string_view{u8"<hr>"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"--- \nt");
    auto answer8 = ::fast_io::u8string_view{u8"<hr>t"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"--- <Br />t");
    auto answer9 = ::fast_io::u8string_view{u8"<hr>t"};
    ::pltxt2htm_test::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"_ _  _");
    auto answer10 = ::fast_io::u8string_view{u8"<hr>"};
    ::pltxt2htm_test::assert_true(html10 == answer10);

    auto html11 = ::pltxt2htm_test::pltxt2advanced_htmld(u8" ** * ***");
    auto answer11 = ::fast_io::u8string_view{u8"<hr>"};
    ::pltxt2htm_test::assert_true(html11 == answer11);

    // TODO FIXME

    // auto html12 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test\n ---");
    // auto answer12 = ::fast_io::u8string_view{u8"<h1>test</h1><hr>"};
    // ::pltxt2htm_test::assert_true(html12 == answer12);

    // auto html13 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test<br> ---");
    // auto answer13 = ::fast_io::u8string_view{u8"<h1>test</h1><hr>"};
    // ::pltxt2htm_test::assert_true(html13 == answer13);

    // auto html14 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n# test\n ---");
    // auto answer14 = ::fast_io::u8string_view{u8"<h1>test</h1><hr>"};
    // ::pltxt2htm_test::assert_true(html14 == answer14);

    // auto html15 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br># test\n ---");
    // auto answer15 = ::fast_io::u8string_view{u8"<h1>test</h1><hr>"};
    // ::pltxt2htm_test::assert_true(html15 == answer15);

    // auto html16 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"---\n# test\n ---");
    // auto answer16 = ::fast_io::u8string_view{u8"<h1>test</h1><hr>"};
    // ::pltxt2htm_test::assert_true(html16 == answer16);

    // auto html17 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"---\n---\n ---");
    // auto answer17 = ::fast_io::u8string_view{u8"<hr><hr><hr>"};
    // ::pltxt2htm_test::assert_true(html17 == answer17);

    return 0;
}
