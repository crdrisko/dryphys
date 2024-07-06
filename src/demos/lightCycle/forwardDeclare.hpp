// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: forwardDeclare.hpp
// Author: crdrisko
// Date: 07/04/2024-09:51:12
// Description:

#ifndef DRYPHYS_SRC_DEMOS_LIGHTCYCLE_FORWARDDECLARE_HPP
#define DRYPHYS_SRC_DEMOS_LIGHTCYCLE_FORWARDDECLARE_HPP

#include <memory>

#include <engine2d/entity.hpp>
#include <engine2d/entityManager.hpp>

namespace LightCycle
{
    struct Directions;

    class ScenePlay;

    class CTransform;
    class CCycle;
    class CAI;

    class AI;
    class AIFactory;

    using ConcreteEntityManager = Engine2D::EntityManager<CTransform, CCycle, CAI>;
    using ConcreteEntityPtr     = std::shared_ptr<Engine2D::Entity<CTransform, CCycle, CAI>>;
}   // namespace LightCycle

#endif
