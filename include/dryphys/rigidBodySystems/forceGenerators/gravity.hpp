// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: gravity.hpp
// Author: crdrisko
// Date: 12/12/2024-06:13:18
// Description:

#ifndef GRAVITY_HPP
#define GRAVITY_HPP

#include "dryphys/math/vector.hpp"
#include "dryphys/types/rigidBody.hpp"
#include "dryphys/rigidBodySystems/forceGenerator.hpp"

namespace DryPhys
{
    /*!
     * A \c RigidBodyForceGenerator responsible for modeling a rigid body's gravitational force or weight
     *  according to the following: 
     *      \f$\textbf{f}_{\textrm{gravity}} = m \textbf{g}.\f$
     */
    class RigidBodyGravity : public RigidBodyForceGenerator
    {
    private:
        Vector3D gravity_;

    public:
        explicit RigidBodyGravity(const Vector3D& gravity) : gravity_ {gravity} {}

        void updateForce(RigidBody* rigidBody, real duration) override;
    };
}   // namespace DryPhys

#endif
