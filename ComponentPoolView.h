#pragma once

#include "Entity.h"
#include "common/Types.h"
#include <array>
#include <iostream>
#include <vector>

template <typename T>
class ComponentPoolView {
public:
    ComponentPoolView(T* memory, T* end, std::vector<Index>& indexToId)
        : components(memory)
        , count(end - memory)
        , indexToId(&indexToId)
    {}

    Index size() const { return count; }

    T& operator[](Index i) { return components[i]; }
    const T& operator[](Index i) const { return components[i]; }

    Index getEntity(Index i) const { return (*indexToId)[i]; }

private:
    T* components;
    Index count;
    std::vector<Index>* indexToId;
};
