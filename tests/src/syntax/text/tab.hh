#pragma once

#include "precompile.hh"

namespace pltxt2htm_test::syntax {

inline void tab() {
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\t");
        auto answer = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\ta");
        auto answer = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;a"};
        pltxt2htm_test_assert_equal(html, answer);
    }
}

} // namespace pltxt2htm_test::syntax
