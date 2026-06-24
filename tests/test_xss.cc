#include "precompile.hh"

namespace {

[[nodiscard]]
constexpr bool contains_u8(::fast_io::u8string_view haystack, ::fast_io::u8string_view needle) noexcept {
    if (needle.size() > haystack.size()) {
        return false;
    }
    for (::std::size_t i{}; i + needle.size() <= haystack.size(); ++i) {
        bool match{true};
        for (::std::size_t j{}; j < needle.size(); ++j) {
            if (haystack.index_unchecked(i + j) != needle.index_unchecked(j)) {
                match = false;
                break;
            }
        }
        if (match) [[unlikely]] {
            return true;
        }
    }
    return false;
}

[[nodiscard]]
constexpr bool has_unescaped_tag(::fast_io::u8string_view html, ::fast_io::u8string_view tag) noexcept {
    ::std::size_t const tag_sz{tag.size()};
    if (tag_sz == 0 || tag_sz + 1 > html.size()) {
        return false;
    }
    ::std::size_t const limit{html.size() - tag_sz};
    for (::std::size_t i{}; i < limit; ++i) {
        if (html.index_unchecked(i) != u8'<') {
            continue;
        }
        bool match{true};
        for (::std::size_t j{}; j < tag_sz; ++j) {
            if (html.index_unchecked(i + 1 + j) != tag.index_unchecked(j)) {
                match = false;
                break;
            }
        }
        if (match) [[unlikely]] {
            return true;
        }
    }
    return false;
}

void assert_no_raw_xss_tags(::fast_io::u8string_view html) noexcept {
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"script"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"iframe"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"object"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"embed"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"svg"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"style"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"base"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"applet"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"meta"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"link"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"form"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"textarea"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"marquee"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"noscript"));
    ::pltxt2htm_test::assert_true(!has_unescaped_tag(html, u8"select"));
}

void assert_no_raw_event_handlers(::fast_io::u8string_view html) noexcept {
    ::pltxt2htm_test::assert_true(!contains_u8(html, u8"<img onerror="));
    ::pltxt2htm_test::assert_true(!contains_u8(html, u8"<img onload="));
    ::pltxt2htm_test::assert_true(!contains_u8(html, u8"<body onload="));
    ::pltxt2htm_test::assert_true(!contains_u8(html, u8"<body onpageshow="));
    ::pltxt2htm_test::assert_true(!contains_u8(html, u8"<svg onload="));
    ::pltxt2htm_test::assert_true(!contains_u8(html, u8"<svg/onload="));
    ::pltxt2htm_test::assert_true(!contains_u8(html, u8"<a onmouseover="));
    ::pltxt2htm_test::assert_true(!contains_u8(html, u8"<a onclick="));
}

// Helper to wrap u8string -> u8string_view for MSVC compat
[[nodiscard]]
auto to_view(::fast_io::u8string const& s) noexcept -> ::fast_io::u8string_view {
    return ::fast_io::u8string_view{s.data(), s.size()};
}

} // unnamed namespace

