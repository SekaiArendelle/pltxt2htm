/**
 * @file physics_lab_node_decl.hh
 * @brief Physics-Lab-specific AST node declarations for pltxt2htm
 * @details Defines Physics-Lab anchors, references, interaction tags, external links, and template macros.
 */

#pragma once
#include <utility>

#include "../../container/string.hh"
#include "../../container/optional.hh"
#include "../../container/string_view.hh"
#include "../../details/literal_string.hh"
#include "../../details/utils.hh"
#include "ast_decl.hh"
#include "url_node_decl.hh"
#include "../node_kind.hh"
#include "../../contracts.hh"

namespace pltxt2htm {

/**
 * @brief A Physics-Lab entity identifier: the canonical spelling of a BSON ObjectId.
 * @details Physics-Lab keeps experiments, discussions, and users in MongoDB, so an
 *          &lt;experiment=&gt;, &lt;discussion=&gt;, or &lt;user=&gt; payload is an ObjectId. The
 *          12-byte value carries no check digits, which makes "exactly 24 hexadecimal
 *          digits" the whole grammar: nothing else is a valid identifier, and every
 *          24-digit string is one. Sources may spell the digits in uppercase; they are
 *          folded to the lowercase form BSON emits, so equal identifiers compare equal
 *          no matter which case the source used.
 */
template<::pltxt2htm::Contracts ndebug>
class PlObjectId {
    ::pltxt2htm::container::U8String text;

    /**
     * @brief Constructs an identifier from an already-canonical spelling.
     * @details Private and trust-based. Unlike ::pltxt2htm::Url, whose spelling has no checkable
     *          grammar, an ObjectId payload can be validated, so try_make stays the only way to
     *          produce a value outside the class and the invariant needs no second guarantee.
     * @param text_ Exactly 24 lowercase hexadecimal digits.
     */
    constexpr explicit PlObjectId(::pltxt2htm::container::U8String&& text_) noexcept
        : text(::std::move(text_)) {
    }

public:
    /// Number of hexadecimal digits in the canonical ObjectId spelling.
    static constexpr ::std::size_t hex_digits{24};

    constexpr PlObjectId(PlObjectId const&) noexcept = default;
    constexpr PlObjectId(PlObjectId&&) noexcept = default;
    constexpr ~PlObjectId() noexcept = default;
    constexpr auto operator=(this PlObjectId&, PlObjectId const&) noexcept -> PlObjectId& = default;
    constexpr auto operator=(this PlObjectId&, PlObjectId&&) noexcept -> PlObjectId& = default;

    [[nodiscard]]
    constexpr auto operator==(this PlObjectId const&, PlObjectId const&) noexcept -> bool = default;

