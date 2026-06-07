module;

#include <meta>
#include <string_view>

import pltxt2htm;

consteval bool is_exported(std::string_view name) {
    for (auto m : ::std::meta::members_of(^^::pltxt2htm, ::std::meta::access_context::unprivileged()))
        if (::std::meta::identifier_of(m) == name)
            return true;
    return false;
}

consteval auto version_reflection() {
    for (auto m : ::std::meta::members_of(^^::pltxt2htm, ::std::meta::access_context::unprivileged()))
        if (::std::meta::identifier_of(m) == "version")
            return m;
    throw;
}

consteval bool is_exported_in_version(std::string_view name) {
    for (auto m : ::std::meta::members_of(version_reflection(), ::std::meta::access_context::unprivileged()))
        if (::std::meta::identifier_of(m) == name)
            return true;
    return false;
}

#define CHECK_EXPORTED(name) \
    static_assert([]() consteval { return is_exported(#name); }(), "not exported: " #name)

#define CHECK_VERSION_EXPORTED(name) \
    static_assert([]() consteval { return is_exported_in_version(#name); }(), "not exported: version::" #name)

// ---- functions ----
CHECK_EXPORTED(pltxt2common_html);
CHECK_EXPORTED(pltxt2fixedadv_html);
CHECK_EXPORTED(pltxt2plunity_introduction);
CHECK_EXPORTED(parse_pltxt);
CHECK_EXPORTED(optimize_ast);
CHECK_EXPORTED(pltxt4unittest);

// ---- contracts ----
CHECK_EXPORTED(Contracts);

// ---- version ----
CHECK_EXPORTED(version);
CHECK_VERSION_EXPORTED(major);
CHECK_VERSION_EXPORTED(minor);
CHECK_VERSION_EXPORTED(patch);

// ---- node kind ----
CHECK_EXPORTED(NodeKind);

// ---- ast types ----
CHECK_EXPORTED(PlTxtNode);
CHECK_EXPORTED(Ast);

// ---- basic nodes ----
CHECK_EXPORTED(U8Char);
CHECK_EXPORTED(InvalidU8Char);
CHECK_EXPORTED(Text);
CHECK_EXPORTED(Url);
CHECK_EXPORTED(LineBreak);
CHECK_EXPORTED(Space);
CHECK_EXPORTED(LessThan);
CHECK_EXPORTED(GreaterThan);
CHECK_EXPORTED(Tab);
CHECK_EXPORTED(Ampersand);
CHECK_EXPORTED(SingleQuotationMark);
CHECK_EXPORTED(DoubleQuotationMark);

// ---- html nodes ----
CHECK_EXPORTED(HtmlBr);
CHECK_EXPORTED(HtmlH1);
CHECK_EXPORTED(HtmlH2);
CHECK_EXPORTED(HtmlH3);
CHECK_EXPORTED(HtmlH4);
CHECK_EXPORTED(HtmlH5);
CHECK_EXPORTED(HtmlH6);
CHECK_EXPORTED(HtmlP);
CHECK_EXPORTED(HtmlDel);
CHECK_EXPORTED(HtmlHr);
CHECK_EXPORTED(HtmlNote);
CHECK_EXPORTED(HtmlEm);
CHECK_EXPORTED(HtmlStrong);
CHECK_EXPORTED(HtmlUl);
CHECK_EXPORTED(HtmlOl);
CHECK_EXPORTED(HtmlLi);
CHECK_EXPORTED(HtmlCode);
CHECK_EXPORTED(HtmlPre);
CHECK_EXPORTED(HtmlBlockquote);

// ---- html table nodes ----
CHECK_EXPORTED(HtmlTable);
CHECK_EXPORTED(HtmlTr);
CHECK_EXPORTED(HtmlTd);
CHECK_EXPORTED(HtmlTh);
CHECK_EXPORTED(HtmlThead);
CHECK_EXPORTED(HtmlTbody);
CHECK_EXPORTED(HtmlTfoot);
CHECK_EXPORTED(HtmlCaption);
CHECK_EXPORTED(HtmlColgroup);
CHECK_EXPORTED(HtmlCol);

// ---- markdown ATX headings ----
CHECK_EXPORTED(MdAtxH1);
CHECK_EXPORTED(MdAtxH2);
CHECK_EXPORTED(MdAtxH3);
CHECK_EXPORTED(MdAtxH4);
CHECK_EXPORTED(MdAtxH5);
CHECK_EXPORTED(MdAtxH6);

// ---- markdown escapes ----
CHECK_EXPORTED(MdEscapeBackslash);
CHECK_EXPORTED(MdEscapeExclamation);
CHECK_EXPORTED(MdEscapeDoubleQuote);
CHECK_EXPORTED(MdEscapeHash);
CHECK_EXPORTED(MdEscapeDollar);
CHECK_EXPORTED(MdEscapePercent);
CHECK_EXPORTED(MdEscapeAmpersand);
CHECK_EXPORTED(MdEscapeSingleQuote);
CHECK_EXPORTED(MdEscapeLeftParen);
CHECK_EXPORTED(MdEscapeRightParen);
CHECK_EXPORTED(MdEscapeAsterisk);
CHECK_EXPORTED(MdEscapePlus);
CHECK_EXPORTED(MdEscapeComma);
CHECK_EXPORTED(MdEscapeHyphen);
CHECK_EXPORTED(MdEscapeDot);
CHECK_EXPORTED(MdEscapeSlash);
CHECK_EXPORTED(MdEscapeColon);
CHECK_EXPORTED(MdEscapeSemicolon);
CHECK_EXPORTED(MdEscapeLessThan);
CHECK_EXPORTED(MdEscapeEquals);
CHECK_EXPORTED(MdEscapeGreaterThan);
CHECK_EXPORTED(MdEscapeQuestion);
CHECK_EXPORTED(MdEscapeAt);
CHECK_EXPORTED(MdEscapeLeftBracket);
CHECK_EXPORTED(MdEscapeRightBracket);
CHECK_EXPORTED(MdEscapeCaret);
CHECK_EXPORTED(MdEscapeUnderscore);
CHECK_EXPORTED(MdEscapeBacktick);
CHECK_EXPORTED(MdEscapeLeftBrace);
CHECK_EXPORTED(MdEscapePipe);
CHECK_EXPORTED(MdEscapeRightBrace);
CHECK_EXPORTED(MdEscapeTilde);

// ---- markdown thematic / code / emphasis / links ----
CHECK_EXPORTED(MdHr);
CHECK_EXPORTED(MdCodeFenceBacktick);
CHECK_EXPORTED(MdCodeFenceTilde);
CHECK_EXPORTED(MdCodeSpan1Backtick);
CHECK_EXPORTED(MdCodeSpan2Backtick);
CHECK_EXPORTED(MdCodeSpan3Backtick);
CHECK_EXPORTED(MdSingleEmphasisAsterisk);
CHECK_EXPORTED(MdDoubleEmphasisAsterisk);
CHECK_EXPORTED(MdTripleEmphasisAsterisk);
CHECK_EXPORTED(MdSingleEmphasisUnderscore);
CHECK_EXPORTED(MdDoubleEmphasisUnderscore);
CHECK_EXPORTED(MdTripleEmphasisUnderscore);
CHECK_EXPORTED(MdDel);
CHECK_EXPORTED(MdLink);
CHECK_EXPORTED(MdImage);
CHECK_EXPORTED(MdBlockQuotes);
CHECK_EXPORTED(MdUl);
CHECK_EXPORTED(MdOl);
CHECK_EXPORTED(MdLi);
CHECK_EXPORTED(MdLatexInline);
CHECK_EXPORTED(MdLatexBlock);

// ---- markdown table alignment ----
CHECK_EXPORTED(MdTableAlign);

// ---- markdown checkbox list item ----
CHECK_EXPORTED(MdLiCheckbox);

// ---- physics-lab nodes ----
CHECK_EXPORTED(PlColor);
CHECK_EXPORTED(PlA);
CHECK_EXPORTED(PlExperiment);
CHECK_EXPORTED(PlDiscussion);
CHECK_EXPORTED(PlUser);
CHECK_EXPORTED(PlSize);
CHECK_EXPORTED(PlExternal);
CHECK_EXPORTED(PlI);
CHECK_EXPORTED(PlB);
CHECK_EXPORTED(PlMacroProject);
CHECK_EXPORTED(PlMacroVisitor);
CHECK_EXPORTED(PlMacroAuthor);
CHECK_EXPORTED(PlMacroCoauthors);

int main() {}
