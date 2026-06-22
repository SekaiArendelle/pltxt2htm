module;

#include <pltxt2htm/pltxt2htm.hh>

export module pltxt2htm;

export namespace pltxt2htm {

// exported functions
using ::pltxt2htm::pltxt2common_html;
using ::pltxt2htm::pltxt2fixedadv_html;
using ::pltxt2htm::pltxt2plunity_introduction;
using ::pltxt2htm::parse_pltxt;
using ::pltxt2htm::optimize_ast;
using ::pltxt2htm::pltxt4unittest;

// contracts
using ::pltxt2htm::Contracts;

// exported constant global variables
namespace version {

using ::pltxt2htm::version::major;
using ::pltxt2htm::version::minor;
using ::pltxt2htm::version::patch;

} // namespace version

// exported nodes
using ::pltxt2htm::NodeKind;

// markdown table alignment
using ::pltxt2htm::MdTableAlign;

// ast2 types
using ::pltxt2htm::PlTxtNode;
using ::pltxt2htm::Ast;

// basic
using ::pltxt2htm::U8Char;
using ::pltxt2htm::InvalidU8Char;
using ::pltxt2htm::Text;
using ::pltxt2htm::Url;

using ::pltxt2htm::LineBreak;
using ::pltxt2htm::Space;
using ::pltxt2htm::LessThan;
using ::pltxt2htm::GreaterThan;
using ::pltxt2htm::Tab;
using ::pltxt2htm::Ampersand;
using ::pltxt2htm::SingleQuote;
using ::pltxt2htm::DoubleQuote;

// html_node
using ::pltxt2htm::HtmlBr;

using ::pltxt2htm::HtmlH1;
using ::pltxt2htm::HtmlH2;
using ::pltxt2htm::HtmlH3;
using ::pltxt2htm::HtmlH4;
using ::pltxt2htm::HtmlH5;
using ::pltxt2htm::HtmlH6;

using ::pltxt2htm::HtmlP;
using ::pltxt2htm::HtmlDel;
using ::pltxt2htm::HtmlHr;
using ::pltxt2htm::HtmlNote;

using ::pltxt2htm::HtmlEm;
using ::pltxt2htm::HtmlStrong;

using ::pltxt2htm::HtmlUl;
using ::pltxt2htm::HtmlOl;
using ::pltxt2htm::HtmlLi;

using ::pltxt2htm::HtmlCode;
using ::pltxt2htm::HtmlPre;
using ::pltxt2htm::HtmlBlockquote;

// html table nodes
using ::pltxt2htm::HtmlTable;
using ::pltxt2htm::HtmlTr;
using ::pltxt2htm::HtmlTd;
using ::pltxt2htm::HtmlTh;
using ::pltxt2htm::HtmlThead;
using ::pltxt2htm::HtmlTbody;
using ::pltxt2htm::HtmlTfoot;
using ::pltxt2htm::HtmlCaption;
using ::pltxt2htm::HtmlColgroup;
using ::pltxt2htm::HtmlCol;

using ::pltxt2htm::HtmlSpan;

// markdown_node
using ::pltxt2htm::MdAtxH1;
using ::pltxt2htm::MdAtxH2;
using ::pltxt2htm::MdAtxH3;
using ::pltxt2htm::MdAtxH4;
using ::pltxt2htm::MdAtxH5;
using ::pltxt2htm::MdAtxH6;
using ::pltxt2htm::MdEscapeBackslash;
using ::pltxt2htm::MdEscapeExclamation;
using ::pltxt2htm::MdEscapeDoubleQuote;
using ::pltxt2htm::MdEscapeHash;
using ::pltxt2htm::MdEscapeDollar;
using ::pltxt2htm::MdEscapePercent;
using ::pltxt2htm::MdEscapeAmpersand;
using ::pltxt2htm::MdEscapeSingleQuote;
using ::pltxt2htm::MdEscapeLeftParen;
using ::pltxt2htm::MdEscapeRightParen;
using ::pltxt2htm::MdEscapeAsterisk;
using ::pltxt2htm::MdEscapePlus;
using ::pltxt2htm::MdEscapeComma;
using ::pltxt2htm::MdEscapeHyphen;
using ::pltxt2htm::MdEscapeDot;
using ::pltxt2htm::MdEscapeSlash;
using ::pltxt2htm::MdEscapeColon;
using ::pltxt2htm::MdEscapeSemicolon;
using ::pltxt2htm::MdEscapeLessThan;
using ::pltxt2htm::MdEscapeEquals;
using ::pltxt2htm::MdEscapeGreaterThan;
using ::pltxt2htm::MdEscapeQuestion;
using ::pltxt2htm::MdEscapeAt;
using ::pltxt2htm::MdEscapeLeftBracket;
using ::pltxt2htm::MdEscapeRightBracket;
using ::pltxt2htm::MdEscapeCaret;
using ::pltxt2htm::MdEscapeUnderscore;
using ::pltxt2htm::MdEscapeBacktick;
using ::pltxt2htm::MdEscapeLeftBrace;
using ::pltxt2htm::MdEscapePipe;
using ::pltxt2htm::MdEscapeRightBrace;
using ::pltxt2htm::MdEscapeTilde;
using ::pltxt2htm::MdHr;
using ::pltxt2htm::MdCodeFenceBacktick;
using ::pltxt2htm::MdCodeFenceTilde;
using ::pltxt2htm::MdCodeSpan1Backtick;
using ::pltxt2htm::MdCodeSpan2Backtick;
using ::pltxt2htm::MdCodeSpan3Backtick;
using ::pltxt2htm::MdSingleEmphasisAsterisk;
using ::pltxt2htm::MdDoubleEmphasisAsterisk;
using ::pltxt2htm::MdTripleEmphasisAsterisk;
using ::pltxt2htm::MdSingleEmphasisUnderscore;
using ::pltxt2htm::MdDoubleEmphasisUnderscore;
using ::pltxt2htm::MdTripleEmphasisUnderscore;
using ::pltxt2htm::MdDel;
using ::pltxt2htm::MdLink;
using ::pltxt2htm::MdImage;
using ::pltxt2htm::MdBlockQuotes;
using ::pltxt2htm::MdUl;
using ::pltxt2htm::MdOl;
using ::pltxt2htm::MdLi;
using ::pltxt2htm::MdLiCheckbox;
using ::pltxt2htm::MdLatexInline;
using ::pltxt2htm::MdLatexBlock;

// md table nodes
using ::pltxt2htm::MdTable;
using ::pltxt2htm::MdTr;
using ::pltxt2htm::MdTd;
using ::pltxt2htm::MdTh;
using ::pltxt2htm::MdThead;
using ::pltxt2htm::MdTbody;

// physics_lab_node
using ::pltxt2htm::PlColor;
using ::pltxt2htm::PlA;
using ::pltxt2htm::PlExperiment;
using ::pltxt2htm::PlDiscussion;
using ::pltxt2htm::PlUser;
using ::pltxt2htm::PlSize;
using ::pltxt2htm::PlExternal;
using ::pltxt2htm::PlI;
using ::pltxt2htm::PlB;
using ::pltxt2htm::PlMacroProject;
using ::pltxt2htm::PlMacroVisitor;
using ::pltxt2htm::PlMacroAuthor;
using ::pltxt2htm::PlMacroCoauthors;

} // namespace pltxt2htm
