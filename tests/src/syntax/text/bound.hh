#pragma once

#include "precompile.hh"

namespace pltxt2htm_test::syntax {

inline void bound() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<");
        auto answer = ::fast_io::u8string_view{u8"test&lt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test\\");
        auto answer = ::fast_io::u8string_view{u8"test\\"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test</");
        auto answer = ::fast_io::u8string_view{u8"test&lt;/"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n");
        auto answer = ::fast_io::u8string_view{u8"<br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
}

} // namespace pltxt2htm_test::syntax
