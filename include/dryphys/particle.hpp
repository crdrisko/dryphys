// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particle.hpp
// Author: crdrisko
// Date: 06/13/2024-12:36:43
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLE_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLE_HPP

#include "dryphys/config.h"
#include "dryphys/math/vector3d.hpp"

namespace DryPhys
{
    class Particle
    {
    private:
        Vector3D position_;           //! Linear position of the particle in world space
        Vector3D velocity_;           //! Linear velocity of the particle in world space
        Vector3D acceleration_;       //! Acceleration of the particle
        Vector3D forceAccumulator_;   //! Accumulated forces acting on this particle

        real damping_ {};       //! Damping applied to linear motion to adjust for numerical instability
        real inverseMass_ {};   //! Inverse mass which can be zero (immovable object) and can not be infinite (massless)

    public:
        void integrate(real duration);

        constexpr bool hasFiniteMass() const { return inverseMass_ > 0; }
        constexpr void addForce(const Vector3D& force) { forceAccumulator_ += force; }
        constexpr void applyForces() { acceleration_ = forceAccumulator_ * inverseMass_; }
        constexpr void clearAccumulator() { forceAccumulator_.clear(); }

        //! Perform movement in one step rather than calling get and set positions
        constexpr void move(const Vector3D& offset) { position_ += offset; }
        constexpr void move(real x, real y, real z) { position_ += Vector3D {x, y, z}; }

        //! Perform velocity kick in one step rather than calling get and set velocities
        constexpr void kick(const Vector3D& offset) { velocity_ += offset; }
        constexpr void kick(real x, real y, real z) { velocity_ += Vector3D {x, y, z}; }

        //! Accessors
        constexpr void setPosition(const Vector3D& position) { position_ = position; }
        constexpr void setVelocity(const Vector3D& velocity) { velocity_ = velocity; }
        constexpr void setAcceleration(const Vector3D& acceleration) { acceleration_ = acceleration; }
        constexpr void setPosition(real x, real y, real z) { position_ = Vector3D {x, y, z}; }
        constexpr void setVelocity(real x, real y, real z) { velocity_ = Vector3D {x, y, z}; }
        constexpr void setAcceleration(real x, real y, real z) { acceleration_ = Vector3D {x, y, z}; }
        constexpr void setDamping(real damping) { damping_ = damping; }
        constexpr void setInverseMass(real inverseMass) { inverseMass_ = inverseMass; }
        void setMass(real mass);

        constexpr void getPosition(Vector3D* position) const { *position = position_; }
        constexpr void getVelocity(Vector3D* velocity) const { *velocity = velocity_; }
        constexpr void getAcceleration(Vector3D* acceleration) const { *acceleration = acceleration_; }
        constexpr Vector3D getPosition() const { return position_; }
        constexpr Vector3D getVelocity() const { return velocity_; }
        constexpr Vector3D getAcceleration() const { return acceleration_; }
        constexpr real getDamping() const { return damping_; }
        constexpr real getInverseMass() const { return inverseMass_; }
        real getMass() const;
    };
}   // namespace DryPhys

#endif
