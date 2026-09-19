#include <concepts>
#include <cstddef>
#include <type_traits>
#include <pltxt2htm/container/string.hh>
#include <pltxt2htm/container/vector.hh>

#include "precompile.hh"

#include <pltxt2htm/ast/ast.hh>
#include <pltxt2htm/details/parser/try_parse.hh>

namespace {

constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;

consteval auto test_md_escape_result() -> bool {
    auto const trailing = ::pltxt2htm::details::try_parse_md_escape<ndebug>(u8"\\");
    if (trailing.has_value() == false) {
        return false;
    }
    auto const& trailing_result = trailing.template value<ndebug>();
    if (trailing_result.character != u8'\\' ||
        trailing_result.kind != ::pltxt2htm::details::MdEscapeKind::literal_backslash ||
        trailing_result.advance_count != 1) {
        return false;
    }

    auto const invalid = ::pltxt2htm::details::try_parse_md_escape<ndebug>(u8"\\a");
    if (invalid.has_value() == false) {
        return false;
    }
    auto const& invalid_result = invalid.template value<ndebug>();
    if (invalid_result.character != u8'\\' ||
        invalid_result.kind != ::pltxt2htm::details::MdEscapeKind::literal_backslash ||
        invalid_result.advance_count != 1) {
        return false;
    }

    auto const escaped = ::pltxt2htm::details::try_parse_md_escape<ndebug>(u8"\\*");
    if (escaped.has_value() == false) {
        return false;
    }
    auto const& escaped_result = escaped.template value<ndebug>();
    return escaped_result.character == u8'*' &&
           escaped_result.kind == ::pltxt2htm::details::MdEscapeKind::escaped_punctuation &&
           escaped_result.advance_count == 2;
}

static_assert(test_md_escape_result());

} // namespace

int main() {
    using Node = ::pltxt2htm::PlTxtNode<ndebug>;

    static_assert(::std::movable<Node>);
    static_assert(::std::is_nothrow_constructible_v<Node, char8_t>);
    static_assert(::std::is_trivially_copyable_v<::pltxt2htm::details::TryParseMdEscapeResult>);
    static_assert(sizeof(::pltxt2htm::details::TryParseMdEscapeResult) <= sizeof(::std::size_t) * 2);

    ::pltxt2htm::container::Vector<Node> arr{};
    arr.emplace_back(u8'a');
    arr.emplace_back(::pltxt2htm::UnityColor<::pltxt2htm::Contracts::quick_enforce>{
        ::pltxt2htm::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::pltxt2htm::container::U8String{u8"red"}});
    arr.emplace_back(::pltxt2htm::PlExperiment<::pltxt2htm::Contracts::quick_enforce>{
        ::pltxt2htm::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::pltxt2htm::container::U8String{u8"123"}});
    arr.emplace_back(::pltxt2htm::PlDiscussion<::pltxt2htm::Contracts::quick_enforce>{
        ::pltxt2htm::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::pltxt2htm::container::U8String{u8"123"}});

    auto const& text = arr.template index<ndebug>(0);
    pltxt2htm_test_assert_true(text.get_node_kind() == ::pltxt2htm::NodeKind::text);
    pltxt2htm_test_assert_true(text.as_text().size() == 1 && text.as_text().index(0) == u8'a');
    pltxt2htm_test_assert_true(arr.template index<ndebug>(1).get_node_kind() == ::pltxt2htm::NodeKind::unity_color);
    pltxt2htm_test_assert_true(arr.template index<ndebug>(2).get_node_kind() == ::pltxt2htm::NodeKind::pl_experiment);
    pltxt2htm_test_assert_true(arr.template index<ndebug>(3).get_node_kind() == ::pltxt2htm::NodeKind::pl_discussion);

    return 0;
}
