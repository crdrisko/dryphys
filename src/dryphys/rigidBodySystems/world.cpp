// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: world.cpp
// Author: crdrisko
// Date: 12/13/2024-14:36:10
// Description:

#include "dryphys/rigidBodySystems/world.hpp"

#include "dryphys/types/rigidBody.hpp"

namespace DryPhys
{
    void World::startFrame()
    {
        for (auto* rigidBody : rigidBodies_)
        {
            rigidBody->clearAccumulators();
            rigidBody->calculateDerivedData();
        }
    }

    void World::runPhysics(real duration)
    {
        registry_.updateForces(duration);

        for (auto* rigidBody : rigidBodies_)
            rigidBody->integrate(duration);
    }
}   // namespace DryPhys
