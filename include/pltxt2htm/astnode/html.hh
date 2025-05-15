#pragma once

#include "basic.hh"

namespace pltxt2htm {

/**
 * @brief Represents a <br> tag.
 */
class Br : public ::pltxt2htm::PlTxtNode {
public:
    constexpr Br() noexcept
        : PlTxtNode{NodeType::br} {
    }

    constexpr Br(Br const& other) noexcept = default;
    constexpr Br(Br&& other) noexcept = default;
    constexpr Br& operator=(Br const& other) noexcept = default;
    constexpr Br& operator=(Br&& other) noexcept = default;
};

} // namespace pltxt2htm
