#pragma once

#include "ComponentPool.h"
#include "ComponentPoolView.h"
#include "Entity.h"
#include "client/Config.h"
#include "common/Types.h"
#include "memory/Memory.h"
#include "memory/PageAllocator.h"

#include <bitset>

class Registry {
public:
    Index componentsTypesCount;

public:
    Registry(Index maxEntitiesCountHint = 1000)
        : componentsTypesCount(0)
        , entityHasComponent(maxEntitiesCountHint, 0)
        , entitiesCount(0)
        , maxComponentsCount(maxEntitiesCountHint)
    {}

    ~Registry()
    {
        for (ComponentPool& pool : componentPools) {
            free(pool.components);
        }
    }

    template <typename Component>
    Index getComponentTypeId()
    {
        static Index id = componentsTypesCount++;
        return id;
    }

    template <typename Component>
    ComponentPool& getComponentPool()
    {
        return componentPools[getComponentTypeId<Component>()];
    }

    template <typename Component, typename... Args>
    Component& addComponent(Entity& entity, Args&&... args)
    {
        Index componentId = getComponentTypeId<Component>();
        ComponentPool& pool = componentPools[componentId];

        if (entity >= pool.max) {
            void* oldMemory = pool.components;
            void* newMemory =
                componentHeap.allocate(sizeof(Component) * maxComponentsCount);
            pool.reset(newMemory, maxComponentsCount, sizeof(Component));
            free(oldMemory);
        }

        entityHasComponent[entity][componentId] = true;
        return pool.add<Component>(entity, std::forward<Args>(args)...);
    }

    template <typename Component>
    void removeComponent(Entity& entity)
    {
        Index componentId = getComponentTypeId<Component>();

        entityHasComponent[entity][componentId] = false;
        componentPools[componentId].remove(entity);
    }

    template <typename Component>
    bool hasComponent(Entity& entity)
    {
        return entityHasComponent[entity][getComponentTypeId<Component>()];
    }

    template <typename Component>
    Component& getComponent(Entity& entity)
    {
        return getComponentPool<Component>().template get<Component>(entity);
    }

    template <typename Component>
    ComponentPoolView<Component> getView()
    {
        return getComponentPool<Component>().template getView<Component>();
    }

    Entity createEntity()
    {
        Index id;

        if (entitiesCount >= maxComponentsCount) {
            maxComponentsCount *= 2;
            entityHasComponent.resize(maxComponentsCount);
        }

        if (!availableEntitiesIds.empty()) {
            id = availableEntitiesIds.back();
            availableEntitiesIds.pop_back();
        } else {
            id = entitiesCount++;
        }

        return id;
    }

    void removeEntity(Entity& entity)
    {
        auto& hasComponent = entityHasComponent[entity];
        for (Index i = 0; i < componentsTypesCount; ++i) {
            if (hasComponent[i]) {
                componentPools[i].remove(entity);
            }
        }
        entityHasComponent[entity].reset();
        availableEntitiesIds.emplace_back(entity);
        --entitiesCount;
    }

    void reset(Index maxEntitiesCountHint = 1000)
    {
        componentHeap.clear();
        for (ComponentPool& pool : componentPools) {
            pool.reset(nullptr, 0, 0);
        }
        availableEntitiesIds.clear();
        entityHasComponent.clear();
        entityHasComponent.resize(maxEntitiesCountHint);
        entitiesCount = 0;
        maxComponentsCount = maxEntitiesCountHint;
    }

private:
    PageAllocator componentHeap;
    std::array<ComponentPool, config::MaxComponentsTypes> componentPools;
    std::vector<std::bitset<config::MaxComponentsTypes>> entityHasComponent;
    std::vector<Index> availableEntitiesIds;
    Index entitiesCount;
    Index maxComponentsCount;
};
