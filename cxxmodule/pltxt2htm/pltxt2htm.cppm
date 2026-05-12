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

// contracts
using ::pltxt2htm::Contracts;

// exported constant global variables
namespace version {

using ::pltxt2htm::version::major;
using ::pltxt2htm::version::minor;
using ::pltxt2htm::version::patch;

} // namespace version

// exported nodes
using ::pltxt2htm::NodeType;

// ast2 types
using ::pltxt2htm::ast2::PlTxtNode;
using ::pltxt2htm::ast2::Ast;

// basic
using ::pltxt2htm::ast2::U8Char;
using ::pltxt2htm::ast2::InvalidU8Char;
using ::pltxt2htm::ast2::Text;
using ::pltxt2htm::ast2::Url;

// html_node
using ::pltxt2htm::ast2::LineBreak;
using ::pltxt2htm::ast2::Br;
using ::pltxt2htm::ast2::Space;
using ::pltxt2htm::ast2::LessThan;
using ::pltxt2htm::ast2::GreaterThan;
using ::pltxt2htm::ast2::Tab;
using ::pltxt2htm::ast2::Ampersand;
using ::pltxt2htm::ast2::SingleQuotationMark;
using ::pltxt2htm::ast2::DoubleQuotationMark;

using ::pltxt2htm::ast2::H1;
using ::pltxt2htm::ast2::H2;
using ::pltxt2htm::ast2::H3;
using ::pltxt2htm::ast2::H4;
using ::pltxt2htm::ast2::H5;
using ::pltxt2htm::ast2::H6;

using ::pltxt2htm::ast2::P;
using ::pltxt2htm::ast2::Del;
using ::pltxt2htm::ast2::Hr;
using ::pltxt2htm::ast2::Note;

using ::pltxt2htm::ast2::Em;
using ::pltxt2htm::ast2::Strong;

using ::pltxt2htm::ast2::Ul;
using ::pltxt2htm::ast2::Ol;
using ::pltxt2htm::ast2::Li;

using ::pltxt2htm::ast2::Code;
using ::pltxt2htm::ast2::Pre;
using ::pltxt2htm::ast2::Blockquote;

// markdown_node
using ::pltxt2htm::ast2::MdAtxH1;
using ::pltxt2htm::ast2::MdAtxH2;
using ::pltxt2htm::ast2::MdAtxH3;
using ::pltxt2htm::ast2::MdAtxH4;
using ::pltxt2htm::ast2::MdAtxH5;
using ::pltxt2htm::ast2::MdAtxH6;
using ::pltxt2htm::ast2::MdEscapeBackslash;
using ::pltxt2htm::ast2::MdEscapeExclamation;
using ::pltxt2htm::ast2::MdEscapeDoubleQuote;
using ::pltxt2htm::ast2::MdEscapeHash;
using ::pltxt2htm::ast2::MdEscapeDollar;
using ::pltxt2htm::ast2::MdEscapePercent;
using ::pltxt2htm::ast2::MdEscapeAmpersand;
using ::pltxt2htm::ast2::MdEscapeSingleQuote;
using ::pltxt2htm::ast2::MdEscapeLeftParen;
using ::pltxt2htm::ast2::MdEscapeRightParen;
using ::pltxt2htm::ast2::MdEscapeAsterisk;
using ::pltxt2htm::ast2::MdEscapePlus;
using ::pltxt2htm::ast2::MdEscapeComma;
using ::pltxt2htm::ast2::MdEscapeHyphen;
using ::pltxt2htm::ast2::MdEscapeDot;
using ::pltxt2htm::ast2::MdEscapeSlash;
using ::pltxt2htm::ast2::MdEscapeColon;
using ::pltxt2htm::ast2::MdEscapeSemicolon;
using ::pltxt2htm::ast2::MdEscapeLessThan;
using ::pltxt2htm::ast2::MdEscapeEquals;
using ::pltxt2htm::ast2::MdEscapeGreaterThan;
using ::pltxt2htm::ast2::MdEscapeQuestion;
using ::pltxt2htm::ast2::MdEscapeAt;
using ::pltxt2htm::ast2::MdEscapeLeftBracket;
using ::pltxt2htm::ast2::MdEscapeRightBracket;
using ::pltxt2htm::ast2::MdEscapeCaret;
using ::pltxt2htm::ast2::MdEscapeUnderscore;
using ::pltxt2htm::ast2::MdEscapeBacktick;
using ::pltxt2htm::ast2::MdEscapeLeftBrace;
using ::pltxt2htm::ast2::MdEscapePipe;
using ::pltxt2htm::ast2::MdEscapeRightBrace;
using ::pltxt2htm::ast2::MdEscapeTilde;
using ::pltxt2htm::ast2::MdHr;
using ::pltxt2htm::ast2::MdCodeFenceBacktick;
using ::pltxt2htm::ast2::MdCodeFenceTilde;
using ::pltxt2htm::ast2::MdCodeSpan1Backtick;
using ::pltxt2htm::ast2::MdCodeSpan2Backtick;
using ::pltxt2htm::ast2::MdCodeSpan3Backtick;
using ::pltxt2htm::ast2::MdSingleEmphasisAsterisk;
using ::pltxt2htm::ast2::MdDoubleEmphasisAsterisk;
using ::pltxt2htm::ast2::MdTripleEmphasisAsterisk;
using ::pltxt2htm::ast2::MdSingleEmphasisUnderscore;
using ::pltxt2htm::ast2::MdDoubleEmphasisUnderscore;
using ::pltxt2htm::ast2::MdTripleEmphasisUnderscore;
using ::pltxt2htm::ast2::MdDel;
using ::pltxt2htm::ast2::MdLink;
using ::pltxt2htm::ast2::MdImage;
using ::pltxt2htm::ast2::MdBlockQuotes;
using ::pltxt2htm::ast2::MdUl;
using ::pltxt2htm::ast2::MdOl;
using ::pltxt2htm::ast2::MdLi;
using ::pltxt2htm::ast2::MdLatexInline;
using ::pltxt2htm::ast2::MdLatexBlock;

// physics_lab_node
using ::pltxt2htm::ast2::PlColor;
using ::pltxt2htm::ast2::PlA;
using ::pltxt2htm::ast2::PlExperiment;
using ::pltxt2htm::ast2::PlDiscussion;
using ::pltxt2htm::ast2::PlUser;
using ::pltxt2htm::ast2::PlSize;
using ::pltxt2htm::ast2::PlExternal;
using ::pltxt2htm::ast2::PlI;
using ::pltxt2htm::ast2::PlB;
using ::pltxt2htm::ast2::PlMacroProject;
using ::pltxt2htm::ast2::PlMacroVisitor;
using ::pltxt2htm::ast2::PlMacroAuthor;
using ::pltxt2htm::ast2::PlMacroCoauthors;

} // namespace pltxt2htm
