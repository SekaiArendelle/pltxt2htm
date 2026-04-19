module;

#include <pltxt2htm/pltxt2htm.hh>

export module pltxt2htm;

export namespace pltxt2htm {

// exported functions
using ::pltxt2htm::pltxt2common_html;
using ::pltxt2htm::pltxt2advanced_html;
using ::pltxt2htm::pltxt2fixedadv_html;
using ::pltxt2htm::pltxt2plunity_introduction;
using ::pltxt2htm::parse_pltxt;
using ::pltxt2htm::optimize_ast;

// contracts
using ::pltxt2htm::Contracts;

// heap_guard
using ::pltxt2htm::HeapGuard;

// exported constant global variables
namespace version {

using ::pltxt2htm::version::major;
using ::pltxt2htm::version::minor;
using ::pltxt2htm::version::patch;

} // namespace version

// exported nodes
using ::pltxt2htm::NodeType;

// basic
using ::pltxt2htm::PlTxtNode;
using ::pltxt2htm::U8Char;
using ::pltxt2htm::InvalidU8Char;
using ::pltxt2htm::Text;

// html_node
using ::pltxt2htm::LineBreak;
using ::pltxt2htm::Br;
using ::pltxt2htm::Space;
using ::pltxt2htm::LessThan;
using ::pltxt2htm::GreaterThan;
using ::pltxt2htm::Tab;
using ::pltxt2htm::Ampersand;
using ::pltxt2htm::SingleQuotationMark;
using ::pltxt2htm::DoubleQuotationMark;

using ::pltxt2htm::H1;
using ::pltxt2htm::H2;
using ::pltxt2htm::H3;
using ::pltxt2htm::H4;
using ::pltxt2htm::H5;
using ::pltxt2htm::H6;

using ::pltxt2htm::P;
using ::pltxt2htm::Del;
using ::pltxt2htm::Hr;
using ::pltxt2htm::Note;

using ::pltxt2htm::Em;
using ::pltxt2htm::Strong;

using ::pltxt2htm::Ul;
using ::pltxt2htm::Ol;
using ::pltxt2htm::Li;

using ::pltxt2htm::Code;
using ::pltxt2htm::Pre;
using ::pltxt2htm::Blockquote;

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
using ::pltxt2htm::MdLatexInline;
using ::pltxt2htm::MdLatexBlock;

// physics_lab_node
using ::pltxt2htm::Color;
using ::pltxt2htm::A;
using ::pltxt2htm::Experiment;
using ::pltxt2htm::Discussion;
using ::pltxt2htm::User;
using ::pltxt2htm::Size;
using ::pltxt2htm::External;
using ::pltxt2htm::I;
using ::pltxt2htm::B;
using ::pltxt2htm::PlMacroProject;
using ::pltxt2htm::PlMacroVisitor;
using ::pltxt2htm::PlMacroAuthor;
using ::pltxt2htm::PlMacroCoauthors;

} // namespace pltxt2htm
