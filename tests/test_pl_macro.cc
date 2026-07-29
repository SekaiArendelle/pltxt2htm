#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{Project}test");
        auto answer = ::fast_io::u8string_view{u8"test$PROJECTtest"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{Visitor}test");
        auto answer = ::fast_io::u8string_view{u8"test$VISITORtest"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{Author}test");
        auto answer = ::fast_io::u8string_view{u8"test$AUTHORtest"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{CoAuthors}test");
        auto answer = ::fast_io::u8string_view{u8"test$CO_AUTHORStest"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"{Project}", u8"localhost", u8"'", u8"", u8"", u8"");
        auto answer = ::fast_io::u8string_view{u8"&apos;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"{Project}{Visitor}{Author}{CoAuthors}", u8"project",
                                                                 u8"visitor", u8"author", u8"coauthors");
        auto answer = ::fast_io::u8string_view{u8"projectvisitorauthorcoauthors"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
