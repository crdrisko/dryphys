// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: groundConstraints.hpp
// Author: crdrisko
// Date: 12/04/2024-05:14:04
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_CONSTRAINTGENERATORS_GROUNDCONSTRAINTS_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_CONSTRAINTGENERATORS_GROUNDCONSTRAINTS_HPP

#include <vector>

#include "dryphys/particleSystems/constraints.hpp"
#include "dryphys/particleSystems/world.hpp"
#include "dryphys/types/particle.hpp"

namespace DryPhys
{
    class GroundConstraints : public ParticleConstraintGenerator
    {
    private:
        ParticleWorld::Particles* particles_;

    public:
        void init(ParticleWorld::Particles* particles) { particles_ = particles; }

        unsigned addConstraint(ParticleConstraint* constraint, unsigned limit) const override;
    };
}   // namespace DryPhys

#endif
