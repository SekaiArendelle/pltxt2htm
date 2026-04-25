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
#include <string>

#include <pltxt2htm/contracts.hh>



class ApiInstantiationVisitor : public ::clang::RecursiveASTVisitor<ApiInstantiationVisitor> {
    ::std::string csharp_code_{};

public:
    ApiInstantiationVisitor() = default;

    [[nodiscard]]
    constexpr auto csharp_code() const noexcept -> ::std::string const& {
        return csharp_code_;
    }

    auto VisitFunctionDecl(::clang::FunctionDecl* fd) -> bool {
        append_function_stub(fd);

        auto const name_storage = fd->getNameAsString();
        auto const name = ::llvm::StringRef{name_storage};
        if (!is_target(name)) {
            return true;
        }

        auto* tsi = fd->getTemplateSpecializationInfo();
        if (tsi == nullptr || tsi->TemplateArguments == nullptr) {
            return true;
        }
        return true;
    }

    auto VisitVarDecl(::clang::VarDecl* vd) -> bool {
        append_var_stub(vd);
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

        csharp_code_ += "    public static ";
        csharp_code_ += map_csharp_type(fd->getReturnType().getAsString()).str();
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
            csharp_code_ += map_csharp_type(param->getType().getAsString()).str();
            csharp_code_ += " arg";
            csharp_code_ += ::std::to_string(i);
            need_comma = true;
        }

        csharp_code_ += ")\n";
        csharp_code_ += "    {\n";
        csharp_code_ += "        throw new NotImplementedException(\"Generated from template instantiation AST.\");\n";
        csharp_code_ += "    }\n\n";
    }

    void append_var_stub(::clang::VarDecl const* vd) {
        if (vd == nullptr || !vd->getIdentifier()) {
            return;
        }

        auto const name = ::llvm::StringRef{vd->getName()};
        if (name != "selected_contract" && name != "advanced" && name != "plunity" && name != "common") {
            return;
        }

        csharp_code_ += "    private static ";
        csharp_code_ += map_csharp_type(vd->getType().getAsString()).str();
        csharp_code_ += " ";
        csharp_code_ += to_pascal_case(name);
        csharp_code_ += ";\n";
    }
};

