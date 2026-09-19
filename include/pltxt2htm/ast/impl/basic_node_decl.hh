/**
 * @file basic_node_decl.hh
 * @brief Basic AST node declarations for pltxt2htm
 * @details Defines fundamental node types: character nodes, text container, and URL.
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <ranges>
#include <type_traits>
#include <utility>
#include "../../container/string.hh"
#include "../../container/optional.hh"
#include "../../details/inplace_string.hh"
#include "ast_decl.hh"

namespace pltxt2htm {

/**
 * @brief Line break (newline) node
 * @details Represents a line break character in the source text.
 */
class LineBreak {
public:
    [[nodiscard]]
    constexpr auto operator==(this LineBreak const&, LineBreak const&) noexcept -> bool = default;
};

/**
 * @brief ::pltxt2htm::Space character node
 * @details Represents a whitespace character (space, tab, etc.).
 */
class Space {
public:
    [[nodiscard]]
    constexpr auto operator==(this Space const&, Space const&) noexcept -> bool = default;
};

/**
 * @brief Less-than sign node
 * @details Represents the '<' character, which may need escaping in HTML.
 */
class LessThan {
public:
    [[nodiscard]]
    constexpr auto operator==(this LessThan const&, LessThan const&) noexcept -> bool = default;
};

/**
 * @brief Greater-than sign node
 * @details Represents the '>' character.
 */
class GreaterThan {
public:
    [[nodiscard]]
    constexpr auto operator==(this GreaterThan const&, GreaterThan const&) noexcept -> bool = default;
};

/**
 * @brief ::pltxt2htm::Tab character node
 * @details Represents a tab character in the source text.
 */
class Tab {
public:
    [[nodiscard]]
    constexpr auto operator==(this Tab const&, Tab const&) noexcept -> bool = default;
};

/**
 * @brief ::pltxt2htm::Ampersand node
 * @details Represents the semantic '&' character.
 */
class Ampersand {
public:
    [[nodiscard]]
    constexpr auto operator==(this Ampersand const&, Ampersand const&) noexcept -> bool = default;
};

/**
 * @brief Single quotation mark node
 * @details Represents the "'" character.
 */
class SingleQuote {
public:
    [[nodiscard]]
    constexpr auto operator==(this SingleQuote const&, SingleQuote const&) noexcept -> bool = default;
};

/**
 * @brief Double quotation mark node
 * @details Represents the '"' character.
 */
class DoubleQuote {
public:
    [[nodiscard]]
    constexpr auto operator==(this DoubleQuote const&, DoubleQuote const&) noexcept -> bool = default;
};

/**
 * @brief A leaf node containing a run of UTF-8 code units.
 * @details Stores up to eight pointer-sized words of UTF-8 code units inline, plus its size field.
 */
template<::pltxt2htm::Contracts ndebug>
class Text {
    // Keep this node smaller than HtmlSpan without penalizing 32-bit targets.
    static constexpr ::std::size_t storage_capacity{sizeof(void*) * 8};
    using Storage = ::pltxt2htm::details::U8InplaceString<storage_capacity, ndebug>;

    Storage storage;

public:
    using size_type = ::std::size_t;
    using iterator = char8_t*;
    using const_iterator = char8_t const*;

    constexpr explicit Text(char8_t character) noexcept
        : storage{character} {
    }

    template<::std::ranges::input_range R>
        requires (!::std::same_as<::std::remove_cvref_t<R>, Text> &&
                  ::std::same_as<::std::ranges::range_value_t<R>, char8_t> &&
                  ::std::constructible_from<char8_t, ::std::ranges::range_reference_t<R>>)
    constexpr explicit Text(R&& range) noexcept(noexcept(Storage{::std::forward<R>(range)}))
        : storage{::std::forward<R>(range)} {
    }

    constexpr Text(Text const&) = default;
    constexpr Text(Text&&) noexcept = default;
    constexpr auto operator=(this Text&, Text const&) -> Text& = default;
    constexpr auto operator=(this Text&, Text&&) noexcept -> Text& = default;
    constexpr ~Text() noexcept = default;

    [[nodiscard]]
    constexpr auto operator==(this Text const&, Text const&) noexcept -> bool = default;

    [[nodiscard]]
    static constexpr auto capacity() noexcept -> size_type {
        return Storage::capacity();
    }

    [[nodiscard]]
    constexpr auto size(this Text const& self) noexcept -> size_type {
        return self.storage.size();
    }

    [[nodiscard]]
    constexpr auto begin(this Text& self) noexcept -> iterator {
        return self.storage.begin();
    }

    [[nodiscard]]
    constexpr auto begin(this Text const& self) noexcept -> const_iterator {
        return self.storage.begin();
    }

