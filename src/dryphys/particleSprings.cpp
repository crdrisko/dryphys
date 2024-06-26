// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particleSpring.cpp
// Author: crdrisko
// Date: 06/22/2024-06:44:57
// Description:

#include "dryphys/forceGenerators/particleSprings.hpp"

#include <config.h>

#include "dryphys/particle.hpp"
#include "dryphys/vector3d.hpp"

namespace DryPhys
{
    void ParticleSpring::updateForce(Particle* particle, real)
    {
        Vector3D force;
        particle->getPosition(&force);
        force -= other_->getPosition();

        real magnitude = hookesLaw(force, restLength_, springConstant_);

        // real magnitude = force.magnitude();
        // magnitude      = std::abs(magnitude - restLength_);
        // magnitude *= springConstant_;

        force.normalize();
        force *= -magnitude;

        particle->addForce(force);
    }

    void ParticleAnchoredSpring::updateForce(Particle* particle, real)
    {
        Vector3D force;
        particle->getPosition(&force);
        force -= *anchor_;

        real magnitude = hookesLaw(force, restLength_, springConstant_);

        // real magnitude = force.magnitude();
        // magnitude      = (restLength_ - magnitude) * springConstant_;

        force.normalize();
        // force *= magnitude;
        force *= -magnitude;


        particle->addForce(force);
    }

    void ParticleBungee::updateForce(Particle* particle, real)
    {
        Vector3D force;
        particle->getPosition(&force);
        force -= other_->getPosition();

        // real magnitude = force.magnitude();

        // if (magnitude <= restLength_)
        //     force.magnitude()

        // magnitude = (restLength_ - magnitude) * springConstant_;

        if (force.magnitude() <= restLength_)
            return;

        real magnitude = hookesLaw(force, restLength_, springConstant_);

        force.normalize();
        force *= -magnitude;

        particle->addForce(force);
    }
}   // namespace DryPhys
