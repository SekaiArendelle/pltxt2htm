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

} // namespace pltxt2htm_test
