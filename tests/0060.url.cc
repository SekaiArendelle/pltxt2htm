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
        auto url = ::fast_io::u8string_view{u8"https://localhost"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://localhost:8080"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://localhost/path"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://localhost:8080/path"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://127.0.0.1"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://127.0.0.1:8080"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://127.0.0.1/path"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://127.0.0.1:8080/path"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.bilibili.com/video/BV1df421f7WB"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.bilibili.com/\"alert(1)"};
        auto result = ::pltxt2htm::details::try_parse_url<false>(url).value();
        ::pltxt2htm_test::assert_true(result == 25);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.bilibili.com/\'hello\'"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.bilibili.com/test<script"};
        auto result = ::pltxt2htm::details::try_parse_url<false>(url).value();
        ::pltxt2htm_test::assert_true(result == 29);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.bilibili.com/script>test"};
        auto result = ::pltxt2htm::details::try_parse_url<false>(url).value();
        ::pltxt2htm_test::assert_true(result == 31);
    }
    {
        auto url = ::fast_io::u8string_view{
            u8"https://www.bilibili.com/video/"
            u8"BV1df421f7WB?spm_id_from=333.788.player.player_end_recommend&vd_source=70bf7fea64a1877f5dd6b019915b25a1&"
            u8"trackid=web_related_0.router-related-2481894-wjgx7.1772331167937.864"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.example.com:65535"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.example.com:65536"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.example.com:123456"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.example.com:2147483648"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.example.com/a/path"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.example.com:8080/a/path"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value());
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.example.wtf"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.example.wtf:8765"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.example.wtf/a/path"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }
    {
        auto url = ::fast_io::u8string_view{u8"https://www.example.wtf:8080/a/path"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::try_parse_url<false>(url).has_value() == false);
    }

    return 0;
}
