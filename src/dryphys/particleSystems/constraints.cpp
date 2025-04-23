// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: constraints.cpp
// Author: crdrisko
// Date: 07/13/2024-06:35:09
// Description:

#include <limits>
#include <vector>

#include "dryphys/particleSystems/constraints.hpp"

namespace DryPhys
{
    void ParticleConstraint::resolve(real duration)
    {
        resolveVelocity(duration);
        resolveInterpenetration(duration);
    }

    real ParticleConstraint::calculateSeparatingVelocity() const
    {
        Vector3D relativeVelocity = particles_[0]->getVelocity();

        // NULL for single-object constraints (i.e. particle and scenery)
        if (particles_[1])
            relativeVelocity -= particles_[1]->getVelocity();

        real separatingVelocity = relativeVelocity.dot(contactNormal_);

        return separatingVelocity;
    }

    void ParticleConstraint::resolveVelocity(real)
    {
        real separatingVelocity = calculateSeparatingVelocity();

        // If the contact is separating or stationary, no impulse is needed
        if (separatingVelocity > 0)
            return;

        real newSeparatingVelocity = -separatingVelocity * restitution_;

        // Resolve resting contacts (contacts caused from accelaration only)
        Vector3D velocityFromAccelaration = particles_[0]->getAcceleration();

        if (particles_[1])
            velocityFromAccelaration += particles_[1]->getAcceleration();

        // Remove any closing velocity due to acceleration buildup from separating velocity
        if (real separatingVelocityFromAcceleration = velocityFromAccelaration.dot(contactNormal_);
            separatingVelocityFromAcceleration < 0)
        {
            newSeparatingVelocity += restitution_ * separatingVelocityFromAcceleration;

            // Unset change if we've resolved more than we should have
            if (newSeparatingVelocity < 0)
                newSeparatingVelocity = 0;
        }

        real deltaSeparatingVelocity = newSeparatingVelocity - separatingVelocity;

        real totalInverseMass = particles_[0]->getInverseMass();

        if (particles_[1])
            totalInverseMass += particles_[1]->getInverseMass();

        // Impulses should have no effect if both particles have infinite mass
        if (totalInverseMass <= 0)
            return;

        real impulse = deltaSeparatingVelocity / totalInverseMass;

        Vector3D impulsePerInvMass = contactNormal_ * impulse;

        particles_[0]->kick(impulsePerInvMass, particles_[0]->getInverseMass());

        if (particles_[1])
            particles_[1]->kick(impulsePerInvMass, -particles_[1]->getInverseMass());
    }

    void ParticleConstraint::resolveInterpenetration(real)
    {
        // We can't resolve interpenetration if there isn't anything to resolve
        if (penetration_ <= 0)
            return;

        real totalInverseMass = particles_[0]->getInverseMass();

        if (particles_[1])
            totalInverseMass += particles_[1]->getInverseMass();

        // Impulses should have no effect if both particles have infinite mass
        if (totalInverseMass <= 0)
            return;

        Vector3D movementPerInvMass = contactNormal_ * (penetration_ / totalInverseMass);

        particleMovements_[0] = movementPerInvMass * particles_[0]->getInverseMass();

        if (particles_[1])
            particleMovements_[1] = movementPerInvMass * -particles_[1]->getInverseMass();
        else
            particleMovements_[1].clear();

        particles_[0]->drift(particleMovements_[0]);

        if (particles_[1])
            particles_[1]->drift(particleMovements_[1]);
    }

    void ParticleConstraintResolver::resolveConstraints(
        ParticleConstraint* constraints, unsigned numConstraints, real duration)
    {
        unsigned i;

        iterationsUsed_ = 0;
        while (iterationsUsed_ < iterations_)
        {
            // Find the contact with the largest closing velocity;
            real max          = std::numeric_limits<real>::max();
            unsigned maxIndex = numConstraints;
            for (i = 0; i < numConstraints; i++)
            {
                real sepVel = constraints[i].calculateSeparatingVelocity();
                if (sepVel < max && (sepVel < 0 || constraints[i].penetration_ > 0))
                {
                    max      = sepVel;
                    maxIndex = i;
                }
            }

            // Do we have anything worth resolving?
            if (maxIndex == numConstraints)
                break;

            // Resolve this contact
            constraints[maxIndex].resolve(duration);

            // Update the interpenetrations for all particles
            Vector3D* move = constraints[maxIndex].particleMovements_;
            for (i = 0; i < numConstraints; i++)
            {
                if (constraints[i].particles_[0] == constraints[maxIndex].particles_[0])
                {
                    constraints[i].penetration_ -= move[0].dot(constraints[i].contactNormal_);
                }
                else if (constraints[i].particles_[0] == constraints[maxIndex].particles_[1])
                {
                    constraints[i].penetration_ -= move[1].dot(constraints[i].contactNormal_);
                }
                if (constraints[i].particles_[1])
                {
                    if (constraints[i].particles_[1] == constraints[maxIndex].particles_[0])
                    {
                        constraints[i].penetration_ += move[0].dot(constraints[i].contactNormal_);
                    }
                    else if (constraints[i].particles_[1] == constraints[maxIndex].particles_[1])
                    {
                        constraints[i].penetration_ += move[1].dot(constraints[i].contactNormal_);
                    }
                }
            }

            iterationsUsed_++;
        }
    }
}   // namespace DryPhys
