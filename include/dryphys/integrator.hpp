// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: integrator.hpp
// Author: crdrisko
// Date: 06/17/2024-06:50:26
// Description:

#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include <config.h>

#include <vector>

#include "dryphys/particle.hpp"

namespace DryPhys
{
    /*!
     *
     */
    class Integrator
    {
    protected:
        Vector3D forceAccumulator_;   //! Accumulated forces acting on this particle

    public:
        ~Integrator() = default;

        DRYPHYS_CONSTEXPR void addForce(const Vector3D& force) { forceAccumulator_ += force; }
        DRYPHYS_CONSTEXPR void clearAccumulator() { forceAccumulator_.clear(); }

        virtual void integrate(Particle* particle, real timestep) = 0;
    };

    /*!
     *
     */
    class IntegratorRegistry
    {
    protected:
        struct IntegratorRegistration
        {
            Particle* particle;
            Integrator* integrator;
        };

        using Registry = std::vector<IntegratorRegistration>;
        Registry registrations_;

    public:
        void integrate(real timestep);

        void add(Particle* particle, Integrator* integrator);
        void remove(Particle* particle, Integrator* integrator);

        void clear() { registrations_.clear(); }
    };
}   // namespace DryPhys

#endif
