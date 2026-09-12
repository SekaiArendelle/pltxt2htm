use crate::support::fixedadv;

#[test]
fn asterisk_emphasis() {
    let html = fixedadv("*test*");

    assert_eq!(html.as_str(), "<em>test</em>");
}

#[test]
fn asterisk_emphasis_with_surrounding_text() {
    let html = fixedadv("t*e*st");

    assert_eq!(html.as_str(), "t<em>e</em>st");
}

#[test]
fn asterisk_strong_with_surrounding_text() {
    let html = fixedadv("t**e**st");

    assert_eq!(html.as_str(), "t<strong>e</strong>st");
}

#[test]
fn triple_asterisk_emphasis() {
    let html = fixedadv("***test***");

    assert_eq!(html.as_str(), "<em><strong>test</strong></em>");
}

#[test]
fn triple_asterisk_emphasis_with_surrounding_text() {
    let html = fixedadv("t***e***st");

    assert_eq!(html.as_str(), "t<em><strong>e</strong></em>st");
}

#[test]
fn underscore_emphasis_with_surrounding_text() {
    let html = fixedadv("t_e_st");

    assert_eq!(html.as_str(), "t<em>e</em>st");
}

#[test]
fn underscore_strong_with_surrounding_text() {
    let html = fixedadv("t__e__st");

    assert_eq!(html.as_str(), "t<strong>e</strong>st");
}

#[test]
fn triple_underscore_emphasis_with_surrounding_text() {
    let html = fixedadv("t___e___st");

    assert_eq!(html.as_str(), "t<em><strong>e</strong></em>st");
}

#[test]
fn mixed_emphasis_delimiters_remain_literal() {
    let html = fixedadv("t_e*st");

    assert_eq!(html.as_str(), "t_e*st");
}

#[test]
fn emphasis_does_not_cross_newline() {
    let html = fixedadv("*test\n*");

    assert_eq!(html.as_str(), "*test<br>*");
}

#[test]
fn strong_does_not_cross_newline() {
    let html = fixedadv("**test\n**");

    assert_eq!(html.as_str(), "**test<br>**");
}

#[test]
fn triple_delimiter_before_newline_remains_literal() {
    let html = fixedadv("te***st\n***");

    assert_eq!(html.as_str(), "te***st<br><hr>");
}

#[test]
fn triple_underscore_emphasis() {
    let html = fixedadv("___test___");

    assert_eq!(html.as_str(), "<em><strong>test</strong></em>");
}

#[test]
fn four_asterisks_remain_literal() {
    let html = fixedadv("t****t");

    assert_eq!(html.as_str(), "t****t");
}

#[test]
fn unclosed_triple_asterisks_remain_literal() {
    let html = fixedadv("t***t");

    assert_eq!(html.as_str(), "t***t");
}

#[test]
fn five_asterisks_remain_literal() {
    let html = fixedadv("t*****t");

    assert_eq!(html.as_str(), "t*****t");
}

#[test]
fn emphasis_escapes_html_characters() {
    let html = fixedadv("&__</_");

    assert_eq!(html.as_str(), "&amp;_<em>&lt;/</em>");
}

#[test]
fn triple_emphasis_inside_em_drops_redundant_emphasis() {
    let html = fixedadv("<em>***text***</em>");

    assert_eq!(html.as_str(), "<em><strong>text</strong></em>");
}

#[test]
fn triple_emphasis_inside_i_drops_redundant_emphasis() {
    let html = fixedadv("<i>***text***</i>");

    assert_eq!(html.as_str(), "<em><strong>text</strong></em>");
}

#[test]
fn triple_emphasis_inside_strong_drops_redundant_strong() {
    let html = fixedadv("<strong>***text***</strong>");

    assert_eq!(html.as_str(), "<strong><em>text</em></strong>");
}

#[test]
fn triple_emphasis_inside_b_drops_redundant_strong() {
    let html = fixedadv("<b>***text***</b>");

    assert_eq!(html.as_str(), "<strong><em>text</em></strong>");
}

#[test]
fn em_inside_triple_emphasis_is_redundant() {
    let html = fixedadv("***<em>text</em>***");

    assert_eq!(html.as_str(), "<em><strong>text</strong></em>");
}

#[test]
fn strong_inside_triple_emphasis_is_redundant() {
    let html = fixedadv("***<strong>text</strong>***");

    assert_eq!(html.as_str(), "<em><strong>text</strong></em>");
}

#[test]
fn triple_emphasis_among_em_text_keeps_only_strong() {
    let html = fixedadv("<em>text***text***text</em>");

    assert_eq!(html.as_str(), "<em>text<strong>text</strong>text</em>");
}

#[test]
fn triple_emphasis_among_strong_text_keeps_only_emphasis() {
    let html = fixedadv("<strong>text***text***text</strong>");

    assert_eq!(html.as_str(), "<strong>text<em>text</em>text</strong>");
}

#[test]
fn em_child_among_triple_emphasis_text_is_redundant() {
    let html = fixedadv("text***<em>text</em>***text");

    assert_eq!(html.as_str(), "text<em><strong>text</strong></em>text");
}

#[test]
fn strong_child_among_triple_emphasis_text_is_redundant() {
    let html = fixedadv("text***<strong>text</strong>***text");

    assert_eq!(html.as_str(), "text<em><strong>text</strong></em>text");
}

#[test]
fn control_character_inside_triple_emphasis_is_replaced() {
    let html = fixedadv("***\u{7}***");

    assert_eq!(html.as_str(), "<em><strong>\u{fffd}</strong></em>");
}
