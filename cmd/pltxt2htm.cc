#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <fast_io/fast_io.h>
#include <pltxt2htm/pltxt2htm.hh>

enum class TargetType : ::std::uint_least32_t {
    indeterminate = 0,
    advanced_html,
    common_html,
    fixedadv_html
};

constexpr ::fast_io::u8string_view usage{
    u8R"(Usage:
    pltxt2htm [-v|--version]
    pltxt2htm [-h|help]
    echo "example" | pltxt2htm --target common_html
    echo "example" | pltxt2htm --target common_html -o <output file>
    echo "example" | pltxt2htm --target advanced_html
    echo "example" | pltxt2htm --target advanced_html -o <output file>
    echo "example" | pltxt2htm --target fixedadv_html --host <host name> --project <project name> --visitor <visitor name> --author <author name> --coauthors <coauthors string>
    echo "example" | pltxt2htm --target fixedadv_html --host <host name> --project <project name> --visitor <visitor name> --author <author name> --coauthors <coauthors string> -o <output file>
)"};

int main(int argc, char const* const* const argv) noexcept {
    if (argc == 1) {
        ::fast_io::print(
            "pltxt2htm\n"
            "* C++ exception: "
#if __cpp_exceptions >= 199711L
            "enable\n"
#else
            "disable\n"
#endif
            "* C++ rtti: "
#if __cpp_rtti >= 199711L
            "enable\n"
#else
            "disable\n"
#endif
            "* build mode: "
#ifdef NDEBUG
            "release\n"
#else
            "debug\n"
#endif
            "* build time: " __TIMESTAMP__
            "\n"
#if defined(__clang__)
            "* compiler: " __VERSION__ "\n"
#elif defined(__GNUC__)
            "* compiler: GCC " __VERSION__ "\n"
#elif defined(_MSC_VER) && !defined(__clang__)
    #pragma push_macro("PLTXT2HTM_DETAILS_TO_STR")
    #define PLTXT2HTM_DETAILS_TO_STR(x) #x
    #pragma push_macro("PLTXT2HTM_TO_STR")
    #define PLTXT2HTM_TO_STR(x) PLTXT2HTM_DETAILS_TO_STR(x)
            "* compiler: MSVC " PLTXT2HTM_TO_STR(_MSC_FULL_VER) "\n"
#endif
#if __has_include("repo_info.ignore")
    #include "repo_info.ignore"
#endif
        );
        return 0;
    }

    // target type
    ::TargetType target_type = TargetType::indeterminate;
    // host/project/visitor/author/coauthors for fixedadv_html target
    char8_t const* host = nullptr;
    char8_t const* project = nullptr;
    char8_t const* visitor = nullptr;
    char8_t const* author = nullptr;
    char8_t const* coauthors = nullptr;
    // store output file path, can be optional
    char const* output_file_path = nullptr;
    for (::std::size_t i{1}; i < static_cast<::std::size_t>(argc); ++i) {
        if (::std::strcmp(argv[i], "--host") == 0) {
            if (i == static_cast<::std::size_t>(argc) - 1) [[unlikely]] {
                ::fast_io::perrln("You must specify host name after `--host`");
                return 1;
            }
            if (host != nullptr) [[unlikely]] {
                ::fast_io::perrln("You can only specify one host name");
                return 1;
            }
            host = reinterpret_cast<char8_t const*>(argv[++i]);
            continue;
        }
        else if (::std::strcmp(argv[i], "--target") == 0) {
            if (i == static_cast<::std::size_t>(argc) - 1) [[unlikely]] {
                ::fast_io::perrln("Missing target");
                return 1;
            }
            if (::std::strcmp(argv[i + 1], "advanced_html") == 0) {
                target_type = ::TargetType::advanced_html;
            }
            else if (::std::strcmp(argv[i + 1], "common_html") == 0) {
                target_type = ::TargetType::common_html;
            }
            else if (::std::strcmp(argv[i + 1], "fixedadv_html") == 0) {
                target_type = ::TargetType::fixedadv_html;
            }
            else {
                ::fast_io::perrln("Invalid target: ", ::fast_io::mnp::os_c_str(argv[i + 1]));
                return 1;
            }
            ++i;
        }
        else if (::std::strcmp(argv[i], "-o") == 0) {
            if (i == static_cast<::std::size_t>(argc) - 1) [[unlikely]] {
                ::fast_io::perrln("You must specify output file after `-o`");
                return 1;
            }
            if (output_file_path != nullptr) [[unlikely]] {
                ::fast_io::perrln("You can only specify one output file");
                return 1;
            }
            output_file_path = argv[++i];
            continue;
        }
        else if (::std::strcmp(argv[i], "--project") == 0) {
            if (i == static_cast<::std::size_t>(argc) - 1) [[unlikely]] {
                ::fast_io::perrln("You must specify project name after `--project`");
                return 1;
            }
            if (project != nullptr) [[unlikely]] {
                ::fast_io::perrln("You can only specify one project name");
                return 1;
            }
            project = reinterpret_cast<char8_t const*>(argv[++i]);
            continue;
        }
        else if (::std::strcmp(argv[i], "--visitor") == 0) {
            if (i == static_cast<::std::size_t>(argc) - 1) [[unlikely]] {
                ::fast_io::perrln("You must specify visitor name after `--visitor`");
                return 1;
            }
            if (visitor != nullptr) [[unlikely]] {
                ::fast_io::perrln("You can only specify one visitor name");
                return 1;
            }
            visitor = reinterpret_cast<char8_t const*>(argv[++i]);
            continue;
        }
        else if (::std::strcmp(argv[i], "--author") == 0) {
            if (i == static_cast<::std::size_t>(argc) - 1) [[unlikely]] {
                ::fast_io::perrln("You must specify author name after `--author`");
                return 1;
            }
            if (author != nullptr) [[unlikely]] {
                ::fast_io::perrln("You can only specify one author name");
                return 1;
            }
            author = reinterpret_cast<char8_t const*>(argv[++i]);
            continue;
        }
        else if (::std::strcmp(argv[i], "--coauthors") == 0) {
            if (i == static_cast<::std::size_t>(argc) - 1) [[unlikely]] {
                ::fast_io::perrln("You must specify coauthors string after `--coauthors`");
                return 1;
            }
            if (coauthors != nullptr) [[unlikely]] {
                ::fast_io::perrln("You can only specify one coauthors string");
                return 1;
            }
            coauthors = reinterpret_cast<char8_t const*>(argv[++i]);
            continue;
        }
        else if (::std::strcmp(argv[i], "-h") == 0 || ::std::strcmp(argv[i], "--help") == 0) {
            if (i != 1) [[unlikely]] {
                ::fast_io::perrln(
                    "You can only use `pltxt2htm [-h|--help]` without another options to show "
                    "helps");
                return 1;
            }
            ::fast_io::println(::fast_io::u8c_stdout(), usage);
            return 0;
        }
        else if (::std::strcmp(argv[i], "-v") == 0 || ::std::strcmp(argv[i], "--version") == 0) {
            if (i != 1) [[unlikely]] {
                ::fast_io::perrln(
                    "You can only use `pltxt2htm [--version|-v]` without another options to show "
                    "version");
                return 1;
            }
            ::fast_io::println(::fast_io::concat_fast_io("pltxt2htm v", ::pltxt2htm::version::major, ".",
                                                         ::pltxt2htm::version::minor, ".",
                                                         ::pltxt2htm::version::patch));
            return 0;
        }
        else [[unlikely]] {
            ::fast_io::perrln("Unknown option: ", ::fast_io::mnp::os_c_str(argv[i]));
            return 1;
        }
    }

    switch (target_type) {
    case ::TargetType::fixedadv_html: {
        if (host == nullptr) [[unlikely]] {
            ::fast_io::perrln("** You must specify host name with `--host`");
            ::fast_io::println(::fast_io::u8c_stderr(), usage);
            return 1;
        }
        if (project == nullptr) [[unlikely]] {
            ::fast_io::perrln("** You must specify project name with `--project`");
            ::fast_io::println(::fast_io::u8c_stderr(), usage);
            return 1;
        }
        if (visitor == nullptr) [[unlikely]] {
            ::fast_io::perrln("** You must specify visitor name with `--visitor`");
            ::fast_io::println(::fast_io::u8c_stderr(), usage);
            return 1;
        }
        if (author == nullptr) [[unlikely]] {
            ::fast_io::perrln("** You must specify author name with `--author`");
            ::fast_io::println(::fast_io::u8c_stderr(), usage);
            return 1;
        }
        if (coauthors == nullptr) [[unlikely]] {
            ::fast_io::perrln("** You must specify coauthors string with `--coauthors`");
            ::fast_io::println(::fast_io::u8c_stderr(), usage);
            return 1;
        }
        break;
    }
    case ::TargetType::advanced_html: {
        if (host != nullptr || project != nullptr || visitor != nullptr || author != nullptr || coauthors != nullptr)
            [[unlikely]] {
            ::fast_io::perrln(
                "** You can not specify host/project/visitor/author/coauthors when `--target` is "
                "advanced_html");
            ::fast_io::println(::fast_io::u8c_stderr(), usage);
            return 1;
        }
        break;
    }
    case ::TargetType::common_html: {
        if (host != nullptr || project != nullptr || visitor != nullptr || author != nullptr || coauthors != nullptr)
            [[unlikely]] {
            ::fast_io::perrln(
                "** You can not specify host/project/visitor/author/coauthors when `--target` is "
                "common_html");
            ::fast_io::println(::fast_io::u8c_stderr(), usage);
            return 1;
        }
        break;
    }
    case ::TargetType::indeterminate: {
        ::fast_io::perrln("** You must specify target type with `--target`");
        return 1;
    }
    }

#if __cpp_exceptions >= 199711L
    try
#endif // __cpp_exceptions >= 199711L
    {
        ::fast_io::u8string input_text{};
        ::fast_io::io::scan(::fast_io::u8c_stdin(), ::fast_io::mnp::whole_get(input_text));

        ::fast_io::u8string html;
        if (target_type == ::TargetType::advanced_html) {
            html = ::pltxt2htm::pltxt2advanced_html<
#ifdef NDEBUG
                true
#else
                false
#endif
                >(::fast_io::mnp::os_c_str(input_text));
        }
        else if (target_type == ::TargetType::common_html) {
            html = ::pltxt2htm::pltxt2common_html<
#ifdef NDEBUG
                true
#else
                false
#endif
                >(::fast_io::mnp::os_c_str(input_text));
        }
        else if (target_type == ::TargetType::fixedadv_html) {
            html = ::pltxt2htm::pltxt2fixedadv_html<
#ifdef NDEBUG
                true
#else
                false
#endif
                >(::fast_io::mnp::os_c_str(input_text), ::fast_io::mnp::os_c_str(host),
                  ::fast_io::mnp::os_c_str(project), ::fast_io::mnp::os_c_str(visitor),
                  ::fast_io::mnp::os_c_str(author), ::fast_io::mnp::os_c_str(coauthors));
        }
        else [[unlikely]] {
            ::exception::unreachable<
#ifdef NDEBUG
                true
#else
                false
#endif
                >();
        }
        if (output_file_path == nullptr) {
            ::fast_io::println(::fast_io::u8c_stdout(), html);
        }
        else {
            auto output_file =
                ::fast_io::native_file{::fast_io::mnp::os_c_str(output_file_path), ::fast_io::open_mode::out};
            auto output_file_handle = ::fast_io::u8native_io_observer{output_file.native_handle()};
            ::fast_io::println(output_file_handle, html);
        }
    }
#if __cpp_exceptions >= 199711L
    catch (::fast_io::error const& e) {
        ::fast_io::perrln(e);
        return 1;
    }
#endif // __cpp_exceptions >= 199711L

    return 0;
}
