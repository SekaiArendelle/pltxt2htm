#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    ::fast_io::u8string_view data{
        u8R"(#oo<Color=<s�<s<s�s�/<h1>e<\�����������<sa\/<h1>�s�/<h1>e<h1>Ns<<�  �����<as\/<h1>�s�/<h1>e<\�������������������<sa\/<h1>Ns<<?���������<sa�/<h1>�s=<s�<s<s�s�/<h1>e<\����TTTTTTTTTTTTT�������>sa\/<h1>�s�/<h1>e<\�����������������<sa\/<h1>Ns<<�  �����<sa\/<h1>�s�/<h1>e���������<s\oo<Color��)"};
    [[maybe_unused]] auto _ = ::pltxt2htm::pltxt2advanced_html(data, u8"_");

    return 0;
}
