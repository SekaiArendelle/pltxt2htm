#include "fast_io/fast_io_dsal/string_view.h"
#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test{Project}test");
        auto answer = ::fast_io::u8string_view{u8"test$PROJECTtest"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test{Visitor}test");
        auto answer = ::fast_io::u8string_view{u8"test$VISITORtest"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test{Author}test");
        auto answer = ::fast_io::u8string_view{u8"test$AUTHORtest"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test{CoAuthors}test");
        auto answer = ::fast_io::u8string_view{u8"test$CO_AUTHORStest"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
