// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: entityManager.hpp
// Author: crdrisko
// Date: 06/17/2024-14:49:43
// Description:

#ifndef DRYPHYS_SRC_DEMOS_CYBERCITY_ENTITYMANAGER_HPP
#define DRYPHYS_SRC_DEMOS_CYBERCITY_ENTITYMANAGER_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "cybercity/entity.hpp"

namespace CyberCity
{
    class EntityManager
    {
        using EntityVec = std::vector<std::shared_ptr<Entity>>;
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

        std::shared_ptr<Entity> addEntity(const std::string& tag)
        {
            auto entity = std::shared_ptr<Entity>(new Entity(totalEntities_++, tag));
            entitiesToAdd_.push_back(entity);

            return entity;
        }

        EntityVec& getEntities() { return entities_; }
        EntityVec& getEntities(const std::string& tag) { return entityMap_[tag]; }
    };
}   // namespace CyberCity

#endif
