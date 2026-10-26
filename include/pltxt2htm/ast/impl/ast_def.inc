#pragma once

// Downstream-only definitions; they copy PlTxtNode values, which internal
// builds disable (see ast_decl.hh).
#if !defined(PLTXT2HTM_INTERNAL_USE)
constexpr ::pltxt2htm::Ast::Ast(::std::initializer_list<::pltxt2htm::Ast::value_type> values) noexcept
    : storage{values} {
}

constexpr ::pltxt2htm::Ast::Ast(::pltxt2htm::Ast const&) noexcept = default;

constexpr auto ::pltxt2htm::Ast::operator=(this ::pltxt2htm::Ast & self, ::pltxt2htm::Ast const& other) noexcept
    -> ::pltxt2htm::Ast& {
    self.storage = other.storage;
    return self;
}
#endif
constexpr ::pltxt2htm::Ast::Ast(::pltxt2htm::Ast&&) noexcept = default;

constexpr auto ::pltxt2htm::Ast::operator=(this ::pltxt2htm::Ast & self, ::pltxt2htm::Ast && other) noexcept
    -> ::pltxt2htm::Ast& {
    self.storage = ::std::move(other.storage);
    return self;
}

constexpr ::pltxt2htm::Ast::~Ast() noexcept = default;

constexpr auto ::pltxt2htm::Ast::data(this ::pltxt2htm::Ast& self) noexcept -> ::pltxt2htm::Ast::pointer {
    return self.storage.data();
}

constexpr auto ::pltxt2htm::Ast::data(this ::pltxt2htm::Ast const& self) noexcept -> ::pltxt2htm::Ast::const_pointer {
    return self.storage.data();
}

constexpr auto ::pltxt2htm::Ast::is_empty(this ::pltxt2htm::Ast const& self) noexcept -> bool {
    return self.storage.is_empty();
}
#if !defined(PLTXT2HTM_INTERNAL_USE)
constexpr auto ::pltxt2htm::Ast::empty(this ::pltxt2htm::Ast const& self) noexcept -> bool {
    return self.storage.is_empty();
}
#endif
constexpr auto ::pltxt2htm::Ast::size(this ::pltxt2htm::Ast const& self) noexcept -> ::pltxt2htm::Ast::size_type {
    return self.storage.size();
}

constexpr auto ::pltxt2htm::Ast::capacity(this ::pltxt2htm::Ast const& self) noexcept -> ::pltxt2htm::Ast::size_type {
    return self.storage.capacity();
}

constexpr auto ::pltxt2htm::Ast::max_size() noexcept -> ::pltxt2htm::Ast::size_type {
    return ::pltxt2htm::Ast::storage_type::max_size();
}

constexpr auto ::pltxt2htm::Ast::begin(this ::pltxt2htm::Ast& self) noexcept -> ::pltxt2htm::Ast::iterator {
    return self.storage.begin();
}

constexpr auto ::pltxt2htm::Ast::begin(this ::pltxt2htm::Ast const& self) noexcept -> ::pltxt2htm::Ast::const_iterator {
    return self.storage.begin();
}

constexpr auto ::pltxt2htm::Ast::end(this ::pltxt2htm::Ast& self) noexcept -> ::pltxt2htm::Ast::iterator {
    return self.storage.end();
}

constexpr auto ::pltxt2htm::Ast::end(this ::pltxt2htm::Ast const& self) noexcept -> ::pltxt2htm::Ast::const_iterator {
    return self.storage.end();
}

constexpr auto ::pltxt2htm::Ast::cbegin(this ::pltxt2htm::Ast const& self) noexcept
    -> ::pltxt2htm::Ast::const_iterator {
    return self.storage.cbegin();
}

constexpr auto ::pltxt2htm::Ast::cend(this ::pltxt2htm::Ast const& self) noexcept -> ::pltxt2htm::Ast::const_iterator {
    return self.storage.cend();
}

constexpr auto ::pltxt2htm::Ast::rbegin(this ::pltxt2htm::Ast& self) noexcept -> ::pltxt2htm::Ast::reverse_iterator {
    return self.storage.rbegin();
}

constexpr auto ::pltxt2htm::Ast::rbegin(this ::pltxt2htm::Ast const& self) noexcept
    -> ::pltxt2htm::Ast::const_reverse_iterator {
    return self.storage.rbegin();
}

constexpr auto ::pltxt2htm::Ast::rend(this ::pltxt2htm::Ast& self) noexcept -> ::pltxt2htm::Ast::reverse_iterator {
    return self.storage.rend();
}

constexpr auto ::pltxt2htm::Ast::rend(this ::pltxt2htm::Ast const& self) noexcept
    -> ::pltxt2htm::Ast::const_reverse_iterator {
    return self.storage.rend();
}

constexpr auto ::pltxt2htm::Ast::front(this ::pltxt2htm::Ast& self) noexcept -> ::pltxt2htm::Ast::reference {
    return self.storage.front();
}

constexpr auto ::pltxt2htm::Ast::front(this ::pltxt2htm::Ast const& self) noexcept
    -> ::pltxt2htm::Ast::const_reference {
    return self.storage.front();
}

constexpr auto ::pltxt2htm::Ast::index(this ::pltxt2htm::Ast& self, ::pltxt2htm::Ast::size_type position) noexcept
    -> ::pltxt2htm::Ast::reference {
    return self.storage.index(position);
}

constexpr auto ::pltxt2htm::Ast::index(this ::pltxt2htm::Ast const& self, ::pltxt2htm::Ast::size_type position) noexcept
    -> ::pltxt2htm::Ast::const_reference {
    return self.storage.index(position);
}

