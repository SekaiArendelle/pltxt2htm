use crate::support::{fixedadv, fixedadv_with_context, Context, DEFAULT_CONTEXT};

#[test]
fn project_macro_uses_context() {
    let html = fixedadv("test{Project}test");

    assert_eq!(html.as_str(), "test$PROJECTtest");
}

#[test]
fn visitor_macro_uses_context() {
    let html = fixedadv("test{Visitor}test");

    assert_eq!(html.as_str(), "test$VISITORtest");
}

#[test]
fn author_macro_uses_context() {
    let html = fixedadv("test{Author}test");

    assert_eq!(html.as_str(), "test$AUTHORtest");
}

#[test]
fn coauthors_macro_uses_context() {
    let html = fixedadv("test{CoAuthors}test");

    assert_eq!(html.as_str(), "test$CO_AUTHORStest");
}

#[test]
fn macro_context_is_html_escaped() {
    let html = fixedadv_with_context(
        "{Project}",
        Context {
            project: "'",
            ..DEFAULT_CONTEXT
        },
    );

    assert_eq!(html.as_str(), "&apos;");
}
