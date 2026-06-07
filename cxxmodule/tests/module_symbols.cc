#include <meta>
#include <string>
#include <vector>

import pltxt2htm;

template<::std::meta::info ns>
consteval auto collect_module_info() -> ::std::vector<::std::meta::info> {
    ::std::vector<::std::meta::info> symbols{};
    template for (constexpr auto m : ::std::define_static_array(
                      ::std::meta::members_of(ns, ::std::meta::access_context::unprivileged()))) {
        if constexpr (::std::meta::is_namespace(m)) {
            if constexpr (!::std::meta::has_identifier(m)) {
                continue;
            }
            symbols.append_range(collect_module_info<m>());
        }
        else {
            symbols.push_back(m);
        }
    }
    return symbols;
}

auto get_module_symbols() -> ::std::vector<::std::string> {
    static constexpr auto info_arr = ::std::define_static_array(collect_module_info<^^::pltxt2htm>());

    ::std::vector<::std::string> result;
    template for (constexpr auto& info : info_arr) {
        result.push_back(::std::string(::std::meta::identifier_of(info)));
    }
    return result;
}
