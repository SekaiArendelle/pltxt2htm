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
#include <exception/exception.hh>
#include "basic.hh"

namespace pltxt2htm {

/**
 * @brief Color formatting node for Physics-Lab text
 * @details Represents text with color formatting using <color=value> syntax
 * @example Examples of valid color syntax:
 *          - <color=red>Red text</color>
 *          - <Color=#FF0000>Hex color</Color>
 *          - <COLOR=blue>Blue text</COLOR>
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
     * @brief Get the color of this anchor node
     * @param[in] self This A instance
     * @return The color string of this anchor
     * @retval fast_io::u8string const& Const reference to the color string
     * @retval fast_io::u8string& Reference to the color string (modifiable)
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
    ::fast_io::u8string color_; ///< The color of the link text

public:
    constexpr A() noexcept = delete;

    /**
     * @brief Construct an anchor node with text content
     * @param[in] text The text content of the anchor
     * @return New A instance
     * @retval A New anchor node instance with blue color
     */
    constexpr A(::pltxt2htm::Ast&& text) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::pl_a, ::std::move(text)},
          color_{u8"#0000AA"} {
    }

    constexpr A(::pltxt2htm::A const&) noexcept = delete;

    constexpr A(::pltxt2htm::A&&) noexcept = default;

    constexpr ~A() noexcept = default;

    constexpr ::pltxt2htm::A& operator=(::pltxt2htm::A const&) noexcept = delete;

    /**
     * @brief move assignment operator
     * @param[in] other The A instance to move from
     * @return Reference to this A instance
     * @retval A& Reference to this anchor node instance
     * @note assign to a rvalue is not allowed
     */
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
 * @details Represents a reference to an experiment using <experiment=id> syntax
 * @example Examples of valid experiment syntax:
 *          - <experiment=12345>Experiment 12345</experiment>
 *          - <Experiment=642cf37a494746375aae306a>My Experiment</Experiment>
 *          - <EXPERIMENT=abc123>Test Experiment</EXPERIMENT>
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
    /**
     * @brief Get the ID of this discussion node
     * @param[in] self This Discussion instance
     * @return The ID string of this discussion
     * @retval fast_io::u8string const& Const reference to the ID string
     * @retval fast_io::u8string& Reference to the ID string (modifiable)
     */
    /**
     * @brief Get the ID of this user node
     * @param[in] self This User instance
     * @return The ID string of this user
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
 * @details Represents a reference to a discussion using <discussion=id> syntax
 * @example Examples of valid discussion syntax:
 *          - <discussion=12345>Discussion 12345</discussion>
 *          - <Discussion=642cf37a494746375aae306a>My Discussion</Discussion>
 *          - <DISCUSSION=abc123>Test Discussion</DISCUSSION>
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
 * @details Represents a reference to a user using <user=id> syntax
 * @example Examples of valid user syntax:
 *          - <user=12345>User 12345</user>
 *          - <User=642cf37a494746375aae306a>John Doe</User>
 *          - <USER=abc123>Test User</USER>
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

/**
 * @brief Font size node for Physics-Lab
 * @details Represents text with specific font size using <size=value> syntax
 * @example Examples of valid size syntax:
 *          - <size=12>12pt text</size>
 *          - <Size=16>Larger text</Size>
 *          - <SIZE=24>Large text</SIZE>
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
 * @details Represents italic text using <i> syntax
 * @example <i>This text will be italic</i>
 * @note This is equivalent to Markdown single emphasis (*text*) and HTML <em>
 */
class I : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr I() noexcept = delete;

    /**
     * @brief Construct an italic node with text content
     * @param[in] subast The text content to be displayed in italic
     * @return New I instance
     * @retval I New italic node instance
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
 * @details Represents bold text using <b> syntax
 * @example <b>This text will be bold</b>
 * @note This is equivalent to Markdown double emphasis (**text**) and HTML <strong>
 */
class B : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr B() noexcept = delete;

    /**
     * @brief Construct a bold node with text content
     * @param[in] subast The text content to be displayed in bold
     * @return New B instance
     * @retval B New bold node instance
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

} // namespace pltxt2htm
