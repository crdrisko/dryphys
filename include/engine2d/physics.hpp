// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: physics.hpp
// Author: crdrisko
// Date: 06/20/2024-06:41:26
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE2D_PHYSICS_HPP
#define DRYPHYS_INCLUDE_ENGINE2D_PHYSICS_HPP

#include <memory>

#include <dryphys/math.hpp>

namespace Engine2D
{
    DryPhys::Vector3D getAABBOverlap(
        DryPhys::Vector3D aPos, DryPhys::Vector3D aHalfSize, DryPhys::Vector3D bPos, DryPhys::Vector3D bHalfSize);
}   // namespace Engine2D

#endif
