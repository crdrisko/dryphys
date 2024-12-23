// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: forceGenerator.hpp
// Author: crdrisko
// Date: 12/12/2024-06:04:10
// Description:

#ifndef FORCEGENERATOR_HPP
#define FORCEGENERATOR_HPP

#include <vector>

#include "dryphys/math/vector.hpp"
#include "dryphys/types/rigidBody.hpp"

namespace DryPhys
{
    /*!
     *
     */
    class RigidBodyForceGenerator
    {
    public:
        virtual ~RigidBodyForceGenerator() = default;

        virtual void updateForce(RigidBody* rigidBody, real duration) = 0;
    };

    /*!
     *
     */
    class RigidBodyForceRegistry
    {
    protected:
        struct RigidBodyForceRegistration
        {
            RigidBody* rigidBody;
            RigidBodyForceGenerator* fg;
        };

        using Registry = std::vector<RigidBodyForceRegistration>;
        Registry registrations_;

    public:
        void updateForces(real duration);

        void add(RigidBody* rigidBody, RigidBodyForceGenerator* fg);
        void remove(RigidBody* rigidBody, RigidBodyForceGenerator* fg);

        void clear() { registrations_.clear(); }
    };
}   // namespace DryPhys

#endif
