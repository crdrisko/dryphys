// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: velocityVerlet.hpp
// Author: crdrisko
// Date: 06/17/2024-06:28:41
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_INTEGRATORS_VELOCITYVERLET_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_INTEGRATORS_VELOCITYVERLET_HPP

#include <config.h>

#include "dryphys/particle.hpp"
#include "dryphys/particleSystems/integrator.hpp"

namespace DryPhys
{
    class VelocityVerlet : public Integrator
    {
    public:
        VelocityVerlet() = default;

        void integrate(Particle* particle, real timestep) override;
    };
}   // namespace DryPhys

#endif
