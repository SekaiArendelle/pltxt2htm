#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/DeclCXX.h>
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
#include <llvm/Support/Path.h>
#include <llvm/Support/raw_ostream.h>

#include <cctype>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include <pltxt2htm/contracts.hh>



class ApiInstantiationVisitor : public ::clang::RecursiveASTVisitor<ApiInstantiationVisitor> {
    using Base = ::clang::RecursiveASTVisitor<ApiInstantiationVisitor>;

    ::std::string csharp_code_{};
    ::std::set<::std::string> emitted_stub_keys_{};
    ::std::set<::std::string> emitted_control_flow_keys_{};
    ::std::map<::std::string, ::std::vector<::std::string>> function_body_snippets_{};
    ::std::string current_function_key_{};
    bool in_target_function_{};

public:
    ApiInstantiationVisitor() = default;

    [[nodiscard]]
    constexpr auto shouldVisitTemplateInstantiations() const noexcept -> bool {
        return true;
    }

    [[nodiscard]]
    constexpr auto csharp_code() const noexcept -> ::std::string const& {
        return csharp_code_;
    }

    auto VisitFunctionDecl(::clang::FunctionDecl* fd) -> bool {
        static_cast<void>(fd);
        return true;
    }

    auto TraverseFunctionDecl(::clang::FunctionDecl* fd) -> bool {
        auto const previous = in_target_function_;
        auto const previous_function_key = current_function_key_;
        in_target_function_ = previous || is_target_function(fd);
        if (is_target_function(fd) && has_template_context(fd)) {
            current_function_key_ = build_stub_key(fd);
        }
        else {
            current_function_key_.clear();
        }
        auto const result = Base::TraverseFunctionDecl(fd);
        if (is_target_function(fd)) {
            append_function_stub(fd);
        }
        in_target_function_ = previous;
        current_function_key_ = previous_function_key;
        return result;
    }

    auto VisitIfStmt(::clang::IfStmt* if_stmt) -> bool {
        if (!in_target_function_ || if_stmt == nullptr) {
            return true;
        }
        append_control_flow_hint("if", if_stmt->getCond());
        return true;
    }

    auto VisitSwitchStmt(::clang::SwitchStmt* switch_stmt) -> bool {
        if (!in_target_function_ || switch_stmt == nullptr) {
            return true;
        }
        append_control_flow_hint("switch", switch_stmt->getCond());
        return true;
    }

    auto VisitWhileStmt(::clang::WhileStmt* while_stmt) -> bool {
        if (!in_target_function_ || while_stmt == nullptr) {
            return true;
        }
        append_control_flow_hint("while", while_stmt->getCond());
        return true;
    }

    auto VisitVarDecl(::clang::VarDecl* vd) -> bool {
        if (vd == nullptr || !vd->getIdentifier()) {
            return true;
        }

        ::clang::DeclContext *parentContext = vd->getDeclContext();

        if (parentContext->isFunctionOrMethod() == false) {
            // if it is not a variable inside a function (that is, if it is global, namespace, or a class static member, etc.), then skip it
            // Continue traversing the other nodes, but do not process this one
            return true;
        }

        // TODO
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
            return true;
        }
        auto* tsi = callee->getTemplateSpecializationInfo();
        if (tsi == nullptr || tsi->TemplateArguments == nullptr) {
            return true;
        }
        return true;
    }

