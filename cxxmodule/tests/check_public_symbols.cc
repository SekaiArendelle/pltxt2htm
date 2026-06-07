#include <algorithm>
#include <string>
#include <vector>
#include <fast_io/fast_io.h>

auto get_header_symbols() -> ::std::vector<::std::string>;
auto get_module_symbols() -> ::std::vector<::std::string>;

int main()
{
    auto header_vec = get_header_symbols();
    auto module_vec = get_module_symbols();

    ::std::sort(header_vec.begin(), header_vec.end());
    ::std::sort(module_vec.begin(), module_vec.end());

    if (header_vec == module_vec)
    {
        ::fast_io::println("PASS: all ", header_vec.size(), " public symbols match.");
        return 0;
    }

    ::fast_io::println("FAIL: symbol sets differ.");
    ::fast_io::println("  header: ", header_vec.size(), " symbols, module: ", module_vec.size(), " symbols");

    for (auto const& s : header_vec)
    {
        if (!::std::binary_search(module_vec.cbegin(), module_vec.cend(), s))
        {
            ::fast_io::println("  only in header: ", s);
        }
    }
    for (auto const& s : module_vec)
    {
        if (!::std::binary_search(header_vec.cbegin(), header_vec.cend(), s))
        {
            ::fast_io::println("  only in module: ", s);
        }
    }
    return 1;
}
