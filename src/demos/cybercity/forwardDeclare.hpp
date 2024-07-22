// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: forwardDeclare.hpp
// Author: crdrisko
// Date: 07/12/2024-08:37:52
// Description:

#ifndef DRYPHYS_SRC_DEMOS_CYBERCITY_FORWARDDECLARE_HPP
#define DRYPHYS_SRC_DEMOS_CYBERCITY_FORWARDDECLARE_HPP

#include <memory>

#include <engine2d/entity.hpp>
#include <engine2d/entityManager.hpp>
#include <engine2d/entityMemoryPool.hpp>

namespace CyberCity
{
    class SceneMenu;
    class ScenePlay;

    class CAnimation;
    class CTransform;
    class CBoundingBox;
    class CInput;
    class CAI;
    class CDraggable;
    class CGravity;
    class CState;
    class CLifespan;

    // using ConcreteEntityManager = Engine2D::
    //     EntityManager<CAnimation, CTransform, CBoundingBox, CInput, CAI, CDraggable, CState, CGravity, CLifespan>;
    // using ConcreteEntityPtr = std::shared_ptr<
    //     Engine2D::Entity<CAnimation, CTransform, CBoundingBox, CInput, CAI, CDraggable, CState, CGravity, CLifespan>>;

    using ConcreteEntityManager = Engine2D::
        EntityManagerMP<CAnimation, CTransform, CBoundingBox, CInput, CAI, CDraggable, CState, CGravity, CLifespan>;

    using ConcreteEntity
        = Engine2D::EntityMP<CAnimation, CTransform, CBoundingBox, CInput, CAI, CDraggable, CState, CGravity, CLifespan>;

    using ConcreteEntityPtr = std::shared_ptr<ConcreteEntity>;
}   // namespace CyberCity

#endif
