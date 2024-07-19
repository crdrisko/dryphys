// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: testAllMathFunctions.cpp
// Author: crdrisko
// Date: 06/12/2024-06:32:33
// Description: Provides ~100% unit test coverage over all mathematical functions

#include <gtest/gtest.h>

#include "testMathematics/testQuaternionFunctions.hpp"
#include "testMathematics/testVector3DFunctions.hpp"

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
