#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <optional>
#include <set>
#include <string>
#include <system_error>
#include <utility>

#include <clang/AST/ASTContext.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>

#include <pltxt2htm/contracts.hh>

namespace {

struct Paths {
    ::std::filesystem::path source_path{};
    ::std::filesystem::path output_path{};
    ::std::filesystem::path include_dir{};
};

struct CliOptions {
    ::std::filesystem::path output_dir{};
};

struct VariantKey {
    ::pltxt2htm::Contracts contract{};
    bool optimize{};

    constexpr auto operator<(VariantKey const& other) const noexcept -> bool {
        if (contract != other.contract) {
            return static_cast<int>(contract) < static_cast<int>(other.contract);
        }
        return optimize < other.optimize;
    }
};

struct ApiInstantiationSet {
    ::std::set<VariantKey> variants{};
};

using ApiInstantiationMap = ::llvm::StringMap<ApiInstantiationSet>;

[[nodiscard]]
auto hardcoded_paths(::std::filesystem::path const& output_dir) -> Paths {
    // Paths are intentionally hardcoded from translang/csharp working directory.
    return Paths{
        .source_path = "./pltxt2htm.cc",
        .output_path = output_dir / "Pltxt2htm.Generated.cs",
        .include_dir = "../../include",
    };
}

[[noreturn]]
void exit_with_error(::std::string const& message) noexcept {
    ::llvm::errs() << "translator error: " << message << '\n';
    ::std::exit(1);
}

[[noreturn]]
void exit_with_error(::llvm::Error error) noexcept {
    exit_with_error(::llvm::toString(::std::move(error)));
}

[[noreturn]]
void terminate_internal_error(::std::string const& message) noexcept {
    ::llvm::errs() << "translator internal error: " << message << '\n';
    ::std::terminate();
}

[[nodiscard]]
auto parse_cli_options(int argc, char const* const* argv) -> ::llvm::Expected<CliOptions> {
    CliOptions options{};
    bool has_output_dir = false;
    for (int i = 1; i < argc; ++i) {
        ::llvm::StringRef arg{argv[i]};
        if (arg == "--output-dir") {
            if (i + 1 >= argc) {
                return ::llvm::createStringError(::std::errc::invalid_argument, "missing value for --output-dir");
            }
            options.output_dir = argv[++i];
            has_output_dir = true;
            continue;
        }
        return ::llvm::createStringError(::std::errc::invalid_argument, "unknown argument: %s", argv[i]);
    }
    if (!has_output_dir) {
        return ::llvm::createStringError(::std::errc::invalid_argument, "missing required argument: --output-dir <directory>");
    }
    return options;
}

[[nodiscard]]
auto read_text_file(::std::filesystem::path const& path) -> ::llvm::Expected<::std::string> {
    ::std::ifstream ifs(path, ::std::ios::binary);
    if (!ifs.is_open()) {
        return ::llvm::createStringError(::std::errc::no_such_file_or_directory, "failed to open file: %s", path.string().c_str());
    }
    ::std::string content{::std::istreambuf_iterator<char>(ifs), ::std::istreambuf_iterator<char>()};
    return content;
}

[[nodiscard]]
auto normalized_include(::std::filesystem::path path) -> ::std::string {
    path.make_preferred();
    auto text = path.string();
    for (auto& ch : text) {
        if (ch == '\\') {
            ch = '/';
        }
    }
    return text;
}

[[nodiscard]]
auto parse_contract_arg(::clang::TemplateArgument const& arg) -> ::std::optional<::pltxt2htm::Contracts> {
    if (arg.getKind() != ::clang::TemplateArgument::Integral) {
        return ::std::nullopt;
    }
    auto value = arg.getAsIntegral().getExtValue();
    if (value == 0) {
        return ::pltxt2htm::Contracts::quick_enforce;
    }
    if (value == 1) {
        return ::pltxt2htm::Contracts::ignore;
    }
    return ::std::nullopt;
}

[[nodiscard]]
auto parse_bool_arg(::clang::TemplateArgument const& arg) -> ::std::optional<bool> {
    if (arg.getKind() != ::clang::TemplateArgument::Integral) {
        return ::std::nullopt;
    }
    auto value = arg.getAsIntegral().getExtValue();
    if (value == 0) {
        return false;
    }
    if (value == 1) {
        return true;
    }
    return ::std::nullopt;
}

[[nodiscard]]
constexpr auto default_optimize_for_api(::llvm::StringRef name) noexcept -> ::std::optional<bool> {
    if (name == "pltxt2advanced_html" || name == "pltxt2fixedadv_html" || name == "pltxt2plunity_introduction") {
        return true;
    }
    if (name == "pltxt2common_html") {
        return false;
    }
    return ::std::nullopt;
}

class ApiInstantiationVisitor : public ::clang::RecursiveASTVisitor<ApiInstantiationVisitor> {
public:
    explicit ApiInstantiationVisitor(ApiInstantiationMap& out) noexcept : out_(out) {}

