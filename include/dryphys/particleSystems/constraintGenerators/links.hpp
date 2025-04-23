// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: links.hpp
// Author: crdrisko
// Date: 07/13/2024-08:34:47
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_CONSTRAINTGENERATORS_LINKS_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_CONSTRAINTGENERATORS_LINKS_HPP

#include "dryphys/particleSystems/constraints.hpp"
#include "dryphys/types/particle.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    /*!
     *
     */
    class ParticleLink : public ParticleConstraintGenerator
    {
    public:
        Particle* particles_[2];

        unsigned addConstraint(ParticleConstraint* constraint, unsigned limit) const override = 0;

    protected:
        real currentLength() const;
    };

    /*!
     *
     */
    class ParticleCable : public ParticleLink
    {
    public:
        real maxLength_ {};
        real restitution_ {};

        unsigned addConstraint(ParticleConstraint* constraint, unsigned limit) const override;
    };

    /*!
     *
     */
    class ParticleRod : public ParticleLink
    {
    public:
        real length_ {};

        unsigned addConstraint(ParticleConstraint* constraint, unsigned limit) const override;
    };


    class ParticleAnchor : public ParticleConstraintGenerator
    {
    public:
        Particle* particle_;
        Vector3D anchor_;

    protected:
        real currentLength() const;

    public:
        unsigned addConstraint(ParticleConstraint* constraint, unsigned limit) const override = 0;
    };


    class ParticleCableConstraint : public ParticleAnchor
    {
    public:
        real maxLength_;
        real restitution_;

        unsigned addConstraint(ParticleConstraint* constraint, unsigned limit) const override;
    };


    class ParticleRodConstraint : public ParticleAnchor
    {
    public:
        real length_;

        unsigned addConstraint(ParticleConstraint* constraint, unsigned limit) const override;
    };
}   // namespace DryPhys

#endif
