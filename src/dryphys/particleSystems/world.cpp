// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: world.cpp
// Author: crdrisko
// Date: 07/13/2024-10:29:33
// Description:

#include "dryphys/particleSystems/world.hpp"

#include <iostream>

#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    ParticleWorld::ParticleWorld(unsigned maxCollisions, unsigned iterations)
        : resolver_ {iterations}, maxCollisions_ {maxCollisions}
    {
        collisions_          = new ParticleCollision[maxCollisions_];
        calculateIterations_ = (iterations == 0);
    }

    ParticleWorld::~ParticleWorld() { delete[] collisions_; }

    void ParticleWorld::startFrame()
    {
        for (auto* particle : particles_)
            particle->clearAccumulator();
    }

    unsigned ParticleWorld::generateCollisions()
    {
        unsigned limit = maxCollisions_;

        ParticleCollision* nextCollision = collisions_;

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

    // void ParticleWorld::integrate(real duration)
    // {
    //     for (auto* particle : particles_)
    //         particle->integrate(duration);
    // }

    void ParticleWorld::moveA(real duration)
    {
        for (auto* particle : particles_)
            particle->moveA(duration);
    }

    void ParticleWorld::moveB(real duration)
    {
        for (auto* particle : particles_)
        {
            particle->moveB(duration);
            particle->drag(std::pow(particle->getDamping(), duration));
        }
    }

    void ParticleWorld::runPhysics(real duration)
    {
        moveA(duration);
        registry_.updateForces(duration);
        moveB(duration);

        unsigned usedCollisions = generateCollisions();

        if (usedCollisions)
        {
            if (calculateIterations_)
                resolver_.setIterations(usedCollisions * 2);

            resolver_.resolveCollisions(collisions_, usedCollisions, duration);
        }
    }
}   // namespace DryPhys
