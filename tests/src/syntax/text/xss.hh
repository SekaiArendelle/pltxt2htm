#pragma once

#include "doctest_config.hh"

TEST_SUITE("xss") {
    [[nodiscard]]
    constexpr bool contains_u8(::pltxt2htm::container::U8StringView haystack,
                               ::pltxt2htm::container::U8StringView needle) noexcept {
        if (needle.size() > haystack.size()) {
            return false;
        }
        for (::std::size_t i{}; i + needle.size() <= haystack.size(); ++i) {
            bool match{true};
            for (::std::size_t j{}; j < needle.size(); ++j) {
                if (haystack.index(i + j) !=
                    needle.index(j)) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]]
    constexpr bool has_unescaped_tag(::pltxt2htm::container::U8StringView html,
                                     ::pltxt2htm::container::U8StringView tag) noexcept {
        ::std::size_t const tag_sz{tag.size()};
        if (tag_sz == 0 || tag_sz + 1 > html.size()) {
            return false;
        }
        ::std::size_t const limit{html.size() - tag_sz};
        for (::std::size_t i{}; i < limit; ++i) {
            if (html.index(i) != u8'<') {
                continue;
            }
            bool match{true};
            for (::std::size_t j{}; j < tag_sz; ++j) {
                if (html.index(i + 1 + j) !=
                    tag.index(j)) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return true;
            }
        }
        return false;
    }

    void assert_no_raw_xss_tags(::pltxt2htm::container::U8StringView html) noexcept {
        CHECK(!has_unescaped_tag(html, u8"script"));
        CHECK(!has_unescaped_tag(html, u8"iframe"));
        CHECK(!has_unescaped_tag(html, u8"object"));
        CHECK(!has_unescaped_tag(html, u8"embed"));
        CHECK(!has_unescaped_tag(html, u8"svg"));
        CHECK(!has_unescaped_tag(html, u8"style"));
        CHECK(!has_unescaped_tag(html, u8"base"));
        CHECK(!has_unescaped_tag(html, u8"applet"));
        CHECK(!has_unescaped_tag(html, u8"meta"));
        CHECK(!has_unescaped_tag(html, u8"link"));
        CHECK(!has_unescaped_tag(html, u8"form"));
        CHECK(!has_unescaped_tag(html, u8"textarea"));
        CHECK(!has_unescaped_tag(html, u8"marquee"));
        CHECK(!has_unescaped_tag(html, u8"noscript"));
        CHECK(!has_unescaped_tag(html, u8"select"));
    }

    void assert_no_raw_event_handlers(::pltxt2htm::container::U8StringView html) noexcept {
        CHECK(!contains_u8(html, u8"<img onerror="));
        CHECK(!contains_u8(html, u8"<img onload="));
        CHECK(!contains_u8(html, u8"<body onload="));
        CHECK(!contains_u8(html, u8"<body onpageshow="));
        CHECK(!contains_u8(html, u8"<svg onload="));
        CHECK(!contains_u8(html, u8"<svg/onload="));
        CHECK(!contains_u8(html, u8"<a onmouseover="));
        CHECK(!contains_u8(html, u8"<a onclick="));
    }

    // Build a view without relying on implicit U8String conversions.
    [[nodiscard]]
    auto to_view(::pltxt2htm::container::U8String const& s) noexcept -> ::pltxt2htm::container::U8StringView {
        return ::pltxt2htm::container::U8StringView{s.data(), s.size()};
    }

    // Raw HTML tags in plain text
    TEST_CASE("basic-script-tag") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<script>alert(1)</script>");
        CHECK(html == u8"&lt;script&gt;alert(1)&lt;/script&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("script-source-attribute") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<script src=\"http://evil/xss.js\"></script>");
        CHECK(html == u8"&lt;script&nbsp;src=&quot;http://evil/xss.js&quot;&gt;&lt;/script&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("iframe-source-attribute") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<iframe src=\"http://evil\"></iframe>");
        CHECK(html == u8"&lt;iframe&nbsp;src=&quot;http://evil&quot;&gt;&lt;/iframe&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("object-data-attribute") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<object data=\"http://evil\"></object>");
        CHECK(html == u8"&lt;object&nbsp;data=&quot;http://evil&quot;&gt;&lt;/object&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("embed-source-attribute") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<embed src=\"http://evil\">");
        CHECK(html == u8"&lt;embed&nbsp;src=&quot;http://evil&quot;&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("svg-onload-attribute") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<svg onload=\"alert(1)\"></svg>");
        CHECK(html == u8"&lt;svg&nbsp;onload=&quot;alert(1)&quot;&gt;&lt;/svg&gt;");
        assert_no_raw_xss_tags(to_view(html));
        assert_no_raw_event_handlers(to_view(html));
    }
    TEST_CASE("marquee-onstart-attribute") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<marquee onstart=\"alert(1)\">text</marquee>");
        CHECK(html == u8"&lt;marquee&nbsp;onstart=&quot;alert(1)&quot;&gt;text&lt;/marquee&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Case variation bypasses
    TEST_CASE("uppercase-script-tag") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<SCRIPT>alert(1)</SCRIPT>");
        CHECK(html == u8"&lt;SCRIPT&gt;alert(1)&lt;/SCRIPT&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("mixed-case-script-tag") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<ScRiPt>alert(1)</ScRiPt>");
        CHECK(html == u8"&lt;ScRiPt&gt;alert(1)&lt;/ScRiPt&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("mixed-case-script-source") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<sCrIpT src=x>");
        CHECK(html == u8"&lt;sCrIpT&nbsp;src=x&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Scripts inside recognized HTML tags
    TEST_CASE("script-inside-paragraph") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<p><script>alert(1)</script></p>");
        CHECK(html == u8"<p style=\"text-align:left\">&lt;script&gt;alert(1)&lt;/script&gt;</p>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("script-inside-strong-tag") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<strong><script>alert(1)</script></strong>");
        CHECK(html == u8"<strong>&lt;script&gt;alert(1)&lt;/script&gt;</strong>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("script-inside-markdown-bold") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"**<script>alert(1)</script>**");
        CHECK(html == u8"<strong>&lt;script&gt;alert(1)&lt;/script&gt;</strong>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("script-inside-table-cell") {
        auto html =
            ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<table><tr><td><script>alert(1)</script></td></tr></table>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Scripts inside Physics-Lab tags
    TEST_CASE("script-inside-color-tag") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<color=red><script>alert(1)</script></color>");
        CHECK(html == u8"<span style=\"color:red;\">&lt;script&gt;alert(1)&lt;/script&gt;</span>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("script-inside-bold-tag") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<b><script>alert(1)</script></b>");
        CHECK(html == u8"<strong>&lt;script&gt;alert(1)&lt;/script&gt;</strong>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("script-inside-italic-tag") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<i><script>alert(1)</script></i>");
        CHECK(html == u8"<em>&lt;script&gt;alert(1)&lt;/script&gt;</em>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Scripts inside Markdown code spans
    TEST_CASE("script-inside-single-backtick-code") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"`<script>alert(1)</script>`");
        CHECK(html == u8"<code>&lt;script&gt;alert(1)&lt;/script&gt;</code>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("script-inside-double-backtick-code") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"``<script>alert(1)</script>``");
        CHECK(html == u8"<code>&lt;script&gt;alert(1)&lt;/script&gt;</code>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Injection inside Markdown headings
    TEST_CASE("script-inside-level-one-heading") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"# <script>alert(1)</script>");
        CHECK(html == u8"<h1>&lt;script&gt;alert(1)&lt;/script&gt;</h1>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("event-handler-inside-level-two-heading") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"## <img src=x onerror=alert(1)>");
        assert_no_raw_xss_tags(to_view(html));
        assert_no_raw_event_handlers(to_view(html));
    }

    // Unsafe Markdown link URLs
    TEST_CASE("javascript-link-scheme") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"[click](javascript:alert(1))");
        CHECK(html == u8"[click](javascript:alert(1))");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("data-html-link-scheme") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"[data](data:text/html,<script>alert(1)</script>)");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("vbscript-link-scheme") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"[click](vbscript:msgbox('XSS'))");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("file-link-scheme") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"[file](file:///etc/passwd)");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Unsafe Markdown image URLs
    TEST_CASE("javascript-image-scheme") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"![x](javascript:alert(1))");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Event-handler injection
    TEST_CASE("image-onerror-handler") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<img src=x onerror=alert(1)>");
        CHECK(html == u8"&lt;img&nbsp;src=x&nbsp;onerror=alert(1)&gt;");
        assert_no_raw_xss_tags(to_view(html));
        assert_no_raw_event_handlers(to_view(html));
    }
    TEST_CASE("body-onload-handler") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<body onload=alert(1)>");
        CHECK(html == u8"&lt;body&nbsp;onload=alert(1)&gt;");
        assert_no_raw_event_handlers(to_view(html));
    }
    TEST_CASE("body-onpageshow-handler") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<body onpageshow=alert(1)>");
        CHECK(html == u8"&lt;body&nbsp;onpageshow=alert(1)&gt;");
        assert_no_raw_event_handlers(to_view(html));
    }
    TEST_CASE("anchor-onmouseover-handler") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<a onmouseover=\"alert(1)\">hover</a>");
        CHECK(html == u8"&lt;a&nbsp;onmouseover=&quot;alert(1)&quot;&gt;hover&lt;/a&gt;");
        assert_no_raw_event_handlers(to_view(html));
    }
    TEST_CASE("anchor-onclick-handler") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<a onclick=\"alert(1)\">click</a>");
        CHECK(html == u8"&lt;a&nbsp;onclick=&quot;alert(1)&quot;&gt;click&lt;/a&gt;");
        assert_no_raw_event_handlers(to_view(html));
    }

    // Attribute injection in Physics-Lab tags
    TEST_CASE("malformed-color-attribute") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<color=\"\"><script>alert(1)</script>\">text</color>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("event-handler-in-color-tag") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<color=red onmouseover=alert(1)>text</color>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Unsafe external-tag URLs
    TEST_CASE("javascript-external-tag-url") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<external=javascript:alert(1)>x</external>");
        CHECK(html == u8"&lt;external=javascript:alert(1)&gt;x&lt;/external&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("quoted-javascript-external-tag-url") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<external=javascript:alert('XSS')>clickme</external>");
        CHECK(html == u8"&lt;external=javascript:alert(&apos;XSS&apos;)&gt;clickme&lt;/external&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("event-handler-in-external-url") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"<external=https://main.com\" onmouseover=\"alert('XSS')\">content</external>");
        assert_no_raw_xss_tags(to_view(html));
        assert_no_raw_event_handlers(to_view(html));
    }

    // Combined and nested injection vectors
    TEST_CASE("script-inside-colored-bold-text") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<color=blue>**<script>alert(1)</script>**</color>");
        CHECK(html == u8"<span style=\"color:blue;\"><strong>&lt;script&gt;alert(1)&lt;/script&gt;</strong></span>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("javascript-link-inside-color-tag") {
        auto html =
            ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<color=red><a href=\"javascript:alert(1)\">click</a></color>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Entity-encoded URL schemes
    TEST_CASE("decimal-entity-encoded-javascript-url") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8R"([click](&#106;&#97;&#118;&#97;&#115;&#99;&#114;&#105;&#112;&#116;&#58;&#97;&#108;&#101;&#114;&#116;&#40;&#41;))");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Polyglot payloads
    TEST_CASE("svg-polyglot-payload") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"javascript:/*--></title></style></textarea></script></xmp> "
            u8"<svg/onload='+/'\"`/+/onmouseover=1/+/[*/[]/+alert(42);//'>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Identifier and parameter injection
    TEST_CASE("experiment-id-event-handler") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<experiment=abc\"onclick=\"alert(1)>text</experiment>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("discussion-id-event-handler") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<discussion=abc\"onclick=\"alert(1)>text</discussion>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("experiments-id-event-handler") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<experiments=abc\"onclick=\"alert(1)>text</experiments>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("discussions-id-event-handler") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<discussions=abc\"onclick=\"alert(1)>text</discussions>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("user-id-event-handler") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<user=abc\"onclick=\"alert(1)>text</user>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Repeated conversion must not introduce raw HTML tags. Entities are escaped again
    // during the second pass, so the exact output can differ.
    TEST_CASE("script-round-trip-safety") {
        auto html1 = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<script>alert(1)</script>");
        auto html2 = ::pltxt2htm_test::pltxt2fixedadv_htmld(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }
    TEST_CASE("image-handler-round-trip-safety") {
        auto html1 = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<img src=x onerror=alert(1)>");
        auto html2 = ::pltxt2htm_test::pltxt2fixedadv_htmld(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }
    TEST_CASE("svg-handler-round-trip-safety") {
        auto html1 = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<svg/onload=alert(1)>");
        auto html2 = ::pltxt2htm_test::pltxt2fixedadv_htmld(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }
    TEST_CASE("body-handler-round-trip-safety") {
        auto html1 = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<body onload=alert(1)>");
        auto html2 = ::pltxt2htm_test::pltxt2fixedadv_htmld(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }
    TEST_CASE("iframe-javascript-round-trip-safety") {
        auto html1 = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<iframe src=javascript:alert(1)>");
        auto html2 = ::pltxt2htm_test::pltxt2fixedadv_htmld(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }
    TEST_CASE("meta-refresh-round-trip-safety") {
        auto html1 =
            ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<meta http-equiv=\"refresh\" content=\"0;url=http://evil\">");
        auto html2 = ::pltxt2htm_test::pltxt2fixedadv_htmld(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }

    // Safety across rendering backends
    TEST_CASE("fixed-backend-script-escaping") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<script>alert(1)</script>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("common-backend-script-escaping") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<script>alert(1)</script>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("common-backend-image-handler-escaping") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<img src=x onerror=alert(1)>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("fixed-backend-svg-handler-escaping") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<svg/onload=alert(1)>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Injection in mixed Markdown contexts
    TEST_CASE("strikethrough-script-escaping") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"~~<script>alert(1)</script>~~");
        CHECK(html == u8"<del>&lt;script&gt;alert(1)&lt;/script&gt;</del>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("unordered-list-script-escaping") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"- <script>alert(1)</script>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("unchecked-checkbox-script-escaping") {
        // Checkbox emits a legitimate <input> tag, so we verify script/iframe/object/svg
        // are escaped rather than using assert_no_raw_xss_tags (which flags <input>).
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"- [ ] <script>alert(1)</script>");
        auto v = to_view(html);
        CHECK(contains_u8(v, u8"&lt;script&gt;alert(1)&lt;/script&gt;"));
        CHECK(!has_unescaped_tag(v, u8"script"));
        CHECK(!has_unescaped_tag(v, u8"iframe"));
        CHECK(!has_unescaped_tag(v, u8"object"));
        CHECK(!has_unescaped_tag(v, u8"svg"));
        // Also verify standard checkbox <input> is present
        CHECK(contains_u8(v, u8"<input type=\"checkbox\" disabled>"));
    }
    TEST_CASE("lowercase-checked-checkbox-script") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"- [x] <script>alert(1)</script>");
        auto v = to_view(html);
        CHECK(contains_u8(v, u8"&lt;script&gt;alert(1)&lt;/script&gt;"));
        CHECK(!has_unescaped_tag(v, u8"script"));
        CHECK(!has_unescaped_tag(v, u8"iframe"));
        CHECK(!has_unescaped_tag(v, u8"object"));
        CHECK(!has_unescaped_tag(v, u8"svg"));
        // Checked variant
        CHECK(contains_u8(v, u8"<input type=\"checkbox\" disabled checked>"));
    }
    TEST_CASE("uppercase-checked-checkbox-script") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"- [X] <script>alert(1)</script>");
        auto v = to_view(html);
        CHECK(contains_u8(v, u8"&lt;script&gt;alert(1)&lt;/script&gt;"));
        CHECK(!has_unescaped_tag(v, u8"script"));
        CHECK(!has_unescaped_tag(v, u8"iframe"));
        CHECK(!has_unescaped_tag(v, u8"object"));
        CHECK(!has_unescaped_tag(v, u8"svg"));
        // Uppercase X also checked
        CHECK(contains_u8(v, u8"<input type=\"checkbox\" disabled checked>"));
    }
    TEST_CASE("blockquote-script-escaping") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"> <script>alert(1)</script>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("fenced-code-script-escaping") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"```html\n<script>alert(1)</script>\n```");
        assert_no_raw_xss_tags(to_view(html));
    }

    // JavaScript schemes split by whitespace
    TEST_CASE("javascript-link-tab-bypass") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"[x](jav\tascript:alert(1))");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("javascript-link-newline-bypass") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"[x](jav\nascript:alert(1))");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Unrecognized angle-bracket sequences
    TEST_CASE("nested-angle-script-tag") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<<script>alert(1)//<</script>");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("incomplete-script-source-tag") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<script src=http://evil/xss.js?<b>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // CSS expressions and style tags
    TEST_CASE("javascript-css-expression") {
        auto html =
            ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<div style=\"background-image: url(javascript:alert(1))\">");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("style-import-url") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<style>@import url(http://evil/xss.css);</style>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // Inline input tags and checkbox rendering
    TEST_CASE("inline-checkbox-literal") {
        // inline <input> is literal; checkbox inputs only render inside list items
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<input type=\"checkbox\" disabled>");
        CHECK(html == u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&gt;");
        // still no dangerous tags
        CHECK(!has_unescaped_tag(to_view(html), u8"script"));
        CHECK(!has_unescaped_tag(to_view(html), u8"iframe"));
        CHECK(!has_unescaped_tag(to_view(html), u8"svg"));
    }
    TEST_CASE("checked-checkbox-literal") {
        // checked checkbox inline is still literal
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<input type=\"checkbox\" disabled checked>");
        CHECK(html == u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&nbsp;checked&gt;");
    }
    TEST_CASE("reversed-checkbox-attributes-literal") {
        // reversed attribute order inline is still literal
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<input disabled checked type=\"checkbox\">");
        CHECK(html == u8"&lt;input&nbsp;disabled&nbsp;checked&nbsp;type=&quot;checkbox&quot;&gt;");
    }
    TEST_CASE("text-input-literal") {
        // <input type="text"> should be escaped (not a checkbox)
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<input type=\"text\">");
        CHECK(html == u8"&lt;input&nbsp;type=&quot;text&quot;&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    TEST_CASE("checkbox-event-handler-literal") {
        // <input with event handler should be escaped
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<input type=\"checkbox\" onfocus=\"alert(1)\">");
        CHECK(html == u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;onfocus=&quot;alert(1)&quot;&gt;");
        assert_no_raw_xss_tags(to_view(html));
        assert_no_raw_event_handlers(to_view(html));
    }
    TEST_CASE("unchecked-checkbox-literal") {
        // <input> without disabled should be escaped
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<input type=\"checkbox\">");
        CHECK(html == u8"&lt;input&nbsp;type=&quot;checkbox&quot;&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
} // TEST_SUITE("xss")
