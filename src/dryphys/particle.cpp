// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particle.cpp
// Author: crdrisko
// Date: 06/13/2024-12:45:00
// Description:

#include "dryphys/particle.hpp"

#include <config.h>

#include <cassert>
#include <limits>

#include "dryphys/math/vector3d.hpp"

namespace DryPhys
{
    void Particle::integrate(real duration)
    {
        assert(duration > 0.0f);

        // We don't integrate things with infinite mass
        if (!hasFiniteMass())
            return;

        // Update linear position
        position_ += velocity_ * duration;

        // Work out the acceleration from the force
        Vector3D resultingAcc = acceleration_;
        resultingAcc += forceAccumulator_ * inverseMass_;

        // Update linear velocity from the acceleration
        velocity_ += resultingAcc * duration;

        // Impose drag
        velocity_ *= std::pow(damping_, duration);

        // Clear the forces
        clearAccumulator();
    }

    void Particle::setMass(real mass)
    {
        assert(mass != 0.0f);
        inverseMass_ = static_cast<real>(1.0f) / mass;
    }

    real Particle::getMass() const
    {
        if (!hasFiniteMass())
            return std::numeric_limits<real>::max();
        else
            return static_cast<real>(1.0f) / inverseMass_;
    }
}   // namespace DryPhys
