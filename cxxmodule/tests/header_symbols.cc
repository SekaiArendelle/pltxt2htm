#include <pltxt2htm/pltxt2htm.hh>

#include <meta>
#include <string>
#include <vector>

template<::std::meta::info ns>
consteval auto collect_header_info() -> ::std::vector<::std::meta::info> {
    ::std::vector<::std::meta::info> symbols{};
    template for (constexpr auto m : ::std::define_static_array(
                      ::std::meta::members_of(ns, ::std::meta::access_context::unprivileged()))) {
        if constexpr (::std::meta::is_namespace(m)) {
            if constexpr (!::std::meta::has_identifier(m) || ::std::meta::identifier_of(m) == "details") {
                continue;
            }
            else {
                symbols.append_range(collect_header_info<m>());
            }
        }
        else {
            symbols.push_back(m);
        }
    }
    return symbols;
}

auto get_header_symbols() -> ::std::vector<::std::string> {
    static constexpr auto info_arr = ::std::define_static_array(collect_header_info<^^::pltxt2htm>());

    ::std::vector<::std::string> result;
    template for (constexpr auto& info : info_arr) {
        result.push_back(::std::string(::std::meta::identifier_of(info)));
    }
    return result;
}
