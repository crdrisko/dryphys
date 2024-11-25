// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particleDrag.hpp
// Author: crdrisko
// Date: 06/16/2024-05:48:52
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_FORCEGENERATORS_PARTICLEDRAG_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_FORCEGENERATORS_PARTICLEDRAG_HPP

#include "dryphys/config.h"
#include "dryphys/particle.hpp"
#include "dryphys/particleSystems/forceGenerator.hpp"

namespace DryPhys
{
    /*!
     * A \c ParticleForceGenerator responsible for modeling simple drag. Here we use a simplified drag equation
     *  based on the velocities according to the following: 
     *      \f$\textbf{f}_{\textrm{drag}} = - \hat{\dot{\textbf{p}}} \left( k_1 |\hat{\dot{\textbf{p}}}| + k_2 |\hat{\dot{\textbf{p}}}|^2 \right).\f$
     */
    class ParticleDrag : public ParticleForceGenerator
    {
    private:
        real k1_;
        real k2_;

    public:
        ParticleDrag(real k1, real k2) : k1_ {k1}, k2_ {k2} {}

        void updateForce(Particle* particle, real duration) override;
    };
}   // namespace DryPhys

#endif
