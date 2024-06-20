// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: entityManager.hpp
// Author: crdrisko
// Date: 06/17/2024-14:49:43
// Description:

#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Engine2D
{
    template<typename... TupleArgs>
    class Entity;

    template<typename... TupleArgs>
    class EntityManager
    {
        using EntityVec = std::vector<std::shared_ptr<Entity<TupleArgs...>>>;
        using EntityMap = std::map<std::string, EntityVec>;

    private:
        EntityVec entities_;
        EntityVec entitiesToAdd_;
        EntityMap entityMap_;

        std::size_t totalEntities_ {};

        void removeDeadEntities(EntityVec& entities)
        {
            entities.erase(
                std::remove_if(entities.begin(), entities.end(), [](auto e) { return !(e->isActive()); }), entities.end());
        }

    public:
        EntityManager() = default;

        void update()
        {
            for (auto e : entitiesToAdd_)
            {
                entities_.push_back(e);
                entityMap_[e->tag()].push_back(e);
            }

            entitiesToAdd_.clear();

            removeDeadEntities(entities_);

            for (auto& [tag, entities] : entityMap_)
            {
                removeDeadEntities(entities);
            }
        }

        std::shared_ptr<Entity<TupleArgs...>> addEntity(const std::string& tag)
        {
            auto entity = std::shared_ptr<Entity<TupleArgs...>>(new Entity<TupleArgs...>(totalEntities_++, tag));
            entitiesToAdd_.push_back(entity);

            return entity;
        }

        EntityVec& getEntities() { return entities_; }
        EntityVec& getEntities(const std::string& tag) { return entityMap_[tag]; }
    };
}   // namespace Engine2D

#endif
