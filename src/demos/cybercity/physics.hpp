// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: physics.hpp
// Author: crdrisko
// Date: 06/20/2024-06:41:26
// Description:

#ifndef DRYPHYS_SRC_DEMOS_CYBERCITY_PHYSICS_HPP
#define DRYPHYS_SRC_DEMOS_CYBERCITY_PHYSICS_HPP

#include <memory>

#include <dryphys/vector3d.hpp>

namespace CyberCity
{
    class Entity;

    using EntityPtr = std::shared_ptr<Entity>;
}   // namespace CyberCity

namespace Physics
{
    DryPhys::Vector3D GetOverlap(CyberCity::EntityPtr a, CyberCity::EntityPtr b);
    DryPhys::Vector3D GetPreviousOverlap(CyberCity::EntityPtr a, CyberCity::EntityPtr b);
}   // namespace Physics

#endif
