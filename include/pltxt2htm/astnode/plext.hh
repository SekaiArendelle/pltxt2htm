#pragma once

/**
 * @file plext.hh
 * @brief Quantum-Physics's text extensions
 */

#include "basic.hh"

namespace pltxt2htm {

/**
 * @brief Color node
 * @example - <color=xxx>...</color>
 *          - <Color=xxx>...</Color>
 *          - <COLOR=xxx>...</COLOR>
 */
class Color : public ::pltxt2htm::PlTxtNode {
    ::pltxt2htm::PlTxtNode text;

public:
    constexpr Color() noexcept = delete;

    constexpr Color(::pltxt2htm::PlTxtNode str) noexcept
        : PlTxtNode{NodeType::color},
          text{str} {
    }
};

class A : public ::pltxt2htm::PlTxtNode {
    ::pltxt2htm::PlTxtNode text;

public:
    constexpr A() noexcept = delete;

    constexpr A(::pltxt2htm::PlTxtNode str) noexcept
        : PlTxtNode{NodeType::a},
          text{str} {
    }
};

/**
 * @brief Experiment node
 * @example - <Experiment=xxx>...</Experiment>
 *          - <experiment=xxx>...</experiment>
 *          - <EXPERIMENT=xxx>...</EXPERIMENT>
 */
class Experiment : public ::pltxt2htm::PlTxtNode {
    ::pltxt2htm::PlTxtNode text;

public:
    constexpr Experiment() noexcept = delete;

    constexpr Experiment(::pltxt2htm::PlTxtNode str) noexcept
        : PlTxtNode{NodeType::experiment},
          text{str} {
    }
};

/**
 * @brief Discussion node
 * @example - <Discussion=xxx>...</Discussion>
 *          - <discussion=xxx>...</discussion>
 */
class Discussion : public ::pltxt2htm::PlTxtNode {
    ::pltxt2htm::PlTxtNode text;

public:
    constexpr Discussion() noexcept = delete;

    constexpr Discussion(::pltxt2htm::PlTxtNode str) noexcept
        : PlTxtNode{NodeType::discussion},
          text{str} {
    }
};

} // namespace pltxt2htm
