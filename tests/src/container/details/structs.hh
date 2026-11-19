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
// check that traversal-only constraints do not require a nothrow iterator
// copy.
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

// Nothrow traversal except for a potentially throwing iterator move; used to
// check that constraints which move their boundaries require nothrow moves.
class ThrowingIteratorMoveRange {
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

        constexpr Iterator(Iterator const&) noexcept = default;

        constexpr Iterator(Iterator&& other) noexcept(false)
            : current{other.current} {
        }

        constexpr auto operator=(this Iterator& self, Iterator const& other) noexcept -> Iterator& {
            self.current = other.current;
            return self;
        }

        constexpr auto operator=(this Iterator& self, Iterator&& other) noexcept(false) -> Iterator& {
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

    constexpr ThrowingIteratorMoveRange(int const* first_, int const* last_) noexcept
        : first{first_},
          last{last_} {
    }

    [[nodiscard]]
    constexpr auto begin(this ThrowingIteratorMoveRange const& self) noexcept -> Iterator {
        return Iterator{self.first};
    }

    [[nodiscard]]
    constexpr auto end(this ThrowingIteratorMoveRange const& self) noexcept -> Iterator {
        return Iterator{self.last};
    }

private:
    int const* first{};
    int const* last{};
};

// Nothrow copy but potentially throwing move; paired with a raw pointer
// iterator to check that a constraint tests a distinct sentinel's move
// separately from the iterator's.
struct ThrowingMoveSentinel {
    int const* last{};

    constexpr ThrowingMoveSentinel() noexcept = default;

    constexpr explicit ThrowingMoveSentinel(int const* last_) noexcept
        : last{last_} {
    }

    constexpr ThrowingMoveSentinel(ThrowingMoveSentinel const&) noexcept = default;

    constexpr ThrowingMoveSentinel(ThrowingMoveSentinel&& other) noexcept(false)
        : last{other.last} {
    }

    constexpr auto operator=(this ThrowingMoveSentinel& self, ThrowingMoveSentinel const&) noexcept
        -> ThrowingMoveSentinel& = default;

    constexpr auto operator=(this ThrowingMoveSentinel& self, ThrowingMoveSentinel&& other) noexcept(false)
        -> ThrowingMoveSentinel& {
        self.last = other.last;
        return self;
    }

    [[nodiscard]]
    friend constexpr auto operator==(int const* iterator, ThrowingMoveSentinel const& sentinel) noexcept -> bool {
        return iterator == sentinel.last;
    }
};

// Move-only input iterator with nothrow traversal; used to check that a
// constraint which moves its boundaries really moves rather than copies.
class MoveOnlyIntIterator {
    int const* current{};

public:
    using value_type = int;
    using difference_type = ::std::ptrdiff_t;
    using iterator_concept = ::std::input_iterator_tag;

    constexpr MoveOnlyIntIterator() noexcept = default;

    constexpr explicit MoveOnlyIntIterator(int const* current_) noexcept
        : current{current_} {
    }

    constexpr MoveOnlyIntIterator(MoveOnlyIntIterator const&) = delete;
    constexpr MoveOnlyIntIterator(MoveOnlyIntIterator&&) noexcept = default;

    constexpr auto operator=(this MoveOnlyIntIterator&, MoveOnlyIntIterator const&) -> MoveOnlyIntIterator& = delete;
    constexpr auto operator=(this MoveOnlyIntIterator& self, MoveOnlyIntIterator&&) noexcept
        -> MoveOnlyIntIterator& = default;

    [[nodiscard]]
    constexpr auto operator*(this MoveOnlyIntIterator const& self) noexcept -> int const& {
        return *self.current;
    }

    constexpr auto operator++(this MoveOnlyIntIterator& self) noexcept -> MoveOnlyIntIterator& {
        ++self.current;
        return self;
    }

    constexpr void operator++(this MoveOnlyIntIterator& self, int) noexcept {
        ++self.current;
    }

    [[nodiscard]]
    friend constexpr auto operator==(MoveOnlyIntIterator const& iterator, int const* sentinel) noexcept -> bool {
        return iterator.current == sentinel;
    }
};

} // namespace pltxt2htm_test
