/**
 * @file physics_lab_node.hh
 * @brief Physics-Lab (Quantum-Physics) specific AST node definitions
 * @details Defines AST nodes for Physics-Lab specific tags including color,
 *          experiments, discussions, users, and size formatting
 * @note These nodes represent the unique features of Physics-Lab text format
 *       that go beyond standard HTML/Markdown syntax
 */

#pragma once

#include <utility>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "basic.hh"

namespace pltxt2htm {

/**
 * @brief Color formatting node for Physics-Lab text
 * @details Represents text with color formatting using &lt;color=value&gt; syntax.
 * @par Examples
 * - &lt;color=red&gt;Red text&lt;/color&gt;
 * - &lt;Color=&#35;FF0000&gt;Hex color&lt;/Color&gt;
 * - &lt;COLOR=blue&gt;Blue text&lt;/COLOR&gt;
 * @note Color values can be CSS color names or hex values
 */
class Color : public ::pltxt2htm::details::PairedTagBase {
    ::fast_io::u8string color_;

public:
    constexpr Color() noexcept = delete;

    constexpr Color(::pltxt2htm::Ast&& text, ::fast_io::u8string&& color) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::pl_color, ::std::move(text)},
          color_(::std::move(color)) {
    }

    constexpr Color(::pltxt2htm::Color const&) noexcept = delete;

    constexpr Color(::pltxt2htm::Color&&) noexcept = default;

    constexpr ~Color() noexcept = default;

    constexpr ::pltxt2htm::Color& operator=(::pltxt2htm::Color const&) noexcept = delete;

    constexpr ::pltxt2htm::Color& operator=(::pltxt2htm::Color&&) noexcept = default;

    /**
     * @brief Get the color of this color node
     * @param[in] self This Color instance
     * @return The color string of this node
     * @retval fast_io::u8string const& Const reference to the color string
     * @retval fast_io::u8string& Reference to the color string (modifiable)
     * @note Color values can be CSS color names (e.g., "red") or hex values (e.g., "#FF0000")
     */
    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_color(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.color_);
    }
};

/**
 * @brief Anchor (link) node for Physics-Lab text
 * @details Represents a hyperlink using <a> syntax with default blue color
 */
class A : public ::pltxt2htm::details::PairedTagBase {
    static constexpr ::fast_io::u8string_view color_{u8"#0000AA"}; ///< The color of the link text

public:
    constexpr A() noexcept = delete;

    /**
     * @brief Construct an anchor node with text content
     * @param[in] text The text content of the anchor
     */
    constexpr A(::pltxt2htm::Ast&& text) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::pl_a, ::std::move(text)} {
    }

    constexpr A(::pltxt2htm::A const&) noexcept = delete;

    constexpr A(::pltxt2htm::A&&) noexcept = default;

    constexpr ~A() noexcept = default;

    constexpr ::pltxt2htm::A& operator=(::pltxt2htm::A const&) noexcept = delete;

    constexpr ::pltxt2htm::A& operator=(::pltxt2htm::A&&) noexcept = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_color(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.color_);
    }
};

/**
 * @brief Experiment reference node for Physics-Lab
 * @details Represents a reference to an experiment using &lt;experiment=id&gt; syntax.
 * @par Examples
 * - &lt;experiment=12345&gt;Experiment 12345&lt;/experiment&gt;
 * - &lt;Experiment=642cf37a494746375aae306a&gt;My Experiment&lt;/Experiment&gt;
 * - &lt;EXPERIMENT=abc123&gt;Test Experiment&lt;/EXPERIMENT&gt;
 * @note The experiment ID is used to generate links to experiment pages
 */
class Experiment : public ::pltxt2htm::details::PairedTagBase {
    ::fast_io::u8string id_;

public:
    constexpr Experiment() noexcept = delete;

    constexpr Experiment(::pltxt2htm::Ast&& subast, ::fast_io::u8string&& id) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::pl_experiment, ::std::move(subast)},
          id_(::std::move(id)) {
    }

    constexpr Experiment(::pltxt2htm::Experiment const&) noexcept = delete;

    constexpr Experiment(::pltxt2htm::Experiment&&) noexcept = default;

    constexpr ~Experiment() noexcept = default;

    constexpr ::pltxt2htm::Experiment& operator=(::pltxt2htm::Experiment const&) noexcept = delete;

    constexpr ::pltxt2htm::Experiment& operator=(::pltxt2htm::Experiment&&) noexcept = default;

    /**
     * @brief Get the ID of this experiment node
     * @param[in] self This Experiment instance
     * @return The ID string of this experiment
     * @retval fast_io::u8string const& Const reference to the ID string
     * @retval fast_io::u8string& Reference to the ID string (modifiable)
     */
    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

