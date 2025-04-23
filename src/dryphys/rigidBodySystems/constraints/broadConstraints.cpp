// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: broadConstraints.cpp
// Author: crdrisko
// Date: 12/29/2024-05:09:47
// Description:

#include "dryphys/rigidBodySystems/constraints/broadConstraints.hpp"

#include <cmath>

#include "dryphys/math/vector.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    BoundingBox::BoundingBox(const BoundingBox& one, const BoundingBox& two) {}

    BoundingSphere::BoundingSphere(const BoundingSphere& one, const BoundingSphere& two)
    {
        Vector3D centerOffset = two.center_ - one.center_;

        real distance   = centerOffset.magnitudeSquared();
        real radiusDiff = two.radius_ - one.radius_;

        if (radiusDiff * radiusDiff >= distance)
        {
            if (one.radius_ > two.radius_)
            {
                center_ = one.center_;
                radius_ = one.radius_;
            }
            else
            {
                center_ = two.center_;
                radius_ = two.radius_;
            }
        }
        else
        {
            distance = std::sqrt(distance);
            radius_  = (distance + one.radius_ + two.radius_) * static_cast<real>(0.5);

            center_ = one.center_;
            if (distance > 0)
            {
                center_ += centerOffset * ((radius_ - one.radius_) / distance);
            }
        }
    }

    real BoundingSphere::getGrowth(const BoundingSphere& other) const
    {
        BoundingSphere newSphere {*this, other};

        return newSphere.radius_ * newSphere.radius_ - radius_ * radius_;
    }
}   // namespace DryPhys
