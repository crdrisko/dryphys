// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: entityManager.hpp
// Author: crdrisko
// Date: 06/19/2024-05:43:49
// Description:

#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Entity;

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager
{
private:
    EntityVec entities_;
    EntityVec entitiesToAdd_;
    EntityMap entityMap_;

    std::size_t totalEntities_ {};

    void removeDeadEntities(EntityVec& entities);

public:
    EntityManager() = default;

    void update();

    std::shared_ptr<Entity> addEntity(const std::string& tag);

    EntityVec& getEntities();
    EntityVec& getEntities(const std::string& tag);
};

#endif
