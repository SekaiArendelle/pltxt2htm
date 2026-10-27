#pragma once

#include <string_view>

[[nodiscard]]
constexpr auto is_excluded_namespace(::std::string_view const name) noexcept -> bool {
    return name == "details" || name.starts_with("sys_");
}
