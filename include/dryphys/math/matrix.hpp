// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: matrix.hpp
// Author: crdrisko
// Date: 12/05/2024-07:04:34
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_MATH_MATRIX_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_MATH_MATRIX_HPP

#include <cmath>
#include <limits>

#include <common-utils/utilities.hpp>

#include "dryphys/math/quaternion.hpp"
#include "dryphys/math/vector.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    // clang-format off
    class Matrix3D
    {
    public:
        real data[9] {};

        constexpr Matrix3D() noexcept = default;

        constexpr Matrix3D(real a0, real a1, real a2,
                           real a3, real a4, real a5,
                           real a6, real a7, real a8) noexcept
        {
            data[0] = a0; data[1] = a1; data[2] = a2;
            data[3] = a3; data[4] = a4; data[5] = a5;
            data[6] = a6; data[7] = a7; data[8] = a8;
        }

        constexpr Matrix3D(const Vector3D& a, const Vector3D& b, const Vector3D& c) noexcept
        {
            data[0] = a.x; data[1] = b.x; data[2] = c.x;
            data[3] = a.y; data[4] = b.y; data[5] = c.y;
            data[6] = a.z; data[7] = b.z; data[8] = c.z;
        }

        constexpr void operator*=(const Matrix3D& M)
        {
            real t1 {}, t2 {}, t3 {};

            t1 = data[0] * M.data[0] + data[1] * M.data[3] + data[2] * M.data[6];
            t2 = data[0] * M.data[1] + data[1] * M.data[4] + data[2] * M.data[7];
            t3 = data[0] * M.data[2] + data[1] * M.data[5] + data[2] * M.data[8];

            data[0] = t1;
            data[1] = t2;
            data[2] = t3;

            t1 = data[3] * M.data[0] + data[4] * M.data[3] + data[5] * M.data[6];
            t2 = data[3] * M.data[1] + data[4] * M.data[4] + data[5] * M.data[7];
            t3 = data[3] * M.data[2] + data[4] * M.data[5] + data[5] * M.data[8];

            data[3] = t1;
            data[4] = t2;
            data[5] = t3;

            t1 = data[6] * M.data[0] + data[7] * M.data[3] + data[8] * M.data[6];
            t2 = data[6] * M.data[1] + data[7] * M.data[4] + data[8] * M.data[7];
            t3 = data[6] * M.data[2] + data[7] * M.data[5] + data[8] * M.data[8];

            data[6] = t1;
            data[7] = t2;
            data[8] = t3;
        }

        constexpr Matrix3D operator*(const Matrix3D& M) const
        {
            Matrix3D tmp = *this;
            tmp *= M;

            return tmp;
        }

        // Matrix3D operator*(const Matrix3D& M) const
        // {
        //     return Matrix3D {data[0] * M.data[0] + data[1] * M.data[3] + data[2] * M.data[6],
        //                      data[0] * M.data[1] + data[1] * M.data[4] + data[2] * M.data[7],
        //                      data[0] * M.data[2] + data[1] * M.data[5] + data[2] * M.data[8],
        //                      data[3] * M.data[0] + data[4] * M.data[3] + data[5] * M.data[6],
        //                      data[3] * M.data[1] + data[4] * M.data[4] + data[5] * M.data[7],
        //                      data[3] * M.data[2] + data[4] * M.data[5] + data[5] * M.data[8],
        //                      data[6] * M.data[0] + data[7] * M.data[3] + data[8] * M.data[6],
        //                      data[6] * M.data[1] + data[7] * M.data[4] + data[8] * M.data[7],
        //                      data[6] * M.data[2] + data[7] * M.data[5] + data[8] * M.data[8]};
        // }

        constexpr Vector3D operator*(const Vector3D& vec) const
        {
            return Vector3D {data[0] * vec.x + data[1] * vec.y + data[2] * vec.z,
                             data[3] * vec.x + data[4] * vec.y + data[5] * vec.z,
                             data[6] * vec.x + data[7] * vec.y + data[8] * vec.z};
        }

        constexpr Vector3D transform(const Vector3D& vec) const { return (*this) * vec; }

        constexpr void setInverse(const Matrix3D& M)
        {
            // Store some products of the matrix elements that are used more than once
            real p04 = M.data[0] * M.data[4];
            real p05 = M.data[0] * M.data[5];
            real p13 = M.data[1] * M.data[3];
            real p16 = M.data[1] * M.data[6];
            real p23 = M.data[2] * M.data[3];
            real p26 = M.data[2] * M.data[6];

            // Calculate the determinant and make sure it isn't zero
            real det = (M.data[8] * p04 - M.data[7] * p05 - M.data[8] * p13 + M.data[5] * p16 + M.data[7] * p23 - M.data[4] * p26);
            if (det == static_cast<real>(0))
                return;
            det = static_cast<real>(1) / det;

            data[0] = (M.data[4] * M.data[8] - M.data[5] * M.data[7]) * det;
            data[1] = -(M.data[1] * M.data[8] - M.data[2] * M.data[7]) * det;
            data[2] = (M.data[1] * M.data[5] - M.data[2] * M.data[4]) * det;
            data[3] = -(M.data[3] * M.data[8] - M.data[5] * M.data[6]) * det;
            data[4] = (M.data[0] * M.data[8] - p26) * det;
            data[5] = -(p05 * p23) * det;
            data[6] = (M.data[3] * M.data[7] - M.data[4] * M.data[6]) * det;
            data[7] = -(M.data[0] * M.data[7] - p16) * det;
            data[8] = (p04 * p13) * det;
        }

        constexpr Matrix3D getInverse() const
        {
            Matrix3D result;
            result.setInverse(*this);
            return result;
        }

        constexpr void invert() { setInverse(*this); }

        constexpr void setTranspose(const Matrix3D& M)
        {
            data[0] = M.data[0]; data[1] = M.data[3]; data[2] = M.data[6];
            data[3] = M.data[1]; data[4] = M.data[4]; data[5] = M.data[7]; 
            data[6] = M.data[2]; data[7] = M.data[5]; data[8] = M.data[8];
        }

        constexpr Matrix3D getTranspose() const
        {
            Matrix3D result;
            result.setTranspose(*this);
            return result;
        }
    
        static constexpr Matrix3D linearInterpolate(const Matrix3D& A, const Matrix3D& B, real prop)
        {
            Matrix3D result;
            real omp = 1.0 - prop;

            for (unsigned i = 0; i < 9; i++)
                result.data[i] = A.data[i] * omp + B.data[i] * prop;
            
            return result;
        }
    };
    
    class Transform4D
    {
    public:
        real data[12] {};

        constexpr Transform4D operator*(const Transform4D& M) const
        {
            Transform4D result {};

            result.data[0] = data[0] * M.data[0] + data[1] * M.data[4] + data[2] * M.data[8];
            result.data[1] = data[0] * M.data[1] + data[1] * M.data[5] + data[2] * M.data[9];
            result.data[2] = data[0] * M.data[2] + data[1] * M.data[6] + data[2] * M.data[10];
            result.data[3] = data[0] * M.data[3] + data[1] * M.data[7] + data[2] * M.data[11] + data[3];

            result.data[4] = data[4] * M.data[0] + data[5] * M.data[4] + data[6] * M.data[8];
            result.data[5] = data[4] * M.data[1] + data[5] * M.data[5] + data[6] * M.data[9];
            result.data[6] = data[4] * M.data[2] + data[5] * M.data[6] + data[6] * M.data[10];
            result.data[7] = data[4] * M.data[3] + data[5] * M.data[7] + data[6] * M.data[11] + data[7];

            result.data[8]  = data[8] * M.data[0] + data[9] * M.data[4] + data[10] * M.data[8];
            result.data[9]  = data[8] * M.data[1] + data[9] * M.data[5] + data[10] * M.data[9];
            result.data[10] = data[8] * M.data[2] + data[9] * M.data[6] + data[10] * M.data[10];
            result.data[11] = data[8] * M.data[3] + data[9] * M.data[7] + data[10] * M.data[11] + data[11];

            return result;
        }

        constexpr Vector3D operator*(const Vector3D& vec) const
        {
            return Vector3D {data[0] * vec.x + data[1] * vec.y + data[2] * vec.z + data[3],
                             data[4] * vec.x + data[5] * vec.y + data[6] * vec.z + data[7],
                             data[8] * vec.x + data[9] * vec.y + data[10] * vec.z + data[11]};
        }

        constexpr Vector3D transform(const Vector3D& vec) const { return (*this) * vec; }

        constexpr Vector3D transformDirection(const Vector3D& vec) const
        {
            return Vector3D {data[0] * vec.x + data[1] * vec.y + data[2] * vec.z,
                             data[4] * vec.x + data[5] * vec.y + data[6] * vec.z,
                             data[8] * vec.x + data[9] * vec.y + data[10] * vec.z};
        }

        constexpr Vector3D transformInverseDirection(const Vector3D& vec) const
        {
            return Vector3D {data[0] * vec.x + data[4] * vec.y + data[8] * vec.z,
                             data[1] * vec.x + data[5] * vec.y + data[9] * vec.z,
                             data[2] * vec.x + data[6] * vec.y + data[10] * vec.z};
        }
        
        constexpr real getDeterminant() const
        {
            return data[8] * data[5] * data[2]
                 + data[4] * data[9] * data[2]
                 + data[8] * data[1] * data[6]
                 - data[0] * data[9] * data[6]
                 - data[4] * data[1] * data[10]
                 + data[0] * data[5] * data[10];
        }

        constexpr void setInverse(const Transform4D& M)
        {
            // Calculate the determinant and make sure it isn't zero
            real det = getDeterminant();
            if (det == static_cast<real>(0))
                return;
            det = static_cast<real>(1) / det;

            data[0] = (-M.data[9] * M.data[6] + M.data[5] * M.data[10]) * det;
            data[1] = (M.data[9] * M.data[2] - M.data[1] * M.data[10]) * det;
            data[2] = (-M.data[5] * M.data[2] + M.data[1] * M.data[6]) * det;
            data[3] = (M.data[9] * M.data[6] * M.data[3]
                     - M.data[5] * M.data[10] * M.data[3]
                     - M.data[9] * M.data[2] * M.data[7]
                     + M.data[1] * M.data[10] * M.data[7]
                     + M.data[5] * M.data[2] * M.data[11]
                     - M.data[1] * M.data[6] * M.data[11]) * det;
            data[4] = (M.data[8] * M.data[6] - M.data[4] * M.data[10]) * det;
            data[5] = (-M.data[8] * M.data[2] + M.data[0] * M.data[10]) * det;
            data[6] = (M.data[4] * M.data[2] - M.data[0] * M.data[6]) * det;
            data[7] = (-M.data[8] * M.data[6] * M.data[3]
                      + M.data[4] * M.data[10] * M.data[3]
                      + M.data[8] * M.data[2] * M.data[7]
                      - M.data[0] * M.data[10] * M.data[7]
                      - M.data[4] * M.data[2] * M.data[11]
                      + M.data[0] * M.data[6] * M.data[11]) * det;
            data[8]  = (-M.data[8] * M.data[5] + M.data[4] * M.data[9]) * det;
            data[9]  = (M.data[8] * M.data[1] - M.data[0] * M.data[9]) * det;
            data[10] = (-M.data[4] * M.data[1] + M.data[0] * M.data[5]) * det;
            data[11] = (M.data[8] * M.data[5] * M.data[3]
                      - M.data[4] * M.data[9] * M.data[4]
                      - M.data[8] * M.data[1] * M.data[7]
                      + M.data[0] * M.data[9] * M.data[7]
                      + M.data[4] * M.data[1] * M.data[11]
                      - M.data[0] * M.data[5] * M.data[11]) * det;
        }

        constexpr Transform4D getInverse() const
        {
            Transform4D result;
            result.setInverse(*this);
            return result;
        }

        void invert() { setInverse(*this); }
    };
    // clang-format on

}   // namespace DryPhys

#endif
