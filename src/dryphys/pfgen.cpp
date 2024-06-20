// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: pfgen.cpp
// Author: crdrisko
// Date: 06/15/2024-06:20:12
// Description:

#include "dryphys/pfgen.hpp"

#include <config.h>

#include "dryphys/particle.hpp"

namespace DryPhys
{
    void ParticleForceRegistry::updateForces(real duration)
    {
        for (auto [particle, fg] : registrations_)
        {
            fg->updateForce(particle, duration);
        }
    }

    void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
    {
        registrations_.emplace_back(ParticleForceRegistration {particle, fg});
    }

    void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg)
    {
        registrations_.erase(std::remove_if(registrations_.begin(),
                                 registrations_.end(),
                                 [particle, fg](auto reg) { return reg.particle == particle && reg.fg == fg; }),
            registrations_.end());
    }
}   // namespace DryPhys
