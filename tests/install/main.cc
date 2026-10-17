/**
 * @file main.cc
 * @brief Consumer-side smoke test of the pltxt2htm package
 * @note Built by tests/install/find_package (against an installation) and by
 *       tests/install/add_subdirectory (against the source tree)
 */

#include <cstddef>

#include <fast_io/fast_io.h>
#include <pltxt2htm/container/string_view.hh>
#include <pltxt2htm/pltxt2htm.hh>

// The version handed over by the consumer project must match the version baked into
// the headers it compiled against. A mismatch means the consumer was given a stale
// or partially overwritten installation.
static_assert(::pltxt2htm::version::major == static_cast<::std::size_t>(PLTXT2HTM_CONSUMER_EXPECTED_MAJOR),
              "the pltxt2htm package and its headers disagree on the major version");
static_assert(::pltxt2htm::version::minor == static_cast<::std::size_t>(PLTXT2HTM_CONSUMER_EXPECTED_MINOR),
              "the pltxt2htm package and its headers disagree on the minor version");
static_assert(::pltxt2htm::version::patch == static_cast<::std::size_t>(PLTXT2HTM_CONSUMER_EXPECTED_PATCH),
              "the pltxt2htm package and its headers disagree on the patch version");

namespace {

[[nodiscard]]
constexpr auto contains(::pltxt2htm::container::U8StringView haystack,
                        ::pltxt2htm::container::U8StringView needle) noexcept -> bool {
    if (needle.size() > haystack.size()) {
        return false;
    }
    for (::std::size_t offset{}; offset + needle.size() <= haystack.size(); ++offset) {
        if (::pltxt2htm::container::U8StringView{haystack.data() + offset, needle.size()} == needle) {
            return true;
        }
    }
    return false;
}

} // namespace

int main() {
    auto const html = ::pltxt2htm::pltxt2fixedadv_html(
        u8R"(
# Install smoke test)",
        u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
    ::fast_io::io::println(::fast_io::u8c_stdout(), html);

    auto const html_view = ::pltxt2htm::container::U8StringView{html.data(), html.size()};
    if (!contains(html_view, u8"<h1")) {
        ::fast_io::io::println(::fast_io::u8c_stdout(), u8"the heading is missing from the generated HTML");
        return 1;
    }

    return 0;
}
