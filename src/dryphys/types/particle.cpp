// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particle.cpp
// Author: crdrisko
// Date: 06/13/2024-12:45:00
// Description:

#include "dryphys/types/particle.hpp"

#include <cassert>
#include <cmath>
#include <limits>

#include "dryphys/math/vector.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    void Particle::moveA(real duration)
    {
        assert(duration > 0.0f);

        kick(acceleration_, duration * 0.5);
        drift(velocity_, duration);
    }

    void Particle::moveB(real duration)
    {
        assert(duration > 0.0f);

        // Update and store acceleration for next timestep's moveA()
        acceleration_ = forceAccumulator_ * inverseMass_;

        kick(acceleration_, duration * 0.5);

        clearAccumulator();
    }

    //! Accessors for the mass
    void Particle::setMass(real mass)
    {
        assert(mass != 0.0f);
        inverseMass_ = static_cast<real>(1) / mass;
    }

    real Particle::getMass() const
    {
        if (!hasFiniteMass())
            return std::numeric_limits<real>::max();
        else
            return static_cast<real>(1) / inverseMass_;
    }
}   // namespace DryPhys
