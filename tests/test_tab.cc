#include <boost/ut.hpp>

#include <string_view>

#include "precompile.hh"

namespace pltxt2htm_test {

void register_tab_tests() {
    using namespace boost::ut;

    "tab/common-parser/expands-to-four-nbsp"_test = [] {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\t");
        auto answer = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };

    "tab/unit-parser/expands-before-text"_test = [] {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\ta");
        auto answer = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;a"};
        expect(html == answer) << "\n  actual: "
                               << ::std::string_view{reinterpret_cast<char const*>(html.data()), html.size()}
                               << "\nexpected: "
                               << ::std::string_view{reinterpret_cast<char const*>(answer.data()), answer.size()};
    };
}

} // namespace pltxt2htm_test
