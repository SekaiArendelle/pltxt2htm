#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    constexpr bool is_equal{::pltxt2htm::pltxt2fixedadv_html(u8"test", u8"localhost:5173", u8"$PROJECT", u8"$VISITOR",
                                                             u8"$AUTHOR",
                                                             u8"$CO_AUTHORS") == ::fast_io::u8string_view(u8"test")};
    static_assert(is_equal);

    return 0;
}
