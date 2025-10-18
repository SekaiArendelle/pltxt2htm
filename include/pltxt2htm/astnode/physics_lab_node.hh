#pragma once

/**
 * @file plext.hh
 * @brief Quantum-Physics's text extensions
 */

#include <utility>
#include <fast_io/fast_io_dsal/string.h>
#include <exception/exception.hh>
#include "basic.hh"

namespace pltxt2htm {

/**
 * @brief Color node
 * @example - <color=xxx>...</color>
 *          - <Color=xxx>...</Color>
 *          - <COLOR=xxx>...</COLOR>
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

    [[nodiscard]]
    constexpr auto&& get_color(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.color_);
    }
};

class A : public ::pltxt2htm::details::PairedTagBase {
    ::fast_io::u8string color_;

public:
    constexpr A() noexcept = delete;

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
     * @note assign to a rvalue is not allowed
     */
    constexpr ::pltxt2htm::A& operator=(::pltxt2htm::A&&) noexcept = default;

    [[nodiscard]]
    constexpr auto&& get_color(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.color_);
    }
};

/**
 * @brief Experiment node
 * @example - <Experiment=xxx>...</Experiment>
 *          - <experiment=xxx>...</experiment>
 *          - <EXPERIMENT=xxx>...</EXPERIMENT>
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

    [[nodiscard]]
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

/**
 * @brief Discussion node
 * @example - <Discussion=xxx>...</Discussion>
 *          - <discussion=xxx>...</discussion>
 *          - <DISCUSSION=xxx>...</DISCUSSION>
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
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

/**
 * @brief User node
 * @example - <user=xxx>...</user>
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
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

/**
 * @brief Size node
 * @example - <size=xxx>...</size>
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
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

class I : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr I() noexcept = delete;

    constexpr I(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::pl_i, ::std::move(subast)} {
    }

    constexpr I(::pltxt2htm::I const&) noexcept = delete;

    constexpr I(::pltxt2htm::I&&) noexcept = default;

    constexpr ~I() noexcept = default;

    constexpr ::pltxt2htm::I& operator=(::pltxt2htm::I const&) noexcept = delete;

    constexpr ::pltxt2htm::I& operator=(::pltxt2htm::I&&) noexcept = default;
};

class B : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr B() noexcept = delete;

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
