// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: world.hpp
// Author: crdrisko
// Date: 07/13/2024-10:17:36
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_WORLD_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_WORLD_HPP

#include <vector>

#include "dryphys/particle.hpp"
#include "dryphys/particleSystems/collisions.hpp"
#include "dryphys/particleSystems/forceGenerator.hpp"

namespace DryPhys
{
    class ParticleWorld
    {
        using Particles           = std::vector<Particle>;
        using CollisionGenerators = std::vector<ParticleCollisionGenerator*>;
        using Collisions          = std::vector<ParticleCollision>;

    protected:
        Particles particles_;
        ParticleForceRegistry registry_;
        ParticleCollisionResolver resolver_;
        CollisionGenerators collisionGenerators_;
        Collisions collisions_;
        unsigned maxCollisions_ {};
        bool calculateIterations_ {};

    public:
        ParticleWorld(unsigned maxCollisions, unsigned iterations = 0);

        void startFrame();
        unsigned generateCollisions();
        void integrate(real duration);
        void runPhysics(real duration);

        Particles& getParticles() { return particles_; }
        const Particles& getParticles() const { return particles_; }
    };
}   // namespace DryPhys

#endif