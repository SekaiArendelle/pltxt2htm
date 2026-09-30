/**
 * @file text_node_decl.hh
 * @brief Inline text AST node declaration.
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <utility>

#include "../../details/inplace_string.hh"
#include "html_node_decl.hh"

namespace pltxt2htm {

/**
 * @brief A leaf node containing a run of UTF-8 code units.
 * @details Uses the full storage footprint required by the largest other AST node.
 */
template<::pltxt2htm::Contracts ndebug>
class Text {
    static constexpr ::std::size_t storage_capacity{sizeof(::pltxt2htm::HtmlSpan<ndebug>) - 1};
    using Storage = ::pltxt2htm::details::U8InplaceString<storage_capacity, ndebug>;

    Storage storage;

public:
    using size_type = ::std::size_t;
    using iterator = char8_t*;
    using const_iterator = char8_t const*;

    constexpr explicit Text(char8_t character) noexcept
        : storage{character} {
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
        requires (::std::same_as<::std::iter_value_t<InputIterator>, char8_t> &&
                  ::std::constructible_from<char8_t, ::std::iter_reference_t<InputIterator>>)
    constexpr Text(InputIterator first,
                   Sentinel last) noexcept(noexcept(Storage{::std::move(first), ::std::move(last)}))
        : storage{::std::move(first), ::std::move(last)} {
    }

    constexpr Text(Text const&) = default;
    constexpr Text(Text&&) noexcept = default;
    constexpr auto operator=(this Text&, Text const&) -> Text& = default;
    constexpr auto operator=(this Text&, Text&&) noexcept -> Text& = default;
    constexpr ~Text() noexcept = default;

    [[nodiscard]]
    constexpr auto operator==(this Text const&, Text const&) noexcept -> bool = default;

    [[nodiscard]]
    static constexpr auto capacity() noexcept -> size_type {
        return Storage::capacity();
    }

    [[nodiscard]]
    constexpr auto size(this Text const& self) noexcept -> size_type {
        return self.storage.size();
    }

    [[nodiscard]]
    constexpr auto begin(this Text& self) noexcept -> iterator {
        return self.storage.begin();
    }

    [[nodiscard]]
    constexpr auto begin(this Text const& self) noexcept -> const_iterator {
        return self.storage.begin();
    }

    [[nodiscard]]
    constexpr auto end(this Text& self) noexcept -> iterator {
        return self.storage.end();
    }

    [[nodiscard]]
    constexpr auto end(this Text const& self) noexcept -> const_iterator {
        return self.storage.end();
    }

    [[nodiscard]]
    constexpr auto index(this Text& self, size_type position) noexcept -> char8_t& {
        return self.storage.index(position);
    }

    [[nodiscard]]
    constexpr auto index(this Text const& self, size_type position) noexcept -> char8_t const& {
        return self.storage.index(position);
    }

    [[nodiscard]]
    constexpr auto try_push_back(this Text& self, char8_t character) noexcept -> bool {
        return self.storage.try_push_back(character);
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
        requires (::std::same_as<::std::iter_value_t<InputIterator>, char8_t> &&
                  ::std::constructible_from<char8_t, ::std::iter_reference_t<InputIterator>>)
    constexpr void append(this Text& self, InputIterator first,
                          Sentinel last) noexcept(noexcept(self.storage.append(::std::move(first),
                                                                               ::std::move(last)))) {
        self.storage.append(::std::move(first), ::std::move(last));
    }
};

} // namespace pltxt2htm
