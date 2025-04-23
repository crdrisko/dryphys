// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: world.cpp
// Author: crdrisko
// Date: 07/13/2024-10:29:33
// Description:

#include "dryphys/particleSystems/world.hpp"

#include <iostream>

#include "dryphys/particleSystems/constraints.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    ParticleWorld::ParticleWorld(unsigned maxContraints, unsigned iterations)
        : resolver_ {iterations}, maxContraints_ {maxContraints}
    {
        constraints_         = new ParticleConstraint[maxContraints_];
        calculateIterations_ = (iterations == 0);
    }

    ParticleWorld::~ParticleWorld() { delete[] constraints_; }

    void ParticleWorld::startFrame()
    {
        for (auto* particle : particles_)
            particle->clearAccumulator();
    }

    unsigned ParticleWorld::generateConstraints()
    {
        unsigned limit = maxContraints_;

        ParticleConstraint* nextConstraint = constraints_;

        for (const auto* collisionGenerator : constraintGenerators_)
        {
            unsigned used = collisionGenerator->addConstraint(nextConstraint, limit);
            limit -= used;
            nextConstraint += used;

            // We've run out of collisions
            if (limit <= 0)
                break;
        }

        return maxContraints_ - limit;
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

        unsigned usedConstraints = generateConstraints();

        if (usedConstraints)
        {
            if (calculateIterations_)
                resolver_.setIterations(usedConstraints * 2);

            resolver_.resolveConstraints(constraints_, usedConstraints, duration);
        }
    }
}   // namespace DryPhys
