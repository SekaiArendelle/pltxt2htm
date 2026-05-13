#include "precompile.hh"

int main() {
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\\\"), ::fast_io::u8string_view{u8"\\"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\\\"), ::fast_io::u8string_view{u8"\\"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\!"), ::fast_io::u8string_view{u8"!"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\!"), ::fast_io::u8string_view{u8"!"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\\""), ::fast_io::u8string_view{u8"&quot;"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\\""), ::fast_io::u8string_view{u8"&quot;"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\#"), ::fast_io::u8string_view{u8"#"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\#"), ::fast_io::u8string_view{u8"#"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\$"), ::fast_io::u8string_view{u8"$"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\$"), ::fast_io::u8string_view{u8"$"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\%"), ::fast_io::u8string_view{u8"%"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\%"), ::fast_io::u8string_view{u8"%"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\&"), ::fast_io::u8string_view{u8"&amp;"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\&"), ::fast_io::u8string_view{u8"&amp;"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\\'"), ::fast_io::u8string_view{u8"&apos;"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\\'"), ::fast_io::u8string_view{u8"&apos;"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\("), ::fast_io::u8string_view{u8"("});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\("), ::fast_io::u8string_view{u8"("});
    {
        auto const h = ::pltxt2htm_test::pltxt4unittest(u8"\\)");
        pltxt2htm_test_assert_equal(h, ::fast_io::u8string_view{u8")"});
    }
    {
        auto const h = ::pltxt2htm_test::pltxt2common_htmld(u8"\\)");
        pltxt2htm_test_assert_equal(h, ::fast_io::u8string_view{u8")"});
    }
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\*"), ::fast_io::u8string_view{u8"*"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\*"), ::fast_io::u8string_view{u8"*"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\+"), ::fast_io::u8string_view{u8"+"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\+"), ::fast_io::u8string_view{u8"+"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\,"), ::fast_io::u8string_view{u8","});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\,"), ::fast_io::u8string_view{u8","});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\-"), ::fast_io::u8string_view{u8"-"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\-"), ::fast_io::u8string_view{u8"-"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\."), ::fast_io::u8string_view{u8"."});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\."), ::fast_io::u8string_view{u8"."});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\/"), ::fast_io::u8string_view{u8"/"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\/"), ::fast_io::u8string_view{u8"/"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\:"), ::fast_io::u8string_view{u8":"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\:"), ::fast_io::u8string_view{u8":"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\;"), ::fast_io::u8string_view{u8";"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\;"), ::fast_io::u8string_view{u8";"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\<"), ::fast_io::u8string_view{u8"&lt;"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\<"), ::fast_io::u8string_view{u8"&lt;"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\="), ::fast_io::u8string_view{u8"="});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\="), ::fast_io::u8string_view{u8"="});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\>"), ::fast_io::u8string_view{u8"&gt;"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\>"), ::fast_io::u8string_view{u8"&gt;"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\?"), ::fast_io::u8string_view{u8"?"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\?"), ::fast_io::u8string_view{u8"?"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\@"), ::fast_io::u8string_view{u8"@"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\@"), ::fast_io::u8string_view{u8"@"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\["), ::fast_io::u8string_view{u8"["});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\["), ::fast_io::u8string_view{u8"["});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\]"), ::fast_io::u8string_view{u8"]"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\]"), ::fast_io::u8string_view{u8"]"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\^"), ::fast_io::u8string_view{u8"^"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\^"), ::fast_io::u8string_view{u8"^"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\_"), ::fast_io::u8string_view{u8"_"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\_"), ::fast_io::u8string_view{u8"_"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\`"), ::fast_io::u8string_view{u8"`"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\`"), ::fast_io::u8string_view{u8"`"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\{"), ::fast_io::u8string_view{u8"{"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\{"), ::fast_io::u8string_view{u8"{"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\|"), ::fast_io::u8string_view{u8"|"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\|"), ::fast_io::u8string_view{u8"|"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\}"), ::fast_io::u8string_view{u8"}"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\}"), ::fast_io::u8string_view{u8"}"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\~"), ::fast_io::u8string_view{u8"~"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\~"), ::fast_io::u8string_view{u8"~"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt4unittest(u8"\\a"), ::fast_io::u8string_view{u8"\\a"});
    pltxt2htm_test_assert_equal(::pltxt2htm_test::pltxt2common_htmld(u8"\\a"), ::fast_io::u8string_view{u8"\\a"});

    return 0;
}
