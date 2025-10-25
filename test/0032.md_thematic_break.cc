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

    auto html12 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test\n ---");
    auto answer12 = ::fast_io::u8string_view{u8"<h1>test</h1><hr>"};
    ::pltxt2htm_test::assert_true(html12 == answer12);

    auto html13 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test<br> ---");
    auto answer13 = ::fast_io::u8string_view{u8"<h1>test</h1><hr>"};
    ::pltxt2htm_test::assert_true(html13 == answer13);

    auto html14 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n# test\n ---");
    auto answer14 = ::fast_io::u8string_view{u8"<br><h1>test</h1><hr>"};
    ::pltxt2htm_test::assert_true(html14 == answer14);

    auto html15 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br># test\n ---");
    auto answer15 = ::fast_io::u8string_view{u8"<br><h1>test</h1><hr>"};
    ::pltxt2htm_test::assert_true(html15 == answer15);

    auto html16 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"---\n# test\n ---");
    auto answer16 = ::fast_io::u8string_view{u8"<hr><h1>test</h1><hr>"};
    ::pltxt2htm_test::assert_true(html16 == answer16);

    auto html17 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"---\n---\n ---");
    auto answer17 = ::fast_io::u8string_view{u8"<hr><hr><hr>"};
    ::pltxt2htm_test::assert_true(html17 == answer17);

    auto html18 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br> ---\n # test");
    auto answer18 = ::fast_io::u8string_view{u8"<br><hr><h1>test</h1>"};
    ::pltxt2htm_test::assert_true(html18 == answer18);

    auto html19 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n---\n---");
    auto answer19 = ::fast_io::u8string_view{u8"<br><hr><hr>"};
    ::pltxt2htm_test::assert_true(html19 == answer19);

    auto html20 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n --- <Br /> # test");
    auto answer20 = ::fast_io::u8string_view{u8"<br><hr><h1>test</h1>"};
    ::pltxt2htm_test::assert_true(html20 == answer20);

    auto html21 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n --- <Br /> # test\ntest");
    auto answer21 = ::fast_io::u8string_view{u8"<br><hr><h1>test</h1>test"};
    ::pltxt2htm_test::assert_true(html21 == answer21);

    auto html22 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n --- <Br /># \ntest");
    auto answer22 = ::fast_io::u8string_view{u8"<br><hr><h1></h1>test"};
    ::pltxt2htm_test::assert_true(html22 == answer22);

    auto html23 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n --- <Br /># \n# \ntest");
    auto answer23 = ::fast_io::u8string_view{u8"<br><hr><h1></h1><h1></h1>test"};
    ::pltxt2htm_test::assert_true(html23 == answer23);

    auto html24 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br> --- <Br /># \ntest");
    auto answer24 = ::fast_io::u8string_view{u8"<br><hr><h1></h1>test"};
    ::pltxt2htm_test::assert_true(html24 == answer24);

    auto html25 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br> --- <Br /># \n# \ntest");
    auto answer25 = ::fast_io::u8string_view{u8"<br><hr><h1></h1><h1></h1>test"};
    ::pltxt2htm_test::assert_true(html25 == answer25);

    auto html26 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n# t\n# t");
    auto answer26 = ::fast_io::u8string_view{u8"<br><h1>t</h1><h1>t</h1>"};
    ::pltxt2htm_test::assert_true(html26 == answer26);

    auto html27 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br># t\n# t");
    auto answer27 = ::fast_io::u8string_view{u8"<br><h1>t</h1><h1>t</h1>"};
    ::pltxt2htm_test::assert_true(html27 == answer27);

    auto html28 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n# t\n# t\n# t");
    auto answer28 = ::fast_io::u8string_view{u8"<br><h1>t</h1><h1>t</h1><h1>t</h1>"};
    ::pltxt2htm_test::assert_true(html28 == answer28);

    auto html29 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n#  #\n# #\n # #");
    auto answer29 = ::fast_io::u8string_view{u8"<br><h1>#</h1><h1>#</h1><h1>#</h1>"};
    ::pltxt2htm_test::assert_true(html29 == answer29);

    return 0;
}
