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
    constexpr Space& operator=(Space&& other) noexcept = default;
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

/**
 * @brief Represents HTML `<h1>`.
 */
class H1 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H1() noexcept = delete;

    constexpr H1(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h1, ::std::move(subast)} {
    }

    constexpr H1(::pltxt2htm::H1 const&) noexcept = delete;

    constexpr H1(::pltxt2htm::H1&& other) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h1, ::std::move(other.subast_)} {
    }

    constexpr ~H1() noexcept = default;

    constexpr H1& operator=(this ::pltxt2htm::H1& self, ::pltxt2htm::H1 const&) noexcept = delete;

    constexpr H1& operator=(this ::pltxt2htm::H1& self, ::pltxt2htm::H1&&) noexcept = delete;
};

/**
 * @brief Represents HTML `<h2>`.
 */
class H2 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H2() noexcept = delete;

    constexpr H2(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h2, ::std::move(subast)} {
    }

    constexpr H2(::pltxt2htm::H2 const&) noexcept = delete;

    constexpr H2(::pltxt2htm::H2&& other) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h2, ::std::move(other.subast_)} {
    }

    constexpr ~H2() noexcept = default;

    constexpr H2& operator=(this ::pltxt2htm::H2& self, ::pltxt2htm::H2 const&) noexcept = delete;

    constexpr H2& operator=(this ::pltxt2htm::H2& self, ::pltxt2htm::H2&&) noexcept = delete;
};

/**
 * @brief Represents HTML `<h3>`.
 */
class H3 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H3() noexcept = delete;

    constexpr H3(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h3, ::std::move(subast)} {
    }

    constexpr H3(::pltxt2htm::H3 const&) noexcept = delete;

    constexpr H3(::pltxt2htm::H3&& other) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h3, ::std::move(other.subast_)} {
    }

    constexpr ~H3() noexcept = default;

    constexpr H3& operator=(this ::pltxt2htm::H3& self, ::pltxt2htm::H3 const&) noexcept = delete;

    constexpr H3& operator=(this ::pltxt2htm::H3& self, ::pltxt2htm::H3&&) noexcept = delete;
};

/**
 * @brief Represents HTML `<h4>`.
 */
class H4 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H4() noexcept = delete;

    constexpr H4(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h4, ::std::move(subast)} {
    }

    constexpr H4(::pltxt2htm::H4 const&) noexcept = delete;

    constexpr H4(::pltxt2htm::H4&& other) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h4, ::std::move(other.subast_)} {
    }

    constexpr ~H4() noexcept = default;

    constexpr H4& operator=(this ::pltxt2htm::H4& self, ::pltxt2htm::H4 const&) noexcept = delete;

    constexpr H4& operator=(this ::pltxt2htm::H4& self, ::pltxt2htm::H4&&) noexcept = delete;
};

/**
 * @brief Represents HTML `<h5>`.
 */
class H5 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H5() noexcept = delete;

    constexpr H5(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h5, ::std::move(subast)} {
    }

    constexpr H5(::pltxt2htm::H5 const&) noexcept = delete;

    constexpr H5(::pltxt2htm::H5&& other) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h5, ::std::move(other.subast_)} {
    }

    constexpr ~H5() noexcept = default;

    constexpr H5& operator=(this ::pltxt2htm::H5& self, ::pltxt2htm::H5 const&) noexcept = delete;

    constexpr H5& operator=(this ::pltxt2htm::H5& self, ::pltxt2htm::H5&&) noexcept = delete;
};

/**
 * @brief Represents HTML `<h6>`.
 */
class H6 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H6() noexcept = delete;

    constexpr H6(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h6, ::std::move(subast)} {
    }

    constexpr H6(::pltxt2htm::H6 const&) noexcept = delete;

    constexpr H6(::pltxt2htm::H6&& other) noexcept
        : ::pltxt2htm::details::PairedTagBase{NodeType::h6, ::std::move(other.subast_)} {
    }

    constexpr ~H6() noexcept = default;

    constexpr H6& operator=(this ::pltxt2htm::H6& self, ::pltxt2htm::H6 const&) noexcept = delete;

    constexpr H6& operator=(this ::pltxt2htm::H6& self, ::pltxt2htm::H6&&) noexcept = delete;
};

} // namespace pltxt2htm
