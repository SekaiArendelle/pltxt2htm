#pragma once

#include "node_type.hh"
#include "basic.hh"

namespace pltxt2htm {

/**
 * @brief Represents '\n'.
 */
class LineBreak : public ::pltxt2htm::PlTxtNode {
public:
    constexpr LineBreak() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::line_break} {
    }

    constexpr LineBreak(::pltxt2htm::LineBreak const&) noexcept = delete;
    constexpr LineBreak(::pltxt2htm::LineBreak&& other) noexcept = default;
    constexpr ~LineBreak() noexcept = default;
    constexpr ::pltxt2htm::LineBreak& operator=(::pltxt2htm::LineBreak const& other) noexcept = delete;
    constexpr ::pltxt2htm::LineBreak& operator=(::pltxt2htm::LineBreak&& other) noexcept = delete;
};

/**
 * @brief Represents a <br> tag.
 */
class Br : public ::pltxt2htm::PlTxtNode {
public:
    constexpr Br() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::html_br} {
    }

    constexpr Br(::pltxt2htm::Br const&) noexcept = delete;
    constexpr Br(::pltxt2htm::Br&& other) noexcept = default;
    constexpr ~Br() noexcept = default;
    constexpr ::pltxt2htm::Br& operator=(::pltxt2htm::Br const& other) noexcept = delete;
    constexpr ::pltxt2htm::Br& operator=(::pltxt2htm::Br&& other) noexcept = delete;
};

/**
 * @brief Represents ` `.
 */
class Space : public ::pltxt2htm::PlTxtNode {
public:
    constexpr Space() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::space} {
    }

    constexpr Space(::pltxt2htm::Space const&) noexcept = delete;
    constexpr Space(::pltxt2htm::Space&& other) noexcept = default;
    constexpr ~Space() noexcept = default;
    constexpr ::pltxt2htm::Space& operator=(::pltxt2htm::Space const& other) noexcept = delete;
    constexpr ::pltxt2htm::Space& operator=(::pltxt2htm::Space&& other) noexcept = delete;
};

/**
 * @brief Represents `<`.
 */
class LessThan : public ::pltxt2htm::PlTxtNode {
public:
    constexpr LessThan() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::less_than} {
    }

    constexpr LessThan(::pltxt2htm::LessThan const&) noexcept = delete;
    constexpr LessThan(::pltxt2htm::LessThan&& other) noexcept = default;
    constexpr ~LessThan() noexcept = default;
    constexpr ::pltxt2htm::LessThan& operator=(::pltxt2htm::LessThan const& other) noexcept = delete;
    constexpr ::pltxt2htm::LessThan& operator=(::pltxt2htm::LessThan&& other) noexcept = delete;
};

/**
 * @brief Represents `>`.
 */
class GreaterThan : public ::pltxt2htm::PlTxtNode {
public:
    constexpr GreaterThan() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::greater_than} {
    }

    constexpr GreaterThan(::pltxt2htm::GreaterThan const&) noexcept = delete;
    constexpr GreaterThan(::pltxt2htm::GreaterThan&& other) noexcept = default;
    constexpr ~GreaterThan() noexcept = default;
    constexpr ::pltxt2htm::GreaterThan& operator=(::pltxt2htm::GreaterThan const& other) noexcept = delete;
    constexpr ::pltxt2htm::GreaterThan& operator=(::pltxt2htm::GreaterThan&& other) noexcept = delete;
};

/**
 * @brief Represents `\t`
 */
class Tab : public ::pltxt2htm::PlTxtNode {
public:
    constexpr Tab() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::tab} {
    }

    constexpr Tab(::pltxt2htm::Tab const&) noexcept = delete;
    constexpr Tab(::pltxt2htm::Tab&& other) noexcept = default;
    constexpr ~Tab() noexcept = default;
    constexpr ::pltxt2htm::Tab& operator=(::pltxt2htm::Tab const& other) noexcept = delete;
    constexpr ::pltxt2htm::Tab& operator=(::pltxt2htm::Tab&& other) noexcept = delete;
};

/**
 * @brief Represents `&`.
 */
