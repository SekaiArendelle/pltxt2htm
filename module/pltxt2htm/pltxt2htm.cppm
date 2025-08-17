module;

#include <pltxt2htm/pltxt2htm.hh>

export module pltxt2htm;

export namespace pltxt2htm {

// exported functions
using ::pltxt2htm::pltxt2common_html;
using ::pltxt2htm::pltxt2advanced_html;
using ::pltxt2htm::pltxt2fixedadv_html;
using ::pltxt2htm::parse_pltxt;

namespace version {
// exported global constant variable (version of pltxt2htm)

using ::pltxt2htm::version::major;
using ::pltxt2htm::version::minor;
using ::pltxt2htm::version::patch;

} // namespace version

// exported nodes
using ::pltxt2htm::NodeType;
// TODO: export all node classes

} // namespace pltxt2htm
