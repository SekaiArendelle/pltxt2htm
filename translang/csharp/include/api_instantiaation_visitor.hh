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

#include <pltxt2htm/contracts.hh>

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
    ApiInstantiationMap& out_;

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
};

