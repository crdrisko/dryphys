// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: vector.hpp
// Author: crdrisko
// Date: 06/12/2024-06:01:10
// Description:

#ifndef DRYPHYS_INCLUDE_DRYPHYS_VECTOR_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_VECTOR_HPP

#include <cmath>

#include <drychem/drychem.hpp>

#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    class Vector2D : private DryChem::EqualityComparable<Vector2D>
    {
    public:
        real x {};
        real y {};

        //! Constructors
        constexpr Vector2D() noexcept = default;
        constexpr Vector2D(real a, real b) noexcept : x {a}, y {b} {}

        //! Comparison operators - only the equality operator is symmetric
        constexpr friend bool operator==(const Vector2D& lhs, const Vector2D& rhs)
        {
            return lhs.x == rhs.x && lhs.y == rhs.y;
        }

        constexpr friend bool operator<(const Vector2D& lhs, const Vector2D& rhs) { return lhs.x < rhs.x && lhs.y < rhs.y; }

        constexpr friend bool operator>(const Vector2D& lhs, const Vector2D& rhs) { return lhs.x > rhs.x && lhs.y > rhs.y; }

        constexpr friend bool operator<=(const Vector2D& lhs, const Vector2D& rhs)
        {
            return lhs.x <= rhs.x && lhs.y <= rhs.y;
        }

        constexpr friend bool operator>=(const Vector2D& lhs, const Vector2D& rhs)
        {
            return lhs.x >= rhs.x && lhs.y >= rhs.y;
        }

        //! Element access
        constexpr real& operator[](unsigned i) { return ((&x)[i]); }
        constexpr const real& operator[](unsigned i) const { return ((&x)[i]); }

        //! Arithmetic Operators
        constexpr Vector2D& operator+=(const Vector2D& rhs)
        {
            x += rhs.x;
            y += rhs.y;

            return *this;
        }

        constexpr Vector2D operator+(const Vector2D& rhs) const { return Vector2D(x + rhs.x, y + rhs.y); }

        constexpr Vector2D& operator-=(const Vector2D& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;

            return *this;
        }

        constexpr Vector2D operator-(const Vector2D& rhs) const { return Vector2D(x - rhs.x, y - rhs.y); }

        constexpr Vector2D& operator*=(real rhs)
        {
            x *= rhs;
            y *= rhs;

            return *this;
        }

        constexpr Vector2D operator*(real rhs) const { return Vector2D(x * rhs, y * rhs); }

        constexpr Vector2D& operator/=(real rhs)
        {
            rhs = static_cast<real>(1) / rhs;

            x *= rhs;
            y *= rhs;

            return *this;
        }

        constexpr Vector2D operator/(real rhs) const
        {
            rhs = static_cast<real>(1) / rhs;
            return Vector2D(x * rhs, y * rhs);
        }
    };

    /*!
     * A class representing a mathematical/physical vector with three dimensions: x, y, z.
     */
    class Vector3D : private DryChem::EqualityComparable<Vector3D>
    {
    public:
        real x {};
        real y {};
        real z {};

    public:
        //! Constructors
        constexpr Vector3D() noexcept = default;
        constexpr Vector3D(real a, real b, real c) noexcept : x {a}, y {b}, z {c} {}

        //! Comparison operators - only the equality operator is symmetric
        constexpr friend bool operator==(const Vector3D& lhs, const Vector3D& rhs)
        {
            return (lhs.x == rhs.x || equality(lhs.x, rhs.x)) && (lhs.y == rhs.y || equality(lhs.y, rhs.y))
                   && (lhs.z == rhs.z || equality(lhs.z, rhs.z));
        }

        constexpr friend bool operator<(const Vector3D& lhs, const Vector3D& rhs)
        {
            return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
        }

        constexpr friend bool operator>(const Vector3D& lhs, const Vector3D& rhs)
        {
            return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z;
        }

        constexpr friend bool operator<=(const Vector3D& lhs, const Vector3D& rhs)
        {
            return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z;
        }

        constexpr friend bool operator>=(const Vector3D& lhs, const Vector3D& rhs)
        {
            return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z;
        }

        //! Element access
        constexpr real& operator[](unsigned i) { return ((&x)[i]); }
        constexpr const real& operator[](unsigned i) const { return ((&x)[i]); }

        //! Arithmetic Operators
        constexpr Vector3D& operator+=(const Vector3D& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;

            return *this;
        }

        constexpr Vector3D operator+(const Vector3D& rhs) const { return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z); }

        constexpr Vector3D& operator-=(const Vector3D& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;

            return *this;
        }

        constexpr Vector3D operator-(const Vector3D& rhs) const { return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z); }

        constexpr Vector3D& operator*=(real rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;

            return *this;
        }

        constexpr Vector3D operator*(real rhs) const { return Vector3D(x * rhs, y * rhs, z * rhs); }

        constexpr Vector3D& operator/=(real rhs)
        {
            rhs = static_cast<real>(1) / rhs;

            x *= rhs;
            y *= rhs;
            z *= rhs;

            return *this;
        }

        constexpr Vector3D operator/(real rhs) const
        {
            rhs = static_cast<real>(1) / rhs;
            return Vector3D(x * rhs, y * rhs, z * rhs);
        }

        /*!
         * Calculates the component product of the two vectors.
         *
         * \note We use operator*= for the component product
         */
        constexpr Vector3D& operator*=(const Vector3D& rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;

            return *this;
        }

        /*!
         * Calculates the component product of the two vectors.
         *
         * \note We use operator* for the component product
         */
        constexpr Vector3D operator*(const Vector3D& rhs) const { return Vector3D(x * rhs.x, y * rhs.y, z * rhs.z); }

        /*!
         * Calculates the dot (scalar) product of the two vectors.
         */
        constexpr real dot(const Vector3D& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }

        /*!
         * Calculates the cross (vector) product of the two vectors.
         */
        constexpr Vector3D cross(const Vector3D& rhs) const
        {
            return Vector3D(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
        }

        /*! 
         * Reset the elements of the vector to zero.
         */
        constexpr Vector3D& clear()
        {
            x = 0;
            y = 0;
            z = 0;

            return *this;
        }

        /*! 
         * Flips all the components of the vector.
         */
        constexpr Vector3D& invert()
        {
            x = -x;
            y = -y;
            z = -z;

            return *this;
        }

        /*! 
         * Calculates the magnitude of this vector squared to avoid a square root call.
         */
        constexpr real magnitudeSquared() const { return x * x + y * y + z * z; }

        /*! 
         * Calculates the magnitude/length of this vector.
         */
        real magnitude() const { return std::sqrt(x * x + y * y + z * z); }

        /*! 
         * Turns a non-zero vector into a vector of unit length.
         */
        void normalize()
        {
            if (real length = magnitude(); length > 0)
                (*this) *= static_cast<real>(1) / length;
        }

        Vector3D unit() const
        {
            Vector3D result = *this;
            result.normalize();
            return result;
        }

        void trim(real size)
        {
            if (magnitudeSquared() > size * size)
            {
                normalize();
                x *= size;
                y *= size;
                z *= size;
                // *this *= size;
            }
        }

        void addScaledVector(const Vector3D& vector, real scale)
        {
            x += vector.x * scale;
            y += vector.y * scale;
            z += vector.z * scale;
        }
    };

    class Vector4D : private DryChem::EqualityComparable<Vector4D>
    {
    public:
        real x {};
        real y {};
        real z {};
        real w {};

        //! Constructors
        constexpr Vector4D() noexcept = default;
        constexpr Vector4D(real a, real b, real c, real d) noexcept : x {a}, y {b}, z {c}, w {d} {}

        //! Comparison operators - only the equality operator is symmetric
        constexpr friend bool operator==(const Vector4D& lhs, const Vector4D& rhs)
        {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
        }

        constexpr friend bool operator<(const Vector4D& lhs, const Vector4D& rhs)
        {
            return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z && lhs.w > rhs.w;
        }

        constexpr friend bool operator>(const Vector4D& lhs, const Vector4D& rhs)
        {
            return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z && lhs.w > rhs.w;
        }

        constexpr friend bool operator<=(const Vector4D& lhs, const Vector4D& rhs)
        {
            return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z && lhs.w <= rhs.w;
        }

        constexpr friend bool operator>=(const Vector4D& lhs, const Vector4D& rhs)
        {
            return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z && lhs.w >= rhs.w;
        }

        //! Element access
        constexpr real& operator[](unsigned i) { return ((&x)[i]); }
        constexpr const real& operator[](unsigned i) const { return ((&x)[i]); }

        //! Arithmetic Operators
        constexpr Vector4D& operator+=(const Vector4D& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;

            return *this;
        }

        constexpr Vector4D operator+(const Vector4D& rhs) const
        {
            return Vector4D(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }

        constexpr Vector4D& operator-=(const Vector4D& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w += rhs.w;

            return *this;
        }

        constexpr Vector4D operator-(const Vector4D& rhs) const
        {
            return Vector4D(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        constexpr Vector4D& operator*=(real rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;

            return *this;
        }

        constexpr Vector4D operator*(real rhs) const { return Vector4D(x * rhs, y * rhs, z * rhs, w * rhs); }

        constexpr Vector4D& operator/=(real rhs)
        {
            rhs = static_cast<real>(1) / rhs;

            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;

            return *this;
        }

        constexpr Vector4D operator/(real rhs) const
        {
            rhs = static_cast<real>(1) / rhs;
            return Vector4D(x * rhs, y * rhs, z * rhs, w * rhs);
        }
    };
}   // namespace DryPhys

#endif
