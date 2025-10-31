module;

#include <pltxt2htm/pltxt2htm.hh>

export module pltxt2htm;

export namespace pltxt2htm {

// exported functions
using ::pltxt2htm::pltxt2common_html;
using ::pltxt2htm::pltxt2advanced_html;
using ::pltxt2htm::pltxt2fixedadv_html;
using ::pltxt2htm::parse_pltxt;
using ::pltxt2htm::optimize_ast;

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
using ::pltxt2htm::EscapeBackslash;
using ::pltxt2htm::EscapeExclamation;
using ::pltxt2htm::EscapeDoubleQuote;
using ::pltxt2htm::EscapeHash;
using ::pltxt2htm::EscapeDollar;
using ::pltxt2htm::EscapePercent;
using ::pltxt2htm::EscapeAmpersand;
using ::pltxt2htm::EscapeSingleQuote;
using ::pltxt2htm::EscapeLeftParen;
using ::pltxt2htm::EscapeRightParen;
using ::pltxt2htm::EscapeAsterisk;
using ::pltxt2htm::EscapePlus;
using ::pltxt2htm::EscapeComma;
using ::pltxt2htm::EscapeHyphen;
using ::pltxt2htm::EscapeDot;
using ::pltxt2htm::EscapeSlash;
using ::pltxt2htm::EscapeColon;
using ::pltxt2htm::EscapeSemicolon;
using ::pltxt2htm::EscapeLessThan;
using ::pltxt2htm::EscapeEquals;
using ::pltxt2htm::EscapeGreaterThan;
using ::pltxt2htm::EscapeQuestion;
using ::pltxt2htm::EscapeAt;
using ::pltxt2htm::EscapeLeftBracket;
using ::pltxt2htm::EscapeRightBracket;
using ::pltxt2htm::EscapeCaret;
using ::pltxt2htm::EscapeUnderscore;
using ::pltxt2htm::EscapeBacktick;
using ::pltxt2htm::EscapeLeftBrace;
using ::pltxt2htm::EscapePipe;
using ::pltxt2htm::EscapeRightBrace;
using ::pltxt2htm::EscapeTilde;
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
using ::pltxt2htm::MdBlockQuotes;

// physics_lab_node
using ::pltxt2htm::Color;
using ::pltxt2htm::A;
using ::pltxt2htm::Experiment;
using ::pltxt2htm::Discussion;
using ::pltxt2htm::User;
using ::pltxt2htm::Size;
using ::pltxt2htm::I;
using ::pltxt2htm::B;

} // namespace pltxt2htm
