#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\\\", u8"_") == ::fast_io::u8string_view{u8"\\"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\\\") == ::fast_io::u8string_view{u8"\\"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\!", u8"_") == ::fast_io::u8string_view{u8"!"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\!") == ::fast_io::u8string_view{u8"!"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\\"", u8"_") == ::fast_io::u8string_view{u8"&quot;"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\\"") == ::fast_io::u8string_view{u8"&quot;"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\#", u8"_") == ::fast_io::u8string_view{u8"#"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\#") == ::fast_io::u8string_view{u8"#"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\$", u8"_") == ::fast_io::u8string_view{u8"$"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\$") == ::fast_io::u8string_view{u8"$"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\%", u8"_") == ::fast_io::u8string_view{u8"%"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\%") == ::fast_io::u8string_view{u8"%"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\&", u8"_") == ::fast_io::u8string_view{u8"&amp;"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\&") == ::fast_io::u8string_view{u8"&amp;"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\\'", u8"_") == ::fast_io::u8string_view{u8"&apos;"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\\'") == ::fast_io::u8string_view{u8"&apos;"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\(", u8"_") == ::fast_io::u8string_view{u8"("});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\(") == ::fast_io::u8string_view{u8"("});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\)", u8"_") == ::fast_io::u8string_view{u8")"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\)") == ::fast_io::u8string_view{u8")"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\*", u8"_") == ::fast_io::u8string_view{u8"*"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\*") == ::fast_io::u8string_view{u8"*"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\+", u8"_") == ::fast_io::u8string_view{u8"+"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\+") == ::fast_io::u8string_view{u8"+"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\,", u8"_") == ::fast_io::u8string_view{u8","});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\,") == ::fast_io::u8string_view{u8","});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\-", u8"_") == ::fast_io::u8string_view{u8"-"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\-") == ::fast_io::u8string_view{u8"-"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\.", u8"_") == ::fast_io::u8string_view{u8"."});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\.") == ::fast_io::u8string_view{u8"."});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\/", u8"_") == ::fast_io::u8string_view{u8"/"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\/") == ::fast_io::u8string_view{u8"/"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\:", u8"_") == ::fast_io::u8string_view{u8":"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\:") == ::fast_io::u8string_view{u8":"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\;", u8"_") == ::fast_io::u8string_view{u8";"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\;") == ::fast_io::u8string_view{u8";"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\<", u8"_") == ::fast_io::u8string_view{u8"&lt;"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\<") == ::fast_io::u8string_view{u8"&lt;"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\=", u8"_") == ::fast_io::u8string_view{u8"="});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\=") == ::fast_io::u8string_view{u8"="});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\>", u8"_") == ::fast_io::u8string_view{u8"&gt;"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\>") == ::fast_io::u8string_view{u8"&gt;"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\?", u8"_") == ::fast_io::u8string_view{u8"?"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\?") == ::fast_io::u8string_view{u8"?"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\@", u8"_") == ::fast_io::u8string_view{u8"@"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\@") == ::fast_io::u8string_view{u8"@"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\[", u8"_") == ::fast_io::u8string_view{u8"["});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\[") == ::fast_io::u8string_view{u8"["});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\]", u8"_") == ::fast_io::u8string_view{u8"]"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\]") == ::fast_io::u8string_view{u8"]"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\^", u8"_") == ::fast_io::u8string_view{u8"^"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\^") == ::fast_io::u8string_view{u8"^"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\_", u8"_") == ::fast_io::u8string_view{u8"_"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\_") == ::fast_io::u8string_view{u8"_"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\`", u8"_") == ::fast_io::u8string_view{u8"`"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\`") == ::fast_io::u8string_view{u8"`"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\{", u8"_") == ::fast_io::u8string_view{u8"{"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\{") == ::fast_io::u8string_view{u8"{"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\|", u8"_") == ::fast_io::u8string_view{u8"|"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\|") == ::fast_io::u8string_view{u8"|"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\}", u8"_") == ::fast_io::u8string_view{u8"}"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\}") == ::fast_io::u8string_view{u8"}"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\~", u8"_") == ::fast_io::u8string_view{u8"~"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\~") == ::fast_io::u8string_view{u8"~"});
    ::exception::assert_true(::pltxt2htm::pltxt2advanced_html(u8"\\a", u8"_") == ::fast_io::u8string_view{u8"\\a"});
    ::exception::assert_true(::pltxt2htm::pltxt2common_html(u8"\\a") == ::fast_io::u8string_view{u8"\\a"});

    return 0;
}
