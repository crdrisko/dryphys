// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: links.cpp
// Author: crdrisko
// Date: 07/13/2024-09:00:04
// Description:

#include "dryphys/particleSystems/constraintGenerators/links.hpp"

#include "dryphys/math/vector.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    real ParticleLink::currentLength() const
    {
        Vector3D relativePosition = particles_[0]->getPosition() - particles_[1]->getPosition();

        return relativePosition.magnitude();
    }

    unsigned ParticleCable::addConstraint(ParticleConstraint* constraint, unsigned) const
    {
        real length = currentLength();

        // Check if we're overextended
        if (length < maxLength_)
            return 0;

        constraint->particles_[0] = particles_[0];
        constraint->particles_[1] = particles_[1];

        Vector3D normal = particles_[1]->getPosition() - particles_[0]->getPosition();
        normal.normalize();
        constraint->contactNormal_ = normal;

        constraint->penetration_ = length - maxLength_;
        constraint->restitution_ = restitution_;

        return 1;
    }

    unsigned ParticleRod::addConstraint(ParticleConstraint* constraint, unsigned) const
    {
        real currLength = currentLength();

        // Check if we're overextended
        if (currLength == length_)
            return 0;

        constraint->particles_[0] = particles_[0];
        constraint->particles_[1] = particles_[1];

        Vector3D normal = particles_[1]->getPosition() - particles_[0]->getPosition();
        normal.normalize();

        // Handle contact normal differently for extension or compression of the rod
        if (currLength > length_)
        {
            constraint->contactNormal_ = normal;
            constraint->penetration_   = currLength - length_;
        }
        else
        {
            constraint->contactNormal_ = normal * -1;
            constraint->penetration_   = length_ - currLength;
        }

        // Restitution shouldn't appear bouncy in a rod, so set to 0
        constraint->restitution_ = 0;

        return 1;
    }

    real ParticleAnchor::currentLength() const
    {
        Vector3D relativePos = particle_->getPosition() - anchor_;
        return relativePos.magnitude();
    }

    unsigned ParticleCableConstraint::addConstraint(ParticleConstraint* constraint, unsigned) const
    {
        // Find the length of the cable
        real length = currentLength();

        // Check if we're over-extended
        if (length < maxLength_)
        {
            return 0;
        }

        // Otherwise return the constraint
        constraint->particles_[0] = particle_;
        constraint->particles_[1] = 0;

        // Calculate the normal
        Vector3D normal = anchor_ - particle_->getPosition();
        normal.normalize();
        constraint->contactNormal_ = normal;

        constraint->penetration_ = length - maxLength_;
        constraint->restitution_ = restitution_;

        return 1;
    }

    unsigned ParticleRodConstraint::addConstraint(ParticleConstraint* constraint, unsigned) const
    {
        // Find the length of the rod
        real currentLen = currentLength();

        // Check if we're over-extended
        if (currentLen == length_)
        {
            return 0;
        }

        // Otherwise return the constraint
        constraint->particles_[0] = particle_;
        constraint->particles_[1] = 0;

        // Calculate the normal
        Vector3D normal = anchor_ - particle_->getPosition();
        normal.normalize();

        // The contact normal depends on whether we're extending or compressing
        if (currentLen > length_)
        {
            constraint->contactNormal_ = normal;
            constraint->penetration_   = currentLen - length_;
        }
        else
        {
            constraint->contactNormal_ = normal * -1;
            constraint->penetration_   = length_ - currentLen;
        }

        // Always use zero restitution (no bounciness)
        constraint->restitution_ = 0;

        return 1;
    }
}   // namespace DryPhys
