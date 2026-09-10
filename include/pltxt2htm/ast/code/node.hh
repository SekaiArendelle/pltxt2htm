/**
 * @file node.hh
 * @brief Language-specific fenced-code node kinds and source ranges.
 */

#pragma once

#include <cstddef>
#include "../../contracts.hh"
#include "language.hh"

namespace pltxt2htm {

enum class CodeRenderedNodeKind : unsigned {
    text = 0,
    entity_reference,
    style_begin,
    style_end
};
enum class CodePlainNodeKind : unsigned {
    text = 0
};

enum class CodeBashNodeKind : unsigned {
    plain = 0,
    reserved_word,
    string_literal,
    number_literal,
    comment,
    command,
    macro,
    preprocessor,
};

enum class CodeCNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro_name,
    preprocessor_directive,
};

enum class CodeCppNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro_name,
    preprocessor_directive,
};

enum class CodeCSharpNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro_name,
    preprocessor_directive,
};

enum class CodeCssNodeKind : unsigned {
    plain = 0,
    at_rule,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

enum class CodeGoNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

enum class CodeHtmlNodeKind : unsigned {
    plain = 0,
    tag_name,
    attribute_value,
    number_literal,
    comment,
    attribute_name,
    macro,
    declaration,
};

enum class CodeJavaNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

enum class CodeJavaScriptNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

enum class CodeJsonNodeKind : unsigned {
    plain = 0,
    literal,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

enum class CodeKotlinNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

enum class CodeLuaNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

enum class CodePythonNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

enum class CodeRustNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro_name,
    preprocessor,
};

enum class CodeSqlNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

enum class CodeTomlNodeKind : unsigned {
    plain = 0,
    literal,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

enum class CodeTypeScriptNodeKind : unsigned {
    plain = 0,
    keyword,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

enum class CodeXmlNodeKind : unsigned {
    plain = 0,
    tag_name,
    attribute_value,
    number_literal,
    comment,
    attribute_name,
    macro,
    declaration,
};

enum class CodeYamlNodeKind : unsigned {
    plain = 0,
    literal,
    string_literal,
    number_literal,
    comment,
    function_name,
    macro,
    preprocessor,
};

template<::pltxt2htm::CodeLanguage>
class CodeNodeKindFor;

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::rendered> {
public:
    using Type = ::pltxt2htm::CodeRenderedNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::plain> {
public:
    using Type = ::pltxt2htm::CodePlainNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::bash> {
public:
    using Type = ::pltxt2htm::CodeBashNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::c> {
public:
    using Type = ::pltxt2htm::CodeCNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::cpp> {
public:
    using Type = ::pltxt2htm::CodeCppNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::csharp> {
public:
    using Type = ::pltxt2htm::CodeCSharpNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::css> {
public:
    using Type = ::pltxt2htm::CodeCssNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::go> {
public:
    using Type = ::pltxt2htm::CodeGoNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::html> {
public:
    using Type = ::pltxt2htm::CodeHtmlNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::java> {
public:
    using Type = ::pltxt2htm::CodeJavaNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::javascript> {
public:
    using Type = ::pltxt2htm::CodeJavaScriptNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::json> {
public:
    using Type = ::pltxt2htm::CodeJsonNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::kotlin> {
public:
    using Type = ::pltxt2htm::CodeKotlinNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::lua> {
public:
    using Type = ::pltxt2htm::CodeLuaNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::python> {
public:
    using Type = ::pltxt2htm::CodePythonNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::rust> {
public:
    using Type = ::pltxt2htm::CodeRustNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::sql> {
public:
    using Type = ::pltxt2htm::CodeSqlNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::toml> {
public:
    using Type = ::pltxt2htm::CodeTomlNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::typescript> {
public:
    using Type = ::pltxt2htm::CodeTypeScriptNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::xml> {
public:
    using Type = ::pltxt2htm::CodeXmlNodeKind;
};

template<>
class CodeNodeKindFor<::pltxt2htm::CodeLanguage::yaml> {
public:
    using Type = ::pltxt2htm::CodeYamlNodeKind;
};

template<::pltxt2htm::CodeLanguage language>
using CodeNodeKind = typename ::pltxt2htm::CodeNodeKindFor<language>::Type;

class CodeNode {
    ::std::size_t begin{};
    ::std::size_t end{};
    ::std::size_t metadata{};
    unsigned kind{};

    template<::pltxt2htm::Contracts>
    friend class CodeAst;

    constexpr CodeNode(::std::size_t const begin_value, ::std::size_t const end_value,
                       ::std::size_t const metadata_value, unsigned const kind_value) noexcept
        : begin(begin_value),
          end(end_value),
          metadata(metadata_value),
          kind(kind_value) {
    }

public:
    [[nodiscard]]
    constexpr auto get_begin(this CodeNode const& self) noexcept -> ::std::size_t {
        return self.begin;
    }

    [[nodiscard]]
    constexpr auto get_end(this CodeNode const& self) noexcept -> ::std::size_t {
        return self.end;
    }

    constexpr auto operator==(this CodeNode const&, CodeNode const&) noexcept -> bool = default;
};

} // namespace pltxt2htm
