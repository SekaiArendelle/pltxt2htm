/**
 * @file language.hh
 * @brief Languages represented by fenced-code ASTs.
 */

#pragma once

namespace pltxt2htm {

enum class CodeLanguage : unsigned {
    rendered = 0,
    plain,
    bash,
    c,
    cpp,
    csharp,
    css,
    go,
    html,
    java,
    javascript,
    json,
    kotlin,
    lua,
    python,
    rust,
    sql,
    toml,
    typescript,
    xml,
    yaml,
};

} // namespace pltxt2htm
