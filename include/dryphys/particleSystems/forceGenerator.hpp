// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: forceGenerator.hpp
// Author: crdrisko
// Date: 06/14/2024-19:00:13
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_FORCEGENERATOR_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_FORCEGENERATOR_HPP

#include <vector>

#include "dryphys/particle.hpp"
#include "dryphys/vector3d.hpp"

namespace DryPhys
{
    /*!
     *
     */
    class ParticleForceGenerator
    {
    public:
        ~ParticleForceGenerator() = default;

        virtual void updateForce(Particle* particle, real duration) = 0;
    };

    /*!
     *
     */
    class ParticleForceRegistry
    {
    protected:
        struct ParticleForceRegistration
        {
            Particle* particle;
            ParticleForceGenerator* fg;
        };

        using Registry = std::vector<ParticleForceRegistration>;
        Registry registrations_;

    public:
        void updateForces(real duration);

        void add(Particle* particle, ParticleForceGenerator* fg);
        void remove(Particle* particle, ParticleForceGenerator* fg);

        void clear() { registrations_.clear(); }
    };
}   // namespace DryPhys

#endif
