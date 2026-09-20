/**
 * @file ast.hh
 * @brief AST tagged union node definition for pltxt2htm
 * @details Defines the main AST node class template ::pltxt2htm::PlTxtNode,
 *          which is a tagged union (discriminated by ::pltxt2htm::NodeKind) over
 *          all concrete node types (basic, HTML, Markdown, Physics-Lab, Unity rich text).
 */

#pragma once

#include <memory>

#include "../contracts.hh"
#include "node_kind.hh"

#include "impl/basic_node_decl.hh"
#include "impl/html_node_decl.hh"
#include "impl/list_node_decl.hh"
#include "impl/markdown_node_decl.hh"
#include "impl/physics_lab_node_decl.hh"
#include "impl/table_node_decl.hh"
#include "impl/unity_node_decl.hh"
#include "impl/pltxt_node_traits.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm {

/**
 * @brief Tagged-union AST node for Physics-Lab, Unity rich-text, Markdown, and HTML elements.
 * @details Dispatched by ::pltxt2htm::NodeKind. Holds exactly one of the concrete
 *          node types (basic, HTML, Markdown, Physics-Lab, Unity rich text) in a type-safe union.
 * @tparam ndebug Contract checking mode.
 */
template<::pltxt2htm::Contracts ndebug>
class PlTxtNode {
    using Storage = ::pltxt2htm::details::PlTxtNodeStorage<ndebug>;

    Storage storage{};
    ::pltxt2htm::NodeKind node_kind{};

    constexpr PlTxtNode() noexcept = default;

public:
    /**
     * @brief Construct a tagged AST node directly from the selected concrete node type.
     * @tparam Node Concrete node type stored in the tagged union.
     * @param args Arguments forwarded to the concrete node constructor.
     * @return A ::pltxt2htm::PlTxtNode containing the constructed node.
     */
    template<typename Node, typename... Args>
        requires (::pltxt2htm::details::PlTxtNodeConcept<ndebug, Node> &&
                 ::std::is_nothrow_constructible_v<Node, Args...>)
    [[nodiscard]]
    static constexpr auto emplace(Args&&... args) noexcept -> ::pltxt2htm::PlTxtNode<ndebug> {
        using Traits = ::pltxt2htm::details::PlTxtNodeTraits<ndebug, Node>;

        auto result = ::pltxt2htm::PlTxtNode<ndebug>{};
        ::std::construct_at(::std::addressof(result.storage.*Traits::member), ::std::forward<Args>(args)...);
        result.node_kind = Traits::kind;
        return result;
    }

