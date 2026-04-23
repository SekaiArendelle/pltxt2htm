#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <clang/AST/ASTContext.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Tooling/Tooling.h>

namespace {

struct Paths {
    std::filesystem::path header_path{};
    std::filesystem::path output_path{};
    std::filesystem::path include_dir{};
};

struct CliOptions {
    std::filesystem::path output_dir{};
};

enum class ContractValue : int {
    quick_enforce = 0,
    ignore = 1,
};

struct VariantKey {
    ContractValue contract{};
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
        .header_path = "../../include/pltxt2htm/pltxt2htm.hh",
        .output_path = output_dir / "Pltxt2htm.Generated.cs",
        .include_dir = "../../include",
    };
}

[[nodiscard]]
auto parse_cli_options(int argc, char const* const* argv) -> CliOptions {
    CliOptions options{};
    bool has_output_dir = false;
    for (int i = 1; i < argc; ++i) {
        std::string_view arg{argv[i]};
        if (arg == "--output-dir") {
            if (i + 1 >= argc) {
                throw std::runtime_error("missing value for --output-dir");
            }
            options.output_dir = argv[++i];
            has_output_dir = true;
            continue;
        }
        throw std::runtime_error("unknown argument: " + std::string(arg));
    }
    if (!has_output_dir) {
        throw std::runtime_error("missing required argument: --output-dir <directory>");
    }
    return options;
}

