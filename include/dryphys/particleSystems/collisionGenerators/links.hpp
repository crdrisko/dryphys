// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: links.hpp
// Author: crdrisko
// Date: 07/13/2024-08:34:47
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_COLLISIONGENERATORS_LINKS_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_COLLISIONGENERATORS_LINKS_HPP

#include "dryphys/config.h"
#include "dryphys/particle.hpp"
#include "dryphys/particleSystems/collisions.hpp"

namespace DryPhys
{
    /*!
     *
     */
    class ParticleLink : public ParticleCollisionGenerator
    {
    public:
        Particle* particles_[2];

        unsigned addCollision(ParticleCollision* collision, unsigned limit) const override = 0;

    protected:
        real currentLength() const;
    };

    /*!
     *
     */
    class ParticleCable : public ParticleLink
    {
    public:
        real maxLength_ {};
        real restitution_ {};

        unsigned addCollision(ParticleCollision* collision, unsigned limit) const override;
    };

    /*!
     *
     */
    class ParticleRod : public ParticleLink
    {
    public:
        real length_ {};

        unsigned addCollision(ParticleCollision* collision, unsigned limit) const override;
    };
}   // namespace DryPhys

#endif
