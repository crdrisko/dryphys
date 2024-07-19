// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: quaternion.hpp
// Author: crdrisko
// Date: 07/17/2024-17:18:35
// Description: A quaternion representation comprising a scalar w component, and vector x, y, and z components

#ifndef DRYPHYS_INCLUDE_DRYPHYS_MATH_QUATERNION_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_MATH_QUATERNION_HPP

#include <config.h>

#include <cmath>
#include <limits>

#include <common-utils/utilities.hpp>

#include "dryphys/vector3d.hpp"

namespace DryPhys
{
    class Quaternion : private DryChem::EqualityComparable<Quaternion>
    {
    public:
        real w {1};
        real x {};
        real y {};
        real z {};

        //! Constructors
        DRYPHYS_CONSTEXPR Quaternion() noexcept = default;
        DRYPHYS_CONSTEXPR Quaternion(real a, real b, real c, real d) noexcept : w {a}, x {b}, y {c}, z {d} {}
        DRYPHYS_CONSTEXPR Quaternion(const Vector3D& v, real s) noexcept : w {s}, x {v.x}, y {v.y}, z {v.z} {}

        //! Comparison operators - only the equality operator is symmetric
        DRYPHYS_CONSTEXPR friend bool operator==(const Quaternion& lhs, const Quaternion& rhs)
        {
            return lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
        }

        //! Element/part access
        DRYPHYS_CONSTEXPR real& getRealPart() { return w; }
        DRYPHYS_CONSTEXPR const real& getRealPart() const { return w; }

        Vector3D& getVectorPart() { return (reinterpret_cast<Vector3D&>(x)); }
        const Vector3D& getVectorPart() const { return (reinterpret_cast<const Vector3D&>(x)); }

        //! Arithmetic Operators
        DRYPHYS_CONSTEXPR Quaternion& operator*=(const Quaternion& rhs)
        {
            Quaternion tmp = *this;
            w              = tmp.w * rhs.w - tmp.x * rhs.x - tmp.y * rhs.y - tmp.z * rhs.z;
            x              = tmp.w * rhs.x + tmp.x * rhs.w + tmp.y * rhs.z - tmp.z * rhs.y;
            y              = tmp.w * rhs.y - tmp.x * rhs.z + tmp.y * rhs.w + tmp.z * rhs.x;
            z              = tmp.w * rhs.z + tmp.x * rhs.y - tmp.y * rhs.x + tmp.z * rhs.w;

            return *this;
        }

        DRYPHYS_CONSTEXPR Quaternion operator*(const Quaternion& rhs) const
        {
            // clang-format off
            return Quaternion(w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
                              w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
                              w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
                              w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w);
            // clang-format on
        }

        /*!
         * Turns a non-zero quaternion into a quaternion of unit length, and turns a
         *  quaternion with length zero into the default, no-rotation quaternion.
         */
        void normalize()
        {
            real dist = w * w + x * x + y * y + z * z;

            if (dist < std::numeric_limits<real>::epsilon())
            {
                w = 1;
                return;
            }

            dist = static_cast<real>(1) / std::sqrt(dist);

            w *= dist;
            x *= dist;
            y *= dist;
            z *= dist;
        }
    };
}   // namespace DryPhys

#endif
