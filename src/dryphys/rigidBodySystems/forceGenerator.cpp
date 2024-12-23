// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: forceGenerator.cpp
// Author: crdrisko
// Date: 12/13/2024-14:45:26
// Description:

#include "dryphys/rigidBodySystems/forceGenerator.hpp"

#include <algorithm>

#include "dryphys/types/rigidBody.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    void RigidBodyForceRegistry::updateForces(real duration)
    {
        for (auto [rigidBody, fg] : registrations_)
        {
            fg->updateForce(rigidBody, duration);
        }
    }

    void RigidBodyForceRegistry::add(RigidBody* rigidBody, RigidBodyForceGenerator* fg)
    {
        registrations_.emplace_back(RigidBodyForceRegistration {rigidBody, fg});
    }

    void RigidBodyForceRegistry::remove(RigidBody* rigidBody, RigidBodyForceGenerator* fg)
    {
        registrations_.erase(std::remove_if(registrations_.begin(),
                                 registrations_.end(),
                                 [rigidBody, fg](auto reg) { return reg.rigidBody == rigidBody && reg.fg == fg; }),
            registrations_.end());
    }
}   // namespace DryPhys
