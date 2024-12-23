// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: springs.hpp
// Author: crdrisko
// Date: 12/12/2024-11:05:54
// Description:

#ifndef SPRINGS_HPP
#define SPRINGS_HPP

#include "dryphys/math/vector.hpp"
#include "dryphys/types/rigidBody.hpp"
#include "dryphys/rigidBodySystems/forceGenerator.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    /*!
     * A \c RigidBodyForceGenerator responsible for modeling two rigid bodies connected via a spring. The
     *  spring is modeled using Hooke's Law:
     *      \f$\textbf{f}_{\textrm{spring}} = k (|\textbf{r}| - r_0)\hat{\textbf{r}}.\f$
     */
    class Spring : public RigidBodyForceGenerator
    {
    private:
        Vector3D connectionPoint_;
        Vector3D otherConnectionPoint_;

        RigidBody* other_;
        real springConstant_;
        real restLength_;

    public:
        Spring(const Vector3D& connectionPoint,
            RigidBody* other,
            const Vector3D& otherConnectionPoint,
            real springConstant,
            real restLength)
            : connectionPoint_ {connectionPoint}, otherConnectionPoint_ {otherConnectionPoint}, other_ {other},
              springConstant_ {springConstant}, restLength_ {restLength}
        {
        }

        void updateForce(RigidBody* rigidBody, real duration) override;
    };
}   // namespace DryPhys

#endif