class Ampersand : public ::pltxt2htm::PlTxtNode {
public:
    constexpr Ampersand() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::ampersand} {
    }

    constexpr Ampersand(::pltxt2htm::Ampersand const&) noexcept = delete;
    constexpr Ampersand(::pltxt2htm::Ampersand&& other) noexcept = default;
    constexpr ~Ampersand() noexcept = default;
    constexpr ::pltxt2htm::Ampersand& operator=(::pltxt2htm::Ampersand const& other) noexcept = delete;
    constexpr ::pltxt2htm::Ampersand& operator=(::pltxt2htm::Ampersand&& other) noexcept = delete;
};

/**
 * @brief Represents `'`.
 */
class SingleQuotationMark : public ::pltxt2htm::PlTxtNode {
public:
    constexpr SingleQuotationMark() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::single_quote} {
    }

    constexpr SingleQuotationMark(::pltxt2htm::SingleQuotationMark const&) noexcept = delete;
    constexpr SingleQuotationMark(::pltxt2htm::SingleQuotationMark&& other) noexcept = default;
    constexpr ~SingleQuotationMark() noexcept = default;
    constexpr ::pltxt2htm::SingleQuotationMark& operator=(::pltxt2htm::SingleQuotationMark const&) noexcept = delete;
    constexpr ::pltxt2htm::SingleQuotationMark& operator=(::pltxt2htm::SingleQuotationMark&&) noexcept = default;
};

/**
 * @brief Represents `"`.
 */
class DoubleQuotationMark : public ::pltxt2htm::PlTxtNode {
public:
    constexpr DoubleQuotationMark() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::double_quote} {
    }

    constexpr DoubleQuotationMark(::pltxt2htm::DoubleQuotationMark const&) noexcept = delete;
    constexpr DoubleQuotationMark(::pltxt2htm::DoubleQuotationMark&&) noexcept = default;
    constexpr ~DoubleQuotationMark() noexcept = default;
    constexpr ::pltxt2htm::DoubleQuotationMark& operator=(::pltxt2htm::DoubleQuotationMark const&) noexcept = delete;
    constexpr ::pltxt2htm::DoubleQuotationMark& operator=(::pltxt2htm::DoubleQuotationMark&&) noexcept = default;
};

/**
 * @brief Represents HTML `<h1>`.
 */
class H1 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H1() noexcept = delete;

    constexpr H1(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_h1, ::std::move(subast)} {
    }

    constexpr H1(::pltxt2htm::H1 const&) noexcept = delete;

    constexpr H1(::pltxt2htm::H1&&) noexcept = default;

    constexpr ~H1() noexcept = default;

    constexpr ::pltxt2htm::H1& operator=(::pltxt2htm::H1 const&) noexcept = delete;

    constexpr ::pltxt2htm::H1& operator=(::pltxt2htm::H1&&) noexcept = default;
};

/**
 * @brief Represents HTML `<h2>`.
 */
class H2 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H2() noexcept = delete;

    constexpr H2(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_h2, ::std::move(subast)} {
    }

    constexpr H2(::pltxt2htm::H2 const&) noexcept = delete;

    constexpr H2(::pltxt2htm::H2&&) noexcept = default;

    constexpr ~H2() noexcept = default;

    constexpr ::pltxt2htm::H2& operator=(::pltxt2htm::H2 const&) noexcept = delete;

    constexpr ::pltxt2htm::H2& operator=(::pltxt2htm::H2&&) noexcept = default;
};

/**
 * @brief Represents HTML `<h3>`.
 */
class H3 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H3() noexcept = delete;

    constexpr H3(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_h3, ::std::move(subast)} {
    }

    constexpr H3(::pltxt2htm::H3 const&) noexcept = delete;

    constexpr H3(::pltxt2htm::H3&&) noexcept = default;

    constexpr ~H3() noexcept = default;

    constexpr ::pltxt2htm::H3& operator=(::pltxt2htm::H3 const&) noexcept = delete;

    constexpr ::pltxt2htm::H3& operator=(::pltxt2htm::H3&&) noexcept = default;
};

/**
 * @brief Represents HTML `<h4>`.
 */
