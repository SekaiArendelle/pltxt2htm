#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"![alt text](https://example.com/image.png)");
        auto answer = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"alt text\">"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"![logo](/images/logo.jpg)");
        auto answer = ::fast_io::u8string_view{u8"![logo](/images/logo.jpg)"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"![alt text](https://example.com/image.wtf)");
        auto answer = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.wtf\" alt=\"alt text\">"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"![](https://example.com)");
        auto answer = ::fast_io::u8string_view{u8"<img src=\"https://example.com\" alt=\"\">"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"![text with spaces](image.gif)");
        auto answer = ::fast_io::u8string_view{u8"![text&nbsp;with&nbsp;spaces](image.gif)"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"![special](https://example.com/path with spaces.jpg)");
        auto answer = ::fast_io::u8string_view{u8"![special](https://example.com/path&nbsp;with&nbsp;spaces.jpg)"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2advanced_htmld(u8"![special](https://example.com/path%20with%20spaces.jpg)");
        auto answer =
            ::fast_io::u8string_view{u8"<img src=\"https://example.com/path%20with%20spaces.jpg\" alt=\"special\">"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"![text](url");
        auto answer = ::fast_io::u8string_view{u8"![text](url"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"![text");
        auto answer = ::fast_io::u8string_view{u8"![text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"![]()");
        auto answer = ::fast_io::u8string_view{u8"![]()"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        // Avoid XSS Attack
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"![x\" onerror=\"alert('XSS')](example.com/image.jpg)");
        auto answer = ::fast_io::u8string_view{u8"![]()"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
