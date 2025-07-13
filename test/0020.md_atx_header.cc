#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"\n# test<br>text", u8"_");
    auto answer1 = ::fast_io::u8string_view{u8"<br><h1>test</h1><br>text"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(u8"# test<br>text", u8"_");
    auto answer2 = ::fast_io::u8string_view{u8"<h1>test</h1><br>text"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(u8"# test\ntext", u8"_");
    auto answer3 = ::fast_io::u8string_view{u8"<h1>test</h1><br>text"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(u8"<br># test\ntext", u8"_");
    auto answer4 = ::fast_io::u8string_view{u8"<br><h1>test</h1><br>text"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2advanced_html(u8"", u8"_");
    auto answer5 = ::fast_io::u8string_view{u8""};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"# test<b>\ntext", u8"_");
    auto answer6 = ::fast_io::u8string_view{u8"<h1>test</h1><br>text"};
    ::exception::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm::pltxt2advanced_html(u8"# te<b>st\ntext", u8"_");
    auto answer7 = ::fast_io::u8string_view{u8"<h1>te<b>st</b></h1><br>text"};
    ::exception::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm::pltxt2advanced_html(u8"## test\ntext", u8"_");
    auto answer8 = ::fast_io::u8string_view{u8"<h2>test</h2><br>text"};
    ::exception::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm::pltxt2advanced_html(u8"###### test\ntext", u8"_");
    auto answer9 = ::fast_io::u8string_view{u8"<h6>test</h6><br>text"};
    ::exception::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm::pltxt2advanced_html(u8"####### test\ntext", u8"_");
    auto answer10 = ::fast_io::u8string_view{u8"#######&nbsp;test<br>text"};
    ::exception::assert_true(html10 == answer10);

    auto html11 = ::pltxt2htm::pltxt2advanced_html(u8"#test\ntext", u8"_");
    auto answer11 = ::fast_io::u8string_view{u8"#test<br>text"};
    ::exception::assert_true(html11 == answer11);

    auto html12 = ::pltxt2htm::pltxt2advanced_html(u8"#            test\ntext", u8"_");
    auto answer12 = ::fast_io::u8string_view{u8"<h1>test</h1><br>text"};
    ::exception::assert_true(html12 == answer12);

    auto html13 = ::pltxt2htm::pltxt2advanced_html(u8"          # test\ntext", u8"_");
    auto answer13 = ::fast_io::u8string_view{u8"<h1>test</h1><br>text"};
    ::exception::assert_true(html13 == answer13);

    auto html14 = ::pltxt2htm::pltxt2advanced_html(u8"# test", u8"_");
    auto answer14 = ::fast_io::u8string_view{u8"<h1>test</h1>"};
    ::exception::assert_true(html14 == answer14);

    auto html15 = ::pltxt2htm::pltxt2advanced_html(u8"# ", u8"_");
    auto answer15 = ::fast_io::u8string_view{u8"<h1></h1>"};
    ::exception::assert_true(html15 == answer15);

    auto html16 = ::pltxt2htm::pltxt2advanced_html(u8"\n# ", u8"_");
    auto answer16 = ::fast_io::u8string_view{u8"<br><h1></h1>"};
    ::exception::assert_true(html16 == answer16);

    auto html17 = ::pltxt2htm::pltxt2advanced_html(u8"<br># ", u8"_");
    auto answer17 = ::fast_io::u8string_view{u8"<br><h1></h1>"};
    ::exception::assert_true(html17 == answer17);

    auto html18 = ::pltxt2htm::pltxt2advanced_html(u8"#", u8"_");
    auto answer18 = ::fast_io::u8string_view{u8"<h1></h1>"};
    ::exception::assert_true(html18 == answer18);

    auto html19 = ::pltxt2htm::pltxt2advanced_html(u8"\n##", u8"_");
    auto answer19 = ::fast_io::u8string_view{u8"<br><h2></h2>"};
    ::exception::assert_true(html19 == answer19);

    auto html20 = ::pltxt2htm::pltxt2advanced_html(u8"<Br/>##", u8"_");
    auto answer20 = ::fast_io::u8string_view{u8"<br><h2></h2>"};
    ::exception::assert_true(html20 == answer20);

    return 0;
}