/**
 * @brief Discussion reference node for Physics-Lab
 * @details Represents a reference to a discussion using &lt;discussion=id&gt; syntax.
 * @par Examples
 * - &lt;discussion=12345&gt;Discussion 12345&lt;/discussion&gt;
 * - &lt;Discussion=642cf37a494746375aae306a&gt;My Discussion&lt;/Discussion&gt;
 * - &lt;DISCUSSION=abc123&gt;Test Discussion&lt;/DISCUSSION&gt;
 * @note The discussion ID is used to generate links to discussion pages
 */
class Discussion : public ::pltxt2htm::details::PairedTagBase {
    ::fast_io::u8string id_{};

public:
    constexpr Discussion() noexcept = delete;

    constexpr Discussion(::pltxt2htm::Ast&& subast, ::fast_io::u8string&& id) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::pl_discussion, ::std::move(subast)},
          id_(::std::move(id)) {
    }

    constexpr Discussion(::pltxt2htm::Discussion const&) noexcept = delete;

    constexpr Discussion(::pltxt2htm::Discussion&&) noexcept = default;

    constexpr ~Discussion() noexcept = default;

    constexpr ::pltxt2htm::Discussion& operator=(::pltxt2htm::Discussion const&) noexcept = delete;

    constexpr ::pltxt2htm::Discussion& operator=(::pltxt2htm::Discussion&&) noexcept = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

/**
 * @brief User reference node for Physics-Lab
 * @details Represents a reference to a user using &lt;user=id&gt; syntax.
 * @par Examples
 * - &lt;user=12345&gt;User 12345&lt;/user&gt;
 * - &lt;User=642cf37a494746375aae306a&gt;John Doe&lt;/User&gt;
 * - &lt;USER=abc123&gt;Test User&lt;/USER&gt;
 * @note The user ID is used to generate links to user profiles
 */
class User : public ::pltxt2htm::details::PairedTagBase {
    ::fast_io::u8string id_{};

public:
    constexpr User() noexcept = delete;

    constexpr User(::pltxt2htm::Ast&& subast, ::fast_io::u8string&& id) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::pl_user, ::std::move(subast)},
          id_(::std::move(id)) {
    }

    constexpr User(::pltxt2htm::User const&) noexcept = delete;

    constexpr User(::pltxt2htm::User&&) noexcept = default;

    constexpr ~User() noexcept = default;

    constexpr ::pltxt2htm::User& operator=(::pltxt2htm::User const&) noexcept = delete;

    constexpr ::pltxt2htm::User& operator=(::pltxt2htm::User&&) noexcept = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

class External : public ::pltxt2htm::details::PairedTagBase {
    ::pltxt2htm::MdUrl url_{};

public:
    constexpr External() noexcept = delete;

    constexpr External(::pltxt2htm::Ast&& subast, ::pltxt2htm::MdUrl&& url) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::pl_external, ::std::move(subast)},
          url_(::std::move(url)) {
    }

    constexpr External(::pltxt2htm::External const&) noexcept = delete;

    constexpr External(::pltxt2htm::External&&) noexcept = default;

    constexpr ~External() noexcept = default;

    constexpr ::pltxt2htm::External& operator=(::pltxt2htm::External const&) noexcept = delete;

    constexpr ::pltxt2htm::External& operator=(::pltxt2htm::External&&) noexcept = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_url(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.url_);
    }
};

/**
 * @brief Font size node for Physics-Lab
 * @details Represents text with specific font size using &lt;size=value&gt; syntax.
 * @par Examples
 * - &lt;size=12&gt;12pt text&lt;/size&gt;
 * - &lt;Size=16&gt;Larger text&lt;/Size&gt;
 * - &lt;SIZE=24&gt;Large text&lt;/SIZE&gt;
 * @note Size values are numeric and represent the font size in points
 */
class Size : public ::pltxt2htm::details::PairedTagBase {
    ::std::size_t id_{};

public:
    constexpr Size() noexcept = delete;

    constexpr Size(::pltxt2htm::Ast&& subast, ::std::size_t id) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::pl_size, ::std::move(subast)},
          id_(::std::move(id)) {
    }

    constexpr Size(::pltxt2htm::Size const&) noexcept = delete;

    constexpr Size(::pltxt2htm::Size&&) noexcept = default;

    constexpr ~Size() noexcept = default;

    constexpr ::pltxt2htm::Size& operator=(::pltxt2htm::Size const&) noexcept = delete;

    constexpr ::pltxt2htm::Size& operator=(::pltxt2htm::Size&&) noexcept = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

/**
 * @brief Italic formatting node for Physics-Lab
 * @details Represents italic text using &lt;i&gt; syntax.
 * @par Example
 * &lt;i&gt;This text will be italic&lt;/i&gt;
 * @note This is equivalent to Markdown single emphasis (*text*) and HTML &lt;em&gt;.
 */
