// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: config.h
// Author: crdrisko
// Date: 06/12/2024-06:01:17
// Description: Generated from config.h.cmake by CMake for @PROJECT_NAME@

#ifndef DRYPHYS_INCLUDE_CONFIG_H
#define DRYPHYS_INCLUDE_CONFIG_H

/* Is defined if DryPhys should be compiled with double precision arithmetic. */
#cmakedefine phys_double_precision

/* Is defined if the Vector3D class should make use of four-word alignment. */
#cmakedefine phys_four_word_alignment

namespace DryPhys
{
#ifdef phys_double_precision
    using real = double;
#else
    using real = float;
#endif
}

#endif
