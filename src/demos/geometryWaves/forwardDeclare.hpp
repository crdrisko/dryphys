// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: forwardDeclare.hpp
// Author: crdrisko
// Date: 07/14/2024-07:13:24
// Description:

#ifndef DRYPHYS_SRC_DEMOS_GEOMETRYWAVES_FORWARDDECLARE_HPP
#define DRYPHYS_SRC_DEMOS_GEOMETRYWAVES_FORWARDDECLARE_HPP

#include <memory>

#include <engine2d/entity.hpp>
#include <engine2d/entityManager.hpp>

namespace GeometryWaves
{
    class ScenePlay;

    class CTransform;
    class CShape;
    class CCollision;
    class CInput;
    class CScore;
    class CLifespan;

    using ConcreteEntityManager = Engine2D::EntityManager<CTransform, CShape, CCollision, CInput, CScore, CLifespan>;
    using ConcreteEntityPtr = std::shared_ptr<Engine2D::Entity<CTransform, CShape, CCollision, CInput, CScore, CLifespan>>;
}   // namespace GeometryWaves

#endif
