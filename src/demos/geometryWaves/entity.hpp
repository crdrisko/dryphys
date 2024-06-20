// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: entity.hpp
// Author: crdrisko
// Date: 06/19/2024-05:41:43
// Description:

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <memory>
#include <string>

#include "components.hpp"
#include "entityManager.hpp"

class Entity
{
private:
    friend class EntityManager;

    bool active_     = true;
    std::size_t id_  = 0;
    std::string tag_ = "default";

    Entity(const std::size_t id, const std::string& tag) : id_ {id}, tag_ {tag} {}

public:
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CInput> cInput;
    std::shared_ptr<CScore> cScore;
    std::shared_ptr<CLifespan> cLifespan;

    bool isActive() const { return active_; }
    const std::string& tag() const { return tag_; }
    std::size_t id() const { return id_; }
    void destroy() { active_ = false; }
};

#endif
