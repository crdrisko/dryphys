// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: entityMemoryPool.hpp
// Author: crdrisko
// Date: 06/19/2024-09:54:10
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE2D_ENTITYMEMORYPOOL_HPP
#define DRYPHYS_INCLUDE_ENGINE2D_ENTITYMEMORYPOOL_HPP

#include <string>
#include <tuple>
#include <vector>

#include "engine2d/forwardDeclare.hpp"

namespace Engine2D
{
    template<typename... ComponentTypes>
    class EntityMemoryPool
    {
    private:
        std::tuple<std::vector<ComponentTypes>...> pool_;
        std::vector<std::string> tags_;
        std::vector<bool> active_;

        explicit EntityMemoryPool(std::size_t maxEntities)
        {
            std::apply([&](auto&... elem) { ((elem.resize(maxEntities)), ...); }, pool_);

            tags_.resize(maxEntities);
            active_.resize(maxEntities);
        }

        std::size_t getNextEntityIndex()
        {
            for (std::size_t i {}; i < active_.size(); ++i)
            {
                if (!active_[i])
                    return i;
            }

            return MAX_ENTITIES;
        }

    public:
        static EntityMemoryPool& getInstance()
        {
            static EntityMemoryPool instance(MAX_ENTITIES);

            return instance;
        }

        EntityMP<ComponentTypes...> addEntity(const std::string& tag)
        {
            std::size_t index = getNextEntityIndex();

            if (index >= MAX_ENTITIES)
                throw;

            std::apply([&](auto&... elem) { ((elem[index].has = false), ...); }, pool_);

            tags_[index]   = tag;
            active_[index] = true;

            return EntityMP<ComponentTypes...> {index};
        }

        template<typename T>
        bool hasComponent(std::size_t entityID) const
        {
            return std::get<std::vector<T>>(pool_)[entityID].has;
        }

        template<typename T>
        T& getComponent(std::size_t entityID)
        {
            return std::get<std::vector<T>>(pool_)[entityID];
        }

        void destroy(std::size_t entityID) { active_[entityID] = false; }
        const std::string& getTag(std::size_t entityID) const { return tags_[entityID]; }
        bool isActive(std::size_t entityID) const { return active_[entityID]; }
    };
}   // namespace Engine2D

#endif
