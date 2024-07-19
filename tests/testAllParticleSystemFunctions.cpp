// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: testAllParticleSystemFunctions.cpp
// Author: crdrisko
// Date: 06/14/2024-06:19:00
// Description: Provides ~100% unit test coverage over all Particle System functions

#include <gtest/gtest.h>

#include "testParticleSystems/testForceGeneratorFunctions.hpp"
#include "testParticleSystems/testParticleFunctions.hpp"

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
