// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particleGravity.cpp
// Author: crdrisko
// Date: 06/16/2024-05:53:22
// Description:

#include "dryphys/particleSystems/forceGenerators/particleGravity.hpp"

#include <config.h>

#include "dryphys/particle.hpp"

namespace DryPhys
{
    void ParticleGravity::updateForce(Particle* particle, real)
    {
        if (!particle->hasFiniteMass())
            return;

        particle->addForce(gravity_ * particle->getMass());
    }
}   // namespace DryPhys
