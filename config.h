#pragma once

#include <cstddef>
#include <limits>

namespace config {
    constexpr size_t MaxComponentsTypes = 100;
}

using Index = size_t;

inline constexpr static Index IdNone = std::numeric_limits<Index>::max();
