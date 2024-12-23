// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particleBuoyancy.hpp
// Author: crdrisko
// Date: 06/23/2024-08:12:49
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_FORCEGENERATORS_PARTICLEBUOYANCY_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_FORCEGENERATORS_PARTICLEBUOYANCY_HPP

#include "dryphys/math/vector.hpp"
#include "dryphys/types/particle.hpp"
#include "dryphys/particleSystems/forceGenerator.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    /*!
     * A \c ParticleForceGenerator responsible for modeling a buoyancy force acting
     *  on a particle according to the following:
     *      \f{equation}\textbf{f}_{\textrm{buoyancy}} = 
     *          \begin{cases}
     *              0, & d \le 0 \\
     *              v\rho, & d \ge 1 \\
     *              dv\rho, & \textrm{otherwise}
     *          \end{cases}.
     *      \f}
     */
    class ParticleBuoyancy : public ParticleForceGenerator
    {
    private:
        real maxDepth_;
        real volume_;
        real waterHeight_;
        real liquidDensity_;

    public:
        ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.0f)
            : maxDepth_ {maxDepth}, volume_ {volume}, waterHeight_ {waterHeight}, liquidDensity_ {liquidDensity}
        {
        }

        void updateForce(Particle* particle, real duration) override;
    };
}   // namespace DryPhys

#endif