    /**
     * @note: Move construction is sufficient for this codebase. Copy construction is provided for external
     *        users only, and is disabled while building pltxt2htm itself, so that implementation code cannot
     *        silently pay for a deep copy (PLTXT2HTM_INTERNAL_USE).
     */
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr PlTxtNode(::pltxt2htm::PlTxtNode<ndebug> const&) noexcept = delete;
#else
    constexpr PlTxtNode(::pltxt2htm::PlTxtNode<ndebug> const& other) noexcept
        : node_kind(other.node_kind) {
        switch (node_kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char: {
            new (::std::addressof(storage.u8char_node))::pltxt2htm::U8Char(other.storage.u8char_node);
            break;
        }
        case ::pltxt2htm::NodeKind::invalid_utf8: {
            new (::std::addressof(storage.invalid_utf8_node))::pltxt2htm::InvalidUtf8(other.storage.invalid_utf8_node);
            break;
        }
        case ::pltxt2htm::NodeKind::group: {
            new (::std::addressof(storage.group_node))::pltxt2htm::Group(other.storage.group_node);
            break;
        }
        case ::pltxt2htm::NodeKind::unity_color: {
            new (::std::addressof(storage.unity_color_node))::pltxt2htm::UnityColor(other.storage.unity_color_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            new (::std::addressof(storage.pl_a_node))::pltxt2htm::PlA<ndebug>(other.storage.pl_a_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            new (::std::addressof(storage.pl_experiment_node))::pltxt2htm::PlExperiment(
                other.storage.pl_experiment_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            new (::std::addressof(storage.pl_discussion_node))::pltxt2htm::PlDiscussion(
                other.storage.pl_discussion_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_experiments: {
            new (::std::addressof(storage.pl_experiments_node))::pltxt2htm::PlExperiments(
                other.storage.pl_experiments_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_discussions: {
            new (::std::addressof(storage.pl_discussions_node))::pltxt2htm::PlDiscussions(
                other.storage.pl_discussions_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            new (::std::addressof(storage.pl_user_node))::pltxt2htm::PlUser(other.storage.pl_user_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_trigger: {
            new (::std::addressof(storage.pl_trigger_node))::pltxt2htm::PlTrigger(other.storage.pl_trigger_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_internal: {
            new (::std::addressof(storage.pl_internal_node))::pltxt2htm::PlInternal(other.storage.pl_internal_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            new (::std::addressof(storage.pl_external_node))::pltxt2htm::PlExternal(other.storage.pl_external_node);
            break;
        }
        case ::pltxt2htm::NodeKind::unity_link: {
            new (::std::addressof(storage.unity_link_node))::pltxt2htm::UnityLink(other.storage.unity_link_node);
            break;
        }
        case ::pltxt2htm::NodeKind::unity_size: {
            new (::std::addressof(storage.unity_size_node))::pltxt2htm::UnitySize(other.storage.unity_size_node);
            break;
        }
        case ::pltxt2htm::NodeKind::unity_voffset: {
            new (::std::addressof(storage.unity_voffset_node))::pltxt2htm::UnityVoffset(
                other.storage.unity_voffset_node);
            break;
        }
        case ::pltxt2htm::NodeKind::unity_align: {
            new (::std::addressof(storage.unity_align_node))::pltxt2htm::UnityAlign(other.storage.unity_align_node);
            break;
        }
        case ::pltxt2htm::NodeKind::unity_mark: {
            new (::std::addressof(storage.unity_mark_node))::pltxt2htm::UnityMark(other.storage.unity_mark_node);
            break;
        }
        case ::pltxt2htm::NodeKind::unity_margin: {
            new (::std::addressof(storage.unity_margin_node))::pltxt2htm::UnityMargin(other.storage.unity_margin_node);
            break;
        }
        case ::pltxt2htm::NodeKind::unity_i: {
            new (::std::addressof(storage.unity_i_node))::pltxt2htm::UnityI(other.storage.unity_i_node);
            break;
        }
        case ::pltxt2htm::NodeKind::unity_b: {
            new (::std::addressof(storage.unity_b_node))::pltxt2htm::UnityB(other.storage.unity_b_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_u: {
            new (::std::addressof(storage.html_u_node))::pltxt2htm::HtmlU(other.storage.html_u_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_s: {
            new (::std::addressof(storage.html_s_node))::pltxt2htm::HtmlS(other.storage.html_s_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_project: {
            new (::std::addressof(storage.pl_macro_project_node))::pltxt2htm::PlMacroProject(
                other.storage.pl_macro_project_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_visitor: {
            new (::std::addressof(storage.pl_macro_visitor_node))::pltxt2htm::PlMacroVisitor(
                other.storage.pl_macro_visitor_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_author: {
            new (::std::addressof(storage.pl_macro_author_node))::pltxt2htm::PlMacroAuthor(
                other.storage.pl_macro_author_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
            new (::std::addressof(storage.pl_macro_coauthors_node))::pltxt2htm::PlMacroCoauthors(
                other.storage.pl_macro_coauthors_node);
            break;
        }
        case ::pltxt2htm::NodeKind::line_break: {
            new (::std::addressof(storage.line_break_node))::pltxt2htm::LineBreak(other.storage.line_break_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_br: {
            new (::std::addressof(storage.br_node))::pltxt2htm::HtmlBr(other.storage.br_node);
            break;
        }
        case ::pltxt2htm::NodeKind::space: {
            new (::std::addressof(storage.space_node))::pltxt2htm::Space(other.storage.space_node);
            break;
        }
        case ::pltxt2htm::NodeKind::less_than: {
            new (::std::addressof(storage.less_than_node))::pltxt2htm::LessThan(other.storage.less_than_node);
            break;
        }
        case ::pltxt2htm::NodeKind::greater_than: {
            new (::std::addressof(storage.greater_than_node))::pltxt2htm::GreaterThan(other.storage.greater_than_node);
            break;
        }
        case ::pltxt2htm::NodeKind::tab: {
            new (::std::addressof(storage.tab_node))::pltxt2htm::Tab(other.storage.tab_node);
            break;
        }
        case ::pltxt2htm::NodeKind::ampersand: {
            new (::std::addressof(storage.ampersand_node))::pltxt2htm::Ampersand(other.storage.ampersand_node);
            break;
        }
        case ::pltxt2htm::NodeKind::single_quote: {
            new (::std::addressof(storage.single_quote_node))::pltxt2htm::SingleQuote(other.storage.single_quote_node);
            break;
        }
        case ::pltxt2htm::NodeKind::double_quote: {
            new (::std::addressof(storage.double_quote_node))::pltxt2htm::DoubleQuote(other.storage.double_quote_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_hr: {
            new (::std::addressof(storage.html_hr_node))::pltxt2htm::HtmlHr(other.storage.html_hr_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            new (::std::addressof(storage.html_h1_node))::pltxt2htm::HtmlH1(other.storage.html_h1_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            new (::std::addressof(storage.html_h2_node))::pltxt2htm::HtmlH2(other.storage.html_h2_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            new (::std::addressof(storage.html_h3_node))::pltxt2htm::HtmlH3(other.storage.html_h3_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            new (::std::addressof(storage.html_h4_node))::pltxt2htm::HtmlH4(other.storage.html_h4_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            new (::std::addressof(storage.html_h5_node))::pltxt2htm::HtmlH5(other.storage.html_h5_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            new (::std::addressof(storage.html_h6_node))::pltxt2htm::HtmlH6(other.storage.html_h6_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            new (::std::addressof(storage.html_p_node))::pltxt2htm::HtmlP(other.storage.html_p_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_del: {
            new (::std::addressof(storage.html_del_node))::pltxt2htm::HtmlDel(other.storage.html_del_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_sup: {
            new (::std::addressof(storage.html_sup_node))::pltxt2htm::HtmlSup(other.storage.html_sup_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_sub: {
            new (::std::addressof(storage.html_sub_node))::pltxt2htm::HtmlSub(other.storage.html_sub_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_note: {
            new (::std::addressof(storage.html_note_node))::pltxt2htm::HtmlNote(other.storage.html_note_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_em: {
            new (::std::addressof(storage.html_em_node))::pltxt2htm::HtmlEm(other.storage.html_em_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_strong: {
            new (::std::addressof(storage.html_strong_node))::pltxt2htm::HtmlStrong(other.storage.html_strong_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_mark: {
            new (::std::addressof(storage.html_mark_node))::pltxt2htm::HtmlMark(other.storage.html_mark_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            new (::std::addressof(storage.html_span_node))::pltxt2htm::HtmlSpan(other.storage.html_span_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_div: {
            new (::std::addressof(storage.html_div_node))::pltxt2htm::HtmlDiv(other.storage.html_div_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_a: {
            new (::std::addressof(storage.html_a_node))::pltxt2htm::HtmlA<ndebug>(other.storage.html_a_node);
            break;
        }
        case ::pltxt2htm::NodeKind::list_ul: {
            new (::std::addressof(storage.list_ul_node))::pltxt2htm::ListUl(other.storage.list_ul_node);
            break;
        }
        case ::pltxt2htm::NodeKind::list_ol: {
            new (::std::addressof(storage.list_ol_node))::pltxt2htm::ListOl(other.storage.list_ol_node);
            break;
        }
        case ::pltxt2htm::NodeKind::list_li: {
            new (::std::addressof(storage.list_li_node))::pltxt2htm::ListLi(other.storage.list_li_node);
            break;
        }
        case ::pltxt2htm::NodeKind::list_li_checkbox: {
            new (::std::addressof(storage.list_li_checkbox_node))::pltxt2htm::ListLiCheckbox(
                other.storage.list_li_checkbox_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            new (::std::addressof(storage.html_blockquote_node))::pltxt2htm::HtmlBlockquote(
                other.storage.html_blockquote_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            new (::std::addressof(storage.html_code_node))::pltxt2htm::HtmlCode(other.storage.html_code_node);
            break;
        }
        case ::pltxt2htm::NodeKind::table_col: {
            new (::std::addressof(storage.col_node))::pltxt2htm::TableCol(other.storage.col_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_img: {
            new (::std::addressof(storage.html_img_node))::pltxt2htm::HtmlImg(other.storage.html_img_node);
            break;
        }
        case ::pltxt2htm::NodeKind::table: {
            new (::std::addressof(storage.table_node))::pltxt2htm::Table(other.storage.table_node);
            break;
        }
        case ::pltxt2htm::NodeKind::table_caption: {
            new (::std::addressof(storage.caption_node))::pltxt2htm::TableCaption(other.storage.caption_node);
            break;
        }
        case ::pltxt2htm::NodeKind::table_colgroup: {
            new (::std::addressof(storage.colgroup_node))::pltxt2htm::TableColgroup(other.storage.colgroup_node);
            break;
        }
        case ::pltxt2htm::NodeKind::table_thead: {
            new (::std::addressof(storage.thead_node))::pltxt2htm::TableThead(other.storage.thead_node);
            break;
        }
        case ::pltxt2htm::NodeKind::table_tbody: {
            new (::std::addressof(storage.tbody_node))::pltxt2htm::TableTbody(other.storage.tbody_node);
            break;
        }
        case ::pltxt2htm::NodeKind::table_tfoot: {
            new (::std::addressof(storage.tfoot_node))::pltxt2htm::TableTfoot(other.storage.tfoot_node);
            break;
        }
        case ::pltxt2htm::NodeKind::table_tr: {
            new (::std::addressof(storage.tr_node))::pltxt2htm::TableTr(other.storage.tr_node);
            break;
        }
        case ::pltxt2htm::NodeKind::table_th: {
            new (::std::addressof(storage.th_node))::pltxt2htm::TableTh(other.storage.th_node);
            break;
        }
        case ::pltxt2htm::NodeKind::table_td: {
            new (::std::addressof(storage.td_node))::pltxt2htm::TableTd(other.storage.td_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h1: {
            new (::std::addressof(storage.md_atx_h1_node))::pltxt2htm::MdAtxH1(other.storage.md_atx_h1_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            new (::std::addressof(storage.md_atx_h2_node))::pltxt2htm::MdAtxH2(other.storage.md_atx_h2_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            new (::std::addressof(storage.md_atx_h3_node))::pltxt2htm::MdAtxH3(other.storage.md_atx_h3_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            new (::std::addressof(storage.md_atx_h4_node))::pltxt2htm::MdAtxH4(other.storage.md_atx_h4_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            new (::std::addressof(storage.md_atx_h5_node))::pltxt2htm::MdAtxH5(other.storage.md_atx_h5_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            new (::std::addressof(storage.md_atx_h6_node))::pltxt2htm::MdAtxH6(other.storage.md_atx_h6_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape: {
            new (::std::addressof(storage.md_escape_node))::pltxt2htm::MdEscape(other.storage.md_escape_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_hr: {
            new (::std::addressof(storage.md_hr_node))::pltxt2htm::MdHr(other.storage.md_hr_node);
            break;
        }
        case ::pltxt2htm::NodeKind::code_fence: {
            new (::std::addressof(storage.code_fence_node))::pltxt2htm::CodeFence<ndebug>(
                other.storage.code_fence_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
            new (::std::addressof(storage.md_code_span_1_backtick_node))::pltxt2htm::MdCodeSpan1Backtick(
                other.storage.md_code_span_1_backtick_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
            new (::std::addressof(storage.md_code_span_2_backtick_node))::pltxt2htm::MdCodeSpan2Backtick(
                other.storage.md_code_span_2_backtick_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            new (::std::addressof(storage.md_code_span_3_backtick_node))::pltxt2htm::MdCodeSpan3Backtick(
                other.storage.md_code_span_3_backtick_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
            new (::std::addressof(storage.md_single_emphasis_asterisk_node))::pltxt2htm::MdSingleEmphasisAsterisk(
                other.storage.md_single_emphasis_asterisk_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
            new (::std::addressof(storage.md_double_emphasis_asterisk_node))::pltxt2htm::MdDoubleEmphasisAsterisk(
                other.storage.md_double_emphasis_asterisk_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            new (::std::addressof(storage.md_triple_emphasis_asterisk_node))::pltxt2htm::MdTripleEmphasisAsterisk(
                other.storage.md_triple_emphasis_asterisk_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
            new (::std::addressof(storage.md_single_emphasis_underscore_node))::pltxt2htm::MdSingleEmphasisUnderscore(
                other.storage.md_single_emphasis_underscore_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
            new (::std::addressof(storage.md_double_emphasis_underscore_node))::pltxt2htm::MdDoubleEmphasisUnderscore(
                other.storage.md_double_emphasis_underscore_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            new (::std::addressof(storage.md_triple_emphasis_underscore_node))::pltxt2htm::MdTripleEmphasisUnderscore(
                other.storage.md_triple_emphasis_underscore_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_del: {
            new (::std::addressof(storage.md_del_node))::pltxt2htm::MdDel(other.storage.md_del_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            new (::std::addressof(storage.md_link_node))::pltxt2htm::MdLink(other.storage.md_link_node);
            break;
        }
        case ::pltxt2htm::NodeKind::url: {
            new (::std::addressof(storage.url_node))::pltxt2htm::Url(other.storage.url_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_image: {
            new (::std::addressof(storage.md_image_node))::pltxt2htm::MdImage(other.storage.md_image_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            new (::std::addressof(storage.md_block_quotes_node))::pltxt2htm::MdBlockQuotes(
                other.storage.md_block_quotes_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_inline: {
            new (::std::addressof(storage.md_latex_inline_node))::pltxt2htm::MdLatexInline(
                other.storage.md_latex_inline_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_block: {
            new (::std::addressof(storage.md_latex_block_node))::pltxt2htm::MdLatexBlock(
                other.storage.md_latex_block_node);
            break;
        }
    #ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in PlTxtNode copy constructor");
            }
    #endif
        }
    }
#endif

    constexpr PlTxtNode(::pltxt2htm::PlTxtNode<ndebug>&& other) noexcept
        : node_kind(other.node_kind) {
        switch (node_kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char: {
            ::std::construct_at(::std::addressof(storage.u8char_node), ::std::move(other.storage.u8char_node));
            break;
        }
        case ::pltxt2htm::NodeKind::invalid_utf8: {
            ::std::construct_at(::std::addressof(storage.invalid_utf8_node),
                                ::std::move(other.storage.invalid_utf8_node));
            break;
        }
        case ::pltxt2htm::NodeKind::group: {
            ::std::construct_at(::std::addressof(storage.group_node), ::std::move(other.storage.group_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_color: {
            ::std::construct_at(::std::addressof(storage.unity_color_node),
                                ::std::move(other.storage.unity_color_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            ::std::construct_at(::std::addressof(storage.pl_a_node), ::std::move(other.storage.pl_a_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            ::std::construct_at(::std::addressof(storage.pl_experiment_node),
                                ::std::move(other.storage.pl_experiment_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            ::std::construct_at(::std::addressof(storage.pl_discussion_node),
                                ::std::move(other.storage.pl_discussion_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_experiments: {
            ::std::construct_at(::std::addressof(storage.pl_experiments_node),
                                ::std::move(other.storage.pl_experiments_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_discussions: {
            ::std::construct_at(::std::addressof(storage.pl_discussions_node),
                                ::std::move(other.storage.pl_discussions_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            ::std::construct_at(::std::addressof(storage.pl_user_node), ::std::move(other.storage.pl_user_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_trigger: {
            ::std::construct_at(::std::addressof(storage.pl_trigger_node), ::std::move(other.storage.pl_trigger_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_internal: {
            ::std::construct_at(::std::addressof(storage.pl_internal_node),
                                ::std::move(other.storage.pl_internal_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            ::std::construct_at(::std::addressof(storage.pl_external_node),
                                ::std::move(other.storage.pl_external_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_link: {
            ::std::construct_at(::std::addressof(storage.unity_link_node), ::std::move(other.storage.unity_link_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_size: {
            ::std::construct_at(::std::addressof(storage.unity_size_node), ::std::move(other.storage.unity_size_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_voffset: {
            ::std::construct_at(::std::addressof(storage.unity_voffset_node),
                                ::std::move(other.storage.unity_voffset_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_align: {
            ::std::construct_at(::std::addressof(storage.unity_align_node),
                                ::std::move(other.storage.unity_align_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_mark: {
            ::std::construct_at(::std::addressof(storage.unity_mark_node), ::std::move(other.storage.unity_mark_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_margin: {
            ::std::construct_at(::std::addressof(storage.unity_margin_node),
                                ::std::move(other.storage.unity_margin_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_i: {
            ::std::construct_at(::std::addressof(storage.unity_i_node), ::std::move(other.storage.unity_i_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_b: {
            ::std::construct_at(::std::addressof(storage.unity_b_node), ::std::move(other.storage.unity_b_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_u: {
            ::std::construct_at(::std::addressof(storage.html_u_node), ::std::move(other.storage.html_u_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_s: {
            ::std::construct_at(::std::addressof(storage.html_s_node), ::std::move(other.storage.html_s_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_project: {
            ::std::construct_at(::std::addressof(storage.pl_macro_project_node),
                                ::std::move(other.storage.pl_macro_project_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_visitor: {
            ::std::construct_at(::std::addressof(storage.pl_macro_visitor_node),
                                ::std::move(other.storage.pl_macro_visitor_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_author: {
            ::std::construct_at(::std::addressof(storage.pl_macro_author_node),
                                ::std::move(other.storage.pl_macro_author_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
            ::std::construct_at(::std::addressof(storage.pl_macro_coauthors_node),
                                ::std::move(other.storage.pl_macro_coauthors_node));
            break;
        }
        case ::pltxt2htm::NodeKind::line_break: {
            ::std::construct_at(::std::addressof(storage.line_break_node), ::std::move(other.storage.line_break_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_br: {
            ::std::construct_at(::std::addressof(storage.br_node), ::std::move(other.storage.br_node));
            break;
        }
        case ::pltxt2htm::NodeKind::space: {
            ::std::construct_at(::std::addressof(storage.space_node), ::std::move(other.storage.space_node));
            break;
        }
        case ::pltxt2htm::NodeKind::less_than: {
            ::std::construct_at(::std::addressof(storage.less_than_node), ::std::move(other.storage.less_than_node));
            break;
        }
        case ::pltxt2htm::NodeKind::greater_than: {
            ::std::construct_at(::std::addressof(storage.greater_than_node),
                                ::std::move(other.storage.greater_than_node));
            break;
        }
        case ::pltxt2htm::NodeKind::tab: {
            ::std::construct_at(::std::addressof(storage.tab_node), ::std::move(other.storage.tab_node));
            break;
        }
        case ::pltxt2htm::NodeKind::ampersand: {
            ::std::construct_at(::std::addressof(storage.ampersand_node), ::std::move(other.storage.ampersand_node));
            break;
        }
        case ::pltxt2htm::NodeKind::single_quote: {
            ::std::construct_at(::std::addressof(storage.single_quote_node),
                                ::std::move(other.storage.single_quote_node));
            break;
        }
        case ::pltxt2htm::NodeKind::double_quote: {
            ::std::construct_at(::std::addressof(storage.double_quote_node),
                                ::std::move(other.storage.double_quote_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_hr: {
            ::std::construct_at(::std::addressof(storage.html_hr_node), ::std::move(other.storage.html_hr_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            ::std::construct_at(::std::addressof(storage.html_h1_node), ::std::move(other.storage.html_h1_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            ::std::construct_at(::std::addressof(storage.html_h2_node), ::std::move(other.storage.html_h2_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            ::std::construct_at(::std::addressof(storage.html_h3_node), ::std::move(other.storage.html_h3_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            ::std::construct_at(::std::addressof(storage.html_h4_node), ::std::move(other.storage.html_h4_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            ::std::construct_at(::std::addressof(storage.html_h5_node), ::std::move(other.storage.html_h5_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            ::std::construct_at(::std::addressof(storage.html_h6_node), ::std::move(other.storage.html_h6_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            ::std::construct_at(::std::addressof(storage.html_p_node), ::std::move(other.storage.html_p_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_del: {
            ::std::construct_at(::std::addressof(storage.html_del_node), ::std::move(other.storage.html_del_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_sup: {
            ::std::construct_at(::std::addressof(storage.html_sup_node), ::std::move(other.storage.html_sup_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_sub: {
            ::std::construct_at(::std::addressof(storage.html_sub_node), ::std::move(other.storage.html_sub_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_note: {
            ::std::construct_at(::std::addressof(storage.html_note_node), ::std::move(other.storage.html_note_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_em: {
            ::std::construct_at(::std::addressof(storage.html_em_node), ::std::move(other.storage.html_em_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_strong: {
            ::std::construct_at(::std::addressof(storage.html_strong_node),
                                ::std::move(other.storage.html_strong_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_mark: {
            ::std::construct_at(::std::addressof(storage.html_mark_node), ::std::move(other.storage.html_mark_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            ::std::construct_at(::std::addressof(storage.html_span_node), ::std::move(other.storage.html_span_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_div: {
            ::std::construct_at(::std::addressof(storage.html_div_node), ::std::move(other.storage.html_div_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_a: {
            ::std::construct_at(::std::addressof(storage.html_a_node), ::std::move(other.storage.html_a_node));
            break;
        }
        case ::pltxt2htm::NodeKind::list_ul: {
            ::std::construct_at(::std::addressof(storage.list_ul_node), ::std::move(other.storage.list_ul_node));
            break;
        }
        case ::pltxt2htm::NodeKind::list_ol: {
            ::std::construct_at(::std::addressof(storage.list_ol_node), ::std::move(other.storage.list_ol_node));
            break;
        }
        case ::pltxt2htm::NodeKind::list_li: {
            ::std::construct_at(::std::addressof(storage.list_li_node), ::std::move(other.storage.list_li_node));
            break;
        }
        case ::pltxt2htm::NodeKind::list_li_checkbox: {
            ::std::construct_at(::std::addressof(storage.list_li_checkbox_node),
                                ::std::move(other.storage.list_li_checkbox_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            ::std::construct_at(::std::addressof(storage.html_blockquote_node),
                                ::std::move(other.storage.html_blockquote_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            ::std::construct_at(::std::addressof(storage.html_code_node), ::std::move(other.storage.html_code_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_col: {
            ::std::construct_at(::std::addressof(storage.col_node), ::std::move(other.storage.col_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_img: {
            ::std::construct_at(::std::addressof(storage.html_img_node), ::std::move(other.storage.html_img_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table: {
            ::std::construct_at(::std::addressof(storage.table_node), ::std::move(other.storage.table_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_caption: {
            ::std::construct_at(::std::addressof(storage.caption_node), ::std::move(other.storage.caption_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_colgroup: {
            ::std::construct_at(::std::addressof(storage.colgroup_node), ::std::move(other.storage.colgroup_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_thead: {
            ::std::construct_at(::std::addressof(storage.thead_node), ::std::move(other.storage.thead_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_tbody: {
            ::std::construct_at(::std::addressof(storage.tbody_node), ::std::move(other.storage.tbody_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_tfoot: {
            ::std::construct_at(::std::addressof(storage.tfoot_node), ::std::move(other.storage.tfoot_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_tr: {
            ::std::construct_at(::std::addressof(storage.tr_node), ::std::move(other.storage.tr_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_th: {
            ::std::construct_at(::std::addressof(storage.th_node), ::std::move(other.storage.th_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_td: {
            ::std::construct_at(::std::addressof(storage.td_node), ::std::move(other.storage.td_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h1: {
            ::std::construct_at(::std::addressof(storage.md_atx_h1_node), ::std::move(other.storage.md_atx_h1_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            ::std::construct_at(::std::addressof(storage.md_atx_h2_node), ::std::move(other.storage.md_atx_h2_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            ::std::construct_at(::std::addressof(storage.md_atx_h3_node), ::std::move(other.storage.md_atx_h3_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            ::std::construct_at(::std::addressof(storage.md_atx_h4_node), ::std::move(other.storage.md_atx_h4_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            ::std::construct_at(::std::addressof(storage.md_atx_h5_node), ::std::move(other.storage.md_atx_h5_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            ::std::construct_at(::std::addressof(storage.md_atx_h6_node), ::std::move(other.storage.md_atx_h6_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape: {
            ::std::construct_at(::std::addressof(storage.md_escape_node), ::std::move(other.storage.md_escape_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_hr: {
            ::std::construct_at(::std::addressof(storage.md_hr_node), ::std::move(other.storage.md_hr_node));
            break;
        }
        case ::pltxt2htm::NodeKind::code_fence: {
            ::std::construct_at(::std::addressof(storage.code_fence_node), ::std::move(other.storage.code_fence_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
            ::std::construct_at(::std::addressof(storage.md_code_span_1_backtick_node),
                                ::std::move(other.storage.md_code_span_1_backtick_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
            ::std::construct_at(::std::addressof(storage.md_code_span_2_backtick_node),
                                ::std::move(other.storage.md_code_span_2_backtick_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            ::std::construct_at(::std::addressof(storage.md_code_span_3_backtick_node),
                                ::std::move(other.storage.md_code_span_3_backtick_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
            ::std::construct_at(::std::addressof(storage.md_single_emphasis_asterisk_node),
                                ::std::move(other.storage.md_single_emphasis_asterisk_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
            ::std::construct_at(::std::addressof(storage.md_double_emphasis_asterisk_node),
                                ::std::move(other.storage.md_double_emphasis_asterisk_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            ::std::construct_at(::std::addressof(storage.md_triple_emphasis_asterisk_node),
                                ::std::move(other.storage.md_triple_emphasis_asterisk_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
            ::std::construct_at(::std::addressof(storage.md_single_emphasis_underscore_node),
                                ::std::move(other.storage.md_single_emphasis_underscore_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
            ::std::construct_at(::std::addressof(storage.md_double_emphasis_underscore_node),
                                ::std::move(other.storage.md_double_emphasis_underscore_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            ::std::construct_at(::std::addressof(storage.md_triple_emphasis_underscore_node),
                                ::std::move(other.storage.md_triple_emphasis_underscore_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_del: {
            ::std::construct_at(::std::addressof(storage.md_del_node), ::std::move(other.storage.md_del_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            ::std::construct_at(::std::addressof(storage.md_link_node), ::std::move(other.storage.md_link_node));
            break;
        }
        case ::pltxt2htm::NodeKind::url: {
            ::std::construct_at(::std::addressof(storage.url_node), ::std::move(other.storage.url_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_image: {
            ::std::construct_at(::std::addressof(storage.md_image_node), ::std::move(other.storage.md_image_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            ::std::construct_at(::std::addressof(storage.md_block_quotes_node),
                                ::std::move(other.storage.md_block_quotes_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_inline: {
            ::std::construct_at(::std::addressof(storage.md_latex_inline_node),
                                ::std::move(other.storage.md_latex_inline_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_block: {
            ::std::construct_at(::std::addressof(storage.md_latex_block_node),
                                ::std::move(other.storage.md_latex_block_node));
            break;
        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in PlTxtNode move constructor");
            }
#endif
        }
    }

    constexpr ~PlTxtNode() noexcept {
        switch (node_kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char: {
            ::std::destroy_at(::std::addressof(storage.u8char_node));
            break;
        }
        case ::pltxt2htm::NodeKind::invalid_utf8: {
            ::std::destroy_at(::std::addressof(storage.invalid_utf8_node));
            break;
        }
        case ::pltxt2htm::NodeKind::group: {
            ::std::destroy_at(::std::addressof(storage.group_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_color: {
            ::std::destroy_at(::std::addressof(storage.unity_color_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            ::std::destroy_at(::std::addressof(storage.pl_a_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            ::std::destroy_at(::std::addressof(storage.pl_experiment_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            ::std::destroy_at(::std::addressof(storage.pl_discussion_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_experiments: {
            ::std::destroy_at(::std::addressof(storage.pl_experiments_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_discussions: {
            ::std::destroy_at(::std::addressof(storage.pl_discussions_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            ::std::destroy_at(::std::addressof(storage.pl_user_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_trigger: {
            ::std::destroy_at(::std::addressof(storage.pl_trigger_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_internal: {
            ::std::destroy_at(::std::addressof(storage.pl_internal_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            ::std::destroy_at(::std::addressof(storage.pl_external_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_link: {
            ::std::destroy_at(::std::addressof(storage.unity_link_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_size: {
            ::std::destroy_at(::std::addressof(storage.unity_size_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_voffset: {
            ::std::destroy_at(::std::addressof(storage.unity_voffset_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_align: {
            ::std::destroy_at(::std::addressof(storage.unity_align_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_mark: {
            ::std::destroy_at(::std::addressof(storage.unity_mark_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_margin: {
            ::std::destroy_at(::std::addressof(storage.unity_margin_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_i: {
            ::std::destroy_at(::std::addressof(storage.unity_i_node));
            break;
        }
        case ::pltxt2htm::NodeKind::unity_b: {
            ::std::destroy_at(::std::addressof(storage.unity_b_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_u: {
            ::std::destroy_at(::std::addressof(storage.html_u_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_s: {
            ::std::destroy_at(::std::addressof(storage.html_s_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_project: {
            ::std::destroy_at(::std::addressof(storage.pl_macro_project_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_visitor: {
            ::std::destroy_at(::std::addressof(storage.pl_macro_visitor_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_author: {
            ::std::destroy_at(::std::addressof(storage.pl_macro_author_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
            ::std::destroy_at(::std::addressof(storage.pl_macro_coauthors_node));
            break;
        }
        case ::pltxt2htm::NodeKind::line_break: {
            ::std::destroy_at(::std::addressof(storage.line_break_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_br: {
            ::std::destroy_at(::std::addressof(storage.br_node));
            break;
        }
        case ::pltxt2htm::NodeKind::space: {
            ::std::destroy_at(::std::addressof(storage.space_node));
            break;
        }
        case ::pltxt2htm::NodeKind::less_than: {
            ::std::destroy_at(::std::addressof(storage.less_than_node));
            break;
        }
        case ::pltxt2htm::NodeKind::greater_than: {
            ::std::destroy_at(::std::addressof(storage.greater_than_node));
            break;
        }
        case ::pltxt2htm::NodeKind::tab: {
            ::std::destroy_at(::std::addressof(storage.tab_node));
            break;
        }
        case ::pltxt2htm::NodeKind::ampersand: {
            ::std::destroy_at(::std::addressof(storage.ampersand_node));
            break;
        }
        case ::pltxt2htm::NodeKind::single_quote: {
            ::std::destroy_at(::std::addressof(storage.single_quote_node));
            break;
        }
        case ::pltxt2htm::NodeKind::double_quote: {
            ::std::destroy_at(::std::addressof(storage.double_quote_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_hr: {
            ::std::destroy_at(::std::addressof(storage.html_hr_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            ::std::destroy_at(::std::addressof(storage.html_h1_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            ::std::destroy_at(::std::addressof(storage.html_h2_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            ::std::destroy_at(::std::addressof(storage.html_h3_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            ::std::destroy_at(::std::addressof(storage.html_h4_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            ::std::destroy_at(::std::addressof(storage.html_h5_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            ::std::destroy_at(::std::addressof(storage.html_h6_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            ::std::destroy_at(::std::addressof(storage.html_p_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_del: {
            ::std::destroy_at(::std::addressof(storage.html_del_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_sup: {
            ::std::destroy_at(::std::addressof(storage.html_sup_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_sub: {
            ::std::destroy_at(::std::addressof(storage.html_sub_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_note: {
            ::std::destroy_at(::std::addressof(storage.html_note_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_em: {
            ::std::destroy_at(::std::addressof(storage.html_em_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_strong: {
            ::std::destroy_at(::std::addressof(storage.html_strong_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_mark: {
            ::std::destroy_at(::std::addressof(storage.html_mark_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            ::std::destroy_at(::std::addressof(storage.html_span_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_div: {
            ::std::destroy_at(::std::addressof(storage.html_div_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_a: {
            ::std::destroy_at(::std::addressof(storage.html_a_node));
            break;
        }
        case ::pltxt2htm::NodeKind::list_ul: {
            ::std::destroy_at(::std::addressof(storage.list_ul_node));
            break;
        }
        case ::pltxt2htm::NodeKind::list_ol: {
            ::std::destroy_at(::std::addressof(storage.list_ol_node));
            break;
        }
        case ::pltxt2htm::NodeKind::list_li: {
            ::std::destroy_at(::std::addressof(storage.list_li_node));
            break;
        }
        case ::pltxt2htm::NodeKind::list_li_checkbox: {
            ::std::destroy_at(::std::addressof(storage.list_li_checkbox_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            ::std::destroy_at(::std::addressof(storage.html_blockquote_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            ::std::destroy_at(::std::addressof(storage.html_code_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_col: {
            ::std::destroy_at(::std::addressof(storage.col_node));
            break;
        }
        case ::pltxt2htm::NodeKind::html_img: {
            ::std::destroy_at(::std::addressof(storage.html_img_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table: {
            ::std::destroy_at(::std::addressof(storage.table_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_caption: {
            ::std::destroy_at(::std::addressof(storage.caption_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_colgroup: {
            ::std::destroy_at(::std::addressof(storage.colgroup_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_thead: {
            ::std::destroy_at(::std::addressof(storage.thead_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_tbody: {
            ::std::destroy_at(::std::addressof(storage.tbody_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_tfoot: {
            ::std::destroy_at(::std::addressof(storage.tfoot_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_tr: {
            ::std::destroy_at(::std::addressof(storage.tr_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_th: {
            ::std::destroy_at(::std::addressof(storage.th_node));
            break;
        }
        case ::pltxt2htm::NodeKind::table_td: {
            ::std::destroy_at(::std::addressof(storage.td_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h1: {
            ::std::destroy_at(::std::addressof(storage.md_atx_h1_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            ::std::destroy_at(::std::addressof(storage.md_atx_h2_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            ::std::destroy_at(::std::addressof(storage.md_atx_h3_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            ::std::destroy_at(::std::addressof(storage.md_atx_h4_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            ::std::destroy_at(::std::addressof(storage.md_atx_h5_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            ::std::destroy_at(::std::addressof(storage.md_atx_h6_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape: {
            ::std::destroy_at(::std::addressof(storage.md_escape_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_hr: {
            ::std::destroy_at(::std::addressof(storage.md_hr_node));
            break;
        }
        case ::pltxt2htm::NodeKind::code_fence: {
            ::std::destroy_at(::std::addressof(storage.code_fence_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
            ::std::destroy_at(::std::addressof(storage.md_code_span_1_backtick_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
            ::std::destroy_at(::std::addressof(storage.md_code_span_2_backtick_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            ::std::destroy_at(::std::addressof(storage.md_code_span_3_backtick_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
            ::std::destroy_at(::std::addressof(storage.md_single_emphasis_asterisk_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
            ::std::destroy_at(::std::addressof(storage.md_double_emphasis_asterisk_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            ::std::destroy_at(::std::addressof(storage.md_triple_emphasis_asterisk_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
            ::std::destroy_at(::std::addressof(storage.md_single_emphasis_underscore_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
            ::std::destroy_at(::std::addressof(storage.md_double_emphasis_underscore_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            ::std::destroy_at(::std::addressof(storage.md_triple_emphasis_underscore_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_del: {
            ::std::destroy_at(::std::addressof(storage.md_del_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            ::std::destroy_at(::std::addressof(storage.md_link_node));
            break;
        }
        case ::pltxt2htm::NodeKind::url: {
            ::std::destroy_at(::std::addressof(storage.url_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_image: {
            ::std::destroy_at(::std::addressof(storage.md_image_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            ::std::destroy_at(::std::addressof(storage.md_block_quotes_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_inline: {
            ::std::destroy_at(::std::addressof(storage.md_latex_inline_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_block: {
            ::std::destroy_at(::std::addressof(storage.md_latex_block_node));
            break;
        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in PlTxtNode destructor");
            }
#endif
        }
    }

    /**
     * @note: Mirrors the copy constructor: available to external users only, and disabled while
     *        building pltxt2htm itself (PLTXT2HTM_INTERNAL_USE). Self assignment returns early,
     *        because the assignment destroys the destination before reconstructing it.
     */
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::PlTxtNode<ndebug> const&) noexcept -> PlTxtNode& = delete;
#else
    constexpr auto operator=(this PlTxtNode<ndebug>& self, ::pltxt2htm::PlTxtNode<ndebug> const& other) noexcept
        -> PlTxtNode& {
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            return self;
        }
        ::std::destroy_at(::std::addressof(self));
        ::std::construct_at(::std::addressof(self), other);
        return self;
    }
#endif

    constexpr auto operator=(this PlTxtNode<ndebug>& self, ::pltxt2htm::PlTxtNode<ndebug>&& other) noexcept
        -> PlTxtNode& {
        pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"self assignment is not allowed");
        ::std::destroy_at(::std::addressof(self));
        ::std::construct_at(::std::addressof(self), ::std::move(other));
        return self;
    }

    [[nodiscard]]
    constexpr auto operator==(this PlTxtNode<ndebug> const& self, ::pltxt2htm::PlTxtNode<ndebug> const& other) noexcept
        -> bool {
        if (self.node_kind != other.node_kind) {
            return false;
        }
        switch (self.node_kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char: {
            return self.storage.u8char_node == other.storage.u8char_node;
        }
        case ::pltxt2htm::NodeKind::invalid_utf8: {
            return self.storage.invalid_utf8_node == other.storage.invalid_utf8_node;
        }
        case ::pltxt2htm::NodeKind::group: {
            return self.storage.group_node == other.storage.group_node;
        }
        case ::pltxt2htm::NodeKind::unity_color: {
            return self.storage.unity_color_node == other.storage.unity_color_node;
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            return self.storage.pl_a_node == other.storage.pl_a_node;
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            return self.storage.pl_experiment_node == other.storage.pl_experiment_node;
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            return self.storage.pl_discussion_node == other.storage.pl_discussion_node;
        }
        case ::pltxt2htm::NodeKind::pl_experiments: {
            return self.storage.pl_experiments_node == other.storage.pl_experiments_node;
        }
        case ::pltxt2htm::NodeKind::pl_discussions: {
            return self.storage.pl_discussions_node == other.storage.pl_discussions_node;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            return self.storage.pl_user_node == other.storage.pl_user_node;
        }
        case ::pltxt2htm::NodeKind::pl_trigger: {
            return self.storage.pl_trigger_node == other.storage.pl_trigger_node;
        }
        case ::pltxt2htm::NodeKind::pl_internal: {
            return self.storage.pl_internal_node == other.storage.pl_internal_node;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            return self.storage.pl_external_node == other.storage.pl_external_node;
        }
        case ::pltxt2htm::NodeKind::unity_link: {
            return self.storage.unity_link_node == other.storage.unity_link_node;
        }
        case ::pltxt2htm::NodeKind::unity_size: {
            return self.storage.unity_size_node == other.storage.unity_size_node;
        }
        case ::pltxt2htm::NodeKind::unity_voffset: {
            return self.storage.unity_voffset_node == other.storage.unity_voffset_node;
        }
        case ::pltxt2htm::NodeKind::unity_align: {
            return self.storage.unity_align_node == other.storage.unity_align_node;
        }
        case ::pltxt2htm::NodeKind::unity_mark: {
            return self.storage.unity_mark_node == other.storage.unity_mark_node;
        }
        case ::pltxt2htm::NodeKind::unity_margin: {
            return self.storage.unity_margin_node == other.storage.unity_margin_node;
        }
        case ::pltxt2htm::NodeKind::unity_i: {
            return self.storage.unity_i_node == other.storage.unity_i_node;
        }
        case ::pltxt2htm::NodeKind::unity_b: {
            return self.storage.unity_b_node == other.storage.unity_b_node;
        }
        case ::pltxt2htm::NodeKind::html_u: {
            return self.storage.html_u_node == other.storage.html_u_node;
        }
        case ::pltxt2htm::NodeKind::html_s: {
            return self.storage.html_s_node == other.storage.html_s_node;
        }
        case ::pltxt2htm::NodeKind::pl_macro_project: {
            return self.storage.pl_macro_project_node == other.storage.pl_macro_project_node;
        }
        case ::pltxt2htm::NodeKind::pl_macro_visitor: {
            return self.storage.pl_macro_visitor_node == other.storage.pl_macro_visitor_node;
        }
        case ::pltxt2htm::NodeKind::pl_macro_author: {
            return self.storage.pl_macro_author_node == other.storage.pl_macro_author_node;
        }
        case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
            return self.storage.pl_macro_coauthors_node == other.storage.pl_macro_coauthors_node;
        }
        case ::pltxt2htm::NodeKind::line_break: {
            return self.storage.line_break_node == other.storage.line_break_node;
        }
        case ::pltxt2htm::NodeKind::html_br: {
            return self.storage.br_node == other.storage.br_node;
        }
        case ::pltxt2htm::NodeKind::space: {
            return self.storage.space_node == other.storage.space_node;
        }
        case ::pltxt2htm::NodeKind::less_than: {
            return self.storage.less_than_node == other.storage.less_than_node;
        }
        case ::pltxt2htm::NodeKind::greater_than: {
            return self.storage.greater_than_node == other.storage.greater_than_node;
        }
        case ::pltxt2htm::NodeKind::tab: {
            return self.storage.tab_node == other.storage.tab_node;
        }
        case ::pltxt2htm::NodeKind::ampersand: {
            return self.storage.ampersand_node == other.storage.ampersand_node;
        }
        case ::pltxt2htm::NodeKind::single_quote: {
            return self.storage.single_quote_node == other.storage.single_quote_node;
        }
        case ::pltxt2htm::NodeKind::double_quote: {
            return self.storage.double_quote_node == other.storage.double_quote_node;
        }
        case ::pltxt2htm::NodeKind::html_hr: {
            return self.storage.html_hr_node == other.storage.html_hr_node;
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            return self.storage.html_h1_node == other.storage.html_h1_node;
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            return self.storage.html_h2_node == other.storage.html_h2_node;
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            return self.storage.html_h3_node == other.storage.html_h3_node;
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            return self.storage.html_h4_node == other.storage.html_h4_node;
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            return self.storage.html_h5_node == other.storage.html_h5_node;
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            return self.storage.html_h6_node == other.storage.html_h6_node;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            return self.storage.html_p_node == other.storage.html_p_node;
        }
        case ::pltxt2htm::NodeKind::html_del: {
            return self.storage.html_del_node == other.storage.html_del_node;
        }
        case ::pltxt2htm::NodeKind::html_sup: {
            return self.storage.html_sup_node == other.storage.html_sup_node;
        }
        case ::pltxt2htm::NodeKind::html_sub: {
            return self.storage.html_sub_node == other.storage.html_sub_node;
        }
        case ::pltxt2htm::NodeKind::html_note: {
            return self.storage.html_note_node == other.storage.html_note_node;
        }
        case ::pltxt2htm::NodeKind::html_em: {
            return self.storage.html_em_node == other.storage.html_em_node;
        }
        case ::pltxt2htm::NodeKind::html_strong: {
            return self.storage.html_strong_node == other.storage.html_strong_node;
        }
        case ::pltxt2htm::NodeKind::html_mark: {
            return self.storage.html_mark_node == other.storage.html_mark_node;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            return self.storage.html_span_node == other.storage.html_span_node;
        }
        case ::pltxt2htm::NodeKind::html_div: {
            return self.storage.html_div_node == other.storage.html_div_node;
        }
        case ::pltxt2htm::NodeKind::html_a: {
            return self.storage.html_a_node == other.storage.html_a_node;
        }
        case ::pltxt2htm::NodeKind::list_ul: {
            return self.storage.list_ul_node == other.storage.list_ul_node;
        }
        case ::pltxt2htm::NodeKind::list_ol: {
            return self.storage.list_ol_node == other.storage.list_ol_node;
        }
        case ::pltxt2htm::NodeKind::list_li: {
            return self.storage.list_li_node == other.storage.list_li_node;
        }
        case ::pltxt2htm::NodeKind::list_li_checkbox: {
            return self.storage.list_li_checkbox_node == other.storage.list_li_checkbox_node;
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            return self.storage.html_blockquote_node == other.storage.html_blockquote_node;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            return self.storage.html_code_node == other.storage.html_code_node;
        }
        case ::pltxt2htm::NodeKind::table_col: {
            return self.storage.col_node == other.storage.col_node;
        }
        case ::pltxt2htm::NodeKind::html_img: {
            return self.storage.html_img_node == other.storage.html_img_node;
        }
        case ::pltxt2htm::NodeKind::table: {
            return self.storage.table_node == other.storage.table_node;
        }
        case ::pltxt2htm::NodeKind::table_caption: {
            return self.storage.caption_node == other.storage.caption_node;
        }
        case ::pltxt2htm::NodeKind::table_colgroup: {
            return self.storage.colgroup_node == other.storage.colgroup_node;
        }
        case ::pltxt2htm::NodeKind::table_thead: {
            return self.storage.thead_node == other.storage.thead_node;
        }
        case ::pltxt2htm::NodeKind::table_tbody: {
            return self.storage.tbody_node == other.storage.tbody_node;
        }
        case ::pltxt2htm::NodeKind::table_tfoot: {
            return self.storage.tfoot_node == other.storage.tfoot_node;
        }
        case ::pltxt2htm::NodeKind::table_tr: {
            return self.storage.tr_node == other.storage.tr_node;
        }
        case ::pltxt2htm::NodeKind::table_th: {
            return self.storage.th_node == other.storage.th_node;
        }
        case ::pltxt2htm::NodeKind::table_td: {
            return self.storage.td_node == other.storage.td_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h1: {
            return self.storage.md_atx_h1_node == other.storage.md_atx_h1_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            return self.storage.md_atx_h2_node == other.storage.md_atx_h2_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            return self.storage.md_atx_h3_node == other.storage.md_atx_h3_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            return self.storage.md_atx_h4_node == other.storage.md_atx_h4_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            return self.storage.md_atx_h5_node == other.storage.md_atx_h5_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            return self.storage.md_atx_h6_node == other.storage.md_atx_h6_node;
        }
        case ::pltxt2htm::NodeKind::md_escape: {
            return self.storage.md_escape_node == other.storage.md_escape_node;
        }
        case ::pltxt2htm::NodeKind::md_hr: {
            return self.storage.md_hr_node == other.storage.md_hr_node;
        }
        case ::pltxt2htm::NodeKind::code_fence: {
            return self.storage.code_fence_node == other.storage.code_fence_node;
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
            return self.storage.md_code_span_1_backtick_node == other.storage.md_code_span_1_backtick_node;
        }
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
            return self.storage.md_code_span_2_backtick_node == other.storage.md_code_span_2_backtick_node;
        }
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            return self.storage.md_code_span_3_backtick_node == other.storage.md_code_span_3_backtick_node;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
            return self.storage.md_single_emphasis_asterisk_node == other.storage.md_single_emphasis_asterisk_node;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
            return self.storage.md_double_emphasis_asterisk_node == other.storage.md_double_emphasis_asterisk_node;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            return self.storage.md_triple_emphasis_asterisk_node == other.storage.md_triple_emphasis_asterisk_node;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
            return self.storage.md_single_emphasis_underscore_node == other.storage.md_single_emphasis_underscore_node;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
            return self.storage.md_double_emphasis_underscore_node == other.storage.md_double_emphasis_underscore_node;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            return self.storage.md_triple_emphasis_underscore_node == other.storage.md_triple_emphasis_underscore_node;
        }
        case ::pltxt2htm::NodeKind::md_del: {
            return self.storage.md_del_node == other.storage.md_del_node;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            return self.storage.md_link_node == other.storage.md_link_node;
        }
        case ::pltxt2htm::NodeKind::url: {
            return self.storage.url_node == other.storage.url_node;
        }
        case ::pltxt2htm::NodeKind::md_image: {
            return self.storage.md_image_node == other.storage.md_image_node;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            return self.storage.md_block_quotes_node == other.storage.md_block_quotes_node;
        }
        case ::pltxt2htm::NodeKind::md_latex_inline: {
            return self.storage.md_latex_inline_node == other.storage.md_latex_inline_node;
        }
        case ::pltxt2htm::NodeKind::md_latex_block: {
            return self.storage.md_latex_block_node == other.storage.md_latex_block_node;
        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in PlTxtNode operator==");
            }
#endif
        }
        // reduce MSVC return-path warning
        pltxt2htm_unreachable(u8"Unreachable after PlTxtNode operator== switch");
    }

    /// @name as_xxx() accessors - one per union member type
    /// @{

    [[nodiscard]]
    constexpr auto as_u8char(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::u8char, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.u8char_node);
    }

    [[nodiscard]]
    constexpr auto as_invalid_utf8(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::invalid_utf8, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.invalid_utf8_node);
    }

    [[nodiscard]]
    constexpr auto as_group(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::group, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.group_node);
    }

    [[nodiscard]]
    constexpr auto as_line_break(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::line_break, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.line_break_node);
    }

    [[nodiscard]]
    constexpr auto as_html_br(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_br, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.br_node);
    }

    [[nodiscard]]
    constexpr auto as_space(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::space, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.space_node);
    }

    [[nodiscard]]
    constexpr auto as_less_than(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::less_than, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.less_than_node);
    }

    [[nodiscard]]
    constexpr auto as_greater_than(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::greater_than, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.greater_than_node);
    }

    [[nodiscard]]
    constexpr auto as_tab(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::tab, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.tab_node);
    }

    [[nodiscard]]
    constexpr auto as_ampersand(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::ampersand, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.ampersand_node);
    }

    [[nodiscard]]
    constexpr auto as_single_quote(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::single_quote, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.single_quote_node);
    }

    [[nodiscard]]
    constexpr auto as_double_quote(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::double_quote, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.double_quote_node);
    }

    [[nodiscard]]
    constexpr auto as_html_hr(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_hr, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_hr_node);
    }

    [[nodiscard]]
    constexpr auto as_html_h1(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_h1, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_h1_node);
    }

    [[nodiscard]]
    constexpr auto as_html_h2(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_h2, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_h2_node);
    }

    [[nodiscard]]
    constexpr auto as_html_h3(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_h3, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_h3_node);
    }

    [[nodiscard]]
    constexpr auto as_html_h4(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_h4, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_h4_node);
    }

    [[nodiscard]]
    constexpr auto as_html_h5(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_h5, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_h5_node);
    }

    [[nodiscard]]
    constexpr auto as_html_h6(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_h6, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_h6_node);
    }

    [[nodiscard]]
    constexpr auto as_html_p(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_p, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_p_node);
    }

    [[nodiscard]]
    constexpr auto as_html_del(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_del, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_del_node);
    }

    [[nodiscard]]
    constexpr auto as_html_sup(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_sup, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_sup_node);
    }

    [[nodiscard]]
    constexpr auto as_html_sub(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_sub, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_sub_node);
    }

    [[nodiscard]]
    constexpr auto as_html_note(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_note, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_note_node);
    }

    [[nodiscard]]
    constexpr auto as_html_em(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_em, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_em_node);
    }

    [[nodiscard]]
    constexpr auto as_html_strong(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_strong, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_strong_node);
    }

    [[nodiscard]]
    constexpr auto as_html_mark(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_mark, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_mark_node);
    }

    [[nodiscard]]
    constexpr auto as_list_ul(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::list_ul, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.list_ul_node);
    }

    [[nodiscard]]
    constexpr auto as_list_ol(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::list_ol, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.list_ol_node);
    }

    [[nodiscard]]
    constexpr auto as_list_li(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::list_li, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.list_li_node);
    }

    [[nodiscard]]
    constexpr auto as_list_li_checkbox(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::list_li_checkbox, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.list_li_checkbox_node);
    }

    [[nodiscard]]
    constexpr auto as_html_code(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_code, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_code_node);
    }

    [[nodiscard]]
    constexpr auto as_html_blockquote(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_blockquote, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_blockquote_node);
    }

    [[nodiscard]]
    constexpr auto as_table_col(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::table_col, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.col_node);
    }

    [[nodiscard]]
    constexpr auto as_html_img(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_img, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_img_node);
    }

    [[nodiscard]]
    constexpr auto as_table(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::table, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.table_node);
    }

    [[nodiscard]]
    constexpr auto as_table_caption(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::table_caption, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.caption_node);
    }

    [[nodiscard]]
    constexpr auto as_table_colgroup(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::table_colgroup, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.colgroup_node);
    }

    [[nodiscard]]
    constexpr auto as_table_thead(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::table_thead, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.thead_node);
    }

    [[nodiscard]]
    constexpr auto as_table_tbody(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::table_tbody, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.tbody_node);
    }

    [[nodiscard]]
    constexpr auto as_table_tfoot(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::table_tfoot, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.tfoot_node);
    }

    [[nodiscard]]
    constexpr auto as_table_tr(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::table_tr, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.tr_node);
    }

    [[nodiscard]]
    constexpr auto as_table_th(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::table_th, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.th_node);
    }

    [[nodiscard]]
    constexpr auto as_table_td(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::table_td, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.td_node);
    }

    [[nodiscard]]
    constexpr auto as_md_atx_h1(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_atx_h1, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_atx_h1_node);
    }

    [[nodiscard]]
    constexpr auto as_md_atx_h2(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_atx_h2, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_atx_h2_node);
    }

    [[nodiscard]]
    constexpr auto as_md_atx_h3(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_atx_h3, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_atx_h3_node);
    }

    [[nodiscard]]
    constexpr auto as_md_atx_h4(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_atx_h4, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_atx_h4_node);
    }

    [[nodiscard]]
    constexpr auto as_md_atx_h5(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_atx_h5, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_atx_h5_node);
    }

    [[nodiscard]]
    constexpr auto as_md_atx_h6(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_atx_h6, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_atx_h6_node);
    }

    [[nodiscard]]
    constexpr auto as_md_escape(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_escape, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_escape_node);
    }

    [[nodiscard]]
    constexpr auto as_md_hr(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_hr, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_hr_node);
    }

    [[nodiscard]]
    constexpr auto as_code_fence(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::code_fence, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.code_fence_node);
    }

    [[nodiscard]]
    constexpr auto as_md_code_span_1_backtick(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_code_span_1_backtick, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_code_span_1_backtick_node);
    }

    [[nodiscard]]
    constexpr auto as_md_code_span_2_backtick(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_code_span_2_backtick, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_code_span_2_backtick_node);
    }

    [[nodiscard]]
    constexpr auto as_md_code_span_3_backtick(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_code_span_3_backtick, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_code_span_3_backtick_node);
    }

    [[nodiscard]]
    constexpr auto as_md_single_emphasis_asterisk(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_single_emphasis_asterisk, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_single_emphasis_asterisk_node);
    }

    [[nodiscard]]
    constexpr auto as_md_double_emphasis_asterisk(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_double_emphasis_asterisk, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_double_emphasis_asterisk_node);
    }

    [[nodiscard]]
    constexpr auto as_md_triple_emphasis_asterisk(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_triple_emphasis_asterisk_node);
    }

    [[nodiscard]]
    constexpr auto as_md_single_emphasis_underscore(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_single_emphasis_underscore,
                         u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_single_emphasis_underscore_node);
    }

    [[nodiscard]]
    constexpr auto as_md_double_emphasis_underscore(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_double_emphasis_underscore,
                         u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_double_emphasis_underscore_node);
    }

    [[nodiscard]]
    constexpr auto as_md_triple_emphasis_underscore(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_triple_emphasis_underscore,
                         u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_triple_emphasis_underscore_node);
    }

    [[nodiscard]]
    constexpr auto as_md_del(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_del, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_del_node);
    }

    [[nodiscard]]
    constexpr auto as_md_link(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_link, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_link_node);
    }

    [[nodiscard]]
    constexpr auto as_url(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::url, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.url_node);
    }

    [[nodiscard]]
    constexpr auto as_md_image(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_image, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_image_node);
    }

    [[nodiscard]]
    constexpr auto as_md_block_quotes(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_block_quotes, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_block_quotes_node);
    }

    [[nodiscard]]
    constexpr auto as_html_span(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_span, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_span_node);
    }

    [[nodiscard]]
    constexpr auto as_html_div(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_div, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_div_node);
    }

    [[nodiscard]]
    constexpr auto as_html_a(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_a, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_a_node);
    }

    [[nodiscard]]
    constexpr auto as_md_latex_inline(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_latex_inline, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_latex_inline_node);
    }

    [[nodiscard]]
    constexpr auto as_md_latex_block(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::md_latex_block, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.md_latex_block_node);
    }

    [[nodiscard]]
    constexpr auto as_unity_color(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::unity_color, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.unity_color_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_a(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_a, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_a_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_experiment(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_experiment, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_experiment_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_discussion(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_discussion, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_discussion_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_experiments(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_experiments, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_experiments_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_discussions(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_discussions, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_discussions_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_user(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_user, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_user_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_trigger(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_trigger, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_trigger_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_internal(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_internal, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_internal_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_external(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_external, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_external_node);
    }

    [[nodiscard]]
    constexpr auto as_unity_link(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::unity_link, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.unity_link_node);
    }

    [[nodiscard]]
    constexpr auto as_unity_size(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::unity_size, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.unity_size_node);
    }

    [[nodiscard]]
    constexpr auto as_unity_voffset(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::unity_voffset, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.unity_voffset_node);
    }

    [[nodiscard]]
    constexpr auto as_unity_align(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::unity_align, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.unity_align_node);
    }

    [[nodiscard]]
    constexpr auto as_unity_mark(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::unity_mark, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.unity_mark_node);
    }

    [[nodiscard]]
    constexpr auto as_unity_margin(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::unity_margin, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.unity_margin_node);
    }

    [[nodiscard]]
    constexpr auto as_unity_i(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::unity_i, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.unity_i_node);
    }

    [[nodiscard]]
    constexpr auto as_unity_b(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::unity_b, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.unity_b_node);
    }

    [[nodiscard]]
    constexpr auto as_html_u(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_u, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_u_node);
    }

    [[nodiscard]]
    constexpr auto as_html_s(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::html_s, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.html_s_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_macro_project(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_macro_project, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_macro_project_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_macro_visitor(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_macro_visitor, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_macro_visitor_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_macro_author(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_macro_author, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_macro_author_node);
    }

    [[nodiscard]]
    constexpr auto as_pl_macro_coauthors(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.node_kind == ::pltxt2htm::NodeKind::pl_macro_coauthors, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.storage.pl_macro_coauthors_node);
    }

    /// @}

    [[nodiscard]]
    constexpr auto get_node_kind(this PlTxtNode const& self) noexcept -> ::pltxt2htm::NodeKind {
        return self.node_kind;
    }
};

} // namespace pltxt2htm

#include "../details/pop_macro.hh"
#include "impl/basic_node_def.inc"
#include "impl/html_node_def.inc"
#include "impl/list_node_def.inc"
#include "impl/markdown_node_def.inc"
#include "impl/physics_lab_node_def.inc"
#include "impl/table_node_def.inc"
#include "impl/unity_node_def.inc"
