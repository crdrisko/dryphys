// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: testForceGeneratorFunctions.hpp
// Author: crdrisko
// Date: 06/22/2024-06:57:19
// Description: Provides ~100% unit test coverage over all Force Generator functions

#ifndef DRYPHYS_TESTS_TESTPARTICLESYSTEMS_TESTFORCEGENERATORFUNCTIONS_HPP
#define DRYPHYS_TESTS_TESTPARTICLESYSTEMS_TESTFORCEGENERATORFUNCTIONS_HPP

#include <dryphys/dryphys.hpp>
#include <gtest/gtest.h>

GTEST_TEST(testForceGeneratorFunctions, test1)
{
    DryPhys::Particle a, b;
    DryPhys::ParticleForceRegistry registry;

    DryPhys::ParticleSpring ps {&b, 1.0f, 2.0f};
    registry.add(&a, &ps);
}

GTEST_TEST(testForceGeneratorFunctions, test2)
{
    DryPhys::Particle a, b;
    DryPhys::ParticleForceRegistry registry;

    DryPhys::ParticleSpring ps_a2b {&b, 1.0f, 2.0f};
    registry.add(&b, &ps_a2b);

    DryPhys::ParticleSpring ps_b2a {&a, 1.0f, 2.0f};
    registry.add(&a, &ps_b2a);
}

#endif
