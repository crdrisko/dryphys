// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: gravity.cpp
// Author: crdrisko
// Date: 12/12/2024-06:16:49
// Description:

#include "dryphys/rigidBodySystems/forceGenerators/gravity.hpp"

#include "dryphys/types/rigidBody.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    void RigidBodyGravity::updateForce(RigidBody* rigidBody, real)
    {
        if (!rigidBody->hasFiniteMass())
            return;

        rigidBody->addForce(gravity_ * rigidBody->getMass());
    }
}   // namespace DryPhys
