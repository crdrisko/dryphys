// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: vector3d.hpp
// Author: crdrisko
// Date: 06/12/2024-06:01:10
// Description: A vector representing a quantity with x, y, and z components

#ifndef DRYPHYS_INCLUDE_DRYPHYS_VECTOR3D_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_VECTOR3D_HPP

#include <config.h>

#include <cmath>

#include <common-utils/utilities.hpp>

namespace DryPhys
{
    /*!
     * A class representing a mathematical/physical vector with three dimensions: x, y, z. The type is
     *  defined using the tuple-like API so it can be used with structured bindings.
     * 
     * \note Enabling \c phys_four_word_alignment when building adds an additional, unused variable
     *       which makes use of four-word alignment. This is off by default.
     */
    class Vector3D : private DryChem::EqualityComparable<Vector3D>
    {
    public:
        real x {};
        real y {};
        real z {};

    private:
#ifdef phys_four_word_alignment
        //! Padding to ensure four word alignment
        [[maybe_unused]] real pad;
#endif

    public:
        //! Constructors
        DRYPHYS_CONSTEXPR Vector3D() noexcept = default;
        DRYPHYS_CONSTEXPR Vector3D(const real x, const real y, const real z) noexcept : x {x}, y {y}, z {z} {}

        //! Comparison operators - only the equality operator is symmetric
        DRYPHYS_CONSTEXPR friend bool operator==(const Vector3D& lhs, const Vector3D& rhs)
        {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
        }

        DRYPHYS_CONSTEXPR friend bool operator<(const Vector3D& lhs, const Vector3D& rhs)
        {
            return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
        }

        DRYPHYS_CONSTEXPR friend bool operator>(const Vector3D& lhs, const Vector3D& rhs)
        {
            return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z;
        }

        DRYPHYS_CONSTEXPR friend bool operator<=(const Vector3D& lhs, const Vector3D& rhs)
        {
            return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z;
        }

        DRYPHYS_CONSTEXPR friend bool operator>=(const Vector3D& lhs, const Vector3D& rhs)
        {
            return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z;
        }

        //! Element access
        DRYPHYS_CONSTEXPR real& operator[](unsigned i) { return ((&x)[i]); }
        DRYPHYS_CONSTEXPR const real& operator[](unsigned i) const { return ((&x)[i]); }

        //! Arithmetic Operators
        DRYPHYS_CONSTEXPR Vector3D& operator+=(const Vector3D& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;

            return *this;
        }

        DRYPHYS_CONSTEXPR Vector3D operator+(const Vector3D& rhs) const { return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z); }

        DRYPHYS_CONSTEXPR Vector3D& operator-=(const Vector3D& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;

            return *this;
        }

        DRYPHYS_CONSTEXPR Vector3D operator-(const Vector3D& rhs) const { return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z); }

        DRYPHYS_CONSTEXPR Vector3D& operator*=(real rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;

            return *this;
        }

        DRYPHYS_CONSTEXPR Vector3D operator*(real rhs) const { return Vector3D(x * rhs, y * rhs, z * rhs); }

        DRYPHYS_CONSTEXPR Vector3D& operator/=(real rhs)
        {
            rhs = static_cast<real>(1) / rhs;

            x *= rhs;
            y *= rhs;
            z *= rhs;

            return *this;
        }

        DRYPHYS_CONSTEXPR Vector3D operator/(real rhs) const
        {
            rhs = static_cast<real>(1) / rhs;
            return Vector3D(x * rhs, y * rhs, z * rhs);
        }

        /*!
         * Calculates the component product of the two vectors.
         *
         * \note We use operator*= for the component product
         */
        DRYPHYS_CONSTEXPR Vector3D& operator*=(const Vector3D& rhs)
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
        DRYPHYS_CONSTEXPR Vector3D operator*(const Vector3D& rhs) const { return Vector3D(x * rhs.x, y * rhs.y, z * rhs.z); }

        /*!
         * Calculates the dot (scalar) product of the two vectors.
         */
        DRYPHYS_CONSTEXPR real dot(const Vector3D& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }

        /*!
         * Calculates the cross (vector) product of the two vectors.
         */
        DRYPHYS_CONSTEXPR Vector3D cross(const Vector3D& rhs) const
        {
            return Vector3D(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
        }

        /*! 
         * Reset the elements of the vector to zero.
         */
        DRYPHYS_CONSTEXPR Vector3D& clear()
        {
            x = 0;
            y = 0;
            z = 0;

            return *this;
        }

        /*! 
         * Flips all the components of the vector.
         */
        DRYPHYS_CONSTEXPR Vector3D& invert()
        {
            x = -x;
            y = -y;
            z = -z;

            return *this;
        }

        /*! 
         * Calculates the magnitude of this vector squared to avoid a square root call.
         */
        DRYPHYS_CONSTEXPR real magnitudeSquared() const { return x * x + y * y + z * z; }

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
    };

    /*!
     * Specific getters for use in the tuple-like API, allows for the structured binding of a \c Vector3D
     */
    template<std::size_t Index>
    decltype(auto) get(Vector3D& vec)
    {
        static_assert(Index < 3, "Index must be within 0 and 2, inclusive.");

        if constexpr (Index == 0)
            return vec[0];
        else if constexpr (Index == 1)
            return vec[1];
        else
            return vec[2];
    }

    /*!
     * \overload
     */
    template<std::size_t Index>
    decltype(auto) get(const Vector3D& vec)
    {
        static_assert(Index < 3, "Index must be within 0 and 2, inclusive.");

        if constexpr (Index == 0)
            return vec[0];
        else if constexpr (Index == 1)
            return vec[1];
        else
            return vec[2];
    }

    /*!
     * \overload
     */
    template<std::size_t Index>
    decltype(auto) get(Vector3D&& vec)
    {
        static_assert(Index < 3, "Index must be within 0 and 2, inclusive.");

        if constexpr (Index == 0)
            return std::move(vec[0]);
        else if constexpr (Index == 1)
            return std::move(vec[1]);
        else
            return std::move(vec[2]);
    }
}   // namespace DryPhys

//! Specializations of the remaining requirements for access to the tuple-like API
template<>
struct std::tuple_size<DryPhys::Vector3D>
{
    static constexpr int value = 3;
};

template<std::size_t Index>
struct std::tuple_element<Index, DryPhys::Vector3D>
{
    using type = DryPhys::real;
};

#endif
