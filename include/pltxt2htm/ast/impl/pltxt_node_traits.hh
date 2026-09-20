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
#include "unity_node_decl.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::Contracts ndebug>
union PlTxtNodeStorage {
    // basic node
    ::pltxt2htm::Text<ndebug> text_node;
    ::pltxt2htm::InvalidUtf8 invalid_utf8_node;
    ::pltxt2htm::Group<ndebug> group_node;

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
    ::pltxt2htm::HtmlH1<ndebug> html_h1_node;
    ::pltxt2htm::HtmlH2<ndebug> html_h2_node;
    ::pltxt2htm::HtmlH3<ndebug> html_h3_node;
    ::pltxt2htm::HtmlH4<ndebug> html_h4_node;
    ::pltxt2htm::HtmlH5<ndebug> html_h5_node;
    ::pltxt2htm::HtmlH6<ndebug> html_h6_node;
    ::pltxt2htm::HtmlP<ndebug> html_p_node;
    ::pltxt2htm::HtmlDel<ndebug> html_del_node;
    ::pltxt2htm::HtmlU<ndebug> html_u_node;
    ::pltxt2htm::HtmlS<ndebug> html_s_node;
    ::pltxt2htm::HtmlSup<ndebug> html_sup_node;
    ::pltxt2htm::HtmlSub<ndebug> html_sub_node;
    ::pltxt2htm::HtmlNote<ndebug> html_note_node;
    ::pltxt2htm::HtmlEm<ndebug> html_em_node;
    ::pltxt2htm::HtmlStrong<ndebug> html_strong_node;
    ::pltxt2htm::HtmlMark<ndebug> html_mark_node;
    ::pltxt2htm::HtmlSpan<ndebug> html_span_node;
    ::pltxt2htm::HtmlDiv<ndebug> html_div_node;
    ::pltxt2htm::HtmlA<ndebug> html_a_node;
    ::pltxt2htm::ListUl<ndebug> list_ul_node;
    ::pltxt2htm::ListOl<ndebug> list_ol_node;
    ::pltxt2htm::ListLi<ndebug> list_li_node;
    ::pltxt2htm::ListLiCheckbox<ndebug> list_li_checkbox_node;
    ::pltxt2htm::HtmlCode<ndebug> html_code_node;
    ::pltxt2htm::HtmlBlockquote<ndebug> html_blockquote_node;

    // html img node
    ::pltxt2htm::HtmlImg html_img_node;

    // table node
    ::pltxt2htm::TableCol col_node;
    ::pltxt2htm::Table<ndebug> table_node;
    ::pltxt2htm::TableTr<ndebug> tr_node;
    ::pltxt2htm::TableTd<ndebug> td_node;
    ::pltxt2htm::TableTh<ndebug> th_node;
    ::pltxt2htm::TableThead<ndebug> thead_node;
    ::pltxt2htm::TableTbody<ndebug> tbody_node;
    ::pltxt2htm::TableTfoot<ndebug> tfoot_node;
    ::pltxt2htm::TableCaption<ndebug> caption_node;
    ::pltxt2htm::TableColgroup<ndebug> colgroup_node;

    // markdown node
    ::pltxt2htm::MdAtxH1<ndebug> md_atx_h1_node;
    ::pltxt2htm::MdAtxH2<ndebug> md_atx_h2_node;
    ::pltxt2htm::MdAtxH3<ndebug> md_atx_h3_node;
    ::pltxt2htm::MdAtxH4<ndebug> md_atx_h4_node;
    ::pltxt2htm::MdAtxH5<ndebug> md_atx_h5_node;
    ::pltxt2htm::MdAtxH6<ndebug> md_atx_h6_node;
    ::pltxt2htm::MdEscape md_escape_node;
    ::pltxt2htm::MdHr md_hr_node;
    ::pltxt2htm::CodeFence<ndebug> code_fence_node;
    ::pltxt2htm::MdCodeSpan1Backtick<ndebug> md_code_span_1_backtick_node;
    ::pltxt2htm::MdCodeSpan2Backtick<ndebug> md_code_span_2_backtick_node;
    ::pltxt2htm::MdCodeSpan3Backtick<ndebug> md_code_span_3_backtick_node;
    ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug> md_single_emphasis_asterisk_node;
    ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug> md_double_emphasis_asterisk_node;
    ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug> md_triple_emphasis_asterisk_node;
    ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug> md_single_emphasis_underscore_node;
    ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug> md_double_emphasis_underscore_node;
    ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug> md_triple_emphasis_underscore_node;
    ::pltxt2htm::MdDel<ndebug> md_del_node;
    ::pltxt2htm::MdLink<ndebug> md_link_node;
    ::pltxt2htm::Url url_node;
    ::pltxt2htm::MdImage<ndebug> md_image_node;
    ::pltxt2htm::MdBlockQuotes<ndebug> md_block_quotes_node;
    ::pltxt2htm::MdLatexInline<ndebug> md_latex_inline_node;
    ::pltxt2htm::MdLatexBlock<ndebug> md_latex_block_node;

