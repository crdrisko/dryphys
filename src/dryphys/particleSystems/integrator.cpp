// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: integrator.cpp
// Author: crdrisko
// Date: 06/17/2024-07:01:28
// Description:

#include "dryphys/particleSystems/integrator.hpp"

#include <config.h>

#include <vector>

#include "dryphys/particle.hpp"

namespace DryPhys
{
    void IntegratorRegistry::integrate(real duration)
    {
        for (auto [particle, integrator] : registrations_)
        {
            integrator->integrate(particle, duration);
        }
    }

    void IntegratorRegistry::add(Particle* particle, Integrator* integrator)
    {
        registrations_.emplace_back(IntegratorRegistration {particle, integrator});
    }

    void IntegratorRegistry::remove(Particle* particle, Integrator* integrator)
    {
        // clang-format off
        registrations_.erase(
            std::remove_if(registrations_.begin(), registrations_.end(),
                [particle, integrator](const auto& reg) {
                    return (reg.particle == particle) && (reg.integrator == integrator);
                }),
            registrations_.end());
        // clang-format on
    }
}   // namespace DryPhys
