// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: eulersMethod.hpp
// Author: crdrisko
// Date: 06/17/2024-06:29:05
// Description:

#ifndef EULERSMETHOD_HPP
#define EULERSMETHOD_HPP

#include <config.h>

#include <cassert>

#include "dryphys/integrator.hpp"
#include "dryphys/particle.hpp"

namespace DryPhys
{
    class EulersMethod : public Integrator
    {
    private:
        real damping_;

    public:
        explicit EulersMethod(real damping = 1.0f) : damping_ {damping} {}

        void integrate(Particle* particle, real timestep) override;
    };
}   // namespace DryPhys

#endif
