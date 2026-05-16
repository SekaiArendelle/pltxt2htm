#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    constexpr bool is_equal{::pltxt2htm::pltxt4unittest(u8"test") == ::fast_io::u8string_view(u8"test")};
    static_assert(is_equal);

    return 0;
}
