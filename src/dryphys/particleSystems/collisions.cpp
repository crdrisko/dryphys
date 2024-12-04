// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: collisions.cpp
// Author: crdrisko
// Date: 07/13/2024-06:35:09
// Description:

#include "dryphys/particleSystems/collisions.hpp"

#include <limits>
#include <vector>

namespace DryPhys
{
    void ParticleCollision::resolve(real duration)
    {
        resolveVelocity(duration);
        resolveInterpenetration(duration);
    }

    real ParticleCollision::calculateSeparatingVelocity() const
    {
        Vector3D relativeVelocity = particles_[0]->getVelocity();

        // NULL for single-object collisions (i.e. particle and scenery)
        if (particles_[1])
            relativeVelocity -= particles_[1]->getVelocity();

        real separatingVelocity = relativeVelocity.dot(contactNormal_);

        return separatingVelocity;
    }

    void ParticleCollision::resolveVelocity(real)
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

        particles_[0]->kick(impulsePerInvMass * particles_[0]->getInverseMass());

        if (particles_[1])
            particles_[1]->kick(impulsePerInvMass * -particles_[1]->getInverseMass());
    }

    void ParticleCollision::resolveInterpenetration(real)
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

        particles_[0]->move(particleMovements_[0]);

        if (particles_[1])
            particles_[1]->move(particleMovements_[1]);
    }

    void ParticleCollisionResolver::resolveCollisions(ParticleCollision* collisions, unsigned numCollisions, real duration)
    {
        unsigned i;
        unsigned iterationsUsed {};

        while (iterationsUsed < iterations_)
        {
            // Find the contact with the largest closing velocity
            real maxVel       = std::numeric_limits<real>::max();
            unsigned maxIndex = numCollisions;

            for (i = 0; i < numCollisions; ++i)
            {
                real separatingVelocity = collisions[i].calculateSeparatingVelocity();

                if (separatingVelocity < maxVel && (separatingVelocity < 0 || collisions[i].penetration_ > 0))
                {
                    maxIndex = i;
                    maxVel   = separatingVelocity;
                }
            }

            if (maxIndex == numCollisions)
                break;

            collisions[maxIndex].resolve(duration);

            // Update the interpenetrations for all particles
            // Vector3D* move = collisions[maxIndex].particleMovements_;

            // for (i = 0; i < numCollisions; i++)
            // {
            //     if (collisions[i].particles_[0] == collisions[maxIndex].particles_[0])
            //     {
            //         collisions[i].penetration_ -= move[0] * collisions[i].contactNormal_;
            //     }
            //     else if (collisions[i].particles_[0] == collisions[maxIndex].particles_[1])
            //     {
            //         collisions[i].penetration_ -= move[1] * collisions[i].contactNormal_;
            //     }
            //     if (collisions[i].particles_[1])
            //     {
            //         if (collisions[i].particles_[1] == collisions[maxIndex].particles_[0])
            //         {
            //             collisions[i].penetration_ += move[0] * collisions[i].contactNormal_;
            //         }
            //         else if (collisions[i].particles_[1] == collisions[maxIndex].particles_[1])
            //         {
            //             collisions[i].penetration_ += move[1] * collisions[i].contactNormal_;
            //         }
            //     }
            // }

            iterationsUsed++;
        }
    }
}   // namespace DryPhys
