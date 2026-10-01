#include <cstddef>
#include <cstring>
#include <cassert>
#include <utility>
#include <fast_io/fast_io.h>
// fast_io only ships a runtime install-path backend for these platforms; the
// banner reports `<unknown>` where there is none (e.g. wasm32-wasip1).
#if defined(__linux__) || defined(_WIN32) || defined(__APPLE__)
    #include <fast_io/fast_io_driver/install_path.h>
#endif
#include <pltxt2htm/pltxt2htm.hh>

enum class TargetType : unsigned {
    indeterminate = 0,
    html4unittest,
    common_html,
    fixedadv_html,
    plunity_text
};

constexpr auto usage = ::pltxt2htm::container::U8StringView{
    u8R"(Usage:
    pltxt2htm [-v|--version]
    pltxt2htm [-h|help]
    echo "example" | pltxt2htm --target common_html
    echo "example" | pltxt2htm --target common_html -o <output file>
    echo "example" | pltxt2htm --target html4unittest
    echo "example" | pltxt2htm --target html4unittest -o <output file>
    echo "example" | pltxt2htm --target fixedadv_html --host <host name> --project <project name> --visitor <visitor name> --author <author name> --coauthors <coauthors string>
    echo "example" | pltxt2htm --target fixedadv_html --host <host name> --project <project name> --visitor <visitor name> --author <author name> --coauthors <coauthors string> -o <output file>
    echo "example" | pltxt2htm --target plunity_text --project <project name> --visitor <visitor name> --author <author name> --coauthors <coauthors string>
    echo "example" | pltxt2htm --target plunity_text --project <project name> --visitor <visitor name> --author <author name> --coauthors <coauthors string> -o <output file>
)"};

namespace {

/**
 * @brief Get the directory the running executable was loaded from
 * @return UTF-8 install directory without a trailing separator, or an empty
 *         string when the current platform has no runtime source for it
 */
[[nodiscard]] ::fast_io::u8string get_installed_dir() noexcept {
#if defined(__linux__) || defined(_WIN32) || defined(__APPLE__)
    #if __cpp_exceptions >= 199711L
    try {
        return ::fast_io::get_module_install_path().path_name;
    } catch (::fast_io::error const&) {
        return {};
    }
    #else
    return ::fast_io::get_module_install_path().path_name;
    #endif
#else
    return {};
#endif
}

void print_installed_dir() noexcept {
    auto const installed_dir = get_installed_dir();
    if (installed_dir.empty()) {
        ::fast_io::println(::fast_io::u8c_stdout(), u8"* installed dir: <unknown>");
        return;
    }
    ::fast_io::println(::fast_io::u8c_stdout(), u8"* installed dir: ", installed_dir);
}

} // namespace

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
        print_installed_dir();
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
    for (::std::size_t i{1}; ::std::cmp_less(i, argc); ++i) {
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
        if (::std::strcmp(argv[i], "--target") == 0) {
            if (i == static_cast<::std::size_t>(argc) - 1) [[unlikely]] {
                ::fast_io::perrln("Missing target");
                return 1;
            }
            if (::std::strcmp(argv[i + 1], "html4unittest") == 0) {
                target_type = ::TargetType::html4unittest;
            }
            else if (::std::strcmp(argv[i + 1], "common_html") == 0) {
                target_type = ::TargetType::common_html;
            }
            else if (::std::strcmp(argv[i + 1], "fixedadv_html") == 0) {
                target_type = ::TargetType::fixedadv_html;
            }
            else if (::std::strcmp(argv[i + 1], "plunity_text") == 0) {
                target_type = ::TargetType::plunity_text;
            }
            else {
                // argv is an OS-encoded string printed to the native stderr, so os_c_str is the right tool here.
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
            ::fast_io::println("pltxt2htm v", ::pltxt2htm::version::major, ".", ::pltxt2htm::version::minor, ".",
                               ::pltxt2htm::version::patch);
            return 0;
        }
        else [[unlikely]] {
            // argv is an OS-encoded string printed to the native stderr, so os_c_str is the right tool here.
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
    case ::TargetType::plunity_text: {
        if (host != nullptr) [[unlikely]] {
            ::fast_io::perrln("** You can not specify host when `--target` is plunity_text");
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
    case ::TargetType::html4unittest: {
        if (host != nullptr || project != nullptr || visitor != nullptr || author != nullptr || coauthors != nullptr)
            [[unlikely]] {
            ::fast_io::perrln(
                "** You can not specify host/project/visitor/author/coauthors when `--target` is "
                "html4unittest");
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
        ::pltxt2htm::container::U8String input_text{};
        ::fast_io::io::scan(::fast_io::u8c_stdin(), ::fast_io::mnp::whole_get(input_text));

        ::pltxt2htm::container::U8String html;
        if (target_type == ::TargetType::html4unittest) {
#ifdef NDEBUG
            constexpr auto ndebug = ::pltxt2htm::Contracts::ignore;
#else
            constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
#endif
            auto ast = ::pltxt2htm::parse_pltxt<ndebug>(::pltxt2htm::container::U8StringView{input_text});
            ::pltxt2htm::optimize_ast<ndebug>(ast);
            html = ::pltxt2htm::details::plweb_text_backend<ndebug,
                                                            ::pltxt2htm::details::PlWebTextBackendMode::pltxt4unittest>(
                ast, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        }
        else if (target_type == ::TargetType::common_html) {
            html = ::pltxt2htm::pltxt2common_html<
#ifdef NDEBUG
                ::pltxt2htm::Contracts::ignore
#else
                ::pltxt2htm::Contracts::quick_enforce
#endif
                >(::pltxt2htm::container::U8StringView{input_text});
        }
        else if (target_type == ::TargetType::fixedadv_html) {
            html = ::pltxt2htm::pltxt2fixedadv_html<
#ifdef NDEBUG
                ::pltxt2htm::Contracts::ignore
#else
                ::pltxt2htm::Contracts::quick_enforce
#endif
                >(::pltxt2htm::container::U8StringView{input_text},
                  ::pltxt2htm::container::U8StringView::from_c_str(host),
                  ::pltxt2htm::container::U8StringView::from_c_str(project),
                  ::pltxt2htm::container::U8StringView::from_c_str(visitor),
                  ::pltxt2htm::container::U8StringView::from_c_str(author),
                  ::pltxt2htm::container::U8StringView::from_c_str(coauthors));
        }
        else if (target_type == ::TargetType::plunity_text) {
            html = ::pltxt2htm::pltxt2plunity_introduction<
#ifdef NDEBUG
                ::pltxt2htm::Contracts::ignore
#else
                ::pltxt2htm::Contracts::quick_enforce
#endif
                >(::pltxt2htm::container::U8StringView{input_text},
                  ::pltxt2htm::container::U8StringView::from_c_str(project),
                  ::pltxt2htm::container::U8StringView::from_c_str(visitor),
                  ::pltxt2htm::container::U8StringView::from_c_str(author),
                  ::pltxt2htm::container::U8StringView::from_c_str(coauthors));
        }
        else [[unlikely]] {
            ::pltxt2htm::details::unreachable<
#ifdef NDEBUG
                ::pltxt2htm::Contracts::ignore
#else
                ::pltxt2htm::Contracts::quick_enforce
#endif
                >();
        }
        if (output_file_path == nullptr) {
            ::fast_io::println(::fast_io::u8c_stdout(), html);
        }
        else {
            // native_file takes an OS path, which is not UTF-8 text, so os_c_str is the right tool here.
            auto const output_file =
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
