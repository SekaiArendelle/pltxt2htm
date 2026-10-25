/**
 * @file pltxt_node_traits.hh
 * @brief Storage and concrete-type metadata for ::pltxt2htm::PlTxtNode.
 */

#pragma once

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

#include "../../contracts.hh"
#include "../node_kind.hh"
#include "basic_node_decl.hh"
#include "html_node_decl.hh"
#include "list_node_decl.hh"
#include "markdown_node_decl.hh"
#include "physics_lab_node_decl.hh"
#include "table_node_decl.hh"
#include "text_node_decl.hh"
#include "unity_node_decl.hh"
#include "url_node_decl.hh"

namespace pltxt2htm::details {

union PlTxtNodeStorage {
    // basic node
    ::pltxt2htm::Text text_node;
    ::pltxt2htm::InvalidUtf8 invalid_utf8_node;
    ::pltxt2htm::Group group_node;

    // html node
    ::pltxt2htm::LineBreak line_break_node;
    ::pltxt2htm::HtmlBr br_node;
    ::pltxt2htm::Space space_node;
    ::pltxt2htm::LessThan less_than_node;
    ::pltxt2htm::GreaterThan greater_than_node;
    ::pltxt2htm::Tab tab_node;
    ::pltxt2htm::Ampersand ampersand_node;
    ::pltxt2htm::SingleQuote single_quote_node;
    ::pltxt2htm::DoubleQuote double_quote_node;
    ::pltxt2htm::HtmlHr html_hr_node;
    ::pltxt2htm::HtmlH1 html_h1_node;
    ::pltxt2htm::HtmlH2 html_h2_node;
    ::pltxt2htm::HtmlH3 html_h3_node;
    ::pltxt2htm::HtmlH4 html_h4_node;
    ::pltxt2htm::HtmlH5 html_h5_node;
    ::pltxt2htm::HtmlH6 html_h6_node;
    ::pltxt2htm::HtmlP html_p_node;
    ::pltxt2htm::HtmlDel html_del_node;
    ::pltxt2htm::HtmlU html_u_node;
    ::pltxt2htm::HtmlS html_s_node;
    ::pltxt2htm::HtmlSup html_sup_node;
    ::pltxt2htm::HtmlSub html_sub_node;
    ::pltxt2htm::HtmlNote html_note_node;
    ::pltxt2htm::HtmlEm html_em_node;
    ::pltxt2htm::HtmlStrong html_strong_node;
    ::pltxt2htm::HtmlMark html_mark_node;
    ::pltxt2htm::HtmlSpan html_span_node;
    ::pltxt2htm::HtmlDiv html_div_node;
    ::pltxt2htm::HtmlA html_a_node;
    ::pltxt2htm::ListUl list_ul_node;
    ::pltxt2htm::ListOl list_ol_node;
    ::pltxt2htm::ListLi list_li_node;
    ::pltxt2htm::ListLiCheckbox list_li_checkbox_node;
    ::pltxt2htm::HtmlCode html_code_node;
    ::pltxt2htm::HtmlBlockquote html_blockquote_node;

    // html img node
    ::pltxt2htm::HtmlImg html_img_node;

    // table node
    ::pltxt2htm::TableCol col_node;
    ::pltxt2htm::Table table_node;
    ::pltxt2htm::TableTr tr_node;
    ::pltxt2htm::TableTd td_node;
    ::pltxt2htm::TableTh th_node;
    ::pltxt2htm::TableThead thead_node;
    ::pltxt2htm::TableTbody tbody_node;
    ::pltxt2htm::TableTfoot tfoot_node;
    ::pltxt2htm::TableCaption caption_node;
    ::pltxt2htm::TableColgroup colgroup_node;

