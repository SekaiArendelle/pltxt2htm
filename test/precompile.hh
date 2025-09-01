#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

namespace pltxt2htm_test {

::fast_io::u8string pltxt2advanced_htmld(::fast_io::u8string_view pltext) /* throws */;

::fast_io::u8string pltxt2common_html(::fast_io::u8string_view pltext) /* throws */;

::fast_io::u8string pltxt2common_htmld(::fast_io::u8string_view pltext) /* throws */;

::fast_io::u8string pltxt2fixedadv_htmld(::fast_io::u8string_view pltext) /* throws */;

void assert_true(bool cond) noexcept;

} // namespace pltxt2htm_test