    [[nodiscard]]
    constexpr auto end(this Text& self) noexcept -> iterator {
        return self.storage.end();
    }

    [[nodiscard]]
    constexpr auto end(this Text const& self) noexcept -> const_iterator {
        return self.storage.end();
    }

    [[nodiscard]]
    constexpr auto index(this Text& self, size_type position) noexcept -> char8_t& {
        return self.storage.index(position);
    }

    [[nodiscard]]
    constexpr auto index(this Text const& self, size_type position) noexcept -> char8_t const& {
        return self.storage.index(position);
    }

    [[nodiscard]]
    constexpr auto try_push_back(this Text& self, char8_t character) noexcept -> bool {
        return self.storage.try_push_back(character);
    }

    template<::std::ranges::input_range R>
        requires (::std::same_as<::std::ranges::range_value_t<R>, char8_t> &&
                  ::std::constructible_from<char8_t, ::std::ranges::range_reference_t<R>>)
    constexpr void append_range(this Text& self,
                                R&& range) noexcept(noexcept(self.storage.append_range(::std::forward<R>(range)))) {
        self.storage.append_range(::std::forward<R>(range));
    }
};

/**
 * @brief Invalid UTF-8 character node
 * @details Represents an invalid UTF-8 character encountered during parsing.
 */
class InvalidUtf8 {
public:
    [[nodiscard]]
    constexpr auto operator==(this InvalidUtf8 const&, InvalidUtf8 const&) noexcept -> bool = default;
};

/**
 * @brief ::pltxt2htm::Group<ndebug> container node
 * @details Holds a sub-AST representing text content and inline formatting.
 */
template<::pltxt2htm::Contracts ndebug>
class Group {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    /**
     * @brief Construct a ::pltxt2htm::Group<ndebug> node with a sub-AST.
     * @param subast The sub-AST to be contained.
     */
    constexpr Group(::pltxt2htm::Ast<ndebug>&& subast) noexcept;
    constexpr Group(::pltxt2htm::Group<ndebug> const&) noexcept;
    constexpr Group(::pltxt2htm::Group<ndebug>&&) noexcept;
    constexpr ~Group() noexcept = default;
    constexpr auto operator=(::pltxt2htm::Group<ndebug> const&) noexcept -> ::pltxt2htm::Group<ndebug>& = default;
    constexpr auto operator=(this Group<ndebug>& self, ::pltxt2htm::Group<ndebug>&&) noexcept
        -> ::pltxt2htm::Group<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this Group const&, Group const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown fenced code block
 * @details Contains code content and an optional language identifier.
 */
template<::pltxt2htm::Contracts ndebug>
class CodeFence {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String> lang;

public:
    /**
     * @brief Construct a fenced code block.
     * @param subast The code content as an AST.
     * @param lang Optional language string.
     */
    constexpr explicit CodeFence(::pltxt2htm::Ast<ndebug>&& subast_,
                                 ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>&& lang_) noexcept;
    constexpr CodeFence(::pltxt2htm::CodeFence<ndebug> const&) noexcept;
    constexpr CodeFence(::pltxt2htm::CodeFence<ndebug>&&) noexcept;
    constexpr ~CodeFence() noexcept = default;
    constexpr auto operator=(::pltxt2htm::CodeFence<ndebug> const&) noexcept
        -> ::pltxt2htm::CodeFence<ndebug>& = default;
    constexpr auto operator=(this CodeFence<ndebug>& self, ::pltxt2htm::CodeFence<ndebug>&&) noexcept
        -> ::pltxt2htm::CodeFence<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this CodeFence const& self, CodeFence const& other) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_language(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.lang);
    }
};

/**
 * @brief URL node
 * @details Represents a semantic URL value stored independently of any backend's escaping syntax.
 */
class Url {
    ::pltxt2htm::container::U8String url_str;

public:
    /**
     * @brief Construct a ::pltxt2htm::Url from a semantic URL string.
     * @param url The URL string without HTML attribute escaping.
     */
    constexpr explicit Url(::pltxt2htm::container::U8String&& url) noexcept
        : url_str(::std::move(url)) {
    }

    constexpr Url(Url const&) noexcept = default;
    constexpr Url(Url&&) noexcept = default;
    constexpr ~Url() noexcept = default;
    constexpr auto operator=(Url const&) noexcept -> Url& = default;
    constexpr auto operator=(this Url& self, Url&&) noexcept -> Url& = default;

    [[nodiscard]]
    constexpr auto operator==(this Url const&, Url const&) noexcept -> bool = default;

    [[nodiscard]]
    constexpr auto as_string(this Url const& self) noexcept -> ::pltxt2htm::container::U8String const& {
        return self.url_str;
    }
};

} // namespace pltxt2htm
