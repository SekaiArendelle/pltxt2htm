#pragma once

/**
 * @file plext.hh
 * @brief Quantum-Physics's text extensions
 */

#include <concepts>
#include <utility>
#include <fast_io/fast_io_dsal/vector.h>
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
class Color : public ::pltxt2htm::PlTxtNode {
    ::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>> subast_;
    ::fast_io::u8string color_;

public:
    constexpr Color() noexcept = delete;

    constexpr Color(Color const&) noexcept = delete;

    constexpr Color(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>&& text, ::fast_io::u8string&& color) noexcept
         : ::pltxt2htm::PlTxtNode{NodeType::color},
          subast_(::std::move(text)),
          color_(::std::move(color)) {
    }

    constexpr Color(::pltxt2htm::Color&& other) noexcept
        : ::pltxt2htm::PlTxtNode{NodeType::color},
          subast_(::std::move(other.subast_)),
          color_(::std::move(other.color_)) {
    }

    constexpr Color& operator=(this Color& self, Color const&) noexcept = delete;

    /**
     * @brief move assignment operator
     * @note assign to a rvalue is not allowed
     */
    constexpr Color& operator=(this Color& self, ::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>&& text) noexcept {
        self.subast_ = ::std::move(text);
        return self;
    }

    constexpr ~Color() noexcept = default;

    template<bool ndebug>
    [[nodiscard]]
    constexpr auto&& get_color(this auto&& self) noexcept {
        if constexpr (::std::is_rvalue_reference_v<decltype((self))>) {
            return ::std::move(::std::as_const(self.color_));
        } else if constexpr (::std::is_lvalue_reference_v<decltype((self))>) {
            return ::std::as_const(self.color_);
        } else {
            ::exception::unreachable<ndebug>();
        }
    }

    template<bool ndebug, typename Self>
    [[nodiscard]]
     constexpr auto&& get_subast(this Self&& self) noexcept {
        return ::std::forward_like<Self>(self.subast_);
    }
};

/**
 * @brief tag <a></a> of Quantum-Physics
 */
class A : public ::pltxt2htm::PlTxtNode {
    ::fast_io::vector<::pltxt2htm::PlTxtNode> text_{};

public:
    constexpr A() noexcept = default;

    template<typename... Args>
        requires ((::std::derived_from<Args, ::pltxt2htm::PlTxtNode> &&
                   !::std::same_as<Args, ::pltxt2htm::PlTxtNode>) &&
                  ...)
    constexpr A(Args&&... args) noexcept
        : PlTxtNode{NodeType::a},
          text_{args...} {
    }
};

/**
 * @brief Experiment node
 * @example - <Experiment=xxx>...</Experiment>
 *          - <experiment=xxx>...</experiment>
 *          - <EXPERIMENT=xxx>...</EXPERIMENT>
 */
class Experiment : public ::pltxt2htm::PlTxtNode {
    ::fast_io::vector<::pltxt2htm::PlTxtNode> text_{};
    ::fast_io::u8string id_{};

public:
    constexpr Experiment() noexcept = default;

    template<typename... Args>
    requires ((::std::derived_from<Args, ::pltxt2htm::PlTxtNode> &&
               !::std::same_as<Args, ::pltxt2htm::PlTxtNode>) &&
              ...)
constexpr Experiment(Args&&... args) noexcept
        : PlTxtNode{NodeType::experiment},
          text_{args...} {
    }
};

/**
 * @brief Discussion node
 * @example - <Discussion=xxx>...</Discussion>
 *          - <discussion=xxx>...</discussion>
 *          - <DISCUSSION=xxx>...</DISCUSSION>
 */
class Discussion : public ::pltxt2htm::PlTxtNode {
    ::fast_io::vector<::pltxt2htm::PlTxtNode> text{};
    ::fast_io::u8string id_{};

public:
    constexpr Discussion() noexcept = default;

    template<typename... Args>
    requires ((::std::derived_from<Args, ::pltxt2htm::PlTxtNode> &&
               !::std::same_as<Args, ::pltxt2htm::PlTxtNode>) &&
              ...)
constexpr Discussion(Args&&... args) noexcept
        : PlTxtNode{NodeType::discussion},
          text{args...} {
    }
};

} // namespace pltxt2htm