[[nodiscard]]
auto read_text_file(std::filesystem::path const& path) -> std::string {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs.is_open()) {
        throw std::runtime_error("failed to open file: " + path.string());
    }
    return {std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
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
auto make_instantiation_tu(std::filesystem::path const& header_path) -> std::string {
    auto header = normalized_include(std::filesystem::absolute(header_path));
    std::ostringstream oss;
    oss << "#include \"" << header << "\"\n";
    oss << "using c = ::pltxt2htm::Contracts;\n";
    oss << "auto* p0 = &::pltxt2htm::pltxt2advanced_html<c::quick_enforce, true>;\n";
    oss << "auto* p1 = &::pltxt2htm::pltxt2advanced_html<c::quick_enforce, false>;\n";
    oss << "auto* p2 = &::pltxt2htm::pltxt2advanced_html<c::ignore, true>;\n";
    oss << "auto* p3 = &::pltxt2htm::pltxt2advanced_html<c::ignore, false>;\n";
    oss << "auto* p4 = &::pltxt2htm::pltxt2fixedadv_html<c::quick_enforce, true>;\n";
    oss << "auto* p5 = &::pltxt2htm::pltxt2fixedadv_html<c::quick_enforce, false>;\n";
    oss << "auto* p6 = &::pltxt2htm::pltxt2fixedadv_html<c::ignore, true>;\n";
    oss << "auto* p7 = &::pltxt2htm::pltxt2fixedadv_html<c::ignore, false>;\n";
    oss << "auto* p8 = &::pltxt2htm::pltxt2plunity_introduction<c::quick_enforce, true>;\n";
    oss << "auto* p9 = &::pltxt2htm::pltxt2plunity_introduction<c::quick_enforce, false>;\n";
    oss << "auto* p10 = &::pltxt2htm::pltxt2plunity_introduction<c::ignore, true>;\n";
    oss << "auto* p11 = &::pltxt2htm::pltxt2plunity_introduction<c::ignore, false>;\n";
    oss << "auto* p12 = &::pltxt2htm::pltxt2common_html<c::quick_enforce, true>;\n";
    oss << "auto* p13 = &::pltxt2htm::pltxt2common_html<c::quick_enforce, false>;\n";
    oss << "auto* p14 = &::pltxt2htm::pltxt2common_html<c::ignore, true>;\n";
    oss << "auto* p15 = &::pltxt2htm::pltxt2common_html<c::ignore, false>;\n";
    return oss.str();
}

[[nodiscard]]
auto parse_contract_arg(clang::TemplateArgument const& arg) -> std::optional<ContractValue> {
    if (arg.getKind() != clang::TemplateArgument::Integral) {
        return std::nullopt;
    }
    auto value = arg.getAsIntegral().getExtValue();
    if (value == 0) {
        return ContractValue::quick_enforce;
    }
    if (value == 1) {
        return ContractValue::ignore;
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

class ApiInstantiationVisitor : public clang::RecursiveASTVisitor<ApiInstantiationVisitor> {
public:
    explicit ApiInstantiationVisitor(std::map<std::string, ApiInstantiationSet>& out) noexcept : out_(out) {}

    auto VisitFunctionDecl(clang::FunctionDecl* fd) -> bool {
        if (!fd->hasBody()) {
            return true;
        }
        if (!fd->isTemplateInstantiation()) {
            return true;
        }

        auto const name = fd->getNameAsString();
        if (!is_target(name)) {
            return true;
        }

        auto* tsi = fd->getTemplateSpecializationInfo();
        if (tsi == nullptr || tsi->TemplateArguments == nullptr) {
            return true;
        }
        auto const targs = tsi->TemplateArguments->asArray();
        if (targs.size() < 2) {
            return true;
        }

        auto contract = parse_contract_arg(targs[0]);
        auto optimize = parse_bool_arg(targs[1]);
        if (!contract.has_value() || !optimize.has_value()) {
            return true;
        }

        out_[name].variants.insert(VariantKey{.contract = *contract, .optimize = *optimize});
        return true;
    }

private:
    static auto is_target(std::string const& name) noexcept -> bool {
        return name == "pltxt2advanced_html" || name == "pltxt2fixedadv_html" || name == "pltxt2plunity_introduction" ||
               name == "pltxt2common_html";
    }

    std::map<std::string, ApiInstantiationSet>& out_;
};

[[nodiscard]]
auto contract_cs_name(ContractValue contract) -> std::string {
    if (contract == ContractValue::quick_enforce) {
        return "Contracts.QuickEnforce";
    }
    return "Contracts.Ignore";
}

[[nodiscard]]
auto variant_suffix(ContractValue contract, bool optimize) -> std::string {
    std::string suffix;
    suffix += (contract == ContractValue::quick_enforce) ? "QuickEnforce" : "Ignore";
    suffix += optimize ? "OptimizeTrue" : "OptimizeFalse";
    return suffix;
}

void emit_wrapper_signature(std::ostringstream& out, std::string const& method, std::string const& args, bool default_optimize) {
    out << "    public static string " << method << "(" << args
        << ", Contracts ndebug = Contracts.QuickEnforce, bool optimize = " << (default_optimize ? "true" : "false")
        << ")\n";
}

void emit_variant_body(std::ostringstream& out, std::string const& method, std::string const& inner_args, std::string const& call_args,
                       ContractValue contract, bool optimize, std::string const& backend_expr) {
    out << "    private static string " << method << "_" << variant_suffix(contract, optimize) << "(" << inner_args << ")\n";
    out << "    {\n";
    out << "        var ast = Pltxt2Internal.ParsePltxt(pltext, " << contract_cs_name(contract) << ");\n";
    if (optimize) {
        out << "        Pltxt2Internal.OptimizeAst(ast, " << contract_cs_name(contract) << ");\n";
    }
    out << "        return " << backend_expr << ";\n";
    out << "    }\n\n";
}

void emit_dispatch_case(std::ostringstream& out, std::string const& method, std::string const& passthrough_args, ContractValue contract) {
    out << "            case " << contract_cs_name(contract) << ":\n";
    out << "                return optimize ? " << method << "_" << variant_suffix(contract, true) << "(" << passthrough_args << ") : "
        << method << "_" << variant_suffix(contract, false) << "(" << passthrough_args << ");\n";
}

[[nodiscard]]
auto generate_csharp() -> std::string {
    std::ostringstream out;
    out << "// <auto-generated />\n";
    out << "// Generated by translang/csharp using clang preprocessor + template instantiation.\n";
    out << "// Source: include/pltxt2htm/pltxt2htm.hh (pltxt2xxx APIs)\n";
    out << "using System;\n";
    out << "using System.Collections.Generic;\n\n";
    out << "namespace Pltxt2htm.Generated;\n\n";
    out << "public enum Contracts\n";
    out << "{\n";
    out << "    QuickEnforce = 0,\n";
    out << "    Ignore = 1\n";
    out << "}\n\n";
    out << "public sealed class Ast\n";
    out << "{\n";
    out << "    // fast_io::vector -> List<T>\n";
    out << "    public List<object> Nodes { get; } = new();\n";
    out << "}\n\n";
    out << "public static class Pltxt2xxxApi\n";
    out << "{\n";

    emit_wrapper_signature(out, "Pltxt2AdvancedHtml", "string pltext", true);
    out << "    {\n";
    out << "        switch (ndebug)\n";
    out << "        {\n";
    emit_dispatch_case(out, "Pltxt2AdvancedHtml", "pltext", ContractValue::quick_enforce);
    emit_dispatch_case(out, "Pltxt2AdvancedHtml", "pltext", ContractValue::ignore);
    out << "            default:\n";
    out << "                throw new ArgumentOutOfRangeException(nameof(ndebug), ndebug, \"Unsupported contract value\");\n";
    out << "        }\n";
    out << "    }\n\n";

    emit_wrapper_signature(out, "Pltxt2FixedadvHtml", "string pltext, string host, string project, string visitor, string author, string coauthors",
                           true);
    out << "    {\n";
    out << "        switch (ndebug)\n";
    out << "        {\n";
    emit_dispatch_case(out, "Pltxt2FixedadvHtml", "pltext, host, project, visitor, author, coauthors", ContractValue::quick_enforce);
    emit_dispatch_case(out, "Pltxt2FixedadvHtml", "pltext, host, project, visitor, author, coauthors", ContractValue::ignore);
    out << "            default:\n";
    out << "                throw new ArgumentOutOfRangeException(nameof(ndebug), ndebug, \"Unsupported contract value\");\n";
    out << "        }\n";
    out << "    }\n\n";

    emit_wrapper_signature(out, "Pltxt2PlunityIntroduction", "string pltext, string project, string visitor, string author, string coauthors",
                           true);
    out << "    {\n";
    out << "        switch (ndebug)\n";
    out << "        {\n";
    emit_dispatch_case(out, "Pltxt2PlunityIntroduction", "pltext, project, visitor, author, coauthors", ContractValue::quick_enforce);
    emit_dispatch_case(out, "Pltxt2PlunityIntroduction", "pltext, project, visitor, author, coauthors", ContractValue::ignore);
    out << "            default:\n";
    out << "                throw new ArgumentOutOfRangeException(nameof(ndebug), ndebug, \"Unsupported contract value\");\n";
    out << "        }\n";
    out << "    }\n\n";

    emit_wrapper_signature(out, "Pltxt2CommonHtml", "string pltext", false);
    out << "    {\n";
    out << "        switch (ndebug)\n";
    out << "        {\n";
    emit_dispatch_case(out, "Pltxt2CommonHtml", "pltext", ContractValue::quick_enforce);
    emit_dispatch_case(out, "Pltxt2CommonHtml", "pltext", ContractValue::ignore);
    out << "            default:\n";
    out << "                throw new ArgumentOutOfRangeException(nameof(ndebug), ndebug, \"Unsupported contract value\");\n";
    out << "        }\n";
    out << "    }\n\n";

    emit_variant_body(out, "Pltxt2AdvancedHtml", "string pltext", "pltext", ContractValue::quick_enforce, true,
                      "Pltxt2Internal.PlwebTextBackend(ast, \"localhost:5173\", \"$PROJECT\", \"$VISITOR\", \"$AUTHOR\", \"$CO_AUTHORS\", Contracts.QuickEnforce)");
    emit_variant_body(out, "Pltxt2AdvancedHtml", "string pltext", "pltext", ContractValue::quick_enforce, false,
                      "Pltxt2Internal.PlwebTextBackend(ast, \"localhost:5173\", \"$PROJECT\", \"$VISITOR\", \"$AUTHOR\", \"$CO_AUTHORS\", Contracts.QuickEnforce)");
    emit_variant_body(out, "Pltxt2AdvancedHtml", "string pltext", "pltext", ContractValue::ignore, true,
                      "Pltxt2Internal.PlwebTextBackend(ast, \"localhost:5173\", \"$PROJECT\", \"$VISITOR\", \"$AUTHOR\", \"$CO_AUTHORS\", Contracts.Ignore)");
    emit_variant_body(out, "Pltxt2AdvancedHtml", "string pltext", "pltext", ContractValue::ignore, false,
                      "Pltxt2Internal.PlwebTextBackend(ast, \"localhost:5173\", \"$PROJECT\", \"$VISITOR\", \"$AUTHOR\", \"$CO_AUTHORS\", Contracts.Ignore)");

    emit_variant_body(out, "Pltxt2FixedadvHtml", "string pltext, string host, string project, string visitor, string author, string coauthors",
                      "pltext, host, project, visitor, author, coauthors", ContractValue::quick_enforce, true,
                      "Pltxt2Internal.PlwebTextBackend(ast, host, project, visitor, author, coauthors, Contracts.QuickEnforce)");
    emit_variant_body(out, "Pltxt2FixedadvHtml", "string pltext, string host, string project, string visitor, string author, string coauthors",
                      "pltext, host, project, visitor, author, coauthors", ContractValue::quick_enforce, false,
                      "Pltxt2Internal.PlwebTextBackend(ast, host, project, visitor, author, coauthors, Contracts.QuickEnforce)");
    emit_variant_body(out, "Pltxt2FixedadvHtml", "string pltext, string host, string project, string visitor, string author, string coauthors",
                      "pltext, host, project, visitor, author, coauthors", ContractValue::ignore, true,
                      "Pltxt2Internal.PlwebTextBackend(ast, host, project, visitor, author, coauthors, Contracts.Ignore)");
    emit_variant_body(out, "Pltxt2FixedadvHtml", "string pltext, string host, string project, string visitor, string author, string coauthors",
                      "pltext, host, project, visitor, author, coauthors", ContractValue::ignore, false,
                      "Pltxt2Internal.PlwebTextBackend(ast, host, project, visitor, author, coauthors, Contracts.Ignore)");

    emit_variant_body(out, "Pltxt2PlunityIntroduction", "string pltext, string project, string visitor, string author, string coauthors",
                      "pltext, project, visitor, author, coauthors", ContractValue::quick_enforce, true,
                      "Pltxt2Internal.PlunityTextBackend(ast, project, visitor, author, coauthors, Contracts.QuickEnforce)");
    emit_variant_body(out, "Pltxt2PlunityIntroduction", "string pltext, string project, string visitor, string author, string coauthors",
                      "pltext, project, visitor, author, coauthors", ContractValue::quick_enforce, false,
                      "Pltxt2Internal.PlunityTextBackend(ast, project, visitor, author, coauthors, Contracts.QuickEnforce)");
    emit_variant_body(out, "Pltxt2PlunityIntroduction", "string pltext, string project, string visitor, string author, string coauthors",
                      "pltext, project, visitor, author, coauthors", ContractValue::ignore, true,
                      "Pltxt2Internal.PlunityTextBackend(ast, project, visitor, author, coauthors, Contracts.Ignore)");
    emit_variant_body(out, "Pltxt2PlunityIntroduction", "string pltext, string project, string visitor, string author, string coauthors",
                      "pltext, project, visitor, author, coauthors", ContractValue::ignore, false,
                      "Pltxt2Internal.PlunityTextBackend(ast, project, visitor, author, coauthors, Contracts.Ignore)");

    emit_variant_body(out, "Pltxt2CommonHtml", "string pltext", "pltext", ContractValue::quick_enforce, true,
                      "Pltxt2Internal.PlwebTitleBackend(ast, Contracts.QuickEnforce)");
    emit_variant_body(out, "Pltxt2CommonHtml", "string pltext", "pltext", ContractValue::quick_enforce, false,
                      "Pltxt2Internal.PlwebTitleBackend(ast, Contracts.QuickEnforce)");
    emit_variant_body(out, "Pltxt2CommonHtml", "string pltext", "pltext", ContractValue::ignore, true,
                      "Pltxt2Internal.PlwebTitleBackend(ast, Contracts.Ignore)");
    emit_variant_body(out, "Pltxt2CommonHtml", "string pltext", "pltext", ContractValue::ignore, false,
                      "Pltxt2Internal.PlwebTitleBackend(ast, Contracts.Ignore)");

    out << "}\n\n";
    out << "internal static class Pltxt2Internal\n";
    out << "{\n";
    out << "    // exception::terminate / exception::unreachable -> throw\n";
    out << "    internal static void Terminate(string message) => throw new InvalidOperationException(message);\n";
    out << "    internal static T Unreachable<T>(string message) => throw new InvalidOperationException(message);\n\n";
    out << "    internal static Ast ParsePltxt(string pltext, Contracts ndebug) => throw new NotImplementedException(\"Translate parser.hh to fill this.\");\n";
    out << "    internal static void OptimizeAst(Ast ast, Contracts ndebug) => throw new NotImplementedException(\"Translate optimizer.hh to fill this.\");\n";
    out << "    internal static string PlwebTextBackend(Ast ast, string host, string project, string visitor, string author, string coauthors, Contracts ndebug) => throw new NotImplementedException(\"Translate details/backend/for_plweb_text.hh to fill this.\");\n";
    out << "    internal static string PlunityTextBackend(Ast ast, string project, string visitor, string author, string coauthors, Contracts ndebug) => throw new NotImplementedException(\"Translate details/backend/for_plunity_text.hh to fill this.\");\n";
    out << "    internal static string PlwebTitleBackend(Ast ast, Contracts ndebug) => throw new NotImplementedException(\"Translate details/backend/for_plweb_title.hh to fill this.\");\n\n";
    out << "    // HeapGuard in C++ is only heap object lifetime guard; in C# use normal variable.\n";
    out << "    internal static T HeapGuard<T>(T value) => value;\n";
    out << "}\n";
    return out.str();
}

void validate_required_instantiations(std::map<std::string, ApiInstantiationSet> const& instantiated) {
    constexpr std::string_view apis[]{
        "pltxt2advanced_html",
        "pltxt2fixedadv_html",
        "pltxt2plunity_introduction",
        "pltxt2common_html",
    };
    for (auto const api : apis) {
        auto it = instantiated.find(std::string(api));
        if (it == instantiated.end()) {
            throw std::runtime_error("missing template instantiations for: " + std::string(api));
        }
        for (auto contract : {ContractValue::quick_enforce, ContractValue::ignore}) {
            for (auto optimize : {false, true}) {
                if (!it->second.variants.contains(VariantKey{.contract = contract, .optimize = optimize})) {
                    throw std::runtime_error("missing specialization for " + std::string(api));
                }
            }
        }
    }
}

void collect_instantiations(Paths const& paths) {
    auto tu = make_instantiation_tu(paths.header_path);

    std::vector<std::string> args;
    args.emplace_back("-std=c++23");
    args.emplace_back("-fsyntax-only");
    args.emplace_back("-I" + normalized_include(std::filesystem::absolute(paths.include_dir)));

    auto ast = clang::tooling::buildASTFromCodeWithArgs(tu, args, "pltxt2htm_instantiations.cc");
    if (!ast) {
        throw std::runtime_error("clang failed to preprocess/template-instantiate the header");
    }

    // buildASTFromCodeWithArgs performs preprocessing and template instantiation.
}

void write_text_file(std::filesystem::path const& path, std::string const& content) {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream ofs(path, std::ios::binary | std::ios::trunc);
    if (!ofs.is_open()) {
        throw std::runtime_error("failed to open output file: " + path.string());
    }
    ofs.write(content.data(), static_cast<std::streamsize>(content.size()));
}

} // namespace

int main(int argc, char const* const* argv) {
    try {
        auto const cli = parse_cli_options(argc, argv);
        auto paths = hardcoded_paths(cli.output_dir);

        auto header_text = read_text_file(paths.header_path);
        if (header_text.find("pltxt2advanced_html") == std::string::npos) {
            throw std::runtime_error("invalid header: pltxt2xxx APIs were not found");
        }

        collect_instantiations(paths);
        auto generated = generate_csharp();
        write_text_file(paths.output_path, generated);

        std::cout << "Generated C# translation to " << paths.output_path.string() << '\n';
        return 0;
    } catch (std::exception const& ex) {
        std::cerr << "translator error: " << ex.what() << '\n';
        return 1;
    }
}
