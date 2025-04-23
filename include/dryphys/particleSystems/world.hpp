// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: world.hpp
// Author: crdrisko
// Date: 07/13/2024-10:17:36
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_WORLD_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_WORLD_HPP

#include <vector>

#include "dryphys/particleSystems/constraints.hpp"
#include "dryphys/particleSystems/forceGenerator.hpp"
#include "dryphys/types/particle.hpp"

namespace DryPhys
{
    class ParticleWorld
    {
    public:
        using Particles            = std::vector<Particle*>;
        using ConstraintGenerators = std::vector<ParticleConstraintGenerator*>;

    protected:
        Particles particles_;
        ParticleForceRegistry registry_;
        ParticleConstraintResolver resolver_;
        ConstraintGenerators constraintGenerators_;
        ParticleConstraint* constraints_;

        unsigned maxContraints_ {};
        bool calculateIterations_ {};

    public:
        ParticleWorld(unsigned maxConstraints, unsigned iterations = 0);
        ~ParticleWorld();

        void startFrame();
        unsigned generateConstraints();
        // void integrate(real duration);
        void moveA(real duration);
        void moveB(real duration);
        void runPhysics(real duration);

        Particles& getParticles() { return particles_; }
        ConstraintGenerators& getConstraintGenerators() { return constraintGenerators_; }
        ParticleForceRegistry& getParticleForceRegistry() { return registry_; }
    };
}   // namespace DryPhys

#endif
