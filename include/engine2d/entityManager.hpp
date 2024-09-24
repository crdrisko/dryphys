// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: entityManager.hpp
// Author: crdrisko
// Date: 06/28/2024-07:28:05
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE2D_ENTITYMANAGER_HPP
#define DRYPHYS_INCLUDE_ENGINE2D_ENTITYMANAGER_HPP

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "engine2d/forwardDeclare.hpp"

namespace Engine2D
{
    template<typename... ComponentTypes>
    class EntityManager
    {
        using EntityVec = std::vector<std::shared_ptr<Entity<ComponentTypes...>>>;
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

        std::shared_ptr<Entity<ComponentTypes...>> addEntity(const std::string& tag)
        {
            auto entity = std::shared_ptr<Entity<ComponentTypes...>>(new Entity<ComponentTypes...>(totalEntities_++, tag));
            entitiesToAdd_.push_back(entity);

            return entity;
        }

        EntityVec& getEntities() { return entities_; }
        EntityVec& getEntities(const std::string& tag) { return entityMap_[tag]; }
    };

    template<typename... ComponentTypes>
    class EntityManagerMP
    {
        using EntityVec = std::vector<EntityMP<ComponentTypes...>>;
        using EntityMap = std::map<std::string, EntityVec>;

    private:
        EntityVec entities_;
        EntityVec entitiesToAdd_;
        EntityMap entityMap_;

        void removeDeadEntities(EntityVec& entities)
        {
            entities.erase(
                std::remove_if(entities.begin(), entities.end(), [](auto e) { return !(e.isActive()); }), entities.end());
        }

    public:
        EntityManagerMP() = default;

        EntityMP<ComponentTypes...> addEntity(const std::string& tag)
        {
            auto entity = EntityMemoryPool<ComponentTypes...>::getInstance().addEntity(tag);
            entitiesToAdd_.push_back(entity);

            return entity;
        }

        void update()
        {
            for (auto& e : entitiesToAdd_)
            {
                entities_.push_back(e);
                entityMap_[e.tag()].push_back(e);
            }

            entitiesToAdd_.clear();

            removeDeadEntities(entities_);

            for (auto& [tag, entities] : entityMap_)
            {
                removeDeadEntities(entities);
            }
        }

        void clearEntities() const
        {
            for (const auto& entity : entities_)
                EntityMemoryPool<ComponentTypes...>::getInstance().destroy(entity.id());
        }

        EntityVec& getEntities() { return entities_; }
        EntityVec& getEntities(const std::string& tag) { return entityMap_[tag]; }
    };

}   // namespace Engine2D

#endif
