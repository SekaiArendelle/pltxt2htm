#include <cstring>
#include <pltxt2htm/pltxt2htm.hh>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) /* throws */ {
    ::fast_io::u8string str(size + 1);
    ::std::memcpy(str.data(), data, size);
    [[maybe_unused]] auto _ = ::pltxt2htm::pltxt2advanced_html(::fast_io::mnp::os_c_str(str), u8"_");

    return 0;
}
