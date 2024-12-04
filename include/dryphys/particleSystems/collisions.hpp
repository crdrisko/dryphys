// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: collisions.hpp
// Author: crdrisko
// Date: 07/13/2024-06:30:04
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_COLLISIONS_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_COLLISIONS_HPP

#include <vector>

#include "dryphys/config.h"
#include "dryphys/math/vector3d.hpp"
#include "dryphys/particle.hpp"

namespace DryPhys
{
    //! Forward declare for friend class functionality
    class ParticleCollisionResolver;

    /*!
     *
     */
    class ParticleCollision
    {
        friend class ParticleCollisionResolver;

    public:
        Particle* particles_[2];
        real restitution_;
        real penetration_;
        Vector3D contactNormal_;
        Vector3D particleMovements_[2];

    protected:
        void resolve(real duration);
        real calculateSeparatingVelocity() const;

    private:
        void resolveVelocity(real duration);
        void resolveInterpenetration(real duration);
    };

    /*!
     *
     */
    class ParticleCollisionResolver
    {
    protected:
        unsigned iterations_ {};

    public:
        constexpr explicit ParticleCollisionResolver(unsigned iterations) noexcept : iterations_ {iterations} {}

        constexpr void setIterations(unsigned iterations) noexcept { iterations_ = iterations; }

        void resolveCollisions(ParticleCollision* collisions, unsigned numCollisions, real duration);
    };

    /*!
     *
     */
    class ParticleCollisionGenerator
    {
    public:
        virtual ~ParticleCollisionGenerator() = default;

        virtual unsigned addCollision(ParticleCollision* collision, unsigned limit) const = 0;
    };
}   // namespace DryPhys

#endif
