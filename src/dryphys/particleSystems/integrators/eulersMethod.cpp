// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: eulersMethod.cpp
// Author: crdrisko
// Date: 06/17/2024-07:05:15
// Description:

#include "dryphys/particleSystems/integrators/eulersMethod.hpp"

#include <config.h>

#include <cassert>

#include "dryphys/particle.hpp"

namespace DryPhys
{
    void EulersMethod::integrate(Particle* particle, real timestep)
    {
        DryPhys::Vector3D pos;
        DryPhys::Vector3D vel;
        DryPhys::Vector3D acc;

        particle->getPosition(&pos);
        particle->getVelocity(&vel);
        particle->getAcceleration(&acc);

        assert(timestep > 0.0f);

        // We don't integrate things with infinite mass
        if (!particle->hasFiniteMass())
            return;

        // Update linear position
        particle->setPosition(particle->getPosition() + (particle->getVelocity() * timestep));

        // Work out the acceleration from the force
        Vector3D resultingAcc = acc;
        resultingAcc += forceAccumulator_ * particle->getInverseMass();

        // Update linear velocity from the acceleration
        // vel += resultingAcc * timestep;
        particle->setVelocity(particle->getVelocity() + (resultingAcc * timestep));

        // Impose drag
        // vel *= std::pow(damping_, timestep);
        particle->setVelocity(particle->getVelocity() * std::pow(damping_, timestep));

        // Clear the forces
        clearAccumulator();
    }
}   // namespace DryPhys
