#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <exception>
#include <optional>
#include <set>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include <clang/AST/ASTContext.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/raw_ostream.h>

#include <pltxt2htm/contracts.hh>

struct Paths {
    ::llvm::SmallString<260> source_path{};
    ::llvm::SmallString<260> output_path{};
    ::llvm::SmallString<260> include_dir{};
};

struct CliOptions {
    ::llvm::SmallString<260> output_dir{};
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

struct AstNodeField {
    ::std::string cpp_name{};
    ::std::string csharp_type{};
};

enum class AstNodeBaseKind {
    plain,
    pltxt_node,
    paired_tag,
};

struct AstNodeClassSpec {
    ::std::string name{};
    AstNodeBaseKind base_kind{AstNodeBaseKind::plain};
    ::std::optional<::std::string> node_type{};
    ::std::vector<AstNodeField> fields{};
    bool has_default_ctor{};
    ::std::string source_file{};
    unsigned source_line{};
};

struct AstNodeModel {
    ::std::vector<::std::string> node_types{};
    ::std::vector<AstNodeClassSpec> classes{};
};

struct ParsePltxtModel {
    struct CsharpSwitchCase {
        ::std::string node_type{};
    };
    ::std::vector<CsharpSwitchCase> nested_tag_switch_cases{};
    ::std::string stack_var_name_csharp{"callStack"};
    ::std::string stack_var_csharp_type{"Stack<BasicFrameContext>"};
};

struct TranslationModel {
    ApiInstantiationMap api_instantiations{};
    AstNodeModel astnodes{};
    ParsePltxtModel parse_pltxt{};
};

[[nodiscard]]
constexpr auto hardcoded_paths(::llvm::StringRef output_dir) -> Paths {
    // Paths are intentionally hardcoded from translang/csharp working directory.
    ::llvm::SmallString<260> source_path{"./pltxt2htm.cc"};
    ::llvm::SmallString<260> include_dir{"../../include"};
    ::llvm::SmallString<260> output_path{output_dir};
    ::llvm::sys::path::append(output_path, "Pltxt2htm.Generated.cs");
    return Paths{
        .source_path = source_path,
        .output_path = output_path,
        .include_dir = include_dir,
    };
}

[[noreturn]]
constexpr void exit_with_error(::std::string const& message) noexcept {
    ::llvm::errs() << "translator error: " << message << '\n';
    ::std::exit(1);
}

[[noreturn]]
constexpr void exit_with_error(::llvm::Error error) noexcept {
    exit_with_error(::llvm::toString(::std::move(error)));
}

[[noreturn]]
constexpr void terminate_internal_error(::std::string const& message) noexcept {
    ::llvm::errs() << "translator internal error: " << message << '\n';
    ::std::terminate();
}

[[nodiscard]]
constexpr auto parse_cli_options(int argc, char const* const* argv) -> ::llvm::Expected<CliOptions> {
    CliOptions options{};
    bool has_output_dir = false;
    for (int i = 1; i < argc; ++i) {
        ::llvm::StringRef arg{argv[i]};
        if (arg == "--output-dir") {
            if (i + 1 >= argc) {
                return ::llvm::createStringError(::std::errc::invalid_argument, "missing value for --output-dir");
            }
            options.output_dir = ::llvm::SmallString<260>{argv[++i]};
            has_output_dir = true;
            continue;
        }
        return ::llvm::createStringError(::std::errc::invalid_argument, "unknown argument: %s", argv[i]);
    }
    if (!has_output_dir) {
        return ::llvm::createStringError(::std::errc::invalid_argument,
                                         "missing required argument: --output-dir <directory>");
    }
    return options;
}

[[nodiscard]]
constexpr auto read_text_file(::llvm::StringRef path) -> ::llvm::Expected<::std::string> {
    auto buffer_or_error = ::llvm::MemoryBuffer::getFile(path, /*IsText=*/false);
    if (!buffer_or_error) {
        auto const ec = buffer_or_error.getError();
        return ::llvm::createStringError(ec, "failed to open file: %s (%s)", path.data(), ec.message().c_str());
    }
    auto& buffer = *buffer_or_error.get();
    ::std::string content{buffer.getBufferStart(), buffer.getBufferEnd()};
    return content;
}

[[nodiscard]]
constexpr auto normalized_include(::llvm::StringRef path) -> ::std::string {
    auto text = path.str();
    for (auto& ch : text) {
        if (ch == '\\') {
            ch = '/';
        }
    }
    return text;
}

[[nodiscard]]
constexpr auto parse_contract_arg(::clang::TemplateArgument const& arg) noexcept
    -> ::std::optional<::pltxt2htm::Contracts> {
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
constexpr auto parse_bool_arg(::clang::TemplateArgument const& arg) noexcept -> ::std::optional<bool> {
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
    constexpr explicit ApiInstantiationVisitor(ApiInstantiationMap& out) noexcept
        : out_(out) {
    }

    constexpr auto VisitFunctionDecl(::clang::FunctionDecl* fd) -> bool {
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

    constexpr auto VisitCallExpr(::clang::CallExpr* ce) -> bool {
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

    constexpr void add_variant(::llvm::StringRef name, ::llvm::ArrayRef<::clang::TemplateArgument> targs) {
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
        }
        else {
            optimize = default_optimize_for_api(name);
        }
        if (!optimize.has_value()) {
            return;
        }

        out_[name].variants.insert(VariantKey{.contract = *contract, .optimize = *optimize});
    }

    ApiInstantiationMap& out_;
};

[[nodiscard]]
constexpr auto is_astnode_header_path(::llvm::StringRef path) -> bool {
    auto normalized = normalized_include(path);
    return normalized.find("/pltxt2htm/astnode/") != ::std::string::npos;
}

[[nodiscard]]
constexpr auto to_pascal_case(::llvm::StringRef text) -> ::std::string {
    ::std::string out;
    out.reserve(text.size());
    bool capitalize = true;
    for (char ch : text) {
        if (ch == '_') {
            capitalize = true;
            continue;
        }
        if (capitalize) {
            out.push_back(static_cast<char>(::std::toupper(static_cast<unsigned char>(ch))));
            capitalize = false;
        }
        else {
            out.push_back(ch);
        }
    }
    while (!out.empty() && out.back() == '_') {
        out.pop_back();
    }
    if (out.empty()) {
        return "Value";
    }
    return out;
}

[[nodiscard]]
constexpr auto to_camel_case(::llvm::StringRef text) -> ::std::string {
    auto out = to_pascal_case(text);
    if (!out.empty()) {
        out[0] = static_cast<char>(::std::tolower(static_cast<unsigned char>(out[0])));
    }
    return out;
}

[[nodiscard]]
constexpr auto csharp_type_for_field(::clang::QualType type) -> ::std::string {
    auto const type_text = type.getAsString();
    if (type_text.find("exception::optional") != ::std::string::npos &&
        type_text.find("u8string") != ::std::string::npos) {
        return "string?";
    }
    if (type_text.find("fast_io::u8string") != ::std::string::npos ||
        type_text.find("basic_string<char8_t") != ::std::string::npos) {
        return "string";
    }
    if (type_text.find("pltxt2htm::Ast") != ::std::string::npos) {
        return "Ast";
    }
    if (type_text.find("pltxt2htm::Url") != ::std::string::npos) {
        return "Url";
    }
    if (type_text.find("char8_t") != ::std::string::npos) {
        return "byte";
    }
    if (type_text.find("size_t") != ::std::string::npos) {
        return "ulong";
    }
    if (type->isBooleanType()) {
        return "bool";
    }
    if (type->isIntegerType()) {
        return "long";
    }
    return "object";
}

[[nodiscard]]
constexpr auto extract_enum_constant_name(::clang::Expr const* expr) -> ::std::optional<::std::string> {
    if (expr == nullptr) {
        return ::std::nullopt;
    }
    expr = expr->IgnoreParenImpCasts();
    if (auto const* dre = ::llvm::dyn_cast<::clang::DeclRefExpr>(expr)) {
        if (auto const* ecd = ::llvm::dyn_cast<::clang::EnumConstantDecl>(dre->getDecl())) {
            return ecd->getNameAsString();
        }
    }
    if (auto const* member = ::llvm::dyn_cast<::clang::MemberExpr>(expr)) {
        if (auto const* ecd = ::llvm::dyn_cast<::clang::EnumConstantDecl>(member->getMemberDecl())) {
            return ecd->getNameAsString();
        }
    }
    if (auto const* ctor = ::llvm::dyn_cast<::clang::CXXConstructExpr>(expr)) {
        if (ctor->getNumArgs() > 0) {
            return extract_enum_constant_name(ctor->getArg(0));
        }
    }
    if (auto const* cast = ::llvm::dyn_cast<::clang::CastExpr>(expr)) {
        return extract_enum_constant_name(cast->getSubExpr());
    }
    if (auto const* bind = ::llvm::dyn_cast<::clang::CXXBindTemporaryExpr>(expr)) {
        return extract_enum_constant_name(bind->getSubExpr());
    }
    if (auto const* materialized = ::llvm::dyn_cast<::clang::MaterializeTemporaryExpr>(expr)) {
        return extract_enum_constant_name(materialized->getSubExpr());
    }
    return ::std::nullopt;
}

[[nodiscard]]
constexpr auto csharp_type_for_parse_pltxt_local(::clang::QualType type) -> ::std::string {
    type = type.getCanonicalType().getUnqualifiedType();
    auto const* type_ptr = type.getTypePtrOrNull();
    if (type_ptr == nullptr) {
        return "object";
    }
    if (auto const* tst = ::llvm::dyn_cast<::clang::TemplateSpecializationType>(type_ptr)) {
        if (auto const* template_decl = tst->getTemplateName().getAsTemplateDecl()) {
            auto const qualified_name = template_decl->getQualifiedNameAsString();
            if (qualified_name == "fast_io::stack" || qualified_name == "::fast_io::stack") {
                if (tst->getNumArgs() == 1U && tst->getArg(0).getKind() == ::clang::TemplateArgument::Type) {
                    return "Stack<" + csharp_type_for_parse_pltxt_local(tst->getArg(0).getAsType()) + ">";
                }
            }
            if (qualified_name == "pltxt2htm::HeapGuard" || qualified_name == "::pltxt2htm::HeapGuard") {
                if (tst->getNumArgs() == 1U && tst->getArg(0).getKind() == ::clang::TemplateArgument::Type) {
                    return csharp_type_for_parse_pltxt_local(tst->getArg(0).getAsType());
                }
            }
        }
    }
    if (auto const* record_type = type->getAs<::clang::RecordType>()) {
        if (auto const* decl = record_type->getDecl()) {
            auto const qualified_name = decl->getQualifiedNameAsString();
            if (qualified_name == "pltxt2htm::details::BasicFrameContext" ||
                qualified_name == "::pltxt2htm::details::BasicFrameContext") {
                return "BasicFrameContext";
            }
            return decl->getNameAsString();
        }
    }
    return csharp_type_for_field(type);
}

[[nodiscard]]
constexpr auto is_derived_from(::clang::CXXRecordDecl const* decl, ::llvm::StringRef qualified_name) -> bool {
    for (auto const& base : decl->bases()) {
        auto const* base_decl = base.getType()->getAsCXXRecordDecl();
        if (base_decl == nullptr) {
            continue;
        }
        auto const base_name = base_decl->getQualifiedNameAsString();
        if (::llvm::StringRef{base_name} == qualified_name) {
            return true;
        }
        if (is_derived_from(base_decl, qualified_name)) {
            return true;
        }
    }
    return false;
}

[[nodiscard]]
constexpr auto extract_node_type_for_record(::clang::CXXRecordDecl const* decl) -> ::std::optional<::std::string> {
    for (auto const* ctor : decl->ctors()) {
        if (ctor->isCopyOrMoveConstructor()) {
            continue;
        }
        for (auto const* init : ctor->inits()) {
            if (!init->isBaseInitializer()) {
                continue;
            }
            auto const* base_decl = init->getBaseClass()->getAsCXXRecordDecl();
            if (base_decl == nullptr) {
                continue;
            }
            auto const base_name = base_decl->getQualifiedNameAsString();
            if (base_name != "pltxt2htm::PlTxtNode" && base_name != "pltxt2htm::details::PairedTagBase") {
                continue;
            }
            if (auto node_type = extract_enum_constant_name(init->getInit())) {
                return node_type;
            }
        }
    }
    return ::std::nullopt;
}

constexpr void collect_astnode_from_decl_context(::clang::DeclContext const* decl_context,
                                                 ::clang::SourceManager& source_manager, AstNodeModel& out,
                                                 ::std::set<::std::string>& seen_classes,
                                                 bool& captured_node_type_enum) {
    for (auto const* decl : decl_context->decls()) {
        if (auto const* nested = ::llvm::dyn_cast<::clang::DeclContext>(decl); nested != nullptr) {
            collect_astnode_from_decl_context(nested, source_manager, out, seen_classes, captured_node_type_enum);
        }
        if (!captured_node_type_enum) {
            if (auto const* ed = ::llvm::dyn_cast<::clang::EnumDecl>(decl);
                ed != nullptr && ed->isCompleteDefinition() &&
                ed->getQualifiedNameAsString() == "pltxt2htm::NodeType") {
                auto const enum_loc = source_manager.getFileLoc(ed->getLocation());
                if (!is_astnode_header_path(source_manager.getFilename(enum_loc))) {
                    continue;
                }
                for (auto const* enumerator : ed->enumerators()) {
                    out.node_types.emplace_back(enumerator->getNameAsString());
                }
                captured_node_type_enum = true;
            }
        }
        auto const* rd = ::llvm::dyn_cast<::clang::CXXRecordDecl>(decl);
        if (rd == nullptr || !rd->isThisDeclarationADefinition() || rd->isImplicit()) {
            continue;
        }
        auto const loc = source_manager.getFileLoc(rd->getLocation());
        auto const source_file_ref = source_manager.getFilename(loc);
        if (!is_astnode_header_path(source_file_ref)) {
            continue;
        }
        auto const qualified_name = rd->getQualifiedNameAsString();
        if (qualified_name.empty()) {
            continue;
        }
        if (::llvm::StringRef{qualified_name}.starts_with("pltxt2htm::details::")) {
            continue;
        }
        auto const derived_from_paired = is_derived_from(rd, "pltxt2htm::details::PairedTagBase");
        auto const derived_from_node = is_derived_from(rd, "pltxt2htm::PlTxtNode");
        if (!derived_from_paired && !derived_from_node && rd->getName() != "Url") {
            continue;
        }
        if (!seen_classes.insert(qualified_name).second) {
            continue;
        }

        AstNodeClassSpec cls{};
        cls.name = rd->getNameAsString();
        cls.source_file = normalized_include(source_file_ref);
        cls.source_line = loc.isValid() ? source_manager.getSpellingLineNumber(loc) : 0U;
        if (derived_from_paired) {
            cls.base_kind = AstNodeBaseKind::paired_tag;
        }
        else if (derived_from_node) {
            cls.base_kind = AstNodeBaseKind::pltxt_node;
        }
        cls.node_type = extract_node_type_for_record(rd);
        for (auto const* field : rd->fields()) {
            cls.fields.push_back(AstNodeField{
                .cpp_name = field->getNameAsString(),
                .csharp_type = csharp_type_for_field(field->getType()),
            });
        }
        for (auto const* ctor : rd->ctors()) {
            if (ctor->getAccess() != ::clang::AS_public || ctor->isCopyOrMoveConstructor()) {
                continue;
            }
            if (ctor->getNumParams() == 0) {
                cls.has_default_ctor = true;
                break;
            }
        }
        out.classes.push_back(::std::move(cls));
    }
}

class AstNodeTranslationVisitor : public ::clang::RecursiveASTVisitor<AstNodeTranslationVisitor> {
public:
    constexpr AstNodeTranslationVisitor(::clang::ASTContext& ast_context, AstNodeModel& out) noexcept
        : source_manager_(ast_context.getSourceManager()),
          out_(out) {
    }

    constexpr auto VisitEnumDecl(::clang::EnumDecl* ed) -> bool {
        if (captured_node_type_enum_) {
            return true;
        }
        if (ed == nullptr || !ed->isCompleteDefinition()) {
            return true;
        }
        if (ed->getName() != "NodeType") {
            return true;
        }
        auto const loc = source_manager_.getSpellingLoc(ed->getLocation());
        if (!loc.isValid()) {
            return true;
        }
        auto const qualified_name = ed->getQualifiedNameAsString();
        if (qualified_name != "pltxt2htm::NodeType") {
            return true;
        }
        for (auto const* enumerator : ed->enumerators()) {
            out_.node_types.emplace_back(enumerator->getNameAsString());
        }
        captured_node_type_enum_ = true;
        return true;
    }

    constexpr auto VisitCXXRecordDecl(::clang::CXXRecordDecl* rd) -> bool {
        if (rd == nullptr || !rd->isThisDeclarationADefinition() || rd->isImplicit()) {
            return true;
        }
        auto const qualified_name = rd->getQualifiedNameAsString();
        if (qualified_name.empty()) {
            return true;
        }
        if (!::llvm::StringRef{qualified_name}.starts_with("pltxt2htm::")) {
            return true;
        }
        if (::llvm::StringRef{qualified_name}.starts_with("pltxt2htm::details::")) {
            return true;
        }
        auto const derived_from_paired = is_derived_from(rd, "pltxt2htm::details::PairedTagBase");
        auto const derived_from_node = is_derived_from(rd, "pltxt2htm::PlTxtNode");
        if (!derived_from_paired && !derived_from_node && rd->getName() != "Url") {
            return true;
        }
        if (!seen_classes_.insert(qualified_name).second) {
            return true;
        }

        AstNodeClassSpec cls{};
        cls.name = rd->getNameAsString();
        auto const loc = source_manager_.getSpellingLoc(rd->getLocation());
        cls.source_file = normalized_include(source_manager_.getFilename(loc));
        cls.source_line = loc.isValid() ? source_manager_.getSpellingLineNumber(loc) : 0U;
        if (derived_from_paired) {
            cls.base_kind = AstNodeBaseKind::paired_tag;
        }
        else if (derived_from_node) {
            cls.base_kind = AstNodeBaseKind::pltxt_node;
        }
        cls.node_type = extract_node_type_for_record(rd);
        for (auto const* field : rd->fields()) {
            cls.fields.push_back(AstNodeField{
                .cpp_name = field->getNameAsString(),
                .csharp_type = csharp_type_for_field(field->getType()),
            });
        }
        for (auto const* ctor : rd->ctors()) {
            if (ctor->getAccess() != ::clang::AS_public || ctor->isCopyOrMoveConstructor()) {
                continue;
            }
            if (ctor->getNumParams() == 0) {
                cls.has_default_ctor = true;
                break;
            }
        }
        out_.classes.push_back(::std::move(cls));
        return true;
    }

private:
    ::clang::SourceManager& source_manager_;
    AstNodeModel& out_;
    bool captured_node_type_enum_{};
    ::std::set<::std::string> seen_classes_{};
};

[[nodiscard]]
constexpr auto is_public_parse_pltxt_decl(::clang::FunctionDecl const* fd) -> bool {
    if (fd == nullptr) {
        return false;
    }
    if (fd->getQualifiedNameAsString() != "pltxt2htm::parse_pltxt") {
        return false;
    }
    if (fd->getNumParams() != 1) {
        return false;
    }
    auto const param_text = fd->getParamDecl(0)->getType().getAsString();
    return param_text.find("u8string_view") != ::std::string::npos;
}

class ParsePltxtToCsharpAstVisitor : public ::clang::RecursiveASTVisitor<ParsePltxtToCsharpAstVisitor> {
public:
    constexpr explicit ParsePltxtToCsharpAstVisitor(ParsePltxtModel& out) noexcept
        : out_(out) {
    }

    constexpr auto TraverseFunctionDecl(::clang::FunctionDecl* fd) -> bool {
        if (fd == nullptr) {
            return true;
        }
        auto const previous_in_target_function = in_target_function_;
        if (!collected_ && is_public_parse_pltxt_decl(fd)) {
            in_target_function_ = true;
        }
        auto const ok = ::clang::RecursiveASTVisitor<ParsePltxtToCsharpAstVisitor>::TraverseFunctionDecl(fd);
        in_target_function_ = previous_in_target_function;
        return ok;
    }

    constexpr auto TraverseSwitchStmt(::clang::SwitchStmt* switch_stmt) -> bool {
        if (switch_stmt == nullptr) {
            return true;
        }
        auto const previous_in_switch = in_target_switch_;
        if (in_target_function_) {
            in_target_switch_ = true;
        }
        auto const ok = ::clang::RecursiveASTVisitor<ParsePltxtToCsharpAstVisitor>::TraverseSwitchStmt(switch_stmt);
        in_target_switch_ = previous_in_switch;
        return ok;
    }

    constexpr auto VisitCaseStmt(::clang::CaseStmt* case_stmt) -> bool {
        if (!in_target_function_ || !in_target_switch_ || case_stmt == nullptr) {
            return true;
        }
        if (auto node_type = extract_enum_constant_name(case_stmt->getLHS()); node_type.has_value()) {
            if (seen_case_labels_.insert(*node_type).second) {
                out_.nested_tag_switch_cases.push_back(ParsePltxtModel::CsharpSwitchCase{
                    .node_type = *node_type,
                });
            }
        }
        return true;
    }

    constexpr auto VisitVarDecl(::clang::VarDecl* var_decl) -> bool {
        if (!in_target_function_ || var_decl == nullptr || captured_stack_decl_) {
            return true;
        }
        auto const mapped_type = csharp_type_for_parse_pltxt_local(var_decl->getType());
        if (!::llvm::StringRef{mapped_type}.starts_with("Stack<")) {
            return true;
        }
        out_.stack_var_name_csharp = to_camel_case(var_decl->getNameAsString());
        out_.stack_var_csharp_type = mapped_type;
        captured_stack_decl_ = true;
        return true;
    }

    [[nodiscard]]
    constexpr auto collected() const noexcept -> bool {
        return collected_ || !out_.nested_tag_switch_cases.empty();
    }

    constexpr void finalize() noexcept {
        collected_ = !out_.nested_tag_switch_cases.empty();
    }

private:
    ParsePltxtModel& out_;
    bool in_target_function_{};
    bool in_target_switch_{};
    bool collected_{};
    bool captured_stack_decl_{};
    ::std::set<::std::string> seen_case_labels_{};
};

constexpr void emit_astnode_enum(::llvm::raw_string_ostream& out, AstNodeModel const& astnode_model) {
    out << "public enum NodeType\n";
    out << "{\n";
    for (auto const& node_type : astnode_model.node_types) {
        out << "    " << node_type << ",\n";
    }
    out << "}\n\n";
}

constexpr void emit_property(::llvm::raw_string_ostream& out, ::llvm::StringRef class_name, AstNodeField const& field) {
    auto property_name = to_pascal_case(field.cpp_name);
    if (property_name == class_name.str()) {
        property_name += "Value";
    }
    out << "    public " << field.csharp_type << " " << property_name << " { get; }\n";
}

constexpr void emit_constructor_assignments(::llvm::raw_string_ostream& out, ::llvm::StringRef class_name,
                                            ::std::vector<AstNodeField> const& fields) {
    for (auto const& field : fields) {
        auto property_name = to_pascal_case(field.cpp_name);
        if (property_name == class_name.str()) {
            property_name += "Value";
        }
        out << "        " << property_name << " = " << to_camel_case(field.cpp_name) << ";\n";
    }
}

constexpr void emit_astnode_class(::llvm::raw_string_ostream& out, AstNodeClassSpec const& cls) {
    if (cls.name == "PlTxtNode" || cls.name == "PairedTagBase") {
        return;
    }
    out << "public sealed class " << cls.name;
    if (cls.base_kind == AstNodeBaseKind::pltxt_node) {
        out << " : PlTxtNode";
    }
    else if (cls.base_kind == AstNodeBaseKind::paired_tag) {
        out << " : PairedTagBase";
    }
    out << "\n{\n";
    for (auto const& field : cls.fields) {
        emit_property(out, cls.name, field);
    }
    if (!cls.fields.empty()) {
        out << "\n";
    }

    bool emitted_parameterless_ctor = false;
    if (cls.has_default_ctor) {
        if (cls.base_kind == AstNodeBaseKind::paired_tag && cls.fields.empty()) {
            out << "    public " << cls.name << "() : this(new Ast())\n";
            out << "    {\n";
            out << "    }\n\n";
            emitted_parameterless_ctor = true;
        }
        else if (cls.fields.empty()) {
            out << "    public " << cls.name << "()";
            if (cls.base_kind == AstNodeBaseKind::pltxt_node && cls.node_type.has_value()) {
                out << " : base(NodeType." << *cls.node_type << ")";
            }
            out << "\n";
            out << "    {\n";
            out << "    }\n\n";
            emitted_parameterless_ctor = true;
        }
        else if (cls.base_kind == AstNodeBaseKind::plain && cls.fields.size() == 1 &&
                 cls.fields.front().csharp_type == "Ast") {
            out << "    public " << cls.name << "() : this(new Ast())\n";
            out << "    {\n";
            out << "    }\n\n";
        }
    }

    bool needs_subast_param = cls.base_kind == AstNodeBaseKind::paired_tag;
    bool has_field_params = !cls.fields.empty();
    if (emitted_parameterless_ctor && !needs_subast_param && !has_field_params) {
        out << "}\n\n";
        return;
    }

    bool need_comma = false;
    out << "    public " << cls.name << "(";
    if (needs_subast_param) {
        out << "Ast subast";
        need_comma = true;
    }
    for (auto const& field : cls.fields) {
        if (need_comma) {
            out << ", ";
        }
        out << field.csharp_type << " " << to_camel_case(field.cpp_name);
        need_comma = true;
    }
    out << ")";
    if (cls.base_kind == AstNodeBaseKind::pltxt_node && cls.node_type.has_value()) {
        out << " : base(NodeType." << *cls.node_type << ")";
    }
    else if (cls.base_kind == AstNodeBaseKind::paired_tag && cls.node_type.has_value()) {
        out << " : base(NodeType." << *cls.node_type << ", subast)";
    }
    out << "\n";
    out << "    {\n";
    emit_constructor_assignments(out, cls.name, cls.fields);
    out << "    }\n";
    out << "}\n\n";
}

constexpr void emit_astnode_translation(::llvm::raw_string_ostream& out, AstNodeModel const& astnode_model) {
    emit_astnode_enum(out, astnode_model);
    out << "public abstract class PlTxtNode\n";
    out << "{\n";
    out << "    public NodeType RuntimeNodeTypeInfo { get; }\n\n";
    out << "    protected PlTxtNode(NodeType type)\n";
    out << "    {\n";
    out << "        RuntimeNodeTypeInfo = type;\n";
    out << "    }\n\n";
    out << "    public NodeType NodeType => RuntimeNodeTypeInfo;\n";
    out << "}\n\n";
    out << "public sealed class Ast\n";
    out << "{\n";
    out << "    // fast_io::vector<HeapGuard<PlTxtNode>> -> List<PlTxtNode>\n";
    out << "    public List<PlTxtNode> Nodes { get; }\n\n";
    out << "    public Ast()\n";
    out << "    {\n";
    out << "        Nodes = new List<PlTxtNode>();\n";
    out << "    }\n\n";
    out << "    public Ast(IEnumerable<PlTxtNode> nodes)\n";
    out << "    {\n";
    out << "        Nodes = new List<PlTxtNode>(nodes);\n";
    out << "    }\n";
    out << "}\n\n";
    out << "public abstract class PairedTagBase : PlTxtNode\n";
    out << "{\n";
    out << "    public Ast Subast { get; }\n\n";
    out << "    protected PairedTagBase(NodeType nodeType, Ast subast) : base(nodeType)\n";
    out << "    {\n";
    out << "        Subast = subast;\n";
    out << "    }\n";
    out << "}\n\n";
    for (auto const& cls : astnode_model.classes) {
        emit_astnode_class(out, cls);
    }
}

[[nodiscard]]
constexpr auto find_paired_tag_class_for_node_type(AstNodeModel const& astnode_model, ::llvm::StringRef node_type)
    -> ::std::optional<::std::string> {
    for (auto const& cls : astnode_model.classes) {
        if (cls.base_kind != AstNodeBaseKind::paired_tag || !cls.node_type.has_value()) {
            continue;
        }
        if (*cls.node_type == node_type.str()) {
            return cls.name;
        }
    }
    return ::std::nullopt;
}

constexpr void emit_parse_pltxt_translation(::llvm::raw_string_ostream& out, TranslationModel const& model) {
    auto const stack_var_name = model.parse_pltxt.stack_var_name_csharp.empty() ? ::llvm::StringRef{"callStack"}
                                                                                 : ::llvm::StringRef{
                                                                                       model.parse_pltxt
                                                                                           .stack_var_name_csharp};
    auto const stack_var_type =
        model.parse_pltxt.stack_var_csharp_type.empty() ? ::llvm::StringRef{"Stack<BasicFrameContext>"}
                                                        : ::llvm::StringRef{model.parse_pltxt.stack_var_csharp_type};
    out << "    internal static Ast ParsePltxt(string pltext)\n";
    out << "    {\n";
    out << "        var " << stack_var_name << " = new " << stack_var_type << "();\n";
    out << "        var result = new Ast();\n\n";
    out << "        ulong startIndex = 0;\n\n";
    out << "        while (true)\n";
    out << "        {\n";
    out << "            var (forwardIndex, hasNewFrame) = DevilStuffAfterLineBreak(U8StringViewSubview(pltext, "
           "startIndex), "
        << stack_var_name << ", result);\n";
    out << "            startIndex += forwardIndex;\n";
    out << "            if (!hasNewFrame)\n";
    out << "            {\n";
    out << "                break;\n";
    out << "            }\n";
    out << "            var typeOfSubast = " << stack_var_name << ".Peek().NestedTagType;\n";
    out << "            var subast = ParsePltxtDetails(" << stack_var_name << ");\n";
    out << "            switch (typeOfSubast)\n";
    out << "            {\n";
    for (auto const& switch_case : model.parse_pltxt.nested_tag_switch_cases) {
        auto class_name = find_paired_tag_class_for_node_type(model.astnodes, switch_case.node_type);
        if (!class_name.has_value()) {
            terminate_internal_error("failed to map parse_pltxt switch node type to paired tag class: " +
                                     switch_case.node_type);
        }
        out << "            case NodeType." << switch_case.node_type << ":\n";
        out << "                result.Nodes.Add(HeapGuard(new " << *class_name << "(subast)));\n";
        out << "                continue;\n";
    }
    out << "            default:\n";
    out << "                Terminate(\"unexpected nested tag type in parse_pltxt\");\n";
    out << "                break;\n";
    out << "            }\n";
    out << "        }\n\n";
    out << "        if (startIndex < (ulong)pltext.Length)\n";
    out << "        {\n";
    out << "            " << stack_var_name
        << ".Push(HeapGuard(new BareTagContext(U8StringViewSubview(pltext, startIndex), "
           "NodeType.base, result)));\n";
    out << "            result = ParsePltxtDetails(" << stack_var_name << ");\n";
    out << "        }\n\n";
    out << "        var callStackIsEmpty = " << stack_var_name << ".Count == 0;\n";
    out << "        Pltxt2Assert(callStackIsEmpty, \"call_stack is not empty\");\n\n";
    out << "        return result;\n";
    out << "    }\n\n";
    out << "    private static string U8StringViewSubview(string text, ulong startIndex)\n";
    out << "    {\n";
    out << "        if (startIndex >= (ulong)text.Length)\n";
    out << "        {\n";
    out << "            return string.Empty;\n";
    out << "        }\n";
    out << "        return text[(int)startIndex..];\n";
    out << "    }\n\n";
    out << "    private static (ulong forwardIndex, bool hasNewFrame) DevilStuffAfterLineBreak(string pltextView, "
        << stack_var_type << " " << stack_var_name << ", Ast result) => throw new NotImplementedException(\"Translate "
           "details::devil_stuff_after_line_break from parser.hh.\");\n";
    out << "    private static Ast ParsePltxtDetails(" << stack_var_type << " " << stack_var_name << ") => throw new "
           "NotImplementedException(\"Translate details::parse_pltxt from details/parser/parser.hh.\");\n";
    out << "    private static void Pltxt2Assert(bool condition, string message)\n";
    out << "    {\n";
    out << "        if (!condition)\n";
    out << "        {\n";
    out << "            Terminate(message);\n";
    out << "        }\n";
    out << "    }\n\n";
    out << "    private class BasicFrameContext\n";
    out << "    {\n";
    out << "        internal BasicFrameContext(NodeType nestedTagType, Ast subast)\n";
    out << "        {\n";
    out << "            NestedTagType = nestedTagType;\n";
    out << "            Subast = subast;\n";
    out << "        }\n\n";
    out << "        internal NodeType NestedTagType { get; }\n";
    out << "        internal Ast Subast { get; }\n";
    out << "    }\n\n";
    out << "    private sealed class BareTagContext : BasicFrameContext\n";
    out << "    {\n";
    out << "        internal BareTagContext(string sourceText, NodeType nestedTagType, Ast subast)\n";
    out << "            : base(nestedTagType, subast)\n";
    out << "        {\n";
    out << "            SourceText = sourceText;\n";
    out << "        }\n\n";
    out << "        internal string SourceText { get; }\n";
    out << "    }\n\n";
}

constexpr void emit_wrapper_signature(::llvm::raw_string_ostream& out, ::llvm::StringRef method,
                                      ::llvm::StringRef args) {
    out << "    public static string " << method << "(" << args << ")\n";
}

constexpr void emit_method_definition(::llvm::raw_string_ostream& out, ::llvm::StringRef method, ::llvm::StringRef args,
                                      bool optimize, ::llvm::StringRef backend_expr) {
    emit_wrapper_signature(out, method, args);
    out << "    {\n";
    out << "        var ast = Pltxt2Internal.ParsePltxt(pltext);\n";
    if (optimize) {
        out << "        Pltxt2Internal.OptimizeAst(ast);\n";
    }
    out << "        return " << backend_expr << ";\n";
    out << "    }\n\n";
}

[[nodiscard]]
constexpr auto get_single_variant(ApiInstantiationMap const& instantiated, ::llvm::StringRef api_name) -> VariantKey {
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
constexpr auto generate_csharp(TranslationModel const& model) -> ::std::string {
    auto const& instantiated = model.api_instantiations;
    ::std::string generated;
    ::llvm::raw_string_ostream out(generated);
    out << "// <auto-generated />\n"
           "// Generated by translang/csharp using clang template instantiation from compiled source.\n"
           "// Source: translang/csharp/pltxt2htm.cc\n"
           "// repo: https://github.com/SekaiArendelle/pltxt2htm\n\n"
           "using System;\n"
           "using System.Collections.Generic;\n\n"
           "namespace Pltxt2htm.Generated;\n\n";
    emit_astnode_translation(out, model.astnodes);
    out << "public static class Pltxt2Htm\n";
    out << "{\n";

    auto const advanced = get_single_variant(instantiated, "pltxt2advanced_html");
    auto const plunity = get_single_variant(instantiated, "pltxt2plunity_introduction");
    auto const common = get_single_variant(instantiated, "pltxt2common_html");

    emit_method_definition(out, "Pltxt2AdvancedHtml", "string pltext", advanced.optimize,
                           "Pltxt2Internal.PlwebTextBackend(ast, \"localhost:5173\", \"$PROJECT\", \"$VISITOR\", "
                           "\"$AUTHOR\", \"$CO_AUTHORS\")");

    emit_method_definition(out, "Pltxt2PlunityIntroduction",
                           "string pltext, string project, string visitor, string author, string coauthors",
                           plunity.optimize,
                           "Pltxt2Internal.PlunityTextBackend(ast, project, visitor, author, coauthors)");

    emit_method_definition(out, "Pltxt2CommonHtml", "string pltext", common.optimize,
                           "Pltxt2Internal.PlwebTitleBackend(ast)");

    out << "}\n\n";
    out << "internal static class Pltxt2Internal\n";
    out << "{\n";
    out << "    // exception::terminate / exception::unreachable -> throw\n";
    out << "    internal static void Terminate(string message) => throw new InvalidOperationException(message);\n";
    out << "    internal static T Unreachable<T>(string message) => throw new InvalidOperationException(message);\n\n";
    emit_parse_pltxt_translation(out, model);
    out << "    internal static void OptimizeAst(Ast ast) => throw new NotImplementedException(\"Translate "
           "optimizer.hh to fill this.\");\n";
    out << "    internal static string PlwebTextBackend(Ast ast, string host, string project, string visitor, string "
           "author, string coauthors) => throw new NotImplementedException(\"Translate "
           "details/backend/for_plweb_text.hh to fill this.\");\n";
    out << "    internal static string PlunityTextBackend(Ast ast, string project, string visitor, string author, "
           "string coauthors) => throw new NotImplementedException(\"Translate details/backend/for_plunity_text.hh to "
           "fill this.\");\n";
    out << "    internal static string PlwebTitleBackend(Ast ast) => throw new NotImplementedException(\"Translate "
           "details/backend/for_plweb_title.hh to fill this.\");\n\n";
    out << "    // HeapGuard in C++ is only heap object lifetime guard; in C# use normal variable.\n";
    out << "    internal static T HeapGuard<T>(T value) => value;\n";
    out << "}\n";
    out.flush();
    return generated;
}

constexpr auto validate_required_instantiations(ApiInstantiationMap const& instantiated) -> ::llvm::Error {
    constexpr ::llvm::StringLiteral apis[]{
        "pltxt2advanced_html",
        "pltxt2plunity_introduction",
        "pltxt2common_html",
    };
    for (auto const api : apis) {
        auto it = instantiated.find(api);
        if (it == instantiated.end()) {
            return ::llvm::createStringError(::std::errc::invalid_argument, "missing template instantiations for: %s",
                                             api.data());
        }
        if (it->second.variants.size() != 1) {
            return ::llvm::createStringError(::std::errc::invalid_argument,
                                             "expected exactly one specialization for: %s", api.data());
        }
    }
    return ::llvm::Error::success();
}

[[nodiscard]]
constexpr auto build_clang_args(Paths const& paths) -> ::llvm::Expected<::std::vector<::std::string>> {
    ::std::vector<::std::string> args;
    args.emplace_back("-std=c++23");
    args.emplace_back("-fsyntax-only");
    args.emplace_back("-fno-delayed-template-parsing");
    ::llvm::SmallString<260> include_abs{paths.include_dir};
    if (auto ec = ::llvm::sys::fs::make_absolute(include_abs)) {
        auto const include_path_text = paths.include_dir.str().str();
        return ::llvm::createStringError(ec, "failed to resolve absolute path: %s", include_path_text.c_str());
    }
    args.emplace_back("-I" + normalized_include(include_abs));
#if defined(NDEBUG)
    args.emplace_back("-DNDEBUG");
#endif
    return args;
}

[[nodiscard]]
constexpr auto collect_astnode_model(Paths const& paths) -> ::llvm::Expected<AstNodeModel> {
    auto args = build_clang_args(paths);
    if (!args) {
        return args.takeError();
    }
    constexpr ::llvm::StringLiteral astnode_probe_source =
        "#include <pltxt2htm/astnode/node_type.hh>\n"
        "#include <pltxt2htm/astnode/basic.hh>\n"
        "#include <pltxt2htm/astnode/html_node.hh>\n"
        "#include <pltxt2htm/astnode/markdown_node.hh>\n"
        "#include <pltxt2htm/astnode/physics_lab_node.hh>\n";
    auto ast = ::clang::tooling::buildASTFromCodeWithArgs(astnode_probe_source.str(), *args, "astnode_probe.cc");
    if (!ast) {
        return ::llvm::createStringError(::std::errc::invalid_argument,
                                         "clang failed to parse include/pltxt2htm/astnode headers");
    }

    AstNodeModel astnode_model{};
    ::std::set<::std::string> seen_classes{};
    bool captured_node_type_enum{};
    collect_astnode_from_decl_context(ast->getASTContext().getTranslationUnitDecl(),
                                      ast->getASTContext().getSourceManager(), astnode_model, seen_classes,
                                      captured_node_type_enum);
    if (astnode_model.node_types.empty()) {
        return ::llvm::createStringError(::std::errc::invalid_argument,
                                         "failed to parse NodeType from include/pltxt2htm/astnode");
    }
    if (astnode_model.classes.empty()) {
        return ::llvm::createStringError(::std::errc::invalid_argument,
                                         "failed to parse astnode classes from include/pltxt2htm/astnode");
    }
    ::std::sort(astnode_model.classes.begin(), astnode_model.classes.end(),
                [](::AstNodeClassSpec const& lhs, ::AstNodeClassSpec const& rhs) {
                    if (lhs.source_file != rhs.source_file) {
                        return lhs.source_file < rhs.source_file;
                    }
                    if (lhs.source_line != rhs.source_line) {
                        return lhs.source_line < rhs.source_line;
                    }
                    return lhs.name < rhs.name;
                });
    return astnode_model;
}

[[nodiscard]]
constexpr auto collect_parse_pltxt_model(Paths const& paths) -> ::llvm::Expected<ParsePltxtModel> {
    auto args = build_clang_args(paths);
    if (!args) {
        return args.takeError();
    }
    constexpr ::llvm::StringLiteral parser_probe_source = "#include <pltxt2htm/parser.hh>\n";
    auto ast = ::clang::tooling::buildASTFromCodeWithArgs(parser_probe_source.str(), *args, "parser_probe.cc");
    if (!ast) {
        return ::llvm::createStringError(::std::errc::invalid_argument, "clang failed to parse include/pltxt2htm/parser.hh");
    }
    ParsePltxtModel model{};
    ParsePltxtToCsharpAstVisitor visitor(model);
    visitor.TraverseDecl(ast->getASTContext().getTranslationUnitDecl());
    visitor.finalize();
    if (!visitor.collected()) {
        return ::llvm::createStringError(::std::errc::invalid_argument,
                                         "failed to derive parse_pltxt switch cases from include/pltxt2htm/parser.hh");
    }
    return model;
}

constexpr auto collect_translation_model(Paths const& paths) -> ::llvm::Expected<TranslationModel> {
    ::llvm::SmallString<260> source_abs{paths.source_path};
    if (auto ec = ::llvm::sys::fs::make_absolute(source_abs)) {
        auto const source_path_text = paths.source_path.str().str();
        return ::llvm::createStringError(ec, "failed to resolve absolute path: %s", source_path_text.c_str());
    }

    auto source_text = read_text_file(source_abs);
    if (!source_text) {
        return source_text.takeError();
    }
    auto args = build_clang_args(paths);
    if (!args) {
        return args.takeError();
    }

    auto ast = ::clang::tooling::buildASTFromCodeWithArgs(*source_text, *args, normalized_include(source_abs));
    if (!ast) {
        return ::llvm::createStringError(::std::errc::invalid_argument,
                                         "clang failed to compile/template-instantiate translang/csharp/pltxt2htm.cc");
    }

    TranslationModel model{};
    auto* translation_unit = ast->getASTContext().getTranslationUnitDecl();
    ApiInstantiationVisitor api_visitor(model.api_instantiations);
    api_visitor.TraverseDecl(translation_unit);
    if (auto err = validate_required_instantiations(model.api_instantiations)) {
        return ::std::move(err);
    }
    auto astnode_model = collect_astnode_model(paths);
    if (!astnode_model) {
        return astnode_model.takeError();
    }
    auto parse_pltxt_model = collect_parse_pltxt_model(paths);
    if (!parse_pltxt_model) {
        return parse_pltxt_model.takeError();
    }
    model.astnodes = ::std::move(*astnode_model);
    model.parse_pltxt = ::std::move(*parse_pltxt_model);
    return model;
}

constexpr auto write_text_file(::llvm::StringRef path, ::std::string const& content) -> ::llvm::Error {
    ::std::error_code ec;
    auto const parent = ::llvm::sys::path::parent_path(path);
    if (!parent.empty()) {
        ec = ::llvm::sys::fs::create_directories(parent, /*IgnoreExisting=*/true);
    }
    if (ec) {
        auto const parent_text = parent.str();
        return ::llvm::createStringError(::std::errc::io_error, "failed to create output directory: %s (%s)",
                                         parent_text.c_str(), ec.message().c_str());
    }
    ::llvm::raw_fd_ostream ofs(path, ec, ::llvm::sys::fs::OF_None);
    if (ec) {
        return ::llvm::createStringError(::std::errc::io_error, "failed to open output file: %s", path.data());
    }
    ofs.write(content.data(), content.size());
    ofs.flush();
    if (ofs.has_error()) {
        return ::llvm::createStringError(::std::errc::io_error, "failed to write output file: %s", path.data());
    }
    return ::llvm::Error::success();
}

int main(int argc, char const* const* argv) {
    auto cli = parse_cli_options(argc, argv);
    if (!cli) {
        exit_with_error(cli.takeError());
    }

    auto paths = hardcoded_paths(cli->output_dir);

    auto collect_result = collect_translation_model(paths);
    if (!collect_result) {
        exit_with_error(collect_result.takeError());
    }

    auto generated = generate_csharp(*collect_result);
    auto write_result = write_text_file(paths.output_path, generated);
    if (write_result) {
        exit_with_error(::std::move(write_result));
    }

    ::llvm::outs() << "Generated C# translation to " << paths.output_path.str() << '\n';
    return 0;
}
