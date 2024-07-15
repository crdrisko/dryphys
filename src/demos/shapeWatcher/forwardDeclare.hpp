// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: forwardDeclare.hpp
// Author: crdrisko
// Date: 07/14/2024-07:03:02
// Description:

#ifndef DRYPHYS_SRC_DEMOS_SHAPEWATCHER_FORWARDDECLARE_HPP
#define DRYPHYS_SRC_DEMOS_SHAPEWATCHER_FORWARDDECLARE_HPP

#include <memory>

#include <engine2d/entity.hpp>
#include <engine2d/entityManager.hpp>

namespace ShapeWatcher
{
    class ScenePlay;

    class CTransform;
    class CShape;

    using ConcreteEntityManager = Engine2D::EntityManager<CTransform, CShape>;
    using ConcreteEntityPtr     = std::shared_ptr<Engine2D::Entity<CTransform, CShape>>;
}   // namespace ShapeWatcher

#endif
