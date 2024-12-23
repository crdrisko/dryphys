// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: aero.hpp
// Author: crdrisko
// Date: 12/13/2024-14:53:26
// Description:

#ifndef AERO_HPP
#define AERO_HPP

#include "dryphys/math/matrix.hpp"
#include "dryphys/math/vector.hpp"
#include "dryphys/rigidBodySystems/forceGenerator.hpp"

namespace DryPhys
{
    class Aero : public RigidBodyForceGenerator
    {
    protected:
        Matrix3D tensor_;             //! The aerodynamic tensor for the surface in body space
        Vector3D position_;           //! The relative position of the surface in body space
        const Vector3D* windspeed_;   //! The current windspeed of the environment

        void updateForceFromTensor(RigidBody* rigidBody, real duration, const Matrix3D& tensor);

    public:
        Aero(const Matrix3D& tensor, const Vector3D& position, const Vector3D* windspeed)
            : tensor_ {tensor}, position_ {position}, windspeed_ {windspeed}
        {
        }

        void updateForce(RigidBody* rigidBody, real duration) override;
    };

    class AeroControl : public Aero
    {
    protected:
        Matrix3D maxTensor_;
        Matrix3D minTensor_;
        real controlSetting_ {0.0f};

    private:
        Matrix3D getTensor();

    public:
        AeroControl(const Matrix3D& base,
            const Matrix3D& min,
            const Matrix3D& max,
            const Vector3D& position,
            const Vector3D* windspeed)
            : Aero {base, position, windspeed}, maxTensor_ {max}, minTensor_ {min}
        {
        }

        void setControl(real value) { controlSetting_ = value; }

        void updateForce(RigidBody* rigidBody, real duration) override;
    };
}   // namespace DryPhys

#endif
