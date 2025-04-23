// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: constraints.hpp
// Author: crdrisko
// Date: 07/13/2024-06:30:04
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_CONSTRAINTS_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_CONSTRAINTS_HPP

#include <vector>

#include "dryphys/math/vector.hpp"
#include "dryphys/types/particle.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    //! Forward declare for friend class functionality
    class ParticleConstraintResolver;

    /*!
     *
     */
    class ParticleConstraint
    {
        friend class ParticleConstraintResolver;

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
    class ParticleConstraintResolver
    {
    protected:
        unsigned iterations_ {};
        unsigned iterationsUsed_ {};

    public:
        constexpr explicit ParticleConstraintResolver(unsigned iterations) noexcept : iterations_ {iterations} {}

        constexpr void setIterations(unsigned iterations) noexcept { iterations_ = iterations; }

        void resolveConstraints(ParticleConstraint* constraints, unsigned numConstraints, real duration);
    };

    /*!
     *
     */
    class ParticleConstraintGenerator
    {
    public:
        virtual ~ParticleConstraintGenerator() = default;

        virtual unsigned addConstraint(ParticleConstraint* collision, unsigned limit) const = 0;
    };
}   // namespace DryPhys

#endif
