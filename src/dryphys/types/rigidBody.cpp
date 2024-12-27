// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: rigidBody.cpp
// Author: crdrisko
// Date: 12/11/2024-06:50:27
// Description:

#include "dryphys/types/rigidBody.hpp"

#include <cmath>
#include <cstring>

#include "dryphys/math/matrix.hpp"
#include "dryphys/math/quaternion.hpp"
#include "dryphys/math/vector.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    real sleepEpsilon = static_cast<real>(0.1);

    void setSleepEpsilon(real value) { sleepEpsilon = value; }
    real getSleepEpsilon() { return sleepEpsilon; }

    namespace details
    {
        static void calculateTransformMatrix(Transform4D& transform, const Vector3D& position, Quaternion& orientation)
        {
            real x2 = orientation.x * orientation.x;
            real y2 = orientation.y * orientation.y;
            real z2 = orientation.z * orientation.z;
            real xy = orientation.x * orientation.y;
            real xz = orientation.x * orientation.z;
            real yz = orientation.y * orientation.z;
            real wx = orientation.w * orientation.x;
            real wy = orientation.w * orientation.y;
            real wz = orientation.w * orientation.z;

            transform.data[0]  = 1.0f - 2.0f * (y2 + z2);
            transform.data[1]  = 2.0f * (xy - wz);
            transform.data[2]  = 2.0f * (xz + wy);
            transform.data[3]  = position.x;
            transform.data[4]  = 2.0f * (xy + wz);
            transform.data[5]  = 1.0f - 2.0f * (x2 + z2);
            transform.data[6]  = 2.0f * (yz - wx);
            transform.data[7]  = position.y;
            transform.data[8]  = 2.0f * (xz - wy);
            transform.data[9]  = 2.0f * (yz + wx);
            transform.data[10] = 1.0f - 2.0f * (x2 + y2);
            transform.data[11] = position.z;
        }

        static void transformInertiaTensor(Matrix3D& iitWorld, const Matrix3D& iitBody, const Transform4D& rotmat)
        {
            real t4 = rotmat.data[0] * iitBody.data[0] + rotmat.data[1] * iitBody.data[3] + rotmat.data[2] * iitBody.data[6];
            real t9 = rotmat.data[0] * iitBody.data[1] + rotmat.data[1] * iitBody.data[4] + rotmat.data[2] * iitBody.data[7];
            real t14
                = rotmat.data[0] * iitBody.data[2] + rotmat.data[1] * iitBody.data[5] + rotmat.data[2] * iitBody.data[8];
            real t28
                = rotmat.data[4] * iitBody.data[0] + rotmat.data[5] * iitBody.data[3] + rotmat.data[6] * iitBody.data[6];
            real t33
                = rotmat.data[4] * iitBody.data[1] + rotmat.data[5] * iitBody.data[4] + rotmat.data[6] * iitBody.data[7];
            real t38
                = rotmat.data[4] * iitBody.data[2] + rotmat.data[5] * iitBody.data[5] + rotmat.data[6] * iitBody.data[8];
            real t52
                = rotmat.data[8] * iitBody.data[0] + rotmat.data[9] * iitBody.data[3] + rotmat.data[10] * iitBody.data[6];
            real t57
                = rotmat.data[8] * iitBody.data[1] + rotmat.data[9] * iitBody.data[4] + rotmat.data[10] * iitBody.data[7];
            real t62
                = rotmat.data[8] * iitBody.data[2] + rotmat.data[9] * iitBody.data[5] + rotmat.data[10] * iitBody.data[8];

            iitWorld.data[0] = t4 * rotmat.data[0] + t9 * rotmat.data[1] + t14 * rotmat.data[2];
            iitWorld.data[1] = t4 * rotmat.data[4] + t9 * rotmat.data[5] + t14 * rotmat.data[6];
            iitWorld.data[2] = t4 * rotmat.data[8] + t9 * rotmat.data[9] + t14 * rotmat.data[10];
            iitWorld.data[3] = t28 * rotmat.data[0] + t33 * rotmat.data[1] + t38 * rotmat.data[2];
            iitWorld.data[4] = t28 * rotmat.data[4] + t33 * rotmat.data[5] + t38 * rotmat.data[6];
            iitWorld.data[5] = t28 * rotmat.data[8] + t33 * rotmat.data[9] + t38 * rotmat.data[10];
            iitWorld.data[6] = t52 * rotmat.data[0] + t57 * rotmat.data[1] + t62 * rotmat.data[2];
            iitWorld.data[7] = t52 * rotmat.data[4] + t57 * rotmat.data[5] + t62 * rotmat.data[6];
            iitWorld.data[8] = t52 * rotmat.data[8] + t57 * rotmat.data[9] + t62 * rotmat.data[10];
        }
    }   // namespace details

    void RigidBody::integrate(real duration)
    {
        if (!isAwake_)
            return;

        Vector3D resultingLinearAcc = acceleration_;
        resultingLinearAcc += forceAccumulator_ * inverseMass_;

        Vector3D resultingAngularAcc = inverseInertiaTensorWorld_.transform(torqueAccumulator_);

        velocity_ += resultingLinearAcc * duration;
        rotation_ += resultingAngularAcc * duration;

        velocity_ *= std::pow(linearDamping_, duration);
        rotation_ *= std::pow(angularDamping_, duration);

        position_ += velocity_ * duration;

        Quaternion q {rotation_ * duration, 0};
        q *= orientation_;

        orientation_.w += q.w * static_cast<real>(0.5);
        orientation_.x += q.x * static_cast<real>(0.5);
        orientation_.y += q.y * static_cast<real>(0.5);
        orientation_.z += q.z * static_cast<real>(0.5);

        calculateDerivedData();

        clearAccumulators();
    }

    void RigidBody::calculateDerivedData()
    {
        orientation_.normalize();

        // Calculate the transformation matrix for the rigid body
        details::calculateTransformMatrix(transformMatrix_, position_, orientation_);

        // Calculate the inertia tensor in world space
        details::transformInertiaTensor(inverseInertiaTensorWorld_, inverseInertiaTensor_, transformMatrix_);
    }

    void RigidBody::addForce(const Vector3D& force)
    {
        forceAccumulator_ += force;
        isAwake_ = true;
    }

    void RigidBody::addForceAtPoint(const Vector3D& force, const Vector3D& point)
    {
        // Convert to coordinates relative to center of mass
        Vector3D pt = point;
        pt -= position_;

        forceAccumulator_ += force;
        torqueAccumulator_ += pt.cross(force);

        isAwake_ = true;
    }

    void RigidBody::addForceAtBodyPoint(const Vector3D& force, const Vector3D& point)
    {
        // Convert to coordinates relative to center of mass
        Vector3D pt = getPointInWorldSpace(point);
        addForceAtPoint(force, pt);

        isAwake_ = true;
    }

    void RigidBody::addTorque(const Vector3D& torque)
    {
        torqueAccumulator_ += torque;
        isAwake_ = true;
    }

    void RigidBody::clearAccumulators()
    {
        forceAccumulator_.clear();
        torqueAccumulator_.clear();
    }

    void RigidBody::setOrientation(const Quaternion& orientation)
    {
        orientation_ = orientation;
        orientation_.normalize();
    }

    void RigidBody::setOrientation(real r, real i, real j, real k)
    {
        orientation_ = Quaternion {r, i, j, k};
        orientation_.normalize();
    }

    void RigidBody::setAwake(bool awake)
    {
        if (awake)
        {
            isAwake_ = true;

            // Add a bit of motion to avoid it falling asleep immediately.
            motion_ = sleepEpsilon * 2.0f;
        }
        else
        {
            isAwake_ = false;
            velocity_.clear();
            rotation_.clear();
        }
    }

    void RigidBody::setCanSleep(bool canSleep)
    {
        canSleep_ = canSleep;

        if (!canSleep && !isAwake_)
            setAwake();
    }

    void RigidBody::getTransform(Transform4D* transform) const
    {
        std::memcpy(transform, transformMatrix_.data, sizeof(Transform4D));
    }
}   // namespace DryPhys