    auto VisitFunctionDecl(::clang::FunctionDecl* fd) -> bool {
        auto const name_storage = fd->getNameAsString();
        auto const name = ::llvm::StringRef{name_storage};
        if (!is_target(name)) {
            return true;
        }

        auto* tsi = fd->getTemplateSpecializationInfo();
        if (tsi == nullptr || tsi->TemplateArguments == nullptr) {
            return true;
        }
        add_variant(name, tsi->TemplateArguments->asArray());
        return true;
    }

    auto VisitCallExpr(::clang::CallExpr* ce) -> bool {
        auto* callee{ce->getDirectCallee()};
        if (callee == nullptr) {
            return true;
        }
        auto const name_storage = callee->getNameAsString();
        auto const name = ::llvm::StringRef{name_storage};
        if (!is_target(name)) {
            return true;
        }
        if (auto const* targs{callee->getTemplateSpecializationArgs()}; targs != nullptr) {
            add_variant(name, targs->asArray());
            return true;
        }
        auto* tsi = callee->getTemplateSpecializationInfo();
        if (tsi == nullptr || tsi->TemplateArguments == nullptr) {
            return true;
        }
        add_variant(name, tsi->TemplateArguments->asArray());
        return true;
    }

private:
    static constexpr auto is_target(::llvm::StringRef name) noexcept -> bool {
        return name == "pltxt2advanced_html" || name == "pltxt2fixedadv_html" || name == "pltxt2plunity_introduction" ||
               name == "pltxt2common_html";
    }

    void add_variant(::llvm::StringRef name, ::llvm::ArrayRef<::clang::TemplateArgument> targs) {
        if (targs.empty()) {
            return;
        }
        auto contract = parse_contract_arg(targs[0]);
        if (!contract.has_value()) {
            return;
        }

        ::std::optional<bool> optimize;
        if (targs.size() >= 2) {
            optimize = parse_bool_arg(targs[1]);
        } else {
            optimize = default_optimize_for_api(name);
        }
        if (!optimize.has_value()) {
            return;
        }

        out_[name].variants.insert(VariantKey{.contract = *contract, .optimize = *optimize});
    }

