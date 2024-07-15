// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: world.cpp
// Author: crdrisko
// Date: 07/13/2024-10:29:33
// Description:

#include "dryphys/particleSystems/world.hpp"

#include <config.h>

namespace DryPhys
{
    ParticleWorld::ParticleWorld(unsigned maxCollisions, unsigned iterations)
        : resolver_ {iterations}, maxCollisions_ {maxCollisions}
    {
        collisions_.resize(maxCollisions_);
        calculateIterations_ = (iterations == 0);
    }

    void ParticleWorld::startFrame()
    {
        for (auto& particle : particles_)
            particle.clearAccumulator();
    }


    unsigned ParticleWorld::generateCollisions()
    {
        unsigned limit      = maxCollisions_;
        auto* nextCollision = collisions_.data();

        for (const auto* collisionGenerator : collisionGenerators_)
        {
            unsigned used = collisionGenerator->addCollision(nextCollision, limit);
            limit -= used;
            nextCollision += used;

            // We've run out of collisions
            if (limit <= 0)
                break;
        }

        return maxCollisions_ - limit;
    }

    void ParticleWorld::integrate(real duration)
    {
        for (auto& particle : particles_)
            particle.integrate(duration);
    }

    void ParticleWorld::runPhysics(real duration)
    {
        registry_.updateForces(duration);

        integrate(duration);

        unsigned usedCollisions = generateCollisions();

        if (usedCollisions)
        {
            if (calculateIterations_)
                resolver_.setIterations(usedCollisions * 2);

            resolver_.resolveCollisions(collisions_, duration);
        }
    }
}   // namespace DryPhys