class H4 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H4() noexcept = delete;

    constexpr H4(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_h4, ::std::move(subast)} {
    }

    constexpr H4(::pltxt2htm::H4 const&) noexcept = delete;

    constexpr H4(::pltxt2htm::H4&&) noexcept = default;

    constexpr ~H4() noexcept = default;

    constexpr ::pltxt2htm::H4& operator=(::pltxt2htm::H4 const&) noexcept = delete;

    constexpr ::pltxt2htm::H4& operator=(::pltxt2htm::H4&&) noexcept = default;
};

/**
 * @brief Represents HTML `<h5>`.
 */
class H5 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H5() noexcept = delete;

    constexpr H5(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_h5, ::std::move(subast)} {
    }

    constexpr H5(::pltxt2htm::H5 const&) noexcept = delete;

    constexpr H5(::pltxt2htm::H5&&) noexcept = default;

    constexpr ~H5() noexcept = default;

    constexpr ::pltxt2htm::H5& operator=(::pltxt2htm::H5 const&) noexcept = delete;

    constexpr ::pltxt2htm::H5& operator=(::pltxt2htm::H5&&) noexcept = default;
};

/**
 * @brief Represents HTML `<h6>`.
 */
class H6 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr H6() noexcept = delete;

    constexpr H6(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_h6, ::std::move(subast)} {
    }

    constexpr H6(::pltxt2htm::H6 const&) noexcept = delete;

    constexpr H6(::pltxt2htm::H6&&) noexcept = default;

    constexpr ~H6() noexcept = default;

    constexpr ::pltxt2htm::H6& operator=(::pltxt2htm::H6 const&) noexcept = delete;

    constexpr ::pltxt2htm::H6& operator=(::pltxt2htm::H6&&) noexcept = default;
};

/**
 * @brief Represents HTML `<p>`.
 */
class P : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr P() noexcept = delete;

    constexpr P(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_p, ::std::move(subast)} {
    }

    constexpr P(::pltxt2htm::P const&) noexcept = delete;

    constexpr P(::pltxt2htm::P&&) noexcept = default;

    constexpr ~P() noexcept = default;

    constexpr ::pltxt2htm::P& operator=(::pltxt2htm::P const&) noexcept = delete;

    constexpr ::pltxt2htm::P& operator=(::pltxt2htm::P&&) noexcept = default;
};

/**
 * @brief Represents HTML `<del>`.
 */
class Del : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr Del() noexcept = delete;

    constexpr Del(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_del, ::std::move(subast)} {
    }

    constexpr Del(::pltxt2htm::Del const&) noexcept = delete;

    constexpr Del(::pltxt2htm::Del&&) noexcept = default;

    constexpr ~Del() noexcept = default;

    constexpr ::pltxt2htm::Del& operator=(::pltxt2htm::Del const&) noexcept = delete;

    constexpr ::pltxt2htm::Del& operator=(::pltxt2htm::Del&&) noexcept = default;
};

/**
 * @brief Represents a <hr> tag.
 */
class Hr : public ::pltxt2htm::PlTxtNode {
public:
    constexpr Hr() noexcept
        : ::pltxt2htm::PlTxtNode{::pltxt2htm::NodeType::html_hr} {
    }

    constexpr Hr(::pltxt2htm::Hr const&) noexcept = delete;
    constexpr Hr(::pltxt2htm::Hr&& other) noexcept = default;
    constexpr ~Hr() noexcept = default;
    constexpr ::pltxt2htm::Hr& operator=(::pltxt2htm::Hr const& other) noexcept = delete;
    constexpr ::pltxt2htm::Hr& operator=(::pltxt2htm::Hr&& other) noexcept = delete;
};

/**
 * @brief Represents HTML comment `<!-- -->`.
 */
class Note : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr Note() noexcept = delete;

    constexpr Note(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_note, ::std::move(subast)} {
    }

    constexpr Note(::pltxt2htm::Note const&) noexcept = delete;

    constexpr Note(::pltxt2htm::Note&&) noexcept = default;

    constexpr ~Note() noexcept = default;

    constexpr ::pltxt2htm::Note& operator=(::pltxt2htm::Note const&) noexcept = delete;

    constexpr ::pltxt2htm::Note& operator=(::pltxt2htm::Note&&) noexcept = default;
};

/**
 * @brief Represents HTML `<em>`.
 */
