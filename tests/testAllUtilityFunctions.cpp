// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: testAllUtilityFunctions.cpp
// Author: crdrisko
// Date: 07/18/2024-15:04:37
// Description: Provides ~100% unit test coverage over all Utility functions

#include <gtest/gtest.h>

#include "testUtilities/testCompileTimeStringHashing.hpp"

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
