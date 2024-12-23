// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: world.hpp
// Author: crdrisko
// Date: 12/13/2024-14:29:39
// Description:

#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>

#include "dryphys/types/rigidBody.hpp"
#include "dryphys/rigidBodySystems/forceGenerator.hpp"

namespace DryPhys
{
    class World
    {
    public:
        using RigidBodies = std::vector<RigidBody*>;

    protected:
        RigidBodies rigidBodies_;
        RigidBodyForceRegistry registry_;

    public:
        void startFrame();
        void runPhysics(real duration);
    };
}   // namespace DryPhys

#endif
