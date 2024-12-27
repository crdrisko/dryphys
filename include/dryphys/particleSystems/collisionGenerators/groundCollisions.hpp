// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: groundCollisions.hpp
// Author: crdrisko
// Date: 12/04/2024-05:14:04
// Description:

#ifndef GROUNDCOLLISIONS_HPP
#define GROUNDCOLLISIONS_HPP

#include <vector>

#include "dryphys/particleSystems/collisions.hpp"
#include "dryphys/particleSystems/world.hpp"
#include "dryphys/types/particle.hpp"

namespace DryPhys
{
    class GroundCollisions : public ParticleCollisionGenerator
    {
    private:
        ParticleWorld::Particles* particles_;

    public:
        void init(ParticleWorld::Particles* particles) { particles_ = particles; }

        virtual unsigned addCollision(ParticleCollision* collision, unsigned limit) const;
    };
}   // namespace DryPhys

#endif
