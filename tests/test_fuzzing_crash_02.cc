#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~~~\n%'#");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-~\">%&apos;#</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
