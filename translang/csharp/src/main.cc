#include <cstdlib>
#include <exception>
#include <expected>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <clang/AST/ASTContext.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Tooling/Tooling.h>

#include <pltxt2htm/contracts.hh>

namespace {

struct Paths {
    std::filesystem::path source_path{};
    std::filesystem::path output_path{};
    std::filesystem::path include_dir{};
};

struct CliOptions {
    std::filesystem::path output_dir{};
};

struct VariantKey {
    ::pltxt2htm::Contracts contract{};
    bool optimize{};

    auto operator<(VariantKey const& other) const noexcept -> bool {
        if (contract != other.contract) {
            return static_cast<int>(contract) < static_cast<int>(other.contract);
        }
        return optimize < other.optimize;
    }
};

struct ApiInstantiationSet {
    std::set<VariantKey> variants{};
};

[[nodiscard]]
auto hardcoded_paths(std::filesystem::path const& output_dir) -> Paths {
    // Paths are intentionally hardcoded from translang/csharp working directory.
    return Paths{
        .source_path = "./pltxt2htm.cc",
        .output_path = output_dir / "Pltxt2htm.Generated.cs",
        .include_dir = "../../include",
    };
}

[[noreturn]]
void exit_with_error(std::string const& message) {
    std::cerr << "translator error: " << message << '\n';
    std::exit(1);
}

[[noreturn]]
void terminate_internal_error(std::string const& message) {
    std::cerr << "translator internal error: " << message << '\n';
    std::terminate();
}

[[nodiscard]]
auto parse_cli_options(int argc, char const* const* argv) -> std::expected<CliOptions, std::string> {
    CliOptions options{};
    bool has_output_dir = false;
    for (int i = 1; i < argc; ++i) {
        std::string_view arg{argv[i]};
        if (arg == "--output-dir") {
            if (i + 1 >= argc) {
                return std::unexpected("missing value for --output-dir");
            }
            options.output_dir = argv[++i];
            has_output_dir = true;
            continue;
        }
        return std::unexpected("unknown argument: " + std::string(arg));
    }
    if (!has_output_dir) {
        return std::unexpected("missing required argument: --output-dir <directory>");
    }
    return options;
}

[[nodiscard]]
auto read_text_file(std::filesystem::path const& path) -> std::expected<std::string, std::string> {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs.is_open()) {
        return std::unexpected("failed to open file: " + path.string());
    }
    std::string content{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
    return content;
}

[[nodiscard]]
auto normalized_include(std::filesystem::path path) -> std::string {
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
auto parse_contract_arg(clang::TemplateArgument const& arg) -> std::optional<::pltxt2htm::Contracts> {
    if (arg.getKind() != clang::TemplateArgument::Integral) {
        return std::nullopt;
    }
    auto value = arg.getAsIntegral().getExtValue();
    if (value == 0) {
        return ::pltxt2htm::Contracts::quick_enforce;
    }
    if (value == 1) {
        return ::pltxt2htm::Contracts::ignore;
    }
    return std::nullopt;
}

[[nodiscard]]
auto parse_bool_arg(clang::TemplateArgument const& arg) -> std::optional<bool> {
    if (arg.getKind() != clang::TemplateArgument::Integral) {
        return std::nullopt;
    }
    auto value = arg.getAsIntegral().getExtValue();
    if (value == 0) {
        return false;
    }
    if (value == 1) {
        return true;
    }
    return std::nullopt;
}

[[nodiscard]]
auto default_optimize_for_api(std::string const& name) -> std::optional<bool> {
    if (name == "pltxt2advanced_html" || name == "pltxt2fixedadv_html" || name == "pltxt2plunity_introduction") {
        return true;
    }
    if (name == "pltxt2common_html") {
        return false;
    }
    return std::nullopt;
}

class ApiInstantiationVisitor : public clang::RecursiveASTVisitor<ApiInstantiationVisitor> {
public:
    explicit ApiInstantiationVisitor(std::map<std::string, ApiInstantiationSet>& out) noexcept : out_(out) {}

    auto VisitFunctionDecl(clang::FunctionDecl* fd) -> bool {
        auto const name = fd->getNameAsString();
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

    auto VisitCallExpr(clang::CallExpr* ce) -> bool {
        auto* callee = ce->getDirectCallee();
        if (callee == nullptr) {
            return true;
        }
        auto const name = callee->getNameAsString();
        if (!is_target(name)) {
            return true;
        }
        if (auto const* targs = callee->getTemplateSpecializationArgs(); targs != nullptr) {
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
    static auto is_target(std::string const& name) noexcept -> bool {
        return name == "pltxt2advanced_html" || name == "pltxt2fixedadv_html" || name == "pltxt2plunity_introduction" ||
               name == "pltxt2common_html";
    }

    void add_variant(std::string const& name, llvm::ArrayRef<clang::TemplateArgument> targs) {
        if (targs.empty()) {
            return;
        }
        auto contract = parse_contract_arg(targs[0]);
        if (!contract.has_value()) {
            return;
        }

        std::optional<bool> optimize;
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

    std::map<std::string, ApiInstantiationSet>& out_;
};

void emit_wrapper_signature(std::ostringstream& out, std::string const& method, std::string const& args) {
    out << "    public static string " << method << "(" << args << ")\n";
}

void emit_variant_body(std::ostringstream& out, std::string const& method, std::string const& inner_args, bool optimize,
                       std::string const& backend_expr) {
    out << "    private static string " << method << "_Impl(" << inner_args << ")\n";
    out << "    {\n";
    out << "        var ast = Pltxt2Internal.ParsePltxt(pltext);\n";
    if (optimize) {
        out << "        Pltxt2Internal.OptimizeAst(ast);\n";
    }
    out << "        return " << backend_expr << ";\n";
    out << "    }\n\n";
}

void emit_dispatch_case(std::ostringstream& out, std::string const& method, std::string const& passthrough_args) {
    out << "        return " << method << "_Impl(" << passthrough_args << ");\n";
}

[[nodiscard]]
auto get_single_variant(std::map<std::string, ApiInstantiationSet> const& instantiated, std::string const& api_name) -> VariantKey {
    auto it = instantiated.find(api_name);
    if (it == instantiated.end()) {
        terminate_internal_error("missing specialization for " + api_name);
    }
    if (it->second.variants.size() != 1) {
        terminate_internal_error("expected exactly one instantiated specialization for " + api_name);
    }
    return *it->second.variants.begin();
}

[[nodiscard]]
auto generate_csharp(std::map<std::string, ApiInstantiationSet> const& instantiated) -> std::string {
    std::ostringstream out;
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
    return out.str();
}

void validate_required_instantiations(std::map<std::string, ApiInstantiationSet> const& instantiated) {
    constexpr std::string_view apis[]{
        "pltxt2advanced_html",
        "pltxt2plunity_introduction",
        "pltxt2common_html",
    };
    for (auto const api : apis) {
        auto it = instantiated.find(std::string(api));
        if (it == instantiated.end()) {
            terminate_internal_error("missing template instantiations for: " + std::string(api));
        }
        if (it->second.variants.size() != 1) {
            terminate_internal_error("expected exactly one specialization for " + std::string(api));
        }
    }
}

auto collect_instantiations(Paths const& paths) -> std::expected<std::map<std::string, ApiInstantiationSet>, std::string> {
    auto source_text = read_text_file(paths.source_path);
    if (!source_text.has_value()) {
        return std::unexpected(source_text.error());
    }
    std::vector<std::string> args;
    args.emplace_back("-std=c++23");
    args.emplace_back("-fsyntax-only");
    args.emplace_back("-fno-delayed-template-parsing");
    args.emplace_back("-I" + normalized_include(std::filesystem::absolute(paths.include_dir)));
#if defined(NDEBUG)
    args.emplace_back("-DNDEBUG");
#endif

    auto ast = clang::tooling::buildASTFromCodeWithArgs(*source_text, args, normalized_include(std::filesystem::absolute(paths.source_path)));
    if (!ast) {
        return std::unexpected("clang failed to compile/template-instantiate translang/csharp/pltxt2htm.cc");
    }

    std::map<std::string, ApiInstantiationSet> instantiated{};
    ApiInstantiationVisitor visitor(instantiated);
    visitor.TraverseDecl(ast->getASTContext().getTranslationUnitDecl());
    validate_required_instantiations(instantiated);
    return instantiated;
}

auto write_text_file(std::filesystem::path const& path, std::string const& content) -> std::expected<void, std::string> {
    std::error_code ec;
    std::filesystem::create_directories(path.parent_path(), ec);
    if (ec) {
        return std::unexpected("failed to create output directory: " + path.parent_path().string() + " (" + ec.message() + ")");
    }
    std::ofstream ofs(path, std::ios::binary | std::ios::trunc);
    if (!ofs.is_open()) {
        return std::unexpected("failed to open output file: " + path.string());
    }
    ofs.write(content.data(), static_cast<std::streamsize>(content.size()));
    if (!ofs.good()) {
        return std::unexpected("failed to write output file: " + path.string());
    }
    return {};
}

} // namespace

int main(int argc, char const* const* argv) {
    auto cli = parse_cli_options(argc, argv);
    if (!cli.has_value()) {
        exit_with_error(cli.error());
    }

    auto paths = hardcoded_paths(cli->output_dir);

    auto collect_result = collect_instantiations(paths);
    if (!collect_result.has_value()) {
        exit_with_error(collect_result.error());
    }

    auto generated = generate_csharp(*collect_result);
    auto write_result = write_text_file(paths.output_path, generated);
    if (!write_result.has_value()) {
        exit_with_error(write_result.error());
    }

    std::cout << "Generated C# translation to " << paths.output_path.string() << '\n';
    return 0;
}
