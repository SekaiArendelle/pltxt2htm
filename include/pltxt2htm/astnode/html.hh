#pragma once

#include "node_type.hh"
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

/**
 * @brief Represents ` `.
 */
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

/**
 * @brief Represents `<`.
 */
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

/**
 * @brief Represents `>`.
 */
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

/**
 * @brief Represents `&`.
 */
class Ampersand : public ::pltxt2htm::PlTxtNode {
public:
    constexpr Ampersand() noexcept
        : PlTxtNode{NodeType::ampersand} {
    }

    constexpr Ampersand(Ampersand const& other) noexcept = default;
    constexpr Ampersand(Ampersand&& other) noexcept = default;
    constexpr Ampersand& operator=(Ampersand const& other) noexcept = default;
    constexpr Ampersand& operator=(Ampersand&& other) noexcept = default;
};

/**
 * @brief Represents `'`.
 */
class SingleQuotationMark : public ::pltxt2htm::PlTxtNode {
public:
    constexpr SingleQuotationMark() noexcept
        : PlTxtNode{NodeType::single_quote} {
    }

    constexpr SingleQuotationMark(SingleQuotationMark const& other) noexcept = default;
    constexpr SingleQuotationMark(SingleQuotationMark&& other) noexcept = default;
    constexpr SingleQuotationMark& operator=(SingleQuotationMark const& other) noexcept = default;
    constexpr SingleQuotationMark& operator=(SingleQuotationMark&&) noexcept = default;
};

/**
 * @brief Represents `"`.
 */
class DoubleQuotationMark : public ::pltxt2htm::PlTxtNode {
public:
    constexpr DoubleQuotationMark() noexcept
        : PlTxtNode{NodeType::double_quote} {
    }

    constexpr DoubleQuotationMark(DoubleQuotationMark const&) noexcept = default;
    constexpr DoubleQuotationMark(DoubleQuotationMark&&) noexcept = default;
   constexpr DoubleQuotationMark& operator=(DoubleQuotationMark const&) noexcept = default;
   constexpr DoubleQuotationMark& operator=(DoubleQuotationMark&&) noexcept = default;
};

} // namespace pltxt2htm
