// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: rigidBody.hpp
// Author: crdrisko
// Date: 12/11/2024-06:38:33
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_RIGIDBODY_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_RIGIDBODY_HPP

#include <cassert>
#include <limits>

#include "dryphys/math/matrix.hpp"
#include "dryphys/math/quaternion.hpp"
#include "dryphys/math/vector.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    extern real sleepEpsilon;

    void setSleepEpsilon(real value);
    real getSleepEpsilon();

    class RigidBody
    {
    protected:
        Vector3D position_;                    //! Linear position of the rigid body in world space
        Vector3D velocity_;                    //! Linear velocity of the rigid body in world space
        Vector3D rotation_;                    //! Angular velocity of the rigid body in world space
        Vector3D acceleration_;                //! Acceleration of the rigid body
        Vector3D forceAccumulator_;            //! Accumulated forces acting on this rigid body
        Vector3D torqueAccumulator_;           //! Accumulated forces acting on this rigid body
        Matrix3D inverseInertiaTensor_;        //! Inverse inertia tensor of the rigid body in body space
        Matrix3D inverseInertiaTensorWorld_;   //! Inverse inertia tensor of the rigid body in world space
        Quaternion orientation_;               //! Angular orientation of the rigid body in world space
        Transform4D transformMatrix_;          //! Transformation matrix for converting between body and world space

        real linearDamping_ {};    //! Damping applied to linear motion to adjust for numerical instability
        real angularDamping_ {};   //! Damping applied to angular motion to adjust for numerical instability
        real inverseMass_ {};      //! Inverse mass which can be zero (immovable object) and can not be infinite (massless)

        real motion_ {};
        bool isAwake_ {}, canSleep_ {};

    public:
        void integrate(real duration);
        void calculateDerivedData();
        void addForce(const Vector3D& force);
        void addForceAtPoint(const Vector3D& force, const Vector3D& point);
        void addForceAtBodyPoint(const Vector3D& force, const Vector3D& point);
        void addTorque(const Vector3D& torque);
        void clearAccumulators();

        constexpr bool hasFiniteMass() const { return inverseMass_ > 0; }
        constexpr Vector3D getPointInWorldSpace(const Vector3D& point) const { return transformMatrix_.transform(point); }

        //! Perform movement in one step rather than calling get and set positions
        constexpr void move(const Vector3D& offset) { position_ += offset; }
        constexpr void move(real x, real y, real z) { position_ += Vector3D {x, y, z}; }

        //! Perform velocity kick in one step rather than calling get and set velocities
        constexpr void kick(const Vector3D& offset) { velocity_ += offset; }
        constexpr void kick(real x, real y, real z) { velocity_ += Vector3D {x, y, z}; }

        //! Perform rotational kick in one step rather than calling get and set rotations
        constexpr void rotate(const Vector3D& offset) { rotation_ += offset; }
        constexpr void rotate(real x, real y, real z) { rotation_ += Vector3D {x, y, z}; }

        //! Accessors
        constexpr void setPosition(const Vector3D& position) { position_ = position; }
        constexpr void setVelocity(const Vector3D& velocity) { velocity_ = velocity; }
        constexpr void setRotation(const Vector3D& rotation) { rotation_ = rotation; }
        constexpr void setAcceleration(const Vector3D& acceleration) { acceleration_ = acceleration; }
        constexpr void setPosition(real x, real y, real z) { position_ = Vector3D {x, y, z}; }
        constexpr void setVelocity(real x, real y, real z) { velocity_ = Vector3D {x, y, z}; }
        constexpr void setRotation(real x, real y, real z) { rotation_ = Vector3D {x, y, z}; }
        constexpr void setAcceleration(real x, real y, real z) { acceleration_ = Vector3D {x, y, z}; }
        constexpr void setLinearDamping(real linearDamping) { linearDamping_ = linearDamping; }
        constexpr void setAngularDamping(real angularDamping) { angularDamping_ = angularDamping; }
        constexpr void setInverseInertiaTensor(const Matrix3D& iitBody) { inverseInertiaTensor_ = iitBody; }
        constexpr void setInverseMass(real inverseMass) { inverseMass_ = inverseMass; }
        constexpr void setInertiaTensor(const Matrix3D& inertiaTensor);
        constexpr void setMass(real mass);
        void setAwake(bool awake = true);
        void setCanSleep(const bool canSleep = true);
        void setOrientation(const Quaternion& orientation);
        void setOrientation(real r, real i, real j, real k);

        constexpr void getPosition(Vector3D* position) const { *position = position_; }
        constexpr void getVelocity(Vector3D* velocity) const { *velocity = velocity_; }
        constexpr void getRotation(Vector3D* rotation) const { *rotation = rotation_; }
        constexpr void getAcceleration(Vector3D* acceleration) const { *acceleration = acceleration_; }
        constexpr void getOrientation(Quaternion* orientation) const { *orientation = orientation_; }
        constexpr void getInverseInertiaTensor(Matrix3D* iitBody) const { *iitBody = inverseInertiaTensor_; }
        constexpr void getInverseInertiaTensorWorld(Matrix3D* iitWorld) const { *iitWorld = inverseInertiaTensorWorld_; }
        constexpr void getInertiaTensor(Matrix3D* inertiaTensor) const;
        constexpr void getInertiaTensorWorld(Matrix3D* inertiaTensor) const;
        void getTransform(Transform4D* transform) const;

        constexpr Vector3D getPosition() const { return position_; }
        constexpr Vector3D getVelocity() const { return velocity_; }
        constexpr Vector3D getRotation() const { return rotation_; }
        constexpr Vector3D getAcceleration() const { return acceleration_; }
        constexpr Quaternion getOrientation() const { return orientation_; }
        constexpr Matrix3D getInverseInertiaTensor() const { return inverseInertiaTensor_; }
        constexpr Matrix3D getInverseInertiaTensorWorld() const { return inverseInertiaTensorWorld_; }
        constexpr Transform4D getTransform() const { return transformMatrix_; }
        constexpr real getInverseMass() const { return inverseMass_; }
        constexpr real getLinearDamping() const { return linearDamping_; }
        constexpr real getAngularDamping() const { return angularDamping_; }
        constexpr Matrix3D getInertiaTensor() const;
        constexpr Matrix3D getInertiaTensorWorld() const;
        constexpr real getMass() const;
        constexpr bool getAwake() const { return isAwake_; }
        constexpr bool getCanSleep() const { return canSleep_; }
    };

    //! Accessors for the inertia tensor in local/body coordinates
    constexpr void RigidBody::setInertiaTensor(const Matrix3D& inertiaTensor)
    {
        inverseInertiaTensor_.setInverse(inertiaTensor);
    }

    constexpr void RigidBody::getInertiaTensor(Matrix3D* inertiaTensor) const
    {
        inertiaTensor->setInverse(inverseInertiaTensor_);
    }

    constexpr Matrix3D RigidBody::getInertiaTensor() const
    {
        Matrix3D it;
        getInertiaTensor(&it);
        return it;
    }

    //! Accessors for the inertia tensor in world coordinates
    constexpr void RigidBody::getInertiaTensorWorld(Matrix3D* inertiaTensor) const
    {
        inertiaTensor->setInverse(inverseInertiaTensorWorld_);
    }

    constexpr Matrix3D RigidBody::getInertiaTensorWorld() const
    {
        Matrix3D it;
        getInertiaTensorWorld(&it);
        return it;
    }

    //! Accessors for the mass
    constexpr void RigidBody::setMass(real mass)
    {
        assert(mass != 0.0f);
        inverseMass_ = static_cast<real>(1.0f) / mass;
    }

    constexpr real RigidBody::getMass() const
    {
        if (!hasFiniteMass())
            return std::numeric_limits<real>::max();
        else
            return static_cast<real>(1.0f) / inverseMass_;
    }
}   // namespace DryPhys

#endif
