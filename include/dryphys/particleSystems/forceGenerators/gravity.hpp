// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: gravity.hpp
// Author: crdrisko
// Date: 06/16/2024-05:49:03
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_FORCEGENERATORS_GRAVITY_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_FORCEGENERATORS_GRAVITY_HPP

#include "dryphys/math/vector.hpp"
#include "dryphys/particleSystems/forceGenerator.hpp"
#include "dryphys/types/particle.hpp"

namespace DryPhys
{
    /*!
     * A \c ParticleForceGenerator responsible for modeling a particle's gravitational force or weight
     *  according to the following: 
     *      \f$\textbf{f}_{\textrm{gravity}} = m \textbf{g}.\f$
     */
    class ParticleGravity : public ParticleForceGenerator
    {
    private:
        Vector3D gravity_ {0, -9.81, 0};

    public:
        ParticleGravity() = default;
        explicit ParticleGravity(const Vector3D& gravity) : gravity_ {gravity} {}

        void updateForce(Particle* particle, real duration) override;
    };
}   // namespace DryPhys

#endif
