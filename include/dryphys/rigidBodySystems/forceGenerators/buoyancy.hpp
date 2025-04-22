// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: buoyancy.hpp
// Author: crdrisko
// Date: 12/27/2024-10:26:39
// Description:

#ifndef BUOYANCY_HPP
#define BUOYANCY_HPP

#include "dryphys/math/vector.hpp"
#include "dryphys/rigidBodySystems/forceGenerator.hpp"
#include "dryphys/types/rigidBody.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    class RigidBodyBuoyancy : public RigidBodyForceGenerator
    {
        real maxDepth_;
        real volume_;
        real waterHeight_;
        real liquidDensity_;

        Vector3D centerOfBuoyancy_;

    public:
        RigidBodyBuoyancy(
            const Vector3D& centerOfBuoyancy, real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.0);

        void updateForce(RigidBody* rigidBody, real duration) override;
    };
}   // namespace DryPhys

#endif
