#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n# test<br>text");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>test</h1>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test<br>text");
        auto answer = ::fast_io::u8string_view{u8"<h1>test</h1>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test\ntext");
        auto answer = ::fast_io::u8string_view{u8"<h1>test</h1>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br># test\ntext");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>test</h1>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"");
        auto answer = ::fast_io::u8string_view{u8""};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test<b>\ntext");
        auto answer = ::fast_io::u8string_view{u8"<h1>test</h1>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# te<b>st\ntext");
        auto answer = ::fast_io::u8string_view{u8"<h1>te<strong>st</strong></h1>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"## test\ntext");
        auto answer = ::fast_io::u8string_view{u8"<h2>test</h2>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    ::pltxt2htm_test::assert_true(::pltxt2htm_test::pltxt2advanced_htmld(u8"### test") ==
                                  ::fast_io::u8string_view{u8"<h3>test</h3>"});
    ::pltxt2htm_test::assert_true(::pltxt2htm_test::pltxt2advanced_htmld(u8"#### test") ==
                                  ::fast_io::u8string_view{u8"<h4>test</h4>"});
    ::pltxt2htm_test::assert_true(::pltxt2htm_test::pltxt2advanced_htmld(u8"##### test") ==
                                  ::fast_io::u8string_view{u8"<h5>test</h5>"});

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"###### test\ntext");
        auto answer = ::fast_io::u8string_view{u8"<h6>test</h6>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"####### test\ntext");
        auto answer = ::fast_io::u8string_view{u8"#######&nbsp;test<br>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"#test\ntext");
        auto answer = ::fast_io::u8string_view{u8"#test<br>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"#            test\ntext");
        auto answer = ::fast_io::u8string_view{u8"<h1>test</h1>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"          # test\ntext");
        auto answer = ::fast_io::u8string_view{u8"<h1>test</h1>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test");
        auto answer = ::fast_io::u8string_view{u8"<h1>test</h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# ");
        auto answer = ::fast_io::u8string_view{u8"<h1></h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n# ");
        auto answer = ::fast_io::u8string_view{u8"<br><h1></h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br># ");
        auto answer = ::fast_io::u8string_view{u8"<br><h1></h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"#");
        auto answer = ::fast_io::u8string_view{u8"<h1></h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n##");
        auto answer = ::fast_io::u8string_view{u8"<br><h2></h2>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Br/>##");
        auto answer = ::fast_io::u8string_view{u8"<br><h2></h2>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\\# test<br>text");
        auto answer = ::fast_io::u8string_view{u8"#&nbsp;test<br>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# <i>test<br>text");
        auto answer = ::fast_io::u8string_view{u8"<h1><em>test</em></h1>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# <i>test\ntext");
        auto answer = ::fast_io::u8string_view{u8"<h1><em>test</em></h1>text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8" ## #");
        auto answer = ::fast_io::u8string_view{u8"<h2>#</h2>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    // Note that this test does not match standard markdown
    // If this issues, I will fix it
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8" #\n");
        auto answer = ::fast_io::u8string_view{u8"&nbsp;#<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"###");
        auto answer = ::fast_io::u8string_view{u8"<h3></h3>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"####");
        auto answer = ::fast_io::u8string_view{u8"<h4></h4>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"#####");
        auto answer = ::fast_io::u8string_view{u8"<h5></h5>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"######");
        auto answer = ::fast_io::u8string_view{u8"<h6></h6>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"#######");
        auto answer = ::fast_io::u8string_view{u8"#######"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
