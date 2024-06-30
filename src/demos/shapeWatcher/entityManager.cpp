// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: entityManager.cpp
// Author: crdrisko
// Date: 06/30/2024-08:43:08
// Description:

#include "shapeWatcher/entityManager.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "shapeWatcher/entity.hpp"

namespace ShapeWatcher
{
    void EntityManager::removeDeadEntities(EntityVec& entities)
    {
        entities.erase(
            std::remove_if(entities.begin(), entities.end(), [](auto e) { return !(e->isActive()); }), entities.end());
    }

    void EntityManager::update()
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

    std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
    {
        auto entity = std::shared_ptr<Entity>(new Entity(totalEntities_++, tag));
        entitiesToAdd_.push_back(entity);

        return entity;
    }
}   // namespace ShapeWatcher
