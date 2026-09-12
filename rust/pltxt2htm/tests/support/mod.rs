use std::ffi::CString;

#[derive(Clone, Copy)]
pub(crate) struct Context<'a> {
    pub(crate) host: &'a str,
    pub(crate) project: &'a str,
    pub(crate) visitor: &'a str,
    pub(crate) author: &'a str,
    pub(crate) coauthors: &'a str,
}

pub(crate) const DEFAULT_CONTEXT: Context<'static> = Context {
    host: "localhost:5173",
    project: "$PROJECT",
    visitor: "$VISITOR",
    author: "$AUTHOR",
    coauthors: "$CO_AUTHORS",
};

pub(crate) fn common(input: &str) -> pltxt2htm::U8String {
    pltxt2htm::pltxt2common_html(c_string(input))
}

pub(crate) fn fixedadv(input: &str) -> pltxt2htm::U8String {
    fixedadv_with_context(input, DEFAULT_CONTEXT)
}

pub(crate) fn fixedadv_with_context(input: &str, context: Context<'_>) -> pltxt2htm::U8String {
    pltxt2htm::pltxt2fixedadv_html(
        c_string(input),
        c_string(context.host),
        c_string(context.project),
        c_string(context.visitor),
        c_string(context.author),
        c_string(context.coauthors),
    )
}

fn c_string(value: &str) -> CString {
    CString::new(value).unwrap()
}
