#include <pltxt2htm/pltxt2htm.hh>

namespace pltxt2htm_bench {

auto bench_parse_pltxt(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    auto ast = ::pltxt2htm::parse_pltxt<::pltxt2htm::Contracts::quick_enforce>(pltext);
    ::pltxt2htm::optimize_ast<::pltxt2htm::Contracts::quick_enforce>(ast);
    return ::pltxt2htm::details::plweb_text_backend<::pltxt2htm::Contracts::quick_enforce, false>(
        ast, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
}

auto bench_parse_pltxt_no_opt(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    auto ast = ::pltxt2htm::parse_pltxt<::pltxt2htm::Contracts::quick_enforce>(pltext);
    return ::pltxt2htm::details::plweb_text_backend<::pltxt2htm::Contracts::quick_enforce, false>(
        ast, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
}

auto bench_full_common(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2common_html<::pltxt2htm::Contracts::quick_enforce>(pltext);
}

auto bench_full_unittest(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt4unittest<::pltxt2htm::Contracts::quick_enforce>(pltext);
}

auto bench_full_fixedadv(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2fixedadv_html<::pltxt2htm::Contracts::quick_enforce>(
        pltext, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
}

auto bench_full_plunity(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2plunity_introduction<::pltxt2htm::Contracts::quick_enforce>(
        pltext, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
}

} // namespace pltxt2htm_bench
