// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: springs.cpp
// Author: crdrisko
// Date: 12/12/2024-11:06:07
// Description:

#include "dryphys/rigidBodySystems/forceGenerators/springs.hpp"

#include <cmath>

#include "dryphys/math/vector.hpp"
#include "dryphys/types/rigidBody.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    inline real hookesLaw(const Vector3D& r, real r_0, real k)
    {
        real force = k * (r.magnitude() - r_0);

        return force;
    }

    void Spring::updateForce(RigidBody* rigidBody, real)
    {
        Vector3D lws = rigidBody->getPointInWorldSpace(connectionPoint_);
        Vector3D ows = other_->getPointInWorldSpace(otherConnectionPoint_);

        Vector3D force = lws - ows;

        real magnitude = hookesLaw(force, restLength_, springConstant_);

        // real magnitude = force.magnitude();
        // magnitude      = std::abs(magnitude - restLength_);
        // magnitude *= springConstant_;

        force.normalize();
        force *= -magnitude;

        rigidBody->addForceAtPoint(force, lws);
    }
}   // namespace DryPhys
