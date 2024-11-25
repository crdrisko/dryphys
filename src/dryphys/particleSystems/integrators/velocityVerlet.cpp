// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: velocityVerlet.cpp
// Author: crdrisko
// Date: 07/16/2024-06:32:54
// Description:

#include "dryphys/particleSystems/integrators/velocityVerlet.hpp"

#include <cassert>
#include <vector>

#include "dryphys/config.h"
#include "dryphys/particle.hpp"

namespace DryPhys
{
    void VelocityVerlet::moveA(Particle* particle, real dt)
    {
        // Velocity half step
        particle->kick(particle->getAcceleration() * dt * 0.5f);

        // Position whole step
        particle->move(particle->getVelocity() * dt);
    }

    void VelocityVerlet::moveB(Particle* particle, real dt)
    {
        // Acceleration whole step
        particle->applyForces();

        // Velocity half step
        particle->kick(particle->getAcceleration() * dt * 0.5f);
    }

    void VelocityVerlet::integrate(std::vector<Particle>& particles, real timestep)
    {
        for (auto& particle : particles)
        {
            moveA(&particle, timestep);
        }

        registry_.updateForces(timestep);

        for (auto& particle : particles)
        {
            moveB(&particle, timestep);
        }
    }
}   // namespace DryPhys
