use crate::support::{common, fixedadv};

const BR_INPUT: &str = r#"
文本1<br   />文本2<bR/>文本3<br   >文本4<br    /
文本5<Br<br>
"#;

#[test]
fn br_tag_accepts_spacing_and_escapes_malformed_tags() {
    let html = fixedadv(BR_INPUT);
    let expected =
        "<br>文本1<br>文本2<br>文本3<br>文本4&lt;br&nbsp;&nbsp;&nbsp;&nbsp;/<br>文本5&lt;Br<br><br>";

    assert_eq!(html.as_str(), expected);
}

#[test]
fn common_parser_removes_br_tags() {
    let html = common(BR_INPUT);
    let expected = "文本1文本2文本3文本4&lt;br&nbsp;&nbsp;&nbsp;&nbsp;/文本5&lt;Br";

    assert_eq!(html.as_str(), expected);
}

#[test]
fn br_tag_accepts_tab_before_slash() {
    let html = fixedadv("<br\t/>text");

    assert_eq!(html.as_str(), "<br>text");
}
