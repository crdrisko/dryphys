// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particleDrag.cpp
// Author: crdrisko
// Date: 06/16/2024-05:53:32
// Description:

#include "dryphys/particleSystems/forceGenerators/particleDrag.hpp"

#include <config.h>

#include "dryphys/math/vector3d.hpp"
#include "dryphys/particle.hpp"

namespace DryPhys
{
    void ParticleDrag::updateForce(Particle* particle, real)
    {
        Vector3D force;
        particle->getVelocity(&force);

        real dragCoefficient = force.magnitude();
        dragCoefficient      = k1_ * dragCoefficient + k2_ * dragCoefficient * dragCoefficient;

        force.normalize();
        force *= -dragCoefficient;

        particle->addForce(force);
    }
}   // namespace DryPhys
