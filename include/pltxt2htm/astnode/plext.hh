#pragma once

/**
 * @file plext.hh
 * @brief Quantum-Physics's text extensions
 */

#include <utility>
#include <fast_io/fast_io_dsal/string.h>
#include <exception/exception.hh>
#include "basic.hh"
#include "../heap_guard.hh"

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

    constexpr Color(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>&& text,
                    ::fast_io::u8string&& color) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::color, ::std::move(text)},
          color_(::std::move(color)) {
    }

    constexpr Color(::pltxt2htm::Color const&) noexcept = delete;

    constexpr Color(::pltxt2htm::Color&& other) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::color, ::std::move(other.subast_)},
          color_(::std::move(other.color_)) {
    }

    constexpr ~Color() noexcept = default;

    constexpr Color& operator=(this Color& self, ::pltxt2htm::Color const&) noexcept = delete;

    /**
     * @brief move assignment operator
     * @note assign to a rvalue is not allowed
     */
    constexpr Color& operator=(this Color& self, ::pltxt2htm::Color&&) noexcept = default;

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

    constexpr Experiment(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>&& subast,
                         ::fast_io::u8string&& id) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::experiment, ::std::move(subast)},
          id_(::std::move(id)) {
    }

    constexpr Experiment(::pltxt2htm::Experiment const&) noexcept = delete;

    constexpr Experiment(::pltxt2htm::Experiment&& other) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::experiment, ::std::move(other.subast_)},
          id_(::std::move(other.id_)) {
    }

    constexpr ~Experiment() noexcept = default;

    constexpr Experiment& operator=(this Experiment& self, Experiment const&) noexcept = delete;

    constexpr Experiment& operator=(this Experiment& self, ::pltxt2htm::Experiment&&) noexcept = default;
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

    constexpr Discussion(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>&& subast,
                         ::fast_io::u8string&& id) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::discussion, ::std::move(subast)},
          id_(::std::move(id)) {
    }

    constexpr Discussion(::pltxt2htm::Discussion const&) noexcept = delete;

    constexpr Discussion(::pltxt2htm::Discussion&& other) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::discussion, ::std::move(other.subast_)},
          id_(::std::move(other.id_)) {
    }

    constexpr ~Discussion() noexcept = default;

    constexpr Discussion& operator=(this Discussion& self, Discussion const&) noexcept = delete;

    constexpr Discussion& operator=(this Discussion& self, ::pltxt2htm::Discussion&&) noexcept = default;

    [[nodiscard]]
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

} // namespace pltxt2htm
