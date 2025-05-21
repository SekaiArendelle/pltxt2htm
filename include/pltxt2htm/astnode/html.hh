#pragma once

#include "basic.hh"

namespace pltxt2htm {

/**
 * @brief Represents a <br> tag.
 */
class Br : public ::pltxt2htm::PlTxtNode {
public:
    constexpr Br() noexcept
        : PlTxtNode{NodeType::br} {
    }

    constexpr Br(Br const& other) noexcept = default;
    constexpr Br(Br&& other) noexcept = default;
    constexpr Br& operator=(Br const& other) noexcept = default;
    constexpr Br& operator=(Br&& other) noexcept = default;
};

class Space : public ::pltxt2htm::PlTxtNode {
public:
    constexpr Space() noexcept
        : PlTxtNode{NodeType::space} {
    }

    constexpr Space(Space const& other) noexcept = default;
    constexpr Space(Space&& other) noexcept = default;
    constexpr Space& operator=(Space const& other) noexcept = default;
    constexpr Space& operator=(Space&& other)noexcept = default;
};

class LessThan : public ::pltxt2htm::PlTxtNode {
public:
    constexpr LessThan() noexcept
        : PlTxtNode{NodeType::less_than} {
    }

    constexpr LessThan(LessThan const& other) noexcept = default;
    constexpr LessThan(LessThan&& other) noexcept = default;
    constexpr LessThan& operator=(LessThan const& other) noexcept = default;
    constexpr LessThan& operator=(LessThan&& other) noexcept = default;
};

class GreaterThan : public ::pltxt2htm::PlTxtNode {
public:
    constexpr GreaterThan() noexcept
        : PlTxtNode{NodeType::greater_than} {
    }

    constexpr GreaterThan(GreaterThan const& other) noexcept = default;
    constexpr GreaterThan(GreaterThan&& other) noexcept = default;
    constexpr GreaterThan& operator=(GreaterThan const& other) noexcept = default;
    constexpr GreaterThan& operator=(GreaterThan&& other) noexcept = default;
};

} // namespace pltxt2htm