    // Physics-Lab and Unity rich-text nodes
    ::pltxt2htm::UnityColor<ndebug> unity_color_node;
    ::pltxt2htm::PlA<ndebug> pl_a_node;
    ::pltxt2htm::PlExperiment<ndebug> pl_experiment_node;
    ::pltxt2htm::PlDiscussion<ndebug> pl_discussion_node;
    ::pltxt2htm::PlExperiments<ndebug> pl_experiments_node;
    ::pltxt2htm::PlDiscussions<ndebug> pl_discussions_node;
    ::pltxt2htm::PlUser<ndebug> pl_user_node;
    ::pltxt2htm::PlTrigger<ndebug> pl_trigger_node;
    ::pltxt2htm::PlInternal<ndebug> pl_internal_node;
    ::pltxt2htm::PlExternal<ndebug> pl_external_node;
    ::pltxt2htm::UnityLink<ndebug> unity_link_node;
    ::pltxt2htm::UnitySize<ndebug> unity_size_node;
    ::pltxt2htm::UnityVoffset<ndebug> unity_voffset_node;
    ::pltxt2htm::UnityAlign<ndebug> unity_align_node;
    ::pltxt2htm::UnityMark<ndebug> unity_mark_node;
    ::pltxt2htm::UnityMargin<ndebug> unity_margin_node;
    ::pltxt2htm::UnityI<ndebug> unity_i_node;
    ::pltxt2htm::UnityB<ndebug> unity_b_node;
    ::pltxt2htm::PlMacroProject pl_macro_project_node;
    ::pltxt2htm::PlMacroVisitor pl_macro_visitor_node;
    ::pltxt2htm::PlMacroAuthor pl_macro_author_node;
    ::pltxt2htm::PlMacroCoauthors pl_macro_coauthors_node;

    constexpr PlTxtNodeStorage() noexcept {
    }

    constexpr ~PlTxtNodeStorage() noexcept {
    }
};

