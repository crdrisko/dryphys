// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: buoyancy.cpp
// Author: crdrisko
// Date: 12/27/2024-10:31:30
// Description:

#include "dryphys/rigidBodySystems/forceGenerators/buoyancy.hpp"

#include "dryphys/math/vector.hpp"
#include "dryphys/types/rigidBody.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    RigidBodyBuoyancy::RigidBodyBuoyancy(
        const Vector3D& centerOfBuoyancy, real maxDepth, real volume, real waterHeight, real liquidDensity)
        : maxDepth_ {maxDepth}, volume_ {volume}, waterHeight_ {waterHeight}, liquidDensity_ {liquidDensity},
          centerOfBuoyancy_ {centerOfBuoyancy}
    {
    }

    void RigidBodyBuoyancy::updateForce(RigidBody* rigidBody, real)
    {
        // Calculate the submersion depth
        Vector3D pointInWorld = rigidBody->getPointInWorldSpace(centerOfBuoyancy_);
        real depth            = pointInWorld.y;

        // Check if we're out of the water
        if (depth >= waterHeight_ + maxDepth_)
            return;

        Vector3D force {};

        // Check if we're at the maximum depth
        if (depth <= waterHeight_ - maxDepth_)
        {
            force.y = liquidDensity_ * volume_;
            rigidBody->addForceAtPoint(force, centerOfBuoyancy_);

            return;
        }

        // Otherwise we are partially submerged
        force.y = liquidDensity_ * volume_ * (depth - maxDepth_ - waterHeight_) / (2 * maxDepth_);
        rigidBody->addForceAtPoint(force, centerOfBuoyancy_);
    }
}   // namespace DryPhys
