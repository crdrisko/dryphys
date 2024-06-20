// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: physics.cpp
// Author: crdrisko
// Date: 06/20/2024-06:41:28
// Description:

#include "physics.hpp"

#include <cmath>

#include <dryphys/vector3d.hpp>

#include "components.hpp"
#include "scenes/scenePlay.hpp"

namespace Physics
{
    DryPhys::Vector3D GetOverlap(EntityPtr a, EntityPtr b)
    {
        auto aPos      = a->getComponent<CTransform>().pos;
        auto aHalfSize = a->getComponent<CBoundingBox>().halfSize;

        auto bPos      = b->getComponent<CTransform>().pos;
        auto bHalfSize = b->getComponent<CBoundingBox>().halfSize;

        DryPhys::Vector3D delta {std::abs(aPos[0] - bPos[0]), std::abs(aPos[1] - bPos[1]), 0};

        float ox = (aHalfSize[0] + bHalfSize[0]) - delta[0];
        float oy = (aHalfSize[1] + bHalfSize[1]) - delta[1];

        return DryPhys::Vector3D {ox, oy, 0};
    }

    DryPhys::Vector3D Physics::GetPreviousOverlap(EntityPtr a, EntityPtr b)
    {
        auto aPos      = a->getComponent<CTransform>().prevPos;
        auto aHalfSize = a->getComponent<CBoundingBox>().halfSize;

        auto bPos      = b->getComponent<CTransform>().prevPos;
        auto bHalfSize = b->getComponent<CBoundingBox>().halfSize;

        DryPhys::Vector3D delta {std::abs(aPos[0] - bPos[0]), std::abs(aPos[1] - bPos[1]), 0};

        float ox = (aHalfSize[0] + bHalfSize[0]) - delta[0];
        float oy = (aHalfSize[1] + bHalfSize[1]) - delta[1];

        return DryPhys::Vector3D {ox, oy, 0};
    }
}   // namespace Physics
