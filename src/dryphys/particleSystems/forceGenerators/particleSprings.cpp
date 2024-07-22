// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particleSpring.cpp
// Author: crdrisko
// Date: 06/22/2024-06:44:57
// Description:

#include "dryphys/particleSystems/forceGenerators/particleSprings.hpp"

#include <config.h>

#include <cmath>

#include "dryphys/math/vector3d.hpp"
#include "dryphys/particle.hpp"

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

    void ParticleFakeSpring::updateForce(Particle* particle, real duration)
    {
        // Check that we do not have infinite mass
        if (!particle->hasFiniteMass())
            return;

        // Calculate the relative position of the particle to the anchor
        Vector3D position;
        particle->getPosition(&position);
        position -= *anchor_;

        // Calculate the constants and check that they are in bounds
        real gamma = 0.5f * std::sqrt(static_cast<real>(4) * springConstant_ - damping_ * damping_);

        if (gamma == 0.0f)
            return;

        Vector3D c = position * (damping_ / (2.0f * gamma)) + particle->getVelocity() * (1.0f / gamma);

        // Calculate the target position
        Vector3D target = position * std::cos(gamma * duration) + c * std::sin(gamma * duration);
        target *= std::exp(-0.5f * duration * damping_);

        // Calculate the resulting acceleration, and therefore the force
        Vector3D acceleration = (target - position) * (1.0f / (duration * duration)) - particle->getVelocity() * duration;

        particle->addForce(acceleration * particle->getMass());
    }
}   // namespace DryPhys