    // markdown node
    ::pltxt2htm::MdAtxH1 md_atx_h1_node;
    ::pltxt2htm::MdAtxH2 md_atx_h2_node;
    ::pltxt2htm::MdAtxH3 md_atx_h3_node;
    ::pltxt2htm::MdAtxH4 md_atx_h4_node;
    ::pltxt2htm::MdAtxH5 md_atx_h5_node;
    ::pltxt2htm::MdAtxH6 md_atx_h6_node;
    ::pltxt2htm::MdEscape md_escape_node;
    ::pltxt2htm::MdHr md_hr_node;
    ::pltxt2htm::CodeFence code_fence_node;
    ::pltxt2htm::MdCodeSpan1Backtick md_code_span_1_backtick_node;
    ::pltxt2htm::MdCodeSpan2Backtick md_code_span_2_backtick_node;
    ::pltxt2htm::MdCodeSpan3Backtick md_code_span_3_backtick_node;
    ::pltxt2htm::MdSingleEmphasisAsterisk md_single_emphasis_asterisk_node;
    ::pltxt2htm::MdDoubleEmphasisAsterisk md_double_emphasis_asterisk_node;
    ::pltxt2htm::MdTripleEmphasisAsterisk md_triple_emphasis_asterisk_node;
    ::pltxt2htm::MdSingleEmphasisUnderscore md_single_emphasis_underscore_node;
    ::pltxt2htm::MdDoubleEmphasisUnderscore md_double_emphasis_underscore_node;
    ::pltxt2htm::MdTripleEmphasisUnderscore md_triple_emphasis_underscore_node;
    ::pltxt2htm::MdDel md_del_node;
    ::pltxt2htm::MdLink md_link_node;
    ::pltxt2htm::Url url_node;
    ::pltxt2htm::MdImage md_image_node;
    ::pltxt2htm::MdBlockQuotes md_block_quotes_node;
    ::pltxt2htm::MdLatexInline md_latex_inline_node;
    ::pltxt2htm::MdLatexBlock md_latex_block_node;

    // Physics-Lab and Unity rich-text nodes
    ::pltxt2htm::UnityColor unity_color_node;
    ::pltxt2htm::PlA pl_a_node;
    ::pltxt2htm::PlExperiment pl_experiment_node;
    ::pltxt2htm::PlDiscussion pl_discussion_node;
    ::pltxt2htm::PlExperiments pl_experiments_node;
    ::pltxt2htm::PlDiscussions pl_discussions_node;
    ::pltxt2htm::PlUser pl_user_node;
    ::pltxt2htm::PlTrigger pl_trigger_node;
    ::pltxt2htm::PlInternal pl_internal_node;
    ::pltxt2htm::PlExternal pl_external_node;
    ::pltxt2htm::UnityLink unity_link_node;
    ::pltxt2htm::UnitySize unity_size_node;
    ::pltxt2htm::UnityVoffset unity_voffset_node;
    ::pltxt2htm::UnityAlign unity_align_node;
    ::pltxt2htm::UnityMark unity_mark_node;
    ::pltxt2htm::UnityMargin unity_margin_node;
    ::pltxt2htm::UnityI unity_i_node;
    ::pltxt2htm::UnityB unity_b_node;
    ::pltxt2htm::PlMacroProject pl_macro_project_node;
    ::pltxt2htm::PlMacroVisitor pl_macro_visitor_node;
    ::pltxt2htm::PlMacroAuthor pl_macro_author_node;
    ::pltxt2htm::PlMacroCoauthors pl_macro_coauthors_node;

    constexpr PlTxtNodeStorage() noexcept {
    }

