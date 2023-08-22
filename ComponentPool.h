#pragma once

#include "ComponentPoolView.h"
#include "common/Std.h"
#include "common/Types.h"
#include "common/Util.h"
#include "memory/Memory.h"

#include <cstring>
#include <iostream>
#include <limits>

class ComponentPool {
public:
    void* components;
    size_t componentTypeSize;
    Index max;
    Index count;
    std::vector<Index> idToIndex;
    std::vector<Index> indexToId;

public:
    ComponentPool(void* memory, size_t componentTypeSize, Index capacity)
        : components(memory)
        , componentTypeSize(componentTypeSize)
        , max(capacity)
        , count(0)
        , idToIndex(max, IdNone)
        , indexToId(max, IdNone)
    {}

    ComponentPool()
        : components(nullptr)
        , componentTypeSize(0)
        , max(0)
        , count(0)
    {}

    ~ComponentPool() {}

    template <typename Component, typename... Args>
    Component& add(Index id, Args&&... args)
    {
        Component* c = new (MEM_OFFSET(components, componentTypeSize * count))
            Component(std::forward<Args>(args)...);
        idToIndex[id] = count;
        indexToId[count] = id;
        ++count;
        return *c;
    }

    template <typename Component>
    Component& get(Index id) const
    {
        return *(Component*)MEM_OFFSET(components, componentTypeSize * idToIndex[id]);
    }

    template <typename Component>
    ComponentPoolView<Component> getView()
    {
        auto end = (Component*)MEM_OFFSET(components, componentTypeSize * count);
        return ComponentPoolView<Component>((Component*)components, end, indexToId);
    }

    void remove(Index id)
    {
        if (count == 1) {
            idToIndex[id] = IdNone;
        } else {
            Index movedIndex = count - 1;
            Index movedId = indexToId[movedIndex];
            Index removedIndex = idToIndex[id];
            void* removed = MEM_OFFSET(components, componentTypeSize * removedIndex);
            void* moved = MEM_OFFSET(components, componentTypeSize * movedIndex);

            memcpy(removed, moved, componentTypeSize);

            idToIndex[id] = IdNone;
            idToIndex[movedId] = removedIndex;
            indexToId[removedIndex] = removedIndex;
        }

        --count;
    }

    bool has(Index id) const { return idToIndex[id] != IdNone; }

    void reset(void* newMemory, Index newCount, size_t newComponentTypeSize)
    {
        memcpy(newMemory, components, newComponentTypeSize * count);
        max = newCount;
        componentTypeSize = newComponentTypeSize;
        idToIndex.resize(max, IdNone);
        indexToId.resize(max, IdNone);
        components = newMemory;
    }

    bool wasInit() const { return components; }
};