int main() {
    // ============================================================
    //  1. Basic raw HTML tag injection (plain text)
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<script>alert(1)</script>");
        pltxt2htm_test_assert_equal(html, u8"&lt;script&gt;alert(1)&lt;/script&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<script src=\"http://evil/xss.js\"></script>");
        pltxt2htm_test_assert_equal(html, u8"&lt;script&nbsp;src=&quot;http://evil/xss.js&quot;&gt;&lt;/script&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<iframe src=\"http://evil\"></iframe>");
        pltxt2htm_test_assert_equal(html, u8"&lt;iframe&nbsp;src=&quot;http://evil&quot;&gt;&lt;/iframe&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<object data=\"http://evil\"></object>");
        pltxt2htm_test_assert_equal(html, u8"&lt;object&nbsp;data=&quot;http://evil&quot;&gt;&lt;/object&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<embed src=\"http://evil\">");
        pltxt2htm_test_assert_equal(html, u8"&lt;embed&nbsp;src=&quot;http://evil&quot;&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<svg onload=\"alert(1)\"></svg>");
        pltxt2htm_test_assert_equal(html, u8"&lt;svg&nbsp;onload=&quot;alert(1)&quot;&gt;&lt;/svg&gt;");
        assert_no_raw_xss_tags(to_view(html));
        assert_no_raw_event_handlers(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<marquee onstart=\"alert(1)\">text</marquee>");
        pltxt2htm_test_assert_equal(html, u8"&lt;marquee&nbsp;onstart=&quot;alert(1)&quot;&gt;text&lt;/marquee&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  2. Case variation bypass
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SCRIPT>alert(1)</SCRIPT>");
        pltxt2htm_test_assert_equal(html, u8"&lt;SCRIPT&gt;alert(1)&lt;/SCRIPT&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ScRiPt>alert(1)</ScRiPt>");
        pltxt2htm_test_assert_equal(html, u8"&lt;ScRiPt&gt;alert(1)&lt;/ScRiPt&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sCrIpT src=x>");
        pltxt2htm_test_assert_equal(html, u8"&lt;sCrIpT&nbsp;src=x&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  3. XSS inside recognized HTML tags
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<p><script>alert(1)</script></p>");
        pltxt2htm_test_assert_equal(html, u8"<p>&lt;script&gt;alert(1)&lt;/script&gt;</p>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong><script>alert(1)</script></strong>");
        pltxt2htm_test_assert_equal(html, u8"<strong>&lt;script&gt;alert(1)&lt;/script&gt;</strong>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"**<script>alert(1)</script>**");
        pltxt2htm_test_assert_equal(html, u8"<strong>&lt;script&gt;alert(1)&lt;/script&gt;</strong>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td><script>alert(1)</script></td></tr></table>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  4. XSS inside pl-tags
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><script>alert(1)</script></color>");
        pltxt2htm_test_assert_equal(html, u8"<span style=\"color:red;\">&lt;script&gt;alert(1)&lt;/script&gt;</span>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><script>alert(1)</script></b>");
        pltxt2htm_test_assert_equal(html, u8"<strong>&lt;script&gt;alert(1)&lt;/script&gt;</strong>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<i><script>alert(1)</script></i>");
        pltxt2htm_test_assert_equal(html, u8"<em>&lt;script&gt;alert(1)&lt;/script&gt;</em>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  5. XSS inside markdown code spans (should be fully escaped)
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`<script>alert(1)</script>`");
        pltxt2htm_test_assert_equal(html, u8"<code>&lt;script&gt;alert(1)&lt;/script&gt;</code>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"``<script>alert(1)</script>``");
        pltxt2htm_test_assert_equal(html, u8"<code>&lt;script&gt;alert(1)&lt;/script&gt;</code>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  6. XSS inside markdown atx headers
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# <script>alert(1)</script>");
        pltxt2htm_test_assert_equal(html, u8"<h1>&lt;script&gt;alert(1)&lt;/script&gt;</h1>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"## <img src=x onerror=alert(1)>");
        assert_no_raw_xss_tags(to_view(html));
        assert_no_raw_event_handlers(to_view(html));
    }

    // ============================================================
    //  7. Markdown link URL injection
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"[click](javascript:alert(1))");
        pltxt2htm_test_assert_equal(html, u8"[click](javascript:alert(1))");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"[data](data:text/html,<script>alert(1)</script>)");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"[click](vbscript:msgbox('XSS'))");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"[file](file:///etc/passwd)");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  8. Markdown image URL injection
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"![x](javascript:alert(1))");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  9. Event handler injection via various vectors
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<img src=x onerror=alert(1)>");
        pltxt2htm_test_assert_equal(html, u8"&lt;img&nbsp;src=x&nbsp;onerror=alert(1)&gt;");
        assert_no_raw_xss_tags(to_view(html));
        assert_no_raw_event_handlers(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<body onload=alert(1)>");
        pltxt2htm_test_assert_equal(html, u8"&lt;body&nbsp;onload=alert(1)&gt;");
        assert_no_raw_event_handlers(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<body onpageshow=alert(1)>");
        pltxt2htm_test_assert_equal(html, u8"&lt;body&nbsp;onpageshow=alert(1)&gt;");
        assert_no_raw_event_handlers(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a onmouseover=\"alert(1)\">hover</a>");
        pltxt2htm_test_assert_equal(html, u8"&lt;a&nbsp;onmouseover=&quot;alert(1)&quot;&gt;hover&lt;/a&gt;");
        assert_no_raw_event_handlers(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a onclick=\"alert(1)\">click</a>");
        pltxt2htm_test_assert_equal(html, u8"&lt;a&nbsp;onclick=&quot;alert(1)&quot;&gt;click&lt;/a&gt;");
        assert_no_raw_event_handlers(to_view(html));
    }

    // ============================================================
    //  10. Attribute injection via color / pl-tag values
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=\"\"><script>alert(1)</script>\">text</color>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red onmouseover=alert(1)>text</color>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  11. External tag URL injection
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=javascript:alert(1)>x</external>");
        pltxt2htm_test_assert_equal(html, u8"&lt;external=javascript:alert(1)&gt;x&lt;/external&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=javascript:alert('XSS')>clickme</external>");
        pltxt2htm_test_assert_equal(html,
                                    u8"&lt;external=javascript:alert(&apos;XSS&apos;)&gt;clickme&lt;/external&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<external=https://main.com\" onmouseover=\"alert('XSS')\">content</external>");
        assert_no_raw_xss_tags(to_view(html));
        assert_no_raw_event_handlers(to_view(html));
    }

    // ============================================================
    //  12. Combined / nested XSS vectors
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=blue>**<script>alert(1)</script>**</color>");
        pltxt2htm_test_assert_equal(
            html, u8"<span style=\"color:blue;\"><strong>&lt;script&gt;alert(1)&lt;/script&gt;</strong></span>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><a href=\"javascript:alert(1)\">click</a></color>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  13. HTML entity encoded XSS (decimal / hex entities in URL)
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8R"([click](&#106;&#97;&#118;&#97;&#115;&#99;&#114;&#105;&#112;&#116;&#58;&#97;&#108;&#101;&#114;&#116;&#40;&#41;))");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  14. Polyglot XSS
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"javascript:/*--></title></style></textarea></script></xmp> "
            u8"<svg/onload='+/'\"`/+/onmouseover=1/+/[*/[]/+alert(42);//'>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  15. ID / parameter injection in experiment/discussion/user
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiment=abc\"onclick=\"alert(1)>text</experiment>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<discussion=abc\"onclick=\"alert(1)>text</discussion>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<user=abc\"onclick=\"alert(1)>text</user>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  16. Round-trip (reflection) stability
    //      Feed XSS input through the converter, then feed the
    //      output through again.  The second pass must also be
    //      XSS-free (no raw HTML tags).  Note: the exact output
    //      differs because & entities are re-escaped on pass 2.
    // ============================================================
    {
        auto html1 = ::pltxt2htm_test::pltxt4unittest(u8"<script>alert(1)</script>");
        auto html2 = ::pltxt2htm_test::pltxt4unittest(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }
    {
        auto html1 = ::pltxt2htm_test::pltxt4unittest(u8"<img src=x onerror=alert(1)>");
        auto html2 = ::pltxt2htm_test::pltxt4unittest(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }
    {
        auto html1 = ::pltxt2htm_test::pltxt4unittest(u8"<svg/onload=alert(1)>");
        auto html2 = ::pltxt2htm_test::pltxt4unittest(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }
    {
        auto html1 = ::pltxt2htm_test::pltxt4unittest(u8"<body onload=alert(1)>");
        auto html2 = ::pltxt2htm_test::pltxt4unittest(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }
    {
        auto html1 = ::pltxt2htm_test::pltxt4unittest(u8"<iframe src=javascript:alert(1)>");
        auto html2 = ::pltxt2htm_test::pltxt4unittest(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }
    {
        auto html1 = ::pltxt2htm_test::pltxt4unittest(u8"<meta http-equiv=\"refresh\" content=\"0;url=http://evil\">");
        auto html2 = ::pltxt2htm_test::pltxt4unittest(to_view(html1));
        assert_no_raw_xss_tags(to_view(html2));
    }

    // ============================================================
    //  17. Cross-backend consistency
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<script>alert(1)</script>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<script>alert(1)</script>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<img src=x onerror=alert(1)>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<svg/onload=alert(1)>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  18. Edge cases — injection in markdown/mixed contexts
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~<script>alert(1)</script>~~");
        pltxt2htm_test_assert_equal(html, u8"<del>&lt;script&gt;alert(1)&lt;/script&gt;</del>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- <script>alert(1)</script>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        // Checkbox emits a legitimate <input> tag, so we verify script/iframe/object/svg
        // are escaped rather than using assert_no_raw_xss_tags (which flags <input>).
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [ ] <script>alert(1)</script>");
        auto v = to_view(html);
        ::pltxt2htm_test::assert_true(contains_u8(v, u8"&lt;script&gt;alert(1)&lt;/script&gt;"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"script"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"iframe"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"object"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"svg"));
        // Also verify standard checkbox <input> is present
        ::pltxt2htm_test::assert_true(contains_u8(v, u8"<input type=\"checkbox\" disabled>"));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [x] <script>alert(1)</script>");
        auto v = to_view(html);
        ::pltxt2htm_test::assert_true(contains_u8(v, u8"&lt;script&gt;alert(1)&lt;/script&gt;"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"script"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"iframe"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"object"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"svg"));
        // Checked variant
        ::pltxt2htm_test::assert_true(contains_u8(v, u8"<input type=\"checkbox\" disabled checked>"));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [X] <script>alert(1)</script>");
        auto v = to_view(html);
        ::pltxt2htm_test::assert_true(contains_u8(v, u8"&lt;script&gt;alert(1)&lt;/script&gt;"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"script"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"iframe"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"object"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"svg"));
        // Uppercase X also checked
        ::pltxt2htm_test::assert_true(contains_u8(v, u8"<input type=\"checkbox\" disabled checked>"));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [x] <script>alert(1)</script>");
        auto v = to_view(html);
        ::pltxt2htm_test::assert_true(contains_u8(v, u8"&lt;script&gt;alert(1)&lt;/script&gt;"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"script"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"iframe"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"object"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"svg"));
        // Checked variant
        ::pltxt2htm_test::assert_true(contains_u8(v, u8"<input type=\"checkbox\" disabled checked>"));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [X] <script>alert(1)</script>");
        auto v = to_view(html);
        ::pltxt2htm_test::assert_true(contains_u8(v, u8"&lt;script&gt;alert(1)&lt;/script&gt;"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"script"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"iframe"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"object"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(v, u8"svg"));
        // Uppercase X also checked
        ::pltxt2htm_test::assert_true(contains_u8(v, u8"<input type=\"checkbox\" disabled checked>"));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"> <script>alert(1)</script>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```html\n<script>alert(1)</script>\n```");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  19. JavaScript scheme via markdown link with tab/newline
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"[x](jav\tascript:alert(1))");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"[x](jav\nascript:alert(1))");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  20. Unrecognised close-angle-bracket tricks
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<<script>alert(1)//<</script>");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<script src=http://evil/xss.js?<b>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  21. CSS expression / style injection
    // ============================================================
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"background-image: url(javascript:alert(1))\">");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<style>@import url(http://evil/xss.css);</style>");
        assert_no_raw_xss_tags(to_view(html));
    }

    // ============================================================
    //  22. Raw <input> tag parsing (checkbox only, others escaped)
    // ============================================================
    {
        // unchecked checkbox should be parsed as raw <input>
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\" disabled>");
        pltxt2htm_test_assert_equal(html, u8"<input type=\"checkbox\" disabled>");
        // still no dangerous tags
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(to_view(html), u8"script"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(to_view(html), u8"iframe"));
        ::pltxt2htm_test::assert_true(!has_unescaped_tag(to_view(html), u8"svg"));
    }
    {
        // checked checkbox
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\" disabled checked>");
        pltxt2htm_test_assert_equal(html, u8"<input type=\"checkbox\" disabled checked>");
    }
    {
        // reversed order: checked before disabled is still valid
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input disabled checked type=\"checkbox\">");
        pltxt2htm_test_assert_equal(html, u8"<input type=\"checkbox\" disabled checked>");
    }
    {
        // <input type="text"> should be escaped (not a checkbox)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"text\">");
        pltxt2htm_test_assert_equal(html, u8"&lt;input&nbsp;type=&quot;text&quot;&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }
    {
        // <input with event handler should be escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\" onfocus=\"alert(1)\">");
        pltxt2htm_test_assert_equal(html, u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;onfocus=&quot;alert(1)&quot;&gt;");
        assert_no_raw_xss_tags(to_view(html));
        assert_no_raw_event_handlers(to_view(html));
    }
    {
        // <input> without disabled should be escaped
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\">");
        pltxt2htm_test_assert_equal(html, u8"&lt;input&nbsp;type=&quot;checkbox&quot;&gt;");
        assert_no_raw_xss_tags(to_view(html));
    }

    return 0;
}
