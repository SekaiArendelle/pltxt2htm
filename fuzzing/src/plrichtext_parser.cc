#include <cstdint>
#include <pltxt2htm/pltxt2htm.hh>

extern "C" int LLVMFuzzerTestOneInput(::std::uint8_t const* const data, ::std::size_t const size) noexcept {
    ::fast_io::u8string_view str{reinterpret_cast<char8_t const*>(data), size};
    [[maybe_unused]] auto _ = ::pltxt2htm::pltxt2plunity_introduction(str, u8"_", u8"_", u8"_", u8"_");

    return 0;
}
