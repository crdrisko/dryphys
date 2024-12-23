// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particleBuoyancy.cpp
// Author: crdrisko
// Date: 06/23/2024-08:13:01
// Description:

#include "dryphys/particleSystems/forceGenerators/particleBuoyancy.hpp"

#include "dryphys/math/vector.hpp"
#include "dryphys/types/particle.hpp"

namespace DryPhys
{
    void DryPhys::ParticleBuoyancy::updateForce(Particle* particle, real)
    {
        // Calculate the submersion depth
        real depth = particle->getPosition()[1];

        // Check if we're out of the water
        if (depth >= waterHeight_ + maxDepth_)
            return;

        Vector3D force {};

        // Check if we're at the maximum depth
        if (depth <= waterHeight_ - maxDepth_)
        {
            force[1] = liquidDensity_ * volume_;
            particle->addForce(force);
        }

        // Otherwise we are partially submerged
        force[1] = liquidDensity_ * volume_ * (depth - maxDepth_ - waterHeight_) / (2 * maxDepth_);
        particle->addForce(force);
    }
}   // namespace DryPhys
