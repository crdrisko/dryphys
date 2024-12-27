// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: groundCollisions.cpp
// Author: crdrisko
// Date: 12/04/2024-05:14:25
// Description:

#include "dryphys/particleSystems/collisionGenerators/groundCollisions.hpp"

#include <vector>

#include "dryphys/particleSystems/collisions.hpp"
#include "dryphys/particleSystems/world.hpp"
#include "dryphys/types/particle.hpp"

namespace DryPhys
{
    unsigned GroundCollisions::addCollision(ParticleCollision* collisions, unsigned limit) const
    {
        unsigned count = 0;

        for (ParticleWorld::Particles::iterator p = particles_->begin(); p != particles_->end(); p++)
        {
            real y = (*p)->getPosition().y;
            if (y < 0.0f)
            {
                collisions->contactNormal_ = Vector3D {0, 1, 0};
                collisions->particles_[0]  = *p;
                collisions->particles_[1]  = nullptr;
                collisions->penetration_   = -y;
                collisions->restitution_   = 0.2f;
                collisions++;
                count++;
            }

            if (count >= limit)
                return count;
        }
        return count;
    }
}   // namespace DryPhys
