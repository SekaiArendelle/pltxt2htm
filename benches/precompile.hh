#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

namespace pltxt2htm_bench {

[[nodiscard]]
auto bench_parse_pltxt(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string;

[[nodiscard]]
auto bench_parse_pltxt_no_opt(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string;

[[nodiscard]]
auto bench_full_common(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string;

[[nodiscard]]
auto bench_full_unittest(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string;

[[nodiscard]]
auto bench_full_fixedadv(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string;

[[nodiscard]]
auto bench_full_plunity(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string;

} // namespace pltxt2htm_bench