    constexpr ~PlTxtNodeStorage() noexcept {
    }
};

template<typename Node>
struct PlTxtNodeTraits;

template<>
struct PlTxtNodeTraits< ::pltxt2htm::Text> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::text};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::text_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::InvalidUtf8> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::invalid_utf8};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::invalid_utf8_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::Group> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::group};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::group_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::UnityColor> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_color};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::unity_color_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlA> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_a};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_a_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlExperiment> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_experiment};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_experiment_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlDiscussion> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_discussion};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_discussion_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlExperiments> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_experiments};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_experiments_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlDiscussions> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_discussions};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_discussions_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlUser> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_user};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_user_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlTrigger> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_trigger};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_trigger_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlInternal> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_internal};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_internal_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlExternal> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_external};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_external_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::UnityLink> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_link};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::unity_link_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::UnitySize> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_size};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::unity_size_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::UnityVoffset> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_voffset};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::unity_voffset_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::UnityAlign> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_align};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::unity_align_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::UnityMark> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_mark};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::unity_mark_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::UnityMargin> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_margin};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::unity_margin_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::UnityI> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_i};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::unity_i_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::UnityB> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_b};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::unity_b_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlU> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_u};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_u_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlS> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_s};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_s_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlMacroProject> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_macro_project};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_macro_project_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlMacroVisitor> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_macro_visitor};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_macro_visitor_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlMacroAuthor> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_macro_author};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_macro_author_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::PlMacroCoauthors> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_macro_coauthors};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::pl_macro_coauthors_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::LineBreak> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::line_break};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::line_break_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlBr> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_br};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::br_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::Space> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::space};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::space_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::LessThan> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::less_than};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::less_than_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::GreaterThan> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::greater_than};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::greater_than_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::Tab> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::tab};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::tab_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::Ampersand> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::ampersand};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::ampersand_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::SingleQuote> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::single_quote};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::single_quote_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::DoubleQuote> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::double_quote};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::double_quote_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlHr> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_hr};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_hr_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlH1> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h1};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_h1_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlH2> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h2};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_h2_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlH3> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h3};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_h3_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlH4> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h4};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_h4_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlH5> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h5};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_h5_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlH6> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h6};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_h6_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlP> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_p};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_p_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlDel> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_del};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_del_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlCode> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_code};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_code_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlSup> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_sup};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_sup_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlSub> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_sub};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_sub_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlNote> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_note};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_note_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlEm> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_em};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_em_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlStrong> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_strong};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_strong_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlMark> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_mark};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_mark_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlSpan> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_span};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_span_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlDiv> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_div};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_div_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlA> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_a};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_a_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::ListUl> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::list_ul};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::list_ul_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::ListOl> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::list_ol};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::list_ol_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::ListLi> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::list_li};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::list_li_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::ListLiCheckbox> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::list_li_checkbox};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::list_li_checkbox_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlBlockquote> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_blockquote};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_blockquote_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::TableCol> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_col};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::col_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::HtmlImg> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_img};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::html_img_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::Table> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::table_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::TableCaption> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_caption};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::caption_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::TableColgroup> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_colgroup};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::colgroup_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::TableThead> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_thead};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::thead_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::TableTbody> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_tbody};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::tbody_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::TableTfoot> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_tfoot};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::tfoot_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::TableTr> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_tr};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::tr_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::TableTh> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_th};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::th_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::TableTd> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_td};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::td_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdAtxH1> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h1};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_atx_h1_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdAtxH2> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h2};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_atx_h2_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdAtxH3> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h3};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_atx_h3_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdAtxH4> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h4};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_atx_h4_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdAtxH5> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h5};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_atx_h5_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdAtxH6> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h6};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_atx_h6_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdEscape> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_escape};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_escape_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdHr> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_hr};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_hr_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::CodeFence> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::code_fence};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::code_fence_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdCodeSpan1Backtick> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_code_span_1_backtick};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_code_span_1_backtick_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdCodeSpan2Backtick> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_code_span_2_backtick};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_code_span_2_backtick_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdCodeSpan3Backtick> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_code_span_3_backtick};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_code_span_3_backtick_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdSingleEmphasisAsterisk> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_single_emphasis_asterisk};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_single_emphasis_asterisk_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdDoubleEmphasisAsterisk> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_double_emphasis_asterisk};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_double_emphasis_asterisk_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdTripleEmphasisAsterisk> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_triple_emphasis_asterisk};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_triple_emphasis_asterisk_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdSingleEmphasisUnderscore> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_single_emphasis_underscore};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_single_emphasis_underscore_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdDoubleEmphasisUnderscore> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_double_emphasis_underscore};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_double_emphasis_underscore_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdTripleEmphasisUnderscore> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_triple_emphasis_underscore};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_triple_emphasis_underscore_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdDel> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_del};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_del_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdLink> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_link};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_link_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::Url> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::url};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::url_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdImage> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_image};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_image_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdBlockQuotes> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_block_quotes};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_block_quotes_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdLatexInline> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_latex_inline};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_latex_inline_node;
};

template<>
struct PlTxtNodeTraits< ::pltxt2htm::MdLatexBlock> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_latex_block};
    static constexpr auto member = &::pltxt2htm::details::PlTxtNodeStorage::md_latex_block_node;
};

template<typename Node>
concept PlTxtNodeConcept = requires {
    requires ::std::same_as<Node, ::std::remove_cvref_t<Node>>;
    requires ::std::same_as<::pltxt2htm::NodeKind,
                            ::std::remove_cvref_t<decltype(::pltxt2htm::details::PlTxtNodeTraits< Node>::kind)>>;
    requires ::std::is_member_object_pointer_v<
        ::std::remove_cvref_t<decltype(::pltxt2htm::details::PlTxtNodeTraits< Node>::member)>>;
};

} // namespace pltxt2htm::details
