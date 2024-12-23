// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: aero.cpp
// Author: crdrisko
// Date: 12/14/2024-06:30:01
// Description:

#include "dryphys/rigidBodySystems/forceGenerators/aero.hpp"

#include "dryphys/types/rigidBody.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    void Aero::updateForce(RigidBody* rigidBody, real duration)
    {
        Aero::updateForceFromTensor(rigidBody, duration, tensor_);
    }

    void Aero::updateForceFromTensor(RigidBody* rigidBody, real, const Matrix3D& tensor)
    {
        Vector3D velocity = rigidBody->getVelocity();
        velocity += *windspeed_;

        Vector3D bodyVelocity = rigidBody->getTransform().transformInverseDirection(velocity);

        Vector3D bodyForce = tensor.transform(bodyVelocity);
        Vector3D force     = rigidBody->getTransform().transformDirection(bodyForce);

        rigidBody->addForceAtBodyPoint(force, position_);
    }

    Matrix3D AeroControl::getTensor()
    {
        if (controlSetting_ <= -1.0f)
            return minTensor_;
        else if (controlSetting_ >= 1.0f)
            return maxTensor_;
        else if (controlSetting_ < 0)
            return Matrix3D::linearInterpolate(minTensor_, tensor_, controlSetting_ + 1.0f);
        else if (controlSetting_ > 0)
            return Matrix3D::linearInterpolate(tensor_, maxTensor_, controlSetting_);
        else
            return tensor_;
    }

    void AeroControl::updateForce(RigidBody* rigidBody, real duration)
    {
        Matrix3D tensor = getTensor();
        Aero::updateForceFromTensor(rigidBody, duration, tensor);
    }
}   // namespace DryPhys
