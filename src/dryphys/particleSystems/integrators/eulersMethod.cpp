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
    void EulersMethod::integrate(std::vector<Particle>& particles, real timestep)
    {
        registry_.updateForces(timestep);

        Vector3D vel;

        for (auto& particle : particles)
        {
            particle.integrate(timestep);

            // // Apply drag
            particle.getVelocity(&vel);
            vel *= std::pow(damping_, timestep);
            particle.setVelocity(vel);
        }
    }
}   // namespace DryPhys
