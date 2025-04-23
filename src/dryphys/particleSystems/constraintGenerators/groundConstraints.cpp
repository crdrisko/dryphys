// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: groundConstraints.cpp
// Author: crdrisko
// Date: 12/04/2024-05:14:25
// Description:

#include <vector>

#include "dryphys/particleSystems/constraintGenerators/groundConstraints.hpp"
#include "dryphys/particleSystems/constraints.hpp"
#include "dryphys/particleSystems/world.hpp"
#include "dryphys/types/particle.hpp"

namespace DryPhys
{
    unsigned GroundConstraints::addConstraint(ParticleConstraint* constraint, unsigned limit) const
    {
        unsigned count = 0;

        for (ParticleWorld::Particles::iterator p = particles_->begin(); p != particles_->end(); p++)
        {
            real y = (*p)->getPosition().y;
            if (y < 0.0f)
            {
                constraint->contactNormal_ = Vector3D {0, 1, 0};
                constraint->particles_[0]  = *p;
                constraint->particles_[1]  = nullptr;
                constraint->penetration_   = -y;
                constraint->restitution_   = 0.2f;
                constraint++;
                count++;
            }

            if (count >= limit)
                return count;
        }
        return count;
    }
}   // namespace DryPhys
