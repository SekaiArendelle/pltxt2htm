#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
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
#include <set>
#include <string>

#include <pltxt2htm/contracts.hh>



class ApiInstantiationVisitor : public ::clang::RecursiveASTVisitor<ApiInstantiationVisitor> {
    ::std::string csharp_code_{};
    ::std::set<::std::string> emitted_stub_keys_{};

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
        append_function_stub(fd);
        return true;
    }

    auto VisitFunctionTemplateDecl(::clang::FunctionTemplateDecl* ftd) -> bool {
        if (ftd == nullptr) {
            return true;
        }
        auto* templated_decl = ftd->getTemplatedDecl();
        if (templated_decl == nullptr) {
            return true;
        }
        append_function_stub(templated_decl);
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

    void append_function_stub(::clang::FunctionDecl const* fd) {
        if (fd == nullptr || !fd->getIdentifier()) {
            return;
        }

        auto const name = ::llvm::StringRef{fd->getName()};
        if (!is_target(name)) {
            return;
        }
        auto const template_specialization_kind = fd->getTemplateSpecializationKind();
        auto const has_template_context = fd->isTemplateInstantiation() || fd->getTemplateSpecializationInfo() != nullptr ||
                                          fd->getPrimaryTemplate() != nullptr ||
                                          fd->getDescribedFunctionTemplate() != nullptr ||
                                          fd->getTemplateSpecializationArgs() != nullptr ||
                                          template_specialization_kind != ::clang::TSK_Undeclared;
        if (!has_template_context) {
            return;
        }

        ::std::string stub_key{};
        stub_key.reserve(64);
        stub_key.append(fd->getQualifiedNameAsString());
        stub_key.push_back('(');
        for (unsigned i = 0; i < fd->getNumParams(); ++i) {
            if (i != 0) {
                stub_key.push_back(',');
            }
            auto const* param = fd->getParamDecl(i);
            if (param == nullptr) {
                stub_key.append("object");
                continue;
            }
            stub_key.append(map_csharp_type(param->getType()));
        }
        stub_key.push_back(')');
        stub_key.append("->");
        stub_key.append(map_csharp_type(fd->getReturnType()));
        if (!emitted_stub_keys_.insert(stub_key).second) {
            return;
        }

        csharp_code_ += "    public static ";
        csharp_code_ += map_csharp_type(fd->getReturnType());
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
        csharp_code_ += "        throw new NotImplementedException(\"Generated from template instantiation AST.\");\n";
        csharp_code_ += "    }\n\n";
    }
};
