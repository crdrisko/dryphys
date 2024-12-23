// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: testCompileTimeStringHashing.hpp
// Author: crdrisko
// Date: 07/18/2024-15:04:17
// Description: Provides ~100% unit test coverage over all compile-time string hashing functions

#ifndef DRYPHYS_TESTS_TESTUTILITIES_TESTCOMPILETIMESTRINGHASHING_HPP
#define DRYPHYS_TESTS_TESTUTILITIES_TESTCOMPILETIMESTRINGHASHING_HPP

#include <iostream>
#include <string>

#include <dryphys/dryphys.hpp>
#include <gtest/gtest.h>

using namespace DryPhys::Literals;

GTEST_TEST(testCompileTimeStringHashing, stringsAreHashedUsingTheDJB2Algorithm)
{
    const std::string str {"Hello, World!"};

    ASSERT_EQ(DryPhys::djb2Hash(str.c_str()), 2977021454);
}


GTEST_TEST(testCompileTimeStringHashing, nullCharacterReturns5381)
{
    std::string str {};

    ASSERT_EQ(DryPhys::djb2Hash(str.c_str()), 5381);
    ASSERT_EQ(DryPhys::djb2Hash("\0"), 5381);
}

GTEST_TEST(testCompileTimeStringHashing, userDefinedLiteralConvertsAStringToStringID)
{
    const std::string str {"Hello, World!"};

    ASSERT_EQ(DryPhys::djb2Hash(str.c_str()), "Hello, World!"_sID);
    ASSERT_NE("hello, world!"_sID, "Hello, World!"_sID);
}

GTEST_TEST(testCompileTimeStringHashing, compileTimeStringHashingCanBeUsedInSwitchStatements)
{
    testing::internal::CaptureStdout();

    const std::string str {"START"};

    switch (DryPhys::StringID sid = DryPhys::djb2Hash(str.c_str()); sid)
    {
    case "START"_sID:
        std::cout << "START selected\n";
        break;
    case "END"_sID:
        std::cout << "END selected\n";
        break;
    default:
        break;
    }

    std::string actualOutput = testing::internal::GetCapturedStdout();
    ASSERT_EQ(actualOutput, "START selected\n");
}

#endif
