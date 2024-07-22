// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: physics.cpp
// Author: crdrisko
// Date: 06/20/2024-06:41:28
// Description:

#include "engine2d/physics.hpp"

#include <cmath>

#include <dryphys/math.hpp>

namespace Engine2D
{
    DryPhys::Vector3D getAABBOverlap(
        DryPhys::Vector3D aPos, DryPhys::Vector3D aHalfSize, DryPhys::Vector3D bPos, DryPhys::Vector3D bHalfSize)
    {
        DryPhys::Vector3D delta {std::abs(aPos[0] - bPos[0]), std::abs(aPos[1] - bPos[1]), std::abs(aPos[2] - bPos[2])};

        float ox = (aHalfSize[0] + bHalfSize[0]) - delta[0];
        float oy = (aHalfSize[1] + bHalfSize[1]) - delta[1];
        float oz = (aHalfSize[2] + bHalfSize[2]) - delta[2];

        return DryPhys::Vector3D {ox, oy, oz};
    }
}   // namespace Engine2D
