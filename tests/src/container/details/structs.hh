#pragma once

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

} // namespace pltxt2htm_test
