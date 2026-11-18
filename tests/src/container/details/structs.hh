#pragma once

#include <cstddef>
#include <iterator>

// Support types shared by several container case headers. Those headers are all
// included into one doctest TU, so a helper used by more than one of them must be
// defined exactly once, here.

namespace pltxt2htm_test {

// Nothrow default construction, move and destruction; potentially throwing copy.
struct ThrowingCopy {
    ThrowingCopy() noexcept = default;

    ThrowingCopy(ThrowingCopy const&) noexcept(false) {
    }

    ThrowingCopy(ThrowingCopy&&) noexcept = default;
    auto operator=(this ThrowingCopy&, ThrowingCopy const&) noexcept -> ThrowingCopy& = default;
    auto operator=(this ThrowingCopy&, ThrowingCopy&&) noexcept -> ThrowingCopy& = default;
    ~ThrowingCopy() noexcept = default;
};

// Potentially throwing equality comparison. The comparison is only ever used in
// unevaluated contexts, so it is declared but never defined.
struct ThrowingEquality {
    friend auto operator==(ThrowingEquality const&, ThrowingEquality const&) -> bool;
};

// Ranges whose begin() or end() may throw; used to check that range wrappers
// include both range-access calls in their noexcept specification.
struct ThrowingBeginRange {
    int const* first{};
    int const* last{};
    bool throw_on_begin{};

    [[nodiscard]]
    auto begin() const noexcept(false) -> int const* {
        if (this->throw_on_begin) {
            throw 1;
        }
        return this->first;
    }

    [[nodiscard]]
    auto end() const noexcept -> int const* {
        return this->last;
    }
};

struct ThrowingEndRange {
    int const* first{};
    int const* last{};
    bool throw_on_end{};

    [[nodiscard]]
    auto begin() const noexcept -> int const* {
        return this->first;
    }

    [[nodiscard]]
    auto end() const noexcept(false) -> int const* {
        if (this->throw_on_end) {
            throw 1;
        }
        return this->last;
    }
};

// Nothrow traversal except for a potentially throwing iterator copy; used to
// check that range constraints include nothrow iterator copies.
class ThrowingIteratorCopyRange {
public:
    class Iterator {
        int const* current{};

    public:
        using value_type = int;
        using difference_type = ::std::ptrdiff_t;
        using iterator_concept = ::std::input_iterator_tag;

        constexpr Iterator() noexcept = default;

        constexpr explicit Iterator(int const* current_) noexcept
            : current{current_} {
        }

        constexpr Iterator(Iterator const& other) noexcept(false)
            : current{other.current} {
        }

        constexpr Iterator(Iterator&&) noexcept = default;

        constexpr auto operator=(this Iterator& self, Iterator const& other) noexcept -> Iterator& {
            self.current = other.current;
            return self;
        }

        constexpr auto operator=(this Iterator& self, Iterator&& other) noexcept -> Iterator& {
            self.current = other.current;
            return self;
        }

        [[nodiscard]]
        constexpr auto operator*(this Iterator const& self) noexcept -> int const& {
            return *self.current;
        }

        constexpr auto operator++(this Iterator& self) noexcept -> Iterator& {
            ++self.current;
            return self;
        }

        constexpr void operator++(this Iterator& self, int) noexcept {
            ++self.current;
        }

        [[nodiscard]]
        friend constexpr auto operator==(Iterator const&, Iterator const&) noexcept -> bool = default;
    };

    constexpr ThrowingIteratorCopyRange(int const* first_, int const* last_) noexcept
        : first{first_},
          last{last_} {
    }

    [[nodiscard]]
    constexpr auto begin(this ThrowingIteratorCopyRange const& self) noexcept -> Iterator {
        return Iterator{self.first};
    }

    [[nodiscard]]
    constexpr auto end(this ThrowingIteratorCopyRange const& self) noexcept -> Iterator {
        return Iterator{self.last};
    }

private:
    int const* first{};
    int const* last{};
};

} // namespace pltxt2htm_test
