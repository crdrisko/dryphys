// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: links.cpp
// Author: crdrisko
// Date: 07/13/2024-09:00:04
// Description:

#include "dryphys/particleSystems/collisionGenerators/links.hpp"

#include <config.h>

#include "dryphys/math/vector3d.hpp"

namespace DryPhys
{
    real ParticleLink::currentLength() const
    {
        Vector3D relativePosition = particles_[0]->getPosition() - particles_[1]->getPosition();

        return relativePosition.magnitude();
    }

    unsigned ParticleCable::addCollision(ParticleCollision* collision, unsigned) const
    {
        real length = currentLength();

        // Check if we're overextended
        if (length < maxLength_)
            return 0;

        collision->particles_[0] = particles_[0];
        collision->particles_[1] = particles_[1];

        Vector3D normal = particles_[1]->getPosition() - particles_[0]->getPosition();
        normal.normalize();
        collision->contactNormal_ = normal;

        collision->penetration_ = length - maxLength_;
        collision->restitution_ = restitution_;

        return 1;
    }

    unsigned ParticleRod::addCollision(ParticleCollision* collision, unsigned) const
    {
        real currLength = currentLength();

        // Check if we're overextended
        if (currLength == length_)
            return 0;

        collision->particles_[0] = particles_[0];
        collision->particles_[1] = particles_[1];

        Vector3D normal = particles_[1]->getPosition() - particles_[0]->getPosition();
        normal.normalize();

        // Handle contact normal differently for extension or compression of the rod
        if (currLength > length_)
        {
            collision->contactNormal_ = normal;
            collision->penetration_   = currLength - length_;
        }
        else
        {
            collision->contactNormal_ = normal * -1;
            collision->penetration_   = length_ - currLength;
        }

        // Restitution shouldn't appear bouncy in a rod, so set to 0
        collision->restitution_ = 0;

        return 1;
    }
}   // namespace DryPhys
