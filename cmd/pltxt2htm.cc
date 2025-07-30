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
    echo "example" | pltxt2htm --target advanced_html --host <host name>
    echo "example" | pltxt2htm --target advanced_html --host <host name> -o <output file>
)"};

int main(int argc, char const* const* const argv)
#if __cpp_exceptions < 199711L
    noexcept
#endif // __cpp_exceptions < 199711L
{
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
            "build mode: "
#ifdef NDEBUG
            "release\n"
#else
            "debug\n"
#endif
            // TODO get install path
        );
        return 0;
    }

    // target type
    ::TargetType target_type = TargetType::indeterminate;
    // host prefix of physics-lab-web
    char8_t const* host = nullptr;
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
        } else if (::std::strcmp(argv[i], "--target") == 0) {
            if (i == static_cast<::std::size_t>(argc) - 1) [[unlikely]] {
                ::fast_io::perrln("Missing target");
                return 1;
            }
            if (::std::strcmp(argv[i + 1], "advanced_html") == 0) {
                target_type = ::TargetType::advanced_html;
            } else if (::std::strcmp(argv[i + 1], "common_html") == 0) {
                target_type = ::TargetType::common_html;
            } else if (::std::strcmp(argv[i + 1], "fixedadv_html") == 0) {
                target_type = ::TargetType::fixedadv_html;
            } else {
                ::fast_io::perrln("Invalid target: ", ::fast_io::mnp::os_c_str(argv[i + 1]));
                return 1;
            }
            ++i;
        } else if (::std::strcmp(argv[i], "-o") == 0) {
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
        } else if (::std::strcmp(argv[i], "-h") == 0 || ::std::strcmp(argv[i], "--help") == 0) {
            if (i != 1) [[unlikely]] {
                ::fast_io::perrln(
                    "You can only use `pltxt2htm [-h|--help]` without another options to show "
                    "helps");
                return 1;
            }
            ::fast_io::println(::fast_io::u8c_stdout(), usage);
            return 0;
        } else if (::std::strcmp(argv[i], "-v") == 0 || ::std::strcmp(argv[i], "--version") == 0) {
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
        } else [[unlikely]] {
            ::fast_io::perrln("Unknown option: ", ::fast_io::mnp::os_c_str(argv[i]));
            return 1;
        }
    }

    switch (target_type) {
    case ::TargetType::advanced_html: {
        if (host == nullptr) [[unlikely]] {
            ::fast_io::perrln("** You must specify host name with `--host`");
            ::fast_io::println(::fast_io::u8c_stderr(), usage);
            return 1;
        }
        break;
    }
    case ::TargetType::fixedadv_html:
        [[fallthrough]];
    case ::TargetType::common_html: {
        if (host != nullptr) [[unlikely]] {
            ::fast_io::perrln("** You can not specify host name with `--host` when `--target` is common_html");
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
                >(::fast_io::mnp::os_c_str(input_text), ::fast_io::mnp::os_c_str(host));
        } else if (target_type == ::TargetType::common_html) {
            html = ::pltxt2htm::pltxt2common_html<
#ifdef NDEBUG
                true
#else
                false
#endif
                >(::fast_io::mnp::os_c_str(input_text));
        } else if (target_type == ::TargetType::fixedadv_html) {
            html = ::pltxt2htm::pltxt2fixedadv_html<
#ifdef NDEBUG
                true
#else
                false
#endif
                >(::fast_io::mnp::os_c_str(input_text), ::fast_io::mnp::os_c_str(host));
        } else [[unlikely]] {
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
        } else {
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
