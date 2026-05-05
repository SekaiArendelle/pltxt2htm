#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```test```");
        auto answer = ::fast_io::u8string_view{u8"<code>test</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\nte\nst\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>te\nst</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~~\nte\nst\n~~~");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>te\nst</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```py\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~~\ntest\n~~~");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"~~~py\nprint(1)\n~~~"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"```py\nprint(1)\n```"};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n```\ntest\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>test</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```\ntest\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>test</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n```py\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```py\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```py\nprint(1)");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```\nte st\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>te&nbsp;st</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n```test");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto data = ::fast_io::u8string_view{
            u8R"(
```py
print("Hello World")
```
```py
print("Hello World")
```)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(data);
        auto answer = ::fast_io::u8string_view{
            u8"<br><pre><code class=\"language-py\">print(&quot;Hello&nbsp;World&quot;)</code></pre><br><pre><code "
            u8"class=\"language-py\">print(&quot;Hello&nbsp;World&quot;)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code></code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\n```\n```t");
        // TODO reduce <br> tag before <pre> tag
        auto answer = ::fast_io::u8string_view{u8"t<br><pre><code></code></pre>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\n```py\n```t");
        // TODO reduce <br> tag before <pre> tag
        auto answer = ::fast_io::u8string_view{u8"t<br><pre><code class=\"language-py\"></code></pre>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\n~~~py\n~~~t");
        // TODO reduce <br> tag before <pre> tag
        auto answer = ::fast_io::u8string_view{u8"t<br><pre><code class=\"language-py\"></code></pre>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\tpy\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```py\t\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo\"onmouseover=\"alert(1)\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{
            u8"<pre><code class=\"language-foo&quot;onmouseover=&quot;alert(1)\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```<svg/onload=alert(1)>\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{
            u8"<pre><code class=\"language-&lt;svg/onload=alert(1)&gt;\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```&#xGG;\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-&amp;#xGG;\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
