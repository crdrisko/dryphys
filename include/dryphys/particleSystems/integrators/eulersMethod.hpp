// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: eulersMethod.hpp
// Author: crdrisko
// Date: 06/17/2024-06:29:05
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_INTEGRATORS_EULERSMETHOD_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_INTEGRATORS_EULERSMETHOD_HPP

#include "dryphys/config.h"
#include "dryphys/particle.hpp"
#include "dryphys/particleSystems/integrator.hpp"

namespace DryPhys
{
    class EulersMethod : public Integrator
    {
    private:
        real damping_;

    public:
        explicit EulersMethod(real damping = 1.0f) : damping_ {damping} {}

        void integrate(std::vector<Particle>& particles, real timestep) override;
    };
}   // namespace DryPhys

#endif
