#include "precompile.hh"

int main() {
    {
        ::pltxt2htm::container::U8String input;
        for (::std::size_t i{}; i < 500; ++i) {
            input.append<::pltxt2htm::Contracts::quick_enforce>(u8"<color=red>");
        }
        input.append<::pltxt2htm::Contracts::quick_enforce>(u8"hello");
        for (::std::size_t i{}; i < 500; ++i) {
            input.append<::pltxt2htm::Contracts::quick_enforce>(u8"</color>");
        }
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(::pltxt2htm::container::U8StringView{input});
        auto const& answer = u8"<span style=\"color:red;\">hello</span>";
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
