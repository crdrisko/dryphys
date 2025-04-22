// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: config.hpp
// Author: crdrisko
// Date: 06/12/2024-06:01:17
// Description: Generated from config.hpp.cmake by CMake for @PROJECT_NAME@

#ifndef DRYPHYS_INCLUDE_UTILITIES_CONFIG_HPP
#define DRYPHYS_INCLUDE_UTILITIES_CONFIG_HPP

#include <cmath>
#include <limits>

/* Is defined if DryPhys should be compiled with single-precision arithmetic. */
#cmakedefine phys_single_precision

namespace DryPhys
{
#ifdef phys_single_precision
    using real = float;
#else
    using real = double;
#endif

    namespace Constants
    {
        static inline constexpr real gravity = -9.81;
        static inline constexpr real pi = 3.14159265358979;
    }

    //! Check equality of two reals by comparing their difference to the machine epsilon
    static inline bool equality(real rhs, real lhs) { return (std::abs(lhs - rhs) <= std::numeric_limits<real>::epsilon()); }
}

#endif
