// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particleSprings.hpp
// Author: crdrisko
// Date: 06/22/2024-06:36:13
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_FORCEGENERATORS_PARTICLESPRINGS_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_PARTICLESYSTEMS_FORCEGENERATORS_PARTICLESPRINGS_HPP

#include <config.h>

#include "dryphys/math/vector3d.hpp"
#include "dryphys/particle.hpp"
#include "dryphys/particleSystems/forceGenerator.hpp"

namespace DryPhys
{
    inline real hookesLaw(const Vector3D& r, real r_0, real k)
    {
        real force = k * (r.magnitude() - r_0);

        return force;
    }

    /*!
     * A \c ParticleForceGenerator responsible for modeling two particles connected via a spring. The
     *  spring is modeled using Hooke's Law:
     *      \f$\textbf{f}_{\textrm{spring}} = k (|\textbf{r}| - r_0)\hat{\textbf{r}}.\f$
     */
    class ParticleSpring : public ParticleForceGenerator
    {
    private:
        Particle* other_;
        real springConstant_;
        real restLength_;

    public:
        ParticleSpring(Particle* other, real springConstant, real restLength)
            : other_ {other}, springConstant_ {springConstant}, restLength_ {restLength}
        {
        }

        void updateForce(Particle* particle, real duration) override;
    };

    /*!
     * A \c ParticleForceGenerator responsible for modeling a particle connected to an anchor point via a
     *  spring. The spring is modeled using Hooke's Law:
     *      \f$\textbf{f}_{\textrm{spring}} = k (|\textbf{r}| - r_0)\hat{\textbf{r}}.\f$
     */
    class ParticleAnchoredSpring : public ParticleForceGenerator
    {
    private:
        Vector3D* anchor_;
        real springConstant_;
        real restLength_;

    public:
        ParticleAnchoredSpring(Vector3D* anchor, real springConstant, real restLength)
            : anchor_ {anchor}, springConstant_ {springConstant}, restLength_ {restLength}
        {
        }

        void updateForce(Particle* particle, real duration) override;
    };

    /*!
     * A \c ParticleForceGenerator responsible for modeling two particles connected via a bungee. The 
     *  bungee is modeled using Hooke's Law when stretched past its equilibrium (rest) length:
     *      \f{equation}\textbf{f}_{\textrm{spring}} = 
     *          \begin{cases}
     *              k (|\textbf{r}| - r_0)\hat{\textbf{r}}, & r > r_0 \\
     *              0, & r \le r_0 
     *          \end{cases}.
     *      \f}         
     */
    class ParticleBungee : public ParticleForceGenerator
    {
    private:
        Particle* other_;
        real springConstant_;
        real restLength_;

    public:
        ParticleBungee(Particle* other, real springConstant, real restLength)
            : other_ {other}, springConstant_ {springConstant}, restLength_ {restLength}
        {
        }

        void updateForce(Particle* particle, real duration) override;
    };

    /*class ParticleAnchoredBungee : public ParticleForceGenerator
    {
    private:
        Vector3D* anchor_;
        real springConstant_;
        real restLength_;

    public:
        ParticleAnchoredBungee(Vector3D* anchor, real springConstant, real restLength)
            : anchor_ {anchor}, springConstant_ {springConstant}, restLength_ {restLength}
        {
        }

        void updateForce(Particle* particle, real duration) override;
    };*/

    class ParticleFakeSpring : public ParticleForceGenerator
    {
    private:
        Vector3D* anchor_;
        real springConstant_;
        real damping_;

    public:
        ParticleFakeSpring(Vector3D* anchor, real springConstant, real damping)
            : anchor_ {anchor}, springConstant_ {springConstant}, damping_ {damping}
        {
        }

        void updateForce(Particle* particle, real duration) override;
    };
}   // namespace DryPhys

#endif
