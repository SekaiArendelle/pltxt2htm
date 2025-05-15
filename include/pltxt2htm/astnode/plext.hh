#pragma once

/**
 * @file plext.hh
 * @brief Quantum-Physics's text extensions
 */

#include <fast_io/fast_io_dsal/vector.h>
#include <concepts>
#include "basic.hh"

namespace pltxt2htm {

/**
 * @brief Color node
 * @example - <color=xxx>...</color>
 *          - <Color=xxx>...</Color>
 *          - <COLOR=xxx>...</COLOR>
 */
class Color : public ::pltxt2htm::PlTxtNode {
    ::fast_io::vector<::pltxt2htm::PlTxtNode> text;

public:
    constexpr Color() noexcept = delete;

    template<typename... Args>
        requires ((::std::derived_from<Args, ::pltxt2htm::PlTxtNode> &&
                   !::std::same_as<Args, ::pltxt2htm::PlTxtNode>) &&
                  ...)
    constexpr Color(Args&&... args) noexcept
        : PlTxtNode{NodeType::color},
          text{args...} {
    }
};

/**
 * @brief tag <a></a> of Quantum-Physics
 */
class A : public ::pltxt2htm::PlTxtNode {
    ::fast_io::vector<::pltxt2htm::PlTxtNode> text;

public:
    constexpr A() noexcept = delete;

    template<typename... Args>
        requires ((::std::derived_from<Args, ::pltxt2htm::PlTxtNode> &&
                   !::std::same_as<Args, ::pltxt2htm::PlTxtNode>) &&
                  ...)
    constexpr A(Args&&... args) noexcept
        : PlTxtNode{NodeType::a},
          text{args...} {
    }
};

/**
 * @brief Experiment node
 * @example - <Experiment=xxx>...</Experiment>
 *          - <experiment=xxx>...</experiment>
 *          - <EXPERIMENT=xxx>...</EXPERIMENT>
 */
class Experiment : public ::pltxt2htm::PlTxtNode {
    ::fast_io::vector<::pltxt2htm::PlTxtNode> text;

public:
    constexpr Experiment() noexcept = delete;

    template<typename... Args>
    requires ((::std::derived_from<Args, ::pltxt2htm::PlTxtNode> &&
               !::std::same_as<Args, ::pltxt2htm::PlTxtNode>) &&
              ...)
constexpr Experiment(Args&&... args) noexcept
        : PlTxtNode{NodeType::experiment},
          text{args...} {
    }
};

/**
 * @brief Discussion node
 * @example - <Discussion=xxx>...</Discussion>
 *          - <discussion=xxx>...</discussion>
 *          - <DISCUSSION=xxx>...</DISCUSSION>
 */
class Discussion : public ::pltxt2htm::PlTxtNode {
    ::fast_io::vector<::pltxt2htm::PlTxtNode> text;

public:
    constexpr Discussion() noexcept = delete;

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
