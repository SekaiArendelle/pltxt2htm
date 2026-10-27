#pragma once

constexpr ::pltxt2htm::Group::Group(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::Group::Group(::pltxt2htm::Group&& other) noexcept = default;

constexpr auto ::pltxt2htm::Group::operator=(this Group & self, ::pltxt2htm::Group && other) noexcept
    -> ::pltxt2htm::Group& = default;

constexpr auto ::pltxt2htm::Group::operator==(this Group const&, ::pltxt2htm::Group const&) noexcept -> bool = default;

constexpr ::pltxt2htm::CodeFence::CodeFence(::pltxt2htm::HighlightedCodeAst&& ast_value) noexcept
    : kind(::pltxt2htm::CodeFenceKind::highlighted),
      highlighted_ast(::std::move(ast_value)) {
}

constexpr ::pltxt2htm::CodeFence::CodeFence(::pltxt2htm::RenderedCodeAst&& ast_value) noexcept
    : kind(::pltxt2htm::CodeFenceKind::rendered),
      rendered_ast(::std::move(ast_value)) {
}

#if !defined(PLTXT2HTM_INTERNAL_USE)
constexpr ::pltxt2htm::CodeFence::CodeFence(::pltxt2htm::CodeFence const& other) noexcept
    : kind(other.kind) {
    switch (kind) /* -Werror=switch */ {
    case ::pltxt2htm::CodeFenceKind::highlighted: {
        ::std::construct_at(::std::addressof(highlighted_ast), other.highlighted_ast);
        return;
    }
    case ::pltxt2htm::CodeFenceKind::rendered: {
        ::std::construct_at(::std::addressof(rendered_ast), other.rendered_ast);
        return;
    }
    }
    ::pltxt2htm::details::unreachable();
}
#endif
constexpr ::pltxt2htm::CodeFence::CodeFence(::pltxt2htm::CodeFence&& other) noexcept
    : kind(other.kind) {
    switch (kind) /* -Werror=switch */ {
    case ::pltxt2htm::CodeFenceKind::highlighted: {
        ::std::construct_at(::std::addressof(highlighted_ast), ::std::move(other.highlighted_ast));
        return;
    }
    case ::pltxt2htm::CodeFenceKind::rendered: {
        ::std::construct_at(::std::addressof(rendered_ast), ::std::move(other.rendered_ast));
        return;
    }
    }
    ::pltxt2htm::details::unreachable();
}

constexpr void ::pltxt2htm::CodeFence::destroy_active(this CodeFence& self) noexcept {
    switch (self.kind) /* -Werror=switch */ {
    case ::pltxt2htm::CodeFenceKind::highlighted: {
        ::std::destroy_at(::std::addressof(self.highlighted_ast));
        return;
    }
    case ::pltxt2htm::CodeFenceKind::rendered: {
        ::std::destroy_at(::std::addressof(self.rendered_ast));
        return;
    }
    }
    ::pltxt2htm::details::unreachable();
}

constexpr ::pltxt2htm::CodeFence::~CodeFence() noexcept {
    destroy_active();
}

#if !defined(PLTXT2HTM_INTERNAL_USE)
constexpr auto ::pltxt2htm::CodeFence::operator=(this CodeFence & self, ::pltxt2htm::CodeFence const& other) noexcept
    -> ::pltxt2htm::CodeFence& {
    if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
        return self;
    }
    self.destroy_active();
    self.kind = other.kind;
    switch (self.kind) /* -Werror=switch */ {
    case ::pltxt2htm::CodeFenceKind::highlighted: {
        ::std::construct_at(::std::addressof(self.highlighted_ast), other.highlighted_ast);
        return self;
    }
    case ::pltxt2htm::CodeFenceKind::rendered: {
        ::std::construct_at(::std::addressof(self.rendered_ast), other.rendered_ast);
        return self;
    }
    }
    ::pltxt2htm::details::unreachable();
}

#endif
constexpr auto ::pltxt2htm::CodeFence::operator=(this CodeFence & self, ::pltxt2htm::CodeFence && other) noexcept
    -> ::pltxt2htm::CodeFence& {
    if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
        return self;
    }
    self.destroy_active();
    self.kind = other.kind;
    switch (self.kind) /* -Werror=switch */ {
    case ::pltxt2htm::CodeFenceKind::highlighted: {
        ::std::construct_at(::std::addressof(self.highlighted_ast), ::std::move(other.highlighted_ast));
        return self;
    }
    case ::pltxt2htm::CodeFenceKind::rendered: {
        ::std::construct_at(::std::addressof(self.rendered_ast), ::std::move(other.rendered_ast));
        return self;
    }
    }
    ::pltxt2htm::details::unreachable();
}

constexpr auto ::pltxt2htm::CodeFence::operator==(this CodeFence const& self,
                                                  ::pltxt2htm::CodeFence const& other) noexcept -> bool {
    if (self.kind != other.kind) {
        return false;
    }
    switch (self.kind) /* -Werror=switch */ {
    case ::pltxt2htm::CodeFenceKind::highlighted: {
        return self.highlighted_ast == other.highlighted_ast;
    }
    case ::pltxt2htm::CodeFenceKind::rendered: {
        return self.rendered_ast == other.rendered_ast;
    }
    }
    ::pltxt2htm::details::unreachable();
}