template<::pltxt2htm::Contracts ndebug, typename Node>
struct PlTxtNodeTraits;

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::Text<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::text};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::text_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::InvalidUtf8> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::invalid_utf8};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::invalid_utf8_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::Group<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::group};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::group_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::UnityColor<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_color};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::unity_color_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlA<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_a};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_a_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlExperiment<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_experiment};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_experiment_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlDiscussion<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_discussion};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_discussion_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlExperiments<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_experiments};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_experiments_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlDiscussions<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_discussions};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_discussions_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlUser<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_user};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_user_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlTrigger<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_trigger};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_trigger_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlInternal<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_internal};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_internal_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlExternal<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_external};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_external_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::UnityLink<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_link};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::unity_link_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::UnitySize<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_size};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::unity_size_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::UnityVoffset<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_voffset};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::unity_voffset_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::UnityAlign<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_align};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::unity_align_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::UnityMark<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_mark};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::unity_mark_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::UnityMargin<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_margin};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::unity_margin_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::UnityI<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_i};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::unity_i_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::UnityB<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::unity_b};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::unity_b_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlU<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_u};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_u_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlS<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_s};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_s_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlMacroProject> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_macro_project};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_macro_project_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlMacroVisitor> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_macro_visitor};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_macro_visitor_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlMacroAuthor> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_macro_author};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_macro_author_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::PlMacroCoauthors> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::pl_macro_coauthors};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::pl_macro_coauthors_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::LineBreak> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::line_break};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::line_break_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlBr> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_br};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::br_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::Space> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::space};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::space_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::LessThan> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::less_than};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::less_than_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::GreaterThan> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::greater_than};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::greater_than_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::Tab> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::tab};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::tab_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::Ampersand> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::ampersand};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::ampersand_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::SingleQuote> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::single_quote};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::single_quote_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::DoubleQuote> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::double_quote};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::double_quote_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlHr> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_hr};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_hr_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlH1<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h1};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_h1_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlH2<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h2};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_h2_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlH3<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h3};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_h3_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlH4<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h4};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_h4_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlH5<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h5};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_h5_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlH6<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_h6};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_h6_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlP<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_p};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_p_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlDel<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_del};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_del_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlCode<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_code};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_code_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlSup<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_sup};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_sup_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlSub<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_sub};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_sub_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlNote<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_note};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_note_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlEm<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_em};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_em_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlStrong<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_strong};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_strong_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlMark<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_mark};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_mark_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlSpan<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_span};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_span_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlDiv<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_div};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_div_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlA<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_a};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_a_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::ListUl<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::list_ul};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::list_ul_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::ListOl<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::list_ol};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::list_ol_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::ListLi<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::list_li};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::list_li_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::ListLiCheckbox<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::list_li_checkbox};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::list_li_checkbox_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlBlockquote<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_blockquote};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_blockquote_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::TableCol> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_col};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::col_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::HtmlImg> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::html_img};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::html_img_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::Table<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::table_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::TableCaption<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_caption};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::caption_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::TableColgroup<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_colgroup};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::colgroup_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::TableThead<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_thead};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::thead_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::TableTbody<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_tbody};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::tbody_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::TableTfoot<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_tfoot};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::tfoot_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::TableTr<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_tr};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::tr_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::TableTh<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_th};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::th_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::TableTd<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::table_td};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::td_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdAtxH1<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h1};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_atx_h1_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdAtxH2<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h2};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_atx_h2_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdAtxH3<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h3};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_atx_h3_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdAtxH4<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h4};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_atx_h4_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdAtxH5<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h5};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_atx_h5_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdAtxH6<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_atx_h6};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_atx_h6_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdEscape> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_escape};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_escape_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdHr> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_hr};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_hr_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::CodeFence<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::code_fence};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::code_fence_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdCodeSpan1Backtick<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_code_span_1_backtick};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_code_span_1_backtick_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdCodeSpan2Backtick<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_code_span_2_backtick};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_code_span_2_backtick_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdCodeSpan3Backtick<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_code_span_3_backtick};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_code_span_3_backtick_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_single_emphasis_asterisk};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_single_emphasis_asterisk_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_double_emphasis_asterisk};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_double_emphasis_asterisk_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_triple_emphasis_asterisk};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_triple_emphasis_asterisk_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_single_emphasis_underscore};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_single_emphasis_underscore_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_double_emphasis_underscore};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_double_emphasis_underscore_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_triple_emphasis_underscore};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_triple_emphasis_underscore_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdDel<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_del};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_del_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdLink<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_link};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_link_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::Url> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::url};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::url_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdImage<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_image};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_image_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdBlockQuotes<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_block_quotes};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_block_quotes_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdLatexInline<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_latex_inline};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_latex_inline_node};
};

template<::pltxt2htm::Contracts ndebug>
struct PlTxtNodeTraits<ndebug, ::pltxt2htm::MdLatexBlock<ndebug>> {
    static constexpr ::pltxt2htm::NodeKind kind{::pltxt2htm::NodeKind::md_latex_block};
    static constexpr auto member{&::pltxt2htm::details::PlTxtNodeStorage<ndebug>::md_latex_block_node};
};

template<::pltxt2htm::Contracts ndebug, typename Node>
concept PlTxtNodeConcept = requires {
    requires ::std::same_as<Node, ::std::remove_cvref_t<Node>>;
    requires ::std::same_as<::pltxt2htm::NodeKind,
                            ::std::remove_cvref_t<decltype(::pltxt2htm::details::PlTxtNodeTraits<ndebug, Node>::kind)>>;
    requires ::std::is_member_object_pointer_v<
        ::std::remove_cvref_t<decltype(::pltxt2htm::details::PlTxtNodeTraits<ndebug, Node>::member)>>;
};

} // namespace pltxt2htm::details
