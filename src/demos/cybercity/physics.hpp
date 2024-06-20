// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: physics.hpp
// Author: crdrisko
// Date: 06/20/2024-06:41:26
// Description:

#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <dryphys/vector3d.hpp>

#include "scenes/scenePlay.hpp"

namespace Physics
{
    DryPhys::Vector3D GetOverlap(EntityPtr a, EntityPtr b);
    DryPhys::Vector3D GetPreviousOverlap(EntityPtr a, EntityPtr b);
}   // namespace Physics

#endif
