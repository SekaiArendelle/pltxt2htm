#include <pltxt2htm/details/parser/try_parse.hh>
#include "precompile.hh"

int main() {
    {
        auto url = ::fast_io::u8string_view{u8"https://www.example.com"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"www.example.com"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"alert(1)"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://localhost:8080"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://127.0.0.1:8080"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.bilibili.com/video/BV1df421f7WB"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.bilibili.com/\"alert(1)"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.bilibili.com/\'hello\'"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.bilibili.com/test<script"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.bilibili.com/script>test"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{
            u8"https://www.bilibili.com/video/"
            u8"BV1df421f7WB?spm_id_from=333.788.player.player_end_recommend&vd_source=70bf7fea64a1877f5dd6b019915b25a1&"
            u8"trackid=web_related_0.router-related-2481894-wjgx7.1772331167937.864"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<true>(url).has_value());
    }

    return 0;
}
