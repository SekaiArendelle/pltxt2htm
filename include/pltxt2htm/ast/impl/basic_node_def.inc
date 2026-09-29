#pragma once

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Group<ndebug>::Group(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Group<ndebug>::Group(::pltxt2htm::Group<ndebug> const&) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Group<ndebug>::Group(::pltxt2htm::Group<ndebug>&& other) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Group<ndebug>::operator=(this Group<ndebug>& self,
                                                     ::pltxt2htm::Group<ndebug>&& other) noexcept
    -> ::pltxt2htm::Group<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Group<ndebug>::operator==(this Group<ndebug> const&,
                                                      ::pltxt2htm::Group<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::CodeFence(::pltxt2htm::HighlightedCodeAst<ndebug>&& ast_value) noexcept
    : kind(::pltxt2htm::CodeFenceKind::highlighted),
      highlighted_ast(::std::move(ast_value)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::CodeFence(::pltxt2htm::RenderedCodeAst<ndebug>&& ast_value) noexcept
    : kind(::pltxt2htm::CodeFenceKind::rendered),
      rendered_ast(::std::move(ast_value)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::CodeFence(::pltxt2htm::CodeFence<ndebug> const& other) noexcept
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
    ::pltxt2htm::details::unreachable<ndebug>();
}
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::CodeFence(::pltxt2htm::CodeFence<ndebug>&& other) noexcept
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
    ::pltxt2htm::details::unreachable<ndebug>();
}
template<::pltxt2htm::Contracts ndebug>
constexpr void ::pltxt2htm::CodeFence<ndebug>::destroy_active(this CodeFence& self) noexcept {
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
    ::pltxt2htm::details::unreachable<ndebug>();
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::~CodeFence() noexcept {
    destroy_active();
}

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::CodeFence<ndebug>::operator=(this CodeFence<ndebug>& self,
                                                         ::pltxt2htm::CodeFence<ndebug> const& other) noexcept
    -> ::pltxt2htm::CodeFence<ndebug>& {
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
    ::pltxt2htm::details::unreachable<ndebug>();
}

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::CodeFence<ndebug>::operator=(this CodeFence<ndebug>& self,
                                                         ::pltxt2htm::CodeFence<ndebug>&& other) noexcept
    -> ::pltxt2htm::CodeFence<ndebug>& {
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
    ::pltxt2htm::details::unreachable<ndebug>();
}

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::CodeFence<ndebug>::operator==(this CodeFence<ndebug> const& self,
                                                          ::pltxt2htm::CodeFence<ndebug> const& other) noexcept
    -> bool {
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
    ::pltxt2htm::details::unreachable<ndebug>();
}
