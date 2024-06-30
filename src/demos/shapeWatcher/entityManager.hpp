// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: entityManager.hpp
// Author: crdrisko
// Date: 06/30/2024-06:47:05
// Description:

#ifndef DRYPHYS_SRC_DEMOS_SHAPEWATCHER_ENTITYMANAGER_HPP
#define DRYPHYS_SRC_DEMOS_SHAPEWATCHER_ENTITYMANAGER_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "shapeWatcher/entity.hpp"

namespace ShapeWatcher
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

        void removeDeadEntities(EntityVec& entities);

    public:
        EntityManager() = default;

        void update();

        std::shared_ptr<Entity> addEntity(const std::string& tag);

        EntityVec& getEntities() { return entities_; }
        EntityVec& getEntities(const std::string& tag) { return entityMap_[tag]; }
    };
}   // namespace ShapeWatcher

#endif
