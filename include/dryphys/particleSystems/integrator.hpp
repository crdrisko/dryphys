// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: integrator.hpp
// Author: crdrisko
// Date: 06/17/2024-06:50:26
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_INTEGRATOR_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_INTEGRATOR_HPP

#include <vector>

#include "dryphys/config.h"
#include "dryphys/particle.hpp"
#include "dryphys/particleSystems/forceGenerator.hpp"

namespace DryPhys
{
    /*!
     *
     */
    class Integrator
    {
    protected:
        ParticleForceRegistry registry_;

    public:
        virtual ~Integrator() = default;

        virtual void integrate(std::vector<Particle>& particles, real timestep) = 0;

        ParticleForceRegistry& getForceRegistry() { return registry_; }
        const ParticleForceRegistry& getForceRegistry() const { return registry_; }
    };
}   // namespace DryPhys

#endif
