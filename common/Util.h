#pragma once

#include <algorithm>
#include <cstdint>

namespace util {
    template <typename Container>
    void removeAndSwapBack(Container& container, size_t index)
    {
        container[index] = std::move(container.back());
        container.pop_back();
    }
}

#define MEM_OFFSET(memory, offset) ((void*)((uint8_t*)(memory) + offset))
