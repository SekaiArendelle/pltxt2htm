#include "precompile.hh"

#include "html/html_a_tag.hh"
#include "html/html_blockquote_tag.hh"
#include "html/html_br_tag.hh"
#include "html/html_code_tag.hh"
#include "html/html_del_tag.hh"
#include "html/html_div_tag.hh"
#include "html/html_em_tag.hh"
#include "html/html_h1_tag.hh"
#include "html/html_h2_tag.hh"
#include "html/html_h3_tag.hh"
#include "html/html_h4_tag.hh"
#include "html/html_h5_tag.hh"
#include "html/html_h6_tag.hh"
#include "html/html_hr_tag.hh"
#include "html/html_img_tag.hh"
#include "html/html_input_tag.hh"
#include "html/html_mark_tag.hh"
#include "html/html_note.hh"
#include "html/html_ol_and_li_tag.hh"
#include "html/html_p_tag.hh"
#include "html/html_parser.hh"
#include "html/html_pre_tag.hh"
#include "html/html_span_tag.hh"
#include "html/html_strong_tag.hh"
#include "html/html_sub_tag.hh"
#include "html/html_sup_tag.hh"
#include "html/html_table.hh"
#include "html/html_ul_and_li_tag.hh"
#include "md/latex.hh"
#include "md/md_atx_header.hh"
#include "md/md_block_quotes.hh"
#include "md/md_checkbox.hh"
#include "md/md_code_fence.hh"
#include "md/md_code_span.hh"
#include "md/md_del.hh"
#include "md/md_emphasis.hh"
#include "md/md_escape.hh"
#include "md/md_image.hh"
#include "md/md_link.hh"
#include "md/md_ol_list.hh"
#include "md/md_table.hh"
#include "md/md_thematic_break.hh"
#include "md/md_ul_list.hh"
#include "pl/pl_a_tag.hh"
#include "pl/pl_discussion_tag.hh"
#include "pl/pl_discussions_tag.hh"
#include "pl/pl_experiment_tag.hh"
#include "pl/pl_experiments_tag.hh"
#include "pl/pl_external_tag.hh"
#include "pl/pl_internal_tag.hh"
#include "pl/pl_macro.hh"
#include "pl/pl_s_tag.hh"
#include "pl/pl_trigger_tag.hh"
#include "pl/pl_u_tag.hh"
#include "pl/pl_user_tag.hh"
#include "text/auto_link.hh"
#include "text/bound.hh"
#include "text/escape.hh"
#include "text/invalid_utf8_char.hh"
#include "text/rm_tail_space.hh"
#include "text/tab.hh"
#include "unity/unity_align_tag.hh"
#include "unity/unity_b_tag.hh"
#include "unity/unity_color_tag.hh"
#include "unity/unity_i_tag.hh"
#include "unity/unity_link_tag.hh"
#include "unity/unity_margin_tag.hh"
#include "unity/unity_size_tag.hh"
#include "unity/unity_voffset_tag.hh"

namespace pltxt2htm_test::syntax {

struct test_case {
    ::fast_io::u8string_view name;
    void (*run)();
};

constexpr test_case syntax_tests_set[]{
    {u8"html_a_tag", &html_a_tag},
    {u8"html_blockquote_tag", &html_blockquote_tag},
    {u8"html_br_tag", &html_br_tag},
    {u8"html_code_tag", &html_code_tag},
    {u8"html_del_tag", &html_del_tag},
    {u8"html_div_tag", &html_div_tag},
    {u8"html_em_tag", &html_em_tag},
    {u8"html_h1_tag", &html_h1_tag},
    {u8"html_h2_tag", &html_h2_tag},
    {u8"html_h3_tag", &html_h3_tag},
    {u8"html_h4_tag", &html_h4_tag},
    {u8"html_h5_tag", &html_h5_tag},
    {u8"html_h6_tag", &html_h6_tag},
    {u8"html_hr_tag", &html_hr_tag},
    {u8"html_img_tag", &html_img_tag},
    {u8"html_input_tag", &html_input_tag},
    {u8"html_mark_tag", &html_mark_tag},
    {u8"html_note", &html_note},
    {u8"html_ol_and_li_tag", &html_ol_and_li_tag},
    {u8"html_p_tag", &html_p_tag},
    {u8"html_parser", &html_parser},
    {u8"html_pre_tag", &html_pre_tag},
    {u8"html_span_tag", &html_span_tag},
    {u8"html_strong_tag", &html_strong_tag},
    {u8"html_sub_tag", &html_sub_tag},
    {u8"html_sup_tag", &html_sup_tag},
    {u8"html_table", &html_table},
    {u8"html_ul_and_li_tag", &html_ul_and_li_tag},
    {u8"latex", &latex},
    {u8"md_atx_header", &md_atx_header},
    {u8"md_block_quotes", &md_block_quotes},
    {u8"md_checkbox", &md_checkbox},
    {u8"md_code_fence", &md_code_fence},
    {u8"md_code_span", &md_code_span},
    {u8"md_del", &md_del},
    {u8"md_emphasis", &md_emphasis},
    {u8"md_escape", &md_escape},
    {u8"md_image", &md_image},
    {u8"md_link", &md_link},
    {u8"md_ol_list", &md_ol_list},
    {u8"md_table", &md_table},
    {u8"md_thematic_break", &md_thematic_break},
    {u8"md_ul_list", &md_ul_list},
    {u8"pl_a_tag", &pl_a_tag},
    {u8"pl_discussion_tag", &pl_discussion_tag},
    {u8"pl_discussions_tag", &pl_discussions_tag},
    {u8"pl_experiment_tag", &pl_experiment_tag},
    {u8"pl_experiments_tag", &pl_experiments_tag},
    {u8"pl_external_tag", &pl_external_tag},
    {u8"pl_internal_tag", &pl_internal_tag},
    {u8"pl_macro", &pl_macro},
    {u8"pl_s_tag", &pl_s_tag},
    {u8"pl_trigger_tag", &pl_trigger_tag},
    {u8"pl_u_tag", &pl_u_tag},
    {u8"pl_user_tag", &pl_user_tag},
    {u8"auto_link", &auto_link},
    {u8"bound", &bound},
    {u8"escape", &escape},
    {u8"invalid_utf8_char", &invalid_utf8_char},
    {u8"rm_tail_space", &rm_tail_space},
    {u8"tab", &tab},
    {u8"unity_align_tag", &unity_align_tag},
    {u8"unity_b_tag", &unity_b_tag},
    {u8"unity_color_tag", &unity_color_tag},
    {u8"unity_i_tag", &unity_i_tag},
    {u8"unity_link_tag", &unity_link_tag},
    {u8"unity_margin_tag", &unity_margin_tag},
    {u8"unity_size_tag", &unity_size_tag},
    {u8"unity_voffset_tag", &unity_voffset_tag},
};

} // namespace pltxt2htm_test::syntax

int main() {
    for (auto const& test : ::pltxt2htm_test::syntax::syntax_tests_set) {
        test.run();
    }

    return 0;
}