private:
    static constexpr auto is_target(::llvm::StringRef name) noexcept -> bool {
        return name == "pltxt2advanced_html" || name == "pltxt2fixedadv_html" || name == "pltxt2plunity_introduction" ||
               name == "pltxt2common_html";
    }

    static constexpr auto map_csharp_type(::llvm::StringRef cpp_type) noexcept -> ::llvm::StringRef {
        if (cpp_type == "void") {
            return "void";
        }
        if (cpp_type == "bool") {
            return "bool";
        }
        if (cpp_type == "char" || cpp_type == "signed char" || cpp_type == "unsigned char") {
            return "byte";
        }
        if (cpp_type == "short" || cpp_type == "unsigned short") {
            return "short";
        }
        if (cpp_type == "int" || cpp_type == "unsigned int") {
            return "int";
        }
        if (cpp_type == "long" || cpp_type == "unsigned long" || cpp_type == "long long" ||
            cpp_type == "unsigned long long") {
            return "long";
        }
        if (cpp_type == "float") {
            return "float";
        }
        if (cpp_type == "double") {
            return "double";
        }
        if (cpp_type == "std::string" || cpp_type == "::std::string" || cpp_type == "fast_io::u8string_view" ||
            cpp_type == "::fast_io::u8string_view") {
            return "string";
        }
        return "object";
    }

    static auto map_csharp_type(::clang::QualType type) -> ::std::string {
        if (type.isNull()) {
            return "object";
        }
        auto canonical = type.getCanonicalType();
        while (canonical->isReferenceType()) {
            canonical = canonical->getPointeeType();
        }
        canonical = canonical.getUnqualifiedType();
        if (canonical->isVoidType()) {
            return "void";
        }
        if (canonical->isBooleanType()) {
            return "bool";
        }
        if (canonical->isSpecificBuiltinType(::clang::BuiltinType::Char_S) ||
            canonical->isSpecificBuiltinType(::clang::BuiltinType::UChar) ||
            canonical->isSpecificBuiltinType(::clang::BuiltinType::SChar)) {
            return "byte";
        }
        if (canonical->isSpecificBuiltinType(::clang::BuiltinType::Short) ||
            canonical->isSpecificBuiltinType(::clang::BuiltinType::UShort)) {
            return "short";
        }
        if (canonical->isSpecificBuiltinType(::clang::BuiltinType::Int) ||
            canonical->isSpecificBuiltinType(::clang::BuiltinType::UInt)) {
            return "int";
        }
        if (canonical->isSpecificBuiltinType(::clang::BuiltinType::Long) ||
            canonical->isSpecificBuiltinType(::clang::BuiltinType::ULong) ||
            canonical->isSpecificBuiltinType(::clang::BuiltinType::LongLong) ||
            canonical->isSpecificBuiltinType(::clang::BuiltinType::ULongLong)) {
            return "long";
        }
        if (canonical->isSpecificBuiltinType(::clang::BuiltinType::Float)) {
            return "float";
        }
        if (canonical->isSpecificBuiltinType(::clang::BuiltinType::Double)) {
            return "double";
        }

        auto const type_text = canonical.getAsString();
        auto const mapped_text = map_csharp_type(type_text);
        if (mapped_text != "object") {
            return mapped_text.str();
        }
        if (type_text.find("string") != ::std::string::npos || type_text.find("u8string_view") != ::std::string::npos) {
            return "string";
        }
        return "object";
    }

    static auto to_pascal_case(::llvm::StringRef name) -> ::std::string {
        ::std::string out{};
        out.reserve(name.size());
        bool upper_next = true;
        for (auto const ch : name) {
            if (ch == '_' || ch == '-') {
                upper_next = true;
                continue;
            }
            if (upper_next) {
                out.push_back(static_cast<char>(::std::toupper(static_cast<unsigned char>(ch))));
                upper_next = false;
            }
            else {
                out.push_back(ch);
            }
        }
        return out;
    }

    static auto is_target_function(::clang::FunctionDecl const* fd) -> bool {
        if (fd == nullptr || !fd->getIdentifier()) {
            return false;
        }
        return is_target(::llvm::StringRef{fd->getName()});
    }

    static auto summarize_condition(::clang::Expr const* expr) -> ::std::string {
        if (expr == nullptr) {
            return "condition: <null>";
        }
        auto const* normalized = expr->IgnoreParenImpCasts();
        ::std::string summary{"condition type: "};
        summary.append(normalized->getType().getAsString());
        return summary;
    }

    static auto has_template_context(::clang::FunctionDecl const* fd) -> bool {
        if (fd == nullptr) {
            return false;
        }
        auto const template_specialization_kind = fd->getTemplateSpecializationKind();
        return fd->isTemplateInstantiation() || fd->getTemplateSpecializationInfo() != nullptr ||
               fd->getTemplateSpecializationArgs() != nullptr ||
               template_specialization_kind != ::clang::TSK_Undeclared;
    }

    static auto build_stub_key(::clang::FunctionDecl const* fd) -> ::std::string {
        if (fd == nullptr) {
            return {};
        }
        ::std::string key{};
        key.reserve(64);
        key.append(fd->getQualifiedNameAsString());
        key.push_back('(');
        for (unsigned i = 0; i < fd->getNumParams(); ++i) {
            if (i != 0) {
                key.push_back(',');
            }
            auto const* param = fd->getParamDecl(i);
            if (param == nullptr) {
                key.append("object");
                continue;
            }
            key.append(map_csharp_type(param->getType()));
        }
        key.push_back(')');
        key.append("->");
        key.append(map_csharp_type(fd->getReturnType()));
        return key;
    }

    void append_control_flow_hint(::llvm::StringRef keyword, ::clang::Expr const* condition_expr) {
        if (current_function_key_.empty()) {
            return;
        }

        ::std::string key{};
        key.reserve(128);
        key.append(current_function_key_);
        key.push_back('|');
        key.append(keyword.data(), keyword.size());
        key.push_back(':');
        key.append(summarize_condition(condition_expr));
        if (!emitted_control_flow_keys_.insert(key).second) {
            return;
        }

        auto& snippets = function_body_snippets_[current_function_key_];
        snippets.emplace_back(::std::string{"        // from C++ "} + keyword.str() + ": " + summarize_condition(condition_expr));
    }

    static auto extract_optimize_template_arg(::clang::FunctionDecl const* fd) -> ::std::optional<bool> {
        if (fd == nullptr) {
            return ::std::nullopt;
        }
        auto const* targs = fd->getTemplateSpecializationArgs();
        if (targs == nullptr || targs->size() < 2) {
            return ::std::nullopt;
        }
        auto const& arg = targs->get(1);
        if (arg.getKind() != ::clang::TemplateArgument::Integral) {
            return ::std::nullopt;
        }
        return arg.getAsIntegral().getBoolValue();
    }

    auto append_target_api_body(::clang::FunctionDecl const* fd) -> bool {
        if (fd == nullptr) {
            return false;
        }
        auto const name = ::llvm::StringRef{fd->getName()};
        auto const optimize = extract_optimize_template_arg(fd);
        csharp_code_ += "        // TODO: translate parse_pltxt<...>(arg0)\n";
        csharp_code_ += "        var ast = new Ast();\n";
        if (optimize.value_or(false)) {
            csharp_code_ += "        Pltxt2Internal.OptimizeAst(ast);\n";
        }

        if (name == "pltxt2advanced_html") {
            csharp_code_ += "        return Pltxt2Internal.PlwebTextBackend(ast, \"localhost:5173\", \"$PROJECT\", \"$VISITOR\", \"$AUTHOR\", \"$CO_AUTHORS\");\n";
            return true;
        }
        if (name == "pltxt2fixedadv_html") {
            csharp_code_ += "        return Pltxt2Internal.PlwebTextBackend(ast, arg1, arg2, arg3, arg4, arg5);\n";
            return true;
        }
        if (name == "pltxt2plunity_introduction") {
            csharp_code_ += "        return Pltxt2Internal.PlunityTextBackend(ast, arg1, arg2, arg3, arg4);\n";
            return true;
        }
        if (name == "pltxt2common_html") {
            csharp_code_ += "        return Pltxt2Internal.PlwebTitleBackend(ast);\n";
            return true;
        }
        return false;
    }

    void append_function_stub(::clang::FunctionDecl const* fd) {
        if (fd == nullptr || !fd->getIdentifier()) {
            return;
        }

        auto const name = ::llvm::StringRef{fd->getName()};
        if (!is_target(name)) {
            return;
        }
        if (!has_template_context(fd)) {
            return;
        }

        auto const stub_key = build_stub_key(fd);
        if (!emitted_stub_keys_.insert(stub_key).second) {
            return;
        }

        auto const return_type = map_csharp_type(fd->getReturnType());
        csharp_code_ += "    public static ";
        csharp_code_ += return_type;
        csharp_code_ += " ";
        csharp_code_ += to_pascal_case(name);
        csharp_code_ += "(";

        bool need_comma = false;
        for (unsigned i = 0; i < fd->getNumParams(); ++i) {
            auto const* param = fd->getParamDecl(i);
            if (param == nullptr) {
                continue;
            }
            if (need_comma) {
                csharp_code_ += ", ";
            }
            csharp_code_ += map_csharp_type(param->getType());
            csharp_code_ += " arg";
            csharp_code_ += ::std::to_string(i);
            need_comma = true;
        }

        csharp_code_ += ")\n";
        csharp_code_ += "    {\n";
        bool has_explicit_return{};
        if (append_target_api_body(fd)) {
            // Body assembled from API semantics + template args.
            has_explicit_return = true;
        }
        else if (auto it = function_body_snippets_.find(stub_key); it != function_body_snippets_.end() && !it->second.empty()) {
            for (auto const& line : it->second) {
                csharp_code_ += line;
                csharp_code_ += "\n";
            }
        }
        else {
            csharp_code_ += "        // TODO: translate C++ body.\n";
        }
        if (return_type != "void" && !has_explicit_return) {
            csharp_code_ += "        return default!;\n";
        }
        csharp_code_ += "    }\n\n";
    }
};