class Em : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr Em() noexcept = delete;

    constexpr Em(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_em, ::std::move(subast)} {
    }

    constexpr Em(::pltxt2htm::Em const&) noexcept = delete;

    constexpr Em(::pltxt2htm::Em&&) noexcept = default;

    constexpr ~Em() noexcept = default;

    constexpr ::pltxt2htm::Em& operator=(::pltxt2htm::Em const&) noexcept = delete;

    constexpr ::pltxt2htm::Em& operator=(::pltxt2htm::Em&&) noexcept = default;
};

/**
 * @brief Represents HTML `<strong>`.
 */
class Strong : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr Strong() noexcept = delete;

    constexpr Strong(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_strong, ::std::move(subast)} {
    }

    constexpr Strong(::pltxt2htm::Strong const&) noexcept = delete;

    constexpr Strong(::pltxt2htm::Strong&&) noexcept = default;

    constexpr ~Strong() noexcept = default;

    constexpr ::pltxt2htm::Strong& operator=(::pltxt2htm::Strong const&) noexcept = delete;

    constexpr ::pltxt2htm::Strong& operator=(::pltxt2htm::Strong&&) noexcept = default;
};

/**
 * @brief Represents HTML `<ul>`.
 */
class Ul : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr Ul() noexcept = delete;

    constexpr Ul(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_ul, ::std::move(subast)} {
    }

    constexpr Ul(::pltxt2htm::Ul const&) noexcept = delete;

    constexpr Ul(::pltxt2htm::Ul&&) noexcept = default;

    constexpr ~Ul() noexcept = default;

    constexpr ::pltxt2htm::Ul& operator=(::pltxt2htm::Ul const&) noexcept = delete;

    constexpr ::pltxt2htm::Ul& operator=(::pltxt2htm::Ul&&) noexcept = default;
};

/**
 * @brief Represents HTML `<li>`.
 */
class Li : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr Li() noexcept = delete;

    constexpr Li(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_li, ::std::move(subast)} {
    }

    constexpr Li(::pltxt2htm::Li const&) noexcept = delete;

    constexpr Li(::pltxt2htm::Li&&) noexcept = default;

    constexpr ~Li() noexcept = default;

    constexpr ::pltxt2htm::Li& operator=(::pltxt2htm::Li const&) noexcept = delete;

    constexpr ::pltxt2htm::Li& operator=(::pltxt2htm::Li&&) noexcept = default;
};

/**
 * @brief Represents HTML `<code>`.
 */
class Code : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr Code() noexcept = delete;

    constexpr Code(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_code, ::std::move(subast)} {
    }

    constexpr Code(::pltxt2htm::Code const&) noexcept = delete;

    constexpr Code(::pltxt2htm::Code&&) noexcept = default;

    constexpr ~Code() noexcept = default;

    constexpr ::pltxt2htm::Code& operator=(::pltxt2htm::Code const&) noexcept = delete;

    constexpr ::pltxt2htm::Code& operator=(::pltxt2htm::Code&&) noexcept = default;
};

/**
 * @brief Represents HTML `<pre>`.
 */
class Pre : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr Pre() noexcept = delete;

    constexpr Pre(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_pre, ::std::move(subast)} {
    }

    constexpr Pre(::pltxt2htm::Pre const&) noexcept = delete;

    constexpr Pre(::pltxt2htm::Pre&&) noexcept = default;

    constexpr ~Pre() noexcept = default;

    constexpr ::pltxt2htm::Pre& operator=(::pltxt2htm::Pre const&) noexcept = delete;

    constexpr ::pltxt2htm::Pre& operator=(::pltxt2htm::Pre&&) noexcept = default;
};

/**
 * @brief Represents HTML `<blockquote>`.
 */
class Blockquote : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr Blockquote() noexcept = delete;

    constexpr Blockquote(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::html_blockquote, ::std::move(subast)} {
    }

    constexpr Blockquote(::pltxt2htm::Blockquote const&) noexcept = delete;

    constexpr Blockquote(::pltxt2htm::Blockquote&&) noexcept = default;

    constexpr ~Blockquote() noexcept = default;

    constexpr ::pltxt2htm::Blockquote& operator=(::pltxt2htm::Blockquote const&) noexcept = delete;

    constexpr ::pltxt2htm::Blockquote& operator=(::pltxt2htm::Blockquote&&) noexcept = default;
};

} // namespace pltxt2htm
