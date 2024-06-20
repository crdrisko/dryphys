// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: entityManager.cpp
// Author: crdrisko
// Date: 06/19/2024-05:44:21
// Description:

#include "entityManager.hpp"

#include <algorithm>

#include "entity.hpp"

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

void EntityManager::removeDeadEntities(EntityVec& entities)
{
    entities.erase(
        std::remove_if(entities.begin(), entities.end(), [](auto e) { return !(e->isActive()); }), entities.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(totalEntities_++, tag));
    entitiesToAdd_.push_back(entity);

    return entity;
}

EntityVec& EntityManager::getEntities() { return entities_; }

EntityVec& EntityManager::getEntities(const std::string& tag) { return entityMap_[tag]; }
