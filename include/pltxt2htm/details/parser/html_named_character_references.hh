/**
 * @file html_named_character_references.hh
 * @brief Lookup semicolon-terminated HTML5 named character references.
 */

#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <ranges>

#include "../../container/array.hh"
#include "../../container/string_view.hh"
#include "../../contracts.hh"

namespace pltxt2htm::details {

struct HtmlNamedCharacterReference {
    char32_t first_code_point;
    char32_t second_code_point;
};

struct HtmlNamedCharacterReferenceTableEntry {
    ::pltxt2htm::container::U8StringView name;
    HtmlNamedCharacterReference reference;
};

class HtmlNamedCharacterReferenceTable {
    struct CompressedEntry {
        ::std::uint_least16_t name_offset;
        ::std::uint_least8_t name_size;
        HtmlNamedCharacterReference reference;
    };

#include "html_named_character_references.inc"

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    static constexpr auto compare(::pltxt2htm::container::U8StringView name, CompressedEntry const& candidate) noexcept
        -> ::std::strong_ordering {
        ::std::size_t const name_size{name.size()};
        ::std::size_t const candidate_size{candidate.name_size};
        ::std::size_t const common_size{name_size < candidate_size ? name_size : candidate_size};
        for (::std::size_t index{}; index < common_size; ++index) {
            auto const comparison{name.template index<ndebug>(index) <=> names[candidate.name_offset + index]};
            if (comparison != 0) {
                return comparison;
            }
        }
        return name_size <=> candidate_size;
    }

    class const_iterator {
        CompressedEntry const* current{};

        friend class HtmlNamedCharacterReferenceTable;

        constexpr explicit const_iterator(CompressedEntry const* current_) noexcept
            : current{current_} {
        }

    public:
        using value_type = HtmlNamedCharacterReferenceTableEntry;
        using difference_type = ::std::ptrdiff_t;
        using iterator_concept = ::std::forward_iterator_tag;

        constexpr const_iterator() noexcept = default;

        [[nodiscard]]
        constexpr auto operator*(this const_iterator const& self) noexcept -> value_type {
            auto const& entry{*self.current};
            return {.name = ::pltxt2htm::container::U8StringView{names + entry.name_offset, entry.name_size},
                    .reference = entry.reference};
        }

        constexpr auto& operator++(this const_iterator& self) noexcept {
            ++self.current;
            return self;
        }

        constexpr auto operator++(this const_iterator& self, int) noexcept -> const_iterator {
            auto const previous{self};
            ++self;
            return previous;
        }

        [[nodiscard]]
        constexpr bool operator==(this const_iterator const&, const_iterator const&) noexcept = default;
    };

public:
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    static constexpr auto try_find(::pltxt2htm::container::U8StringView name) noexcept -> HtmlNamedCharacterReference
        const* /* TODO Maybe Optional<HtmlNamedCharacterReference const&> is better here? */ {
        ::std::size_t first{};
        ::std::size_t last{compressed_entries.size()};
        while (first < last) {
            ::std::size_t const middle{first + (last - first) / 2};
            auto const& candidate{compressed_entries.template index<ndebug>(middle)};
            auto const comparison{compare<ndebug>(name, candidate)};
            if (comparison < 0) {
                last = middle;
            }
            else if (comparison > 0) {
                first = middle + 1;
            }
            else {
                return ::std::addressof(candidate.reference);
            }
        }
        return nullptr;
    }

    [[nodiscard]]
    static constexpr auto entries() noexcept {
        return ::std::ranges::subrange{const_iterator{compressed_entries.begin()},
                                       const_iterator{compressed_entries.end()}};
    }
};

} // namespace pltxt2htm::details
