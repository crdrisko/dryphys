// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particle.hpp
// Author: crdrisko
// Date: 06/13/2024-12:36:43
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLE_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLE_HPP

#include <config.h>

#include "dryphys/vector3d.hpp"

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

        DRYPHYS_CONSTEXPR bool hasFiniteMass() const { return inverseMass_ > 0; }
        DRYPHYS_CONSTEXPR void addForce(const Vector3D& force) { forceAccumulator_ += force; }
        DRYPHYS_CONSTEXPR void clearAccumulator() { forceAccumulator_.clear(); }

        //! Perform movement in one step rather than calling get and set positions
        DRYPHYS_CONSTEXPR void move(const Vector3D& offset) { position_ += offset; }
        DRYPHYS_CONSTEXPR void move(real x, real y, real z) { position_ += Vector3D {x, y, z}; }

        //! Perform velocity kick in one step rather than calling get and set velocities
        DRYPHYS_CONSTEXPR void kick(const Vector3D& offset) { velocity_ += offset; }
        DRYPHYS_CONSTEXPR void kick(real x, real y, real z) { velocity_ += Vector3D {x, y, z}; }

        //! Accessors
        DRYPHYS_CONSTEXPR void setPosition(const Vector3D& position) { position_ = position; }
        DRYPHYS_CONSTEXPR void setVelocity(const Vector3D& velocity) { velocity_ = velocity; }
        DRYPHYS_CONSTEXPR void setAcceleration(const Vector3D& acceleration) { acceleration_ = acceleration; }
        DRYPHYS_CONSTEXPR void setPosition(real x, real y, real z) { position_ = Vector3D {x, y, z}; }
        DRYPHYS_CONSTEXPR void setVelocity(real x, real y, real z) { velocity_ = Vector3D {x, y, z}; }
        DRYPHYS_CONSTEXPR void setAcceleration(real x, real y, real z) { acceleration_ = Vector3D {x, y, z}; }
        DRYPHYS_CONSTEXPR void setDamping(real damping) { damping_ = damping; }
        DRYPHYS_CONSTEXPR void setInverseMass(real inverseMass) { inverseMass_ = inverseMass; }
        void setMass(real mass);

        DRYPHYS_CONSTEXPR void getPosition(Vector3D* position) const { *position = position_; }
        DRYPHYS_CONSTEXPR void getVelocity(Vector3D* velocity) const { *velocity = velocity_; }
        DRYPHYS_CONSTEXPR void getAcceleration(Vector3D* acceleration) const { *acceleration = acceleration_; }
        DRYPHYS_CONSTEXPR Vector3D getPosition() const { return position_; }
        DRYPHYS_CONSTEXPR Vector3D getVelocity() const { return velocity_; }
        DRYPHYS_CONSTEXPR Vector3D getAcceleration() const { return acceleration_; }
        DRYPHYS_CONSTEXPR real getDamping() const { return damping_; }
        DRYPHYS_CONSTEXPR real getInverseMass() const { return inverseMass_; }
        real getMass() const;
    };
}   // namespace DryPhys

#endif
