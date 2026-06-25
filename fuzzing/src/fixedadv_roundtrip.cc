// Roundtrip / idempotency fuzzer for pltxt2fixedadv_html.
//
// The pltext-to-HTML converter also accepts inline HTML as input, so its
// output can be fed back as input for a second pass.  If the conversion is
// idempotent, the two passes should produce identical HTML:
//
//   f(pltext) == f(f(pltext))
//
// A mismatch indicates either:
//   - XSS-like problems: content "leaks" through differently on the second pass
//   - Incomplete HTML syntax support: the parser does not fully understand the
//     HTML that the backend itself emits
//
// Both cases need fixing, so we abort to let libFuzzer save the input.

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fast_io/fast_io.h>
#include <pltxt2htm/pltxt2htm.hh>

extern "C" int LLVMFuzzerTestOneInput(::std::uint8_t const* const data, ::std::size_t const size) noexcept {
    ::fast_io::u8string str(size + 1);
    ::std::memcpy(str.data(), data, size);

    auto const html1 = ::pltxt2htm::pltxt2fixedadv_html(
        ::fast_io::mnp::os_c_str(str), u8"_", u8"_", u8"_", u8"_", u8"_");

    auto const html2 = ::pltxt2htm::pltxt2fixedadv_html(
        ::fast_io::mnp::os_c_str(html1), u8"_", u8"_", u8"_", u8"_", u8"_");

    if (html1 != html2) {
        ::fast_io::perr(::fast_io::u8c_stderr(), u8"Mismatch detected in roundtrip conversion:\n"
                        u8"Original pltext: ", str, u8"\n"
                        u8"First pass HTML: ", html1, u8"\n"
                        u8"Second pass HTML: ", html2, u8"\n");
        ::std::fflush(stderr);
        ::std::abort();
    }

    return 0;
}