    /**
     * @brief Builds an identifier from a source payload, or fails when it is not one.
     * @details Uppercase digits are folded to the lowercase spelling BSON emits.
     * @param input Candidate payload.
     * @return The canonical identifier, or nullopt when the length or a digit is wrong.
     */
    [[nodiscard]]
    static constexpr auto try_make(::pltxt2htm::container::U8StringView input) noexcept
        -> ::pltxt2htm::container::Optional<PlObjectId> {
        if (input.size() != hex_digits) {
            return ::pltxt2htm::container::nullopt;
        }
        ::pltxt2htm::container::U8String canonical{};
        canonical.template reserve<ndebug>(hex_digits);
        for (::std::size_t index{}; index < hex_digits; ++index) {
            char8_t const digit{input.template index<ndebug>(index)};
            if (::pltxt2htm::details::is_ascii_hexdigit(digit) == false) {
                return ::pltxt2htm::container::nullopt;
            }
            // ASCII case bit 0x20 is already set for '0'-'9', so OR-ing it folds 'A'-'F'
            // to 'a'-'f' without changing the digits or an already-lowercase spelling.
            canonical.template push_back<ndebug>(static_cast<char8_t>(digit | char8_t{0x20}));
        }
        return PlObjectId{::std::move(canonical)};
    }
    [[nodiscard]]
    constexpr auto as_string(this PlObjectId const& self) noexcept -> ::pltxt2htm::container::U8String const& {
        return self.text;
    }
};

/**
 * @brief Physics-Lab anchor tag node
 * @details Represents &lt;a&gt;...&lt;/a&gt; with a fixed blue color used for styled links.
 */
template<::pltxt2htm::Contracts ndebug>
class PlA {
    ::pltxt2htm::Ast<ndebug> subast;
    static constexpr ::pltxt2htm::details::U8LiteralString<7> color{u8"#0000AA"};

public:
    constexpr PlA(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr PlA(::pltxt2htm::PlA<ndebug> const&) noexcept;
    constexpr PlA(::pltxt2htm::PlA<ndebug>&&) noexcept;
    constexpr ~PlA() noexcept = default;
    constexpr auto operator=(::pltxt2htm::PlA<ndebug> const&) noexcept -> ::pltxt2htm::PlA<ndebug>& = default;
    constexpr auto operator=(this PlA<ndebug>& self, ::pltxt2htm::PlA<ndebug>&&) noexcept -> ::pltxt2htm::PlA<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlA const&, PlA const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    static constexpr auto const& get_color_literal() noexcept {
        return color;
    }
};

/**
 * @brief Physics-Lab experiment reference tag node
 * @details Represents &lt;experiment=id&gt;...&lt;/experiment&gt; with an experiment ObjectId.
 */
template<::pltxt2htm::Contracts ndebug>
class PlExperiment {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::PlObjectId<ndebug> id;

public:
    constexpr PlExperiment(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::PlObjectId<ndebug>&& id_) noexcept;
    constexpr PlExperiment(::pltxt2htm::PlExperiment<ndebug> const&) noexcept;
    constexpr PlExperiment(::pltxt2htm::PlExperiment<ndebug>&&) noexcept;
    constexpr ~PlExperiment() noexcept = default;
    constexpr auto operator=(::pltxt2htm::PlExperiment<ndebug> const&) noexcept
        -> ::pltxt2htm::PlExperiment<ndebug>& = default;
    constexpr auto operator=(this PlExperiment<ndebug>& self, ::pltxt2htm::PlExperiment<ndebug>&&) noexcept
        -> ::pltxt2htm::PlExperiment<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlExperiment const&, PlExperiment const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_id(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.id);
    }
};

/**
 * @brief Physics-Lab discussion reference tag node
 * @details Represents &lt;discussion=id&gt;...&lt;/discussion&gt; with a discussion ObjectId.
 */
template<::pltxt2htm::Contracts ndebug>
class PlDiscussion {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::PlObjectId<ndebug> id;

public:
    constexpr PlDiscussion(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::PlObjectId<ndebug>&& id_) noexcept;
    constexpr PlDiscussion(::pltxt2htm::PlDiscussion<ndebug> const&) noexcept;
    constexpr PlDiscussion(::pltxt2htm::PlDiscussion<ndebug>&&) noexcept;
    constexpr ~PlDiscussion() noexcept = default;
    constexpr auto operator=(::pltxt2htm::PlDiscussion<ndebug> const&) noexcept
        -> ::pltxt2htm::PlDiscussion<ndebug>& = default;
    constexpr auto operator=(this PlDiscussion<ndebug>& self, ::pltxt2htm::PlDiscussion<ndebug>&&) noexcept
        -> ::pltxt2htm::PlDiscussion<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlDiscussion const&, PlDiscussion const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_id(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.id);
    }
};

/**
 * @brief Physics-Lab experiments list-entry tag node
 * @details Represents &lt;experiments=params&gt;...&lt;/experiments&gt; with a slash-separated
 *          key-value parameter string (e.g. `UserID/123/UserName/...`) and sub-AST.
 */
template<::pltxt2htm::Contracts ndebug>
class PlExperiments {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::container::U8String value;

public:
    constexpr PlExperiments(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::container::U8String&& value_) noexcept;
    constexpr PlExperiments(::pltxt2htm::PlExperiments<ndebug> const&) noexcept;
    constexpr PlExperiments(::pltxt2htm::PlExperiments<ndebug>&&) noexcept;
    constexpr ~PlExperiments() noexcept = default;
    constexpr auto operator=(::pltxt2htm::PlExperiments<ndebug> const&) noexcept
        -> ::pltxt2htm::PlExperiments<ndebug>& = default;
    constexpr auto operator=(this PlExperiments<ndebug>& self, ::pltxt2htm::PlExperiments<ndebug>&&) noexcept
        -> ::pltxt2htm::PlExperiments<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlExperiments const&, PlExperiments const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_value(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.value);
    }
};

/**
 * @brief Physics-Lab discussions list-entry tag node
 * @details Represents &lt;discussions=params&gt;...&lt;/discussions&gt; with a slash-separated
 *          key-value parameter string (e.g. `UserID/123/UserName/...`) and sub-AST.
 */
template<::pltxt2htm::Contracts ndebug>
class PlDiscussions {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::container::U8String value;

public:
    constexpr PlDiscussions(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::container::U8String&& value_) noexcept;
    constexpr PlDiscussions(::pltxt2htm::PlDiscussions<ndebug> const&) noexcept;
    constexpr PlDiscussions(::pltxt2htm::PlDiscussions<ndebug>&&) noexcept;
    constexpr ~PlDiscussions() noexcept = default;
    constexpr auto operator=(::pltxt2htm::PlDiscussions<ndebug> const&) noexcept
        -> ::pltxt2htm::PlDiscussions<ndebug>& = default;
    constexpr auto operator=(this PlDiscussions<ndebug>& self, ::pltxt2htm::PlDiscussions<ndebug>&&) noexcept
        -> ::pltxt2htm::PlDiscussions<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlDiscussions const&, PlDiscussions const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_value(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.value);
    }
};

/**
 * @brief Physics-Lab user reference tag node
 * @details Represents &lt;user=id&gt;...&lt;/user&gt; with a user ObjectId.
 */
