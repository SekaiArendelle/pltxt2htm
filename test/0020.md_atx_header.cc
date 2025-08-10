#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n# test<br>text");
    auto answer1 = ::fast_io::u8string_view{u8"<br><h1>test</h1><br>text"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test<br>text");
    auto answer2 = ::fast_io::u8string_view{u8"<h1>test</h1><br>text"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test\ntext");
    auto answer3 = ::fast_io::u8string_view{u8"<h1>test</h1><br>text"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br># test\ntext");
    auto answer4 = ::fast_io::u8string_view{u8"<br><h1>test</h1><br>text"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"");
    auto answer5 = ::fast_io::u8string_view{u8""};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test<b>\ntext");
    auto answer6 = ::fast_io::u8string_view{u8"<h1>test</h1><br>text"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# te<b>st\ntext");
    auto answer7 = ::fast_io::u8string_view{u8"<h1>te<strong>st</strong></h1><br>text"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"## test\ntext");
    auto answer8 = ::fast_io::u8string_view{u8"<h2>test</h2><br>text"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    ::pltxt2htm_test::assert_true(::pltxt2htm_test::pltxt2advanced_htmld(u8"### test") ==
                                  ::fast_io::u8string_view{u8"<h3>test</h3>"});
    ::pltxt2htm_test::assert_true(::pltxt2htm_test::pltxt2advanced_htmld(u8"#### test") ==
                                  ::fast_io::u8string_view{u8"<h4>test</h4>"});
    ::pltxt2htm_test::assert_true(::pltxt2htm_test::pltxt2advanced_htmld(u8"##### test") ==
                                  ::fast_io::u8string_view{u8"<h5>test</h5>"});

    auto html9 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"###### test\ntext");
    auto answer9 = ::fast_io::u8string_view{u8"<h6>test</h6><br>text"};
    ::pltxt2htm_test::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"####### test\ntext");
    auto answer10 = ::fast_io::u8string_view{u8"#######&nbsp;test<br>text"};
    ::pltxt2htm_test::assert_true(html10 == answer10);

    auto html11 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"#test\ntext");
    auto answer11 = ::fast_io::u8string_view{u8"#test<br>text"};
    ::pltxt2htm_test::assert_true(html11 == answer11);

    auto html12 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"#            test\ntext");
    auto answer12 = ::fast_io::u8string_view{u8"<h1>test</h1><br>text"};
    ::pltxt2htm_test::assert_true(html12 == answer12);

    auto html13 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"          # test\ntext");
    auto answer13 = ::fast_io::u8string_view{u8"<h1>test</h1><br>text"};
    ::pltxt2htm_test::assert_true(html13 == answer13);

    auto html14 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test");
    auto answer14 = ::fast_io::u8string_view{u8"<h1>test</h1>"};
    ::pltxt2htm_test::assert_true(html14 == answer14);

    auto html15 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# ");
    auto answer15 = ::fast_io::u8string_view{u8"<h1></h1>"};
    ::pltxt2htm_test::assert_true(html15 == answer15);

    auto html16 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n# ");
    auto answer16 = ::fast_io::u8string_view{u8"<br><h1></h1>"};
    ::pltxt2htm_test::assert_true(html16 == answer16);

    auto html17 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br># ");
    auto answer17 = ::fast_io::u8string_view{u8"<br><h1></h1>"};
    ::pltxt2htm_test::assert_true(html17 == answer17);

    auto html18 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"#");
    auto answer18 = ::fast_io::u8string_view{u8"<h1></h1>"};
    ::pltxt2htm_test::assert_true(html18 == answer18);

    auto html19 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n##");
    auto answer19 = ::fast_io::u8string_view{u8"<br><h2></h2>"};
    ::pltxt2htm_test::assert_true(html19 == answer19);

    auto html20 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Br/>##");
    auto answer20 = ::fast_io::u8string_view{u8"<br><h2></h2>"};
    ::pltxt2htm_test::assert_true(html20 == answer20);

    auto html21 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\\# test<br>text");
    auto answer21 = ::fast_io::u8string_view{u8"#&nbsp;test<br>text"};
    ::pltxt2htm_test::assert_true(html21 == answer21);

    auto html22 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# <i>test<br>text");
    auto answer22 = ::fast_io::u8string_view{u8"<h1><em>test</em></h1><br>text"};
    ::pltxt2htm_test::assert_true(html22 == answer22);

    auto html23 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# <i>test\ntext");
    auto answer23 = ::fast_io::u8string_view{u8"<h1><em>test</em></h1><br>text"};
    ::pltxt2htm_test::assert_true(html23 == answer23);

    auto html24 = ::pltxt2htm_test::pltxt2advanced_htmld(u8" ## #");
    auto answer24 = ::fast_io::u8string_view{u8"<h2>#</h2>"};
    ::pltxt2htm_test::assert_true(html24 == answer24);

    return 0;
}