    ApiInstantiationMap& out_;
};

void emit_wrapper_signature(::llvm::raw_string_ostream& out, ::llvm::StringRef method, ::llvm::StringRef args) {
    out << "    public static string " << method << "(" << args << ")\n";
}

void emit_variant_body(::llvm::raw_string_ostream& out, ::llvm::StringRef method, ::llvm::StringRef inner_args, bool optimize,
                       ::llvm::StringRef backend_expr) {
    out << "    private static string " << method << "_Impl(" << inner_args << ")\n";
    out << "    {\n";
    out << "        var ast = Pltxt2Internal.ParsePltxt(pltext);\n";
    if (optimize) {
        out << "        Pltxt2Internal.OptimizeAst(ast);\n";
    }
    out << "        return " << backend_expr << ";\n";
    out << "    }\n\n";
}

void emit_dispatch_case(::llvm::raw_string_ostream& out, ::llvm::StringRef method, ::llvm::StringRef passthrough_args) {
    out << "        return " << method << "_Impl(" << passthrough_args << ");\n";
}

[[nodiscard]]
auto get_single_variant(ApiInstantiationMap const& instantiated, ::llvm::StringRef api_name) -> VariantKey {
    auto it = instantiated.find(api_name);
    if (it == instantiated.end()) {
        terminate_internal_error("missing specialization for " + api_name.str());
    }
    if (it->second.variants.size() != 1) {
        terminate_internal_error("expected exactly one instantiated specialization for " + api_name.str());
    }
    return *it->second.variants.begin();
}

[[nodiscard]]
auto generate_csharp(ApiInstantiationMap const& instantiated) -> ::std::string {
    ::std::string generated;
    ::llvm::raw_string_ostream out(generated);
    out << "// <auto-generated />\n";
    out << "// Generated by translang/csharp using clang template instantiation from compiled source.\n";
    out << "// Source: translang/csharp/pltxt2htm.cc\n";
    out << "using System;\n";
    out << "using System.Collections.Generic;\n\n";
    out << "namespace Pltxt2htm.Generated;\n\n";
    out << "public sealed class Ast\n";
    out << "{\n";
    out << "    // fast_io::vector -> List<T>\n";
    out << "    public List<object> Nodes { get; } = new();\n";
    out << "}\n\n";
    out << "public static class Pltxt2xxxApi\n";
    out << "{\n";

    auto const advanced = get_single_variant(instantiated, "pltxt2advanced_html");
    auto const plunity = get_single_variant(instantiated, "pltxt2plunity_introduction");
    auto const common = get_single_variant(instantiated, "pltxt2common_html");

    emit_wrapper_signature(out, "Pltxt2AdvancedHtml", "string pltext");
    out << "    {\n";
    emit_dispatch_case(out, "Pltxt2AdvancedHtml", "pltext");
    out << "    }\n\n";

    emit_wrapper_signature(out, "Pltxt2PlunityIntroduction", "string pltext, string project, string visitor, string author, string coauthors");
    out << "    {\n";
    emit_dispatch_case(out, "Pltxt2PlunityIntroduction", "pltext, project, visitor, author, coauthors");
    out << "    }\n\n";

    emit_wrapper_signature(out, "Pltxt2CommonHtml", "string pltext");
    out << "    {\n";
    emit_dispatch_case(out, "Pltxt2CommonHtml", "pltext");
    out << "    }\n\n";

    emit_variant_body(out, "Pltxt2AdvancedHtml", "string pltext", advanced.optimize,
                      "Pltxt2Internal.PlwebTextBackend(ast, \"localhost:5173\", \"$PROJECT\", \"$VISITOR\", \"$AUTHOR\", \"$CO_AUTHORS\")");

    emit_variant_body(out, "Pltxt2PlunityIntroduction", "string pltext, string project, string visitor, string author, string coauthors",
                      plunity.optimize, "Pltxt2Internal.PlunityTextBackend(ast, project, visitor, author, coauthors)");

    emit_variant_body(out, "Pltxt2CommonHtml", "string pltext", common.optimize, "Pltxt2Internal.PlwebTitleBackend(ast)");

    out << "}\n\n";
    out << "internal static class Pltxt2Internal\n";
    out << "{\n";
    out << "    // exception::terminate / exception::unreachable -> throw\n";
    out << "    internal static void Terminate(string message) => throw new InvalidOperationException(message);\n";
    out << "    internal static T Unreachable<T>(string message) => throw new InvalidOperationException(message);\n\n";
    out << "    internal static Ast ParsePltxt(string pltext) => throw new NotImplementedException(\"Translate parser.hh to fill this.\");\n";
    out << "    internal static void OptimizeAst(Ast ast) => throw new NotImplementedException(\"Translate optimizer.hh to fill this.\");\n";
    out << "    internal static string PlwebTextBackend(Ast ast, string host, string project, string visitor, string author, string coauthors) => throw new NotImplementedException(\"Translate details/backend/for_plweb_text.hh to fill this.\");\n";
    out << "    internal static string PlunityTextBackend(Ast ast, string project, string visitor, string author, string coauthors) => throw new NotImplementedException(\"Translate details/backend/for_plunity_text.hh to fill this.\");\n";
    out << "    internal static string PlwebTitleBackend(Ast ast) => throw new NotImplementedException(\"Translate details/backend/for_plweb_title.hh to fill this.\");\n\n";
    out << "    // HeapGuard in C++ is only heap object lifetime guard; in C# use normal variable.\n";
    out << "    internal static T HeapGuard<T>(T value) => value;\n";
    out << "}\n";
    out.flush();
    return generated;
}

auto validate_required_instantiations(ApiInstantiationMap const& instantiated) -> ::llvm::Error {
    constexpr ::llvm::StringLiteral apis[]{
        "pltxt2advanced_html",
        "pltxt2plunity_introduction",
        "pltxt2common_html",
    };
    for (auto const api : apis) {
        auto it = instantiated.find(api);
        if (it == instantiated.end()) {
            return ::llvm::createStringError(::std::errc::invalid_argument, "missing template instantiations for: %s", api.data());
        }
        if (it->second.variants.size() != 1) {
            return ::llvm::createStringError(::std::errc::invalid_argument, "expected exactly one specialization for: %s", api.data());
        }
    }
    return ::llvm::Error::success();
}

auto collect_instantiations(Paths const& paths) -> ::llvm::Expected<ApiInstantiationMap> {
    auto source_text = read_text_file(paths.source_path);
    if (!source_text) {
        return source_text.takeError();
    }
    ::std::vector<::std::string> args;
    args.emplace_back("-std=c++23");
    args.emplace_back("-fsyntax-only");
    args.emplace_back("-fno-delayed-template-parsing");
    args.emplace_back("-I" + normalized_include(::std::filesystem::absolute(paths.include_dir)));
#if defined(NDEBUG)
    args.emplace_back("-DNDEBUG");
#endif

    auto ast = ::clang::tooling::buildASTFromCodeWithArgs(*source_text, args, normalized_include(::std::filesystem::absolute(paths.source_path)));
    if (!ast) {
        return ::llvm::createStringError(::std::errc::invalid_argument,
                                       "clang failed to compile/template-instantiate translang/csharp/pltxt2htm.cc");
    }

    ApiInstantiationMap instantiated{};
    ApiInstantiationVisitor visitor(instantiated);
    visitor.TraverseDecl(ast->getASTContext().getTranslationUnitDecl());
    if (auto err = validate_required_instantiations(instantiated)) {
        return ::std::move(err);
    }
    return instantiated;
}

auto write_text_file(::std::filesystem::path const& path, ::std::string const& content) -> ::llvm::Error {
    ::std::error_code ec;
    ::std::filesystem::create_directories(path.parent_path(), ec);
    if (ec) {
        return ::llvm::createStringError(::std::errc::io_error, "failed to create output directory: %s (%s)",
                                       path.parent_path().string().c_str(), ec.message().c_str());
    }
    ::std::ofstream ofs(path, ::std::ios::binary | ::std::ios::trunc);
    if (!ofs.is_open()) {
        return ::llvm::createStringError(::std::errc::io_error, "failed to open output file: %s", path.string().c_str());
    }
    ofs.write(content.data(), static_cast<::std::streamsize>(content.size()));
    if (!ofs.good()) {
        return ::llvm::createStringError(::std::errc::io_error, "failed to write output file: %s", path.string().c_str());
    }
    return ::llvm::Error::success();
}

} // namespace

int main(int argc, char const* const* argv) {
    auto cli = parse_cli_options(argc, argv);
    if (!cli) {
        exit_with_error(cli.takeError());
    }

    auto paths = hardcoded_paths(cli->output_dir);

    auto collect_result = collect_instantiations(paths);
    if (!collect_result) {
        exit_with_error(collect_result.takeError());
    }

    auto generated = generate_csharp(*collect_result);
    auto write_result = write_text_file(paths.output_path, generated);
    if (write_result) {
        exit_with_error(::std::move(write_result));
    }

    ::llvm::outs() << "Generated C# translation to " << paths.output_path.string() << '\n';
    return 0;
}
