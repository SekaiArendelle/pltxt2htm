/**
 * @file call_stack.hh
 * @brief Explicit stack of frames used to replace recursive traversal.
 */

#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

#include "../container/deque.hh"
#include "../contracts.hh"
#include "push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Manually managed call stack used by iterative parsers and AST traversals.
 * @tparam Frame Nothrow-movable activation-record type.
 */
template<typename Frame>
    requires ::std::is_nothrow_move_constructible_v<Frame>
class CallStack {
public:
    using frame_type = Frame;
    using size_type = typename ::pltxt2htm::container::Deque<frame_type>::size_type;
    using reference = frame_type&;
    using const_reference = frame_type const&;

private:
    ::pltxt2htm::container::Deque<frame_type> frames{};

public:
    constexpr CallStack() noexcept = default;

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto current_frame(this CallStack& self) noexcept -> reference {
        pltxt2htm_assert(self.empty() == false, u8"Accessing current frame but CallStack is empty");
        return self.frames.back_unchecked();
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto current_frame(this CallStack const& self) noexcept -> const_reference {
        pltxt2htm_assert(self.empty() == false, u8"Accessing current frame but CallStack is empty");
        return self.frames.back_unchecked();
    }

    [[nodiscard]]
    constexpr auto empty(this CallStack const& self) noexcept -> bool {
        return self.frames.is_empty();
    }

    [[nodiscard]]
    constexpr auto frame_count(this CallStack const& self) noexcept -> size_type {
        return self.frames.size();
    }

    [[nodiscard]]
    constexpr auto is_root(this CallStack const& self) noexcept -> bool {
        return self.frame_count() == 1;
    }

    [[nodiscard]]
    constexpr auto has_parent(this CallStack const& self) noexcept -> bool {
        return self.frame_count() > 1;
    }

    constexpr void push_frame(this CallStack& self, frame_type&& frame) noexcept {
        self.frames.push_back(::std::move(frame));
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void discard_current_frame(this CallStack& self) noexcept {
        pltxt2htm_assert(self.empty() == false, u8"Popping current frame but CallStack is empty");
        self.frames.pop_back_unchecked();
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto pop_frame(this CallStack& self) noexcept -> frame_type {
        pltxt2htm_assert(self.empty() == false, u8"Popping current frame but CallStack is empty");
        frame_type frame{::std::move(self.frames.back_unchecked())};
        self.frames.pop_back_unchecked();
        return frame;
    }

    template<typename Predicate>
        requires requires(Predicate& predicate, const_reference value) {
            { predicate(value) } noexcept -> ::std::convertible_to<bool>;
        }
    [[nodiscard]]
    constexpr auto contains_frame_if(this CallStack const& self, Predicate&& predicate) noexcept -> bool {
        for (auto const& frame : self.frames) {
            if (predicate(frame)) {
                return true;
            }
        }
        return false;
    }
};

} // namespace pltxt2htm::details

#include "pop_macro.hh"