class I : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr I() noexcept = delete;

    /**
     * @brief Construct an italic node with text content
     * @param[in] subast The text content to be displayed in italic
     */
    constexpr I(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::pl_i, ::std::move(subast)} {
    }

    constexpr I(::pltxt2htm::I const&) noexcept = delete;

    constexpr I(::pltxt2htm::I&&) noexcept = default;

    constexpr ~I() noexcept = default;

    constexpr ::pltxt2htm::I& operator=(::pltxt2htm::I const&) noexcept = delete;

    constexpr ::pltxt2htm::I& operator=(::pltxt2htm::I&&) noexcept = default;
};

/**
 * @brief Bold formatting node for Physics-Lab
 * @details Represents bold text using &lt;b&gt; syntax.
 * @par Example
 * &lt;b&gt;This text will be bold&lt;/b&gt;
 * @note This is equivalent to Markdown double emphasis (**text**) and HTML &lt;strong&gt;.
 */
class B : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr B() noexcept = delete;

    /**
     * @brief Construct a bold node with text content
     * @param[in] subast The text content to be displayed in bold
     */
    constexpr B(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::pl_b, ::std::move(subast)} {
    }

    constexpr B(::pltxt2htm::B const&) noexcept = delete;

    constexpr B(::pltxt2htm::B&&) noexcept = default;

    constexpr ~B() noexcept = default;

    constexpr ::pltxt2htm::B& operator=(::pltxt2htm::B const&) noexcept = delete;

    constexpr ::pltxt2htm::B& operator=(::pltxt2htm::B&&) noexcept = default;
};

class PlMacroProject : public ::pltxt2htm::PlTxtNode {
public:
    constexpr PlMacroProject() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::pl_macro_project} {
    }

    constexpr PlMacroProject(::pltxt2htm::PlMacroProject const&) noexcept = delete;

    constexpr PlMacroProject(::pltxt2htm::PlMacroProject&&) noexcept = default;

    constexpr ~PlMacroProject() noexcept = default;

    constexpr ::pltxt2htm::PlMacroProject& operator=(::pltxt2htm::PlMacroProject const&) noexcept = delete;

    constexpr ::pltxt2htm::PlMacroProject& operator=(::pltxt2htm::PlMacroProject&&) noexcept = default;
};

class PlMacroVisitor : public ::pltxt2htm::PlTxtNode {
public:
    constexpr PlMacroVisitor() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::pl_macro_visitor} {
    }

    constexpr PlMacroVisitor(::pltxt2htm::PlMacroVisitor const&) noexcept = delete;

    constexpr PlMacroVisitor(::pltxt2htm::PlMacroVisitor&&) noexcept = default;

    constexpr ~PlMacroVisitor() noexcept = default;

    constexpr ::pltxt2htm::PlMacroVisitor& operator=(::pltxt2htm::PlMacroVisitor const&) noexcept = delete;

    constexpr ::pltxt2htm::PlMacroVisitor& operator=(::pltxt2htm::PlMacroVisitor&&) noexcept = default;
};

class PlMacroAuthor : public ::pltxt2htm::PlTxtNode {
public:
    constexpr PlMacroAuthor() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::pl_macro_author} {
    }

    constexpr PlMacroAuthor(::pltxt2htm::PlMacroAuthor const&) noexcept = delete;

    constexpr PlMacroAuthor(::pltxt2htm::PlMacroAuthor&&) noexcept = default;

    constexpr ~PlMacroAuthor() noexcept = default;

    constexpr ::pltxt2htm::PlMacroAuthor& operator=(::pltxt2htm::PlMacroAuthor const&) noexcept = delete;

    constexpr ::pltxt2htm::PlMacroAuthor& operator=(::pltxt2htm::PlMacroAuthor&&) noexcept = default;
};

class PlMacroCoauthors : public ::pltxt2htm::PlTxtNode {
public:
    constexpr PlMacroCoauthors() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::pl_macro_coauthors} {
    }

    constexpr PlMacroCoauthors(::pltxt2htm::PlMacroCoauthors const&) noexcept = delete;

    constexpr PlMacroCoauthors(::pltxt2htm::PlMacroCoauthors&&) noexcept = default;

    constexpr ~PlMacroCoauthors() noexcept = default;

    constexpr ::pltxt2htm::PlMacroCoauthors& operator=(::pltxt2htm::PlMacroCoauthors const&) noexcept = delete;

    constexpr ::pltxt2htm::PlMacroCoauthors& operator=(::pltxt2htm::PlMacroCoauthors&&) noexcept = default;
};

} // namespace pltxt2htm