constexpr void ::pltxt2htm::Ast::reserve(this ::pltxt2htm::Ast& self,
                                         ::pltxt2htm::Ast::size_type requested_capacity) noexcept {
    self.storage.reserve(requested_capacity);
}

template<typename... Args>
constexpr auto ::pltxt2htm::Ast::emplace_back(this ::pltxt2htm::Ast& self, Args&&... args) noexcept
    -> ::pltxt2htm::Ast::reference {
    return self.storage.emplace_back(::std::forward<Args>(args)...);
}

#if !defined(PLTXT2HTM_INTERNAL_USE)
constexpr void ::pltxt2htm::Ast::push_back(this ::pltxt2htm::Ast& self,
                                           ::pltxt2htm::Ast::const_reference value) noexcept {
    self.storage.push_back(value);
}
#endif
constexpr void ::pltxt2htm::Ast::push_back(this ::pltxt2htm::Ast& self, ::pltxt2htm::Ast::value_type&& value) noexcept {
    self.storage.push_back(::std::move(value));
}

constexpr void ::pltxt2htm::Ast::pop_back(this ::pltxt2htm::Ast& self) noexcept {
    self.storage.pop_back();
}

constexpr void ::pltxt2htm::Ast::clear(this ::pltxt2htm::Ast& self) noexcept {
    self.storage.clear();
}

template<::std::ranges::input_range R>
constexpr void ::pltxt2htm::Ast::append_range(this ::pltxt2htm::Ast& self, R&& range) noexcept {
    self.storage.append_range(::std::forward<R>(range));
}

constexpr auto ::pltxt2htm::Ast::erase(this ::pltxt2htm::Ast& self, ::pltxt2htm::Ast::const_iterator position) noexcept
    -> ::pltxt2htm::Ast::iterator {
    return self.storage.erase(position);
}

constexpr auto ::pltxt2htm::Ast::erase(this ::pltxt2htm::Ast& self, ::pltxt2htm::Ast::const_iterator first,
                                       ::pltxt2htm::Ast::const_iterator last) noexcept -> ::pltxt2htm::Ast::iterator {
    return self.storage.erase(first, last);
}

constexpr void ::pltxt2htm::Ast::swap(this ::pltxt2htm::Ast& self, ::pltxt2htm::Ast& other) noexcept {
    self.storage.swap(other.storage);
}

constexpr auto ::pltxt2htm::Ast::operator==(this ::pltxt2htm::Ast const& self, ::pltxt2htm::Ast const& other) noexcept
    -> bool {
    return self.storage == other.storage;
}

constexpr void ::pltxt2htm::Ast::append_code_point(this ::pltxt2htm::Ast& self, char32_t code_point) noexcept {
    switch (code_point) {
    case U'\n': {
        self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LineBreak>());
        return;
    }
    case U' ':
    case char32_t{0xA0}: {
        self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Space>());
        return;
    }
    case U'&': {
        self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Ampersand>());
        return;
    }
    case U'\'': {
        self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::SingleQuote>());
        return;
    }
    case U'"': {
        self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::DoubleQuote>());
        return;
    }
    case U'<': {
        self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
        return;
    }
    case U'>': {
        self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::GreaterThan>());
        return;
    }
    case U'\t': {
        self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Tab>());
        return;
    }
    default: {
        break;
    }
    }

    if (::pltxt2htm::details::is_ascii_control_code_point(code_point)) {
        self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::InvalidUtf8>());
        return;
    }

    auto const encoded = ::pltxt2htm::details::encode_utf8_code_point(code_point);
    if (encoded.size == 0) {
        self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::InvalidUtf8>());
        return;
    }
    for (::std::size_t index{}; index < encoded.size; ++index) {
        self.append_text(encoded.code_units[index]);
    }
}

constexpr void ::pltxt2htm::Ast::append_text(this ::pltxt2htm::Ast& self, char8_t code_unit) noexcept {
    if (self.is_empty() == false) {
        auto&& last_node = self.index(self.size() - 1);
        if (last_node.get_node_kind() == ::pltxt2htm::NodeKind::text && last_node.as_text().try_push_back(code_unit)) {
            return;
        }
    }
    self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(code_unit));
}

constexpr void ::pltxt2htm::Ast::append_text(this ::pltxt2htm::Ast& self,
                                             ::pltxt2htm::container::U8StringView text) noexcept {
    auto current = text.begin();
    auto const end = text.end();
    if (current == end) {
        return;
    }

    constexpr auto text_capacity = ::pltxt2htm::Text::capacity();
    if (self.is_empty() == false) {
        auto&& last_node = self.index(self.size() - 1);
        if (last_node.get_node_kind() == ::pltxt2htm::NodeKind::text) {
            auto&& last_text = last_node.as_text();
            auto const available = text_capacity - last_text.size();
            auto const remaining = static_cast<::std::size_t>(end - current);
            auto const append_size = remaining < available ? remaining : available;
            last_text.append(current, current + append_size);
            current += append_size;
            if (current == end) {
                return;
            }
        }
    }

    auto const remaining = static_cast<::std::size_t>(end - current);
    auto const additional_nodes =
        remaining / text_capacity + static_cast<::std::size_t>(remaining % text_capacity != 0);
    pltxt2htm_assert(additional_nodes <= self.max_size() - self.size(), u8"AST size exceeds max_size");
    self.reserve(self.size() + additional_nodes);

    while (current != end) {
        auto const remaining_size = static_cast<::std::size_t>(end - current);
        auto const node_size = remaining_size < text_capacity ? remaining_size : text_capacity;
        self.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(current, current + node_size));
        current += node_size;
    }
}