template<::pltxt2htm::Contracts ndebug>
class PlUser {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::PlObjectId<ndebug> id;

public:
    constexpr PlUser(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::PlObjectId<ndebug>&& id_) noexcept;
    constexpr PlUser(::pltxt2htm::PlUser<ndebug> const&) noexcept;
    constexpr PlUser(::pltxt2htm::PlUser<ndebug>&&) noexcept;
    constexpr ~PlUser() noexcept = default;
    constexpr auto operator=(::pltxt2htm::PlUser<ndebug> const&) noexcept -> ::pltxt2htm::PlUser<ndebug>& = default;
    constexpr auto operator=(this PlUser<ndebug>& self, ::pltxt2htm::PlUser<ndebug>&&) noexcept
        -> ::pltxt2htm::PlUser<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlUser const&, PlUser const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_id(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.id);
    }
};

/**
 * @brief Physics-Lab trigger tag node
 * @details Represents &lt;trigger=value&gt;...&lt;/trigger&gt; with a string value.
 */
template<::pltxt2htm::Contracts ndebug>
class PlTrigger {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::container::U8String value;

public:
    constexpr PlTrigger(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::container::U8String&& value_) noexcept;
    constexpr PlTrigger(::pltxt2htm::PlTrigger<ndebug> const&) noexcept;
    constexpr PlTrigger(::pltxt2htm::PlTrigger<ndebug>&&) noexcept;
    constexpr ~PlTrigger() noexcept = default;
    constexpr auto operator=(::pltxt2htm::PlTrigger<ndebug> const&) noexcept
        -> ::pltxt2htm::PlTrigger<ndebug>& = default;
    constexpr auto operator=(this PlTrigger<ndebug>& self, ::pltxt2htm::PlTrigger<ndebug>&&) noexcept
        -> ::pltxt2htm::PlTrigger<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlTrigger const&, PlTrigger const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_value(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.value);
    }
};

/**
 * @brief Physics-Lab internal tag node
 * @details Represents &lt;internal=value&gt;...&lt;/internal&gt; with a string value, rendered verbatim.
 */
template<::pltxt2htm::Contracts ndebug>
class PlInternal {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::container::U8String value;

public:
    constexpr PlInternal(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::container::U8String&& value_) noexcept;
    constexpr PlInternal(::pltxt2htm::PlInternal<ndebug> const&) noexcept;
    constexpr PlInternal(::pltxt2htm::PlInternal<ndebug>&&) noexcept;
    constexpr ~PlInternal() noexcept = default;
    constexpr auto operator=(::pltxt2htm::PlInternal<ndebug> const&) noexcept
        -> ::pltxt2htm::PlInternal<ndebug>& = default;
    constexpr auto operator=(this PlInternal<ndebug>& self, ::pltxt2htm::PlInternal<ndebug>&&) noexcept
        -> ::pltxt2htm::PlInternal<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlInternal const&, PlInternal const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_value(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.value);
    }
};

/**
 * @brief Physics-Lab external link tag node
 * @details Represents &lt;external=url&gt;...&lt;/external&gt; with a URL.
 */
template<::pltxt2htm::Contracts ndebug>
class PlExternal {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::Url url;

public:
    constexpr PlExternal(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::Url&& url_) noexcept;
    constexpr PlExternal(::pltxt2htm::PlExternal<ndebug> const&) noexcept;
    constexpr PlExternal(::pltxt2htm::PlExternal<ndebug>&&) noexcept;
    constexpr ~PlExternal() noexcept = default;
    constexpr auto operator=(::pltxt2htm::PlExternal<ndebug> const&) noexcept
        -> ::pltxt2htm::PlExternal<ndebug>& = default;
    constexpr auto operator=(this PlExternal<ndebug>& self, ::pltxt2htm::PlExternal<ndebug>&&) noexcept
        -> ::pltxt2htm::PlExternal<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlExternal const&, PlExternal const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_url(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.url);
    }
};

/**
 * @brief Physics-Lab {Project} macro node
 * @details Represents the {Project} template placeholder.
 */
class PlMacroProject {
public:
    [[nodiscard]]
    constexpr auto operator==(this PlMacroProject const&, PlMacroProject const&) noexcept -> bool = default;
};

/**
 * @brief Physics-Lab {Visitor} macro node
 * @details Represents the {Visitor} template placeholder.
 */
class PlMacroVisitor {
public:
    [[nodiscard]]
    constexpr auto operator==(this PlMacroVisitor const&, PlMacroVisitor const&) noexcept -> bool = default;
};

/**
 * @brief Physics-Lab {Author} macro node
 * @details Represents the {Author} template placeholder.
 */
class PlMacroAuthor {
public:
    [[nodiscard]]
    constexpr auto operator==(this PlMacroAuthor const&, PlMacroAuthor const&) noexcept -> bool = default;
};

/**
 * @brief Physics-Lab {CoAuthors} macro node
 * @details Represents the {CoAuthors} template placeholder.
 */
class PlMacroCoauthors {
public:
    [[nodiscard]]
    constexpr auto operator==(this PlMacroCoauthors const&, PlMacroCoauthors const&) noexcept -> bool = default;
};

} // namespace pltxt2htm
