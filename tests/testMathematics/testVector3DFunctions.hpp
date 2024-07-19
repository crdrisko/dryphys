// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: testVector3DFunctions.hpp
// Author: crdrisko
// Date: 07/18/2024-06:30:43
// Description: Provides ~100% unit test coverage over all Vector3D functions

#ifndef DRYPHYS_TESTS_TESTMATHEMATICS_TESTVECTOR3DFUNCTIONS_HPP
#define DRYPHYS_TESTS_TESTMATHEMATICS_TESTVECTOR3DFUNCTIONS_HPP

#include <iostream>

#include <dryphys/vector3d.hpp>
#include <gtest/gtest.h>

GTEST_TEST(testVector3DFunctions, paddingIncreasesClassSizeByTheSizeOfOneAdditionalReal)
{
#ifdef phys_four_word_alignment
    ASSERT_EQ(4 * sizeof(DryPhys::real), sizeof(DryPhys::Vector3D));
#else
    ASSERT_EQ(3 * sizeof(DryPhys::real), sizeof(DryPhys::Vector3D));
#endif
}

GTEST_TEST(testVector3DFunctions, differentConstructorsInitializeObjectsAsExpected)
{
    DryPhys::Vector3D defaultInitialized {};
    DryPhys::Vector3D valuesInitialized {1.0f, 3.0f, 5.0f};

    for (unsigned i {}; i < 3; ++i)
    {
        ASSERT_FLOAT_EQ(0.0f, defaultInitialized[i]);
        ASSERT_FLOAT_EQ(static_cast<float>(2 * (i + 1) - 1), valuesInitialized[i]);
    }
}

GTEST_TEST(testVector3DFunctions, overloadedComparsionOperatorsPerformElementwiseComparisons)
{
    DryPhys::Vector3D value1 {1.0f, 2.0f, 3.0f};
    DryPhys::Vector3D value2 {};
    DryPhys::Vector3D value3 {1.0f, 2.0f, 3.0f};
    DryPhys::Vector3D value4 {1.0f, 0.0f, 3.0f};

    ASSERT_TRUE(value1 == value3);
    ASSERT_FALSE(value1 == value2);

    ASSERT_TRUE(value3 != value2);
    ASSERT_FALSE(value3 != value1);

    ASSERT_TRUE(value2 < value1);
    ASSERT_FALSE(value1 < value2);

    ASSERT_TRUE(value1 > value2);
    ASSERT_FALSE(value1 > value4);

    ASSERT_TRUE(value1 <= value3);
    ASSERT_FALSE(value3 <= value4);

    ASSERT_TRUE(value1 >= value4);
    ASSERT_TRUE(value3 >= value2);
    ASSERT_FALSE(value2 >= value4);
}

GTEST_TEST(testVector3DFunctions, theInternalDataIsAccessibleWithoutFunctionCalls)
{
    DryPhys::Vector3D coordinates {1.0f, -3.0f, 5.0f};

    ASSERT_FLOAT_EQ(1.0f, coordinates.x);
    ASSERT_FLOAT_EQ(-3.0f, coordinates.y);
    ASSERT_FLOAT_EQ(5.0f, coordinates.z);

    coordinates.x += 4.0f;
    coordinates.y += 4.0f;
    coordinates.z += 4.0f;

    ASSERT_FLOAT_EQ(5.0f, coordinates.x);
    ASSERT_FLOAT_EQ(1.0f, coordinates.y);
    ASSERT_FLOAT_EQ(9.0f, coordinates.z);
}

GTEST_TEST(testVector3DFunctions, subscriptOperatorsCanReturnAndSetTheInternalData)
{
    DryPhys::Vector3D coordinates {1.0f, -3.0f, 5.0f};

    ASSERT_FLOAT_EQ(1.0f, coordinates[0]);
    ASSERT_FLOAT_EQ(-3.0f, coordinates[1]);
    ASSERT_FLOAT_EQ(5.0f, coordinates[2]);

    coordinates[0] += 4.0f;
    coordinates[1] += 4.0f;
    coordinates[2] += 4.0f;

    ASSERT_FLOAT_EQ(5.0f, coordinates[0]);
    ASSERT_FLOAT_EQ(1.0f, coordinates[1]);
    ASSERT_FLOAT_EQ(9.0f, coordinates[2]);
}

GTEST_TEST(testVector3DFunctions, overloadedArithmeticOperatorsActOnElementsOfTheVector)
{
    DryPhys::Vector3D vec1 {1.0f, 2.0f, 5.0f};
    DryPhys::Vector3D vec2 {2.0f, -8.0f, 2.0f};

    // Addition
    vec1 += vec2;
    ASSERT_EQ(vec1, DryPhys::Vector3D(3.0f, -6.0f, 7.0f));

    DryPhys::Vector3D sum = vec1 + vec2;
    ASSERT_EQ(sum, DryPhys::Vector3D(5.0f, -14.0f, 9.0f));

    // Subtraction
    vec1 -= vec2;
    ASSERT_EQ(vec1, DryPhys::Vector3D(1.0f, 2.0f, 5.0f));

    DryPhys::Vector3D diff = vec1 - vec2;
    ASSERT_EQ(diff, DryPhys::Vector3D(-1.0f, 10.0f, 3.0f));

    // Multiplication
    vec1 *= 3.0f;
    ASSERT_EQ(vec1, DryPhys::Vector3D(3.0f, 6.0f, 15.0f));

    DryPhys::Vector3D prod = vec2 * 2.0f;
    ASSERT_EQ(prod, DryPhys::Vector3D(4.0f, -16.0f, 4.0f));

    // Division
    vec1 /= 3.0f;
    ASSERT_EQ(vec1, DryPhys::Vector3D(1.0f, 2.0f, 5.0f));

    DryPhys::Vector3D quot = vec2 / 2.0f;
    ASSERT_EQ(quot, DryPhys::Vector3D(1.0f, -4.0f, 1.0f));
}

GTEST_TEST(testVector3DFunctions, opEqualOperatorsCanBeChainedTogether)
{
    DryPhys::Vector3D vec1 {1.0f, 2.0f, 5.0f};
    DryPhys::Vector3D vec2 {2.0f, -8.0f, 2.0f};

    (((((vec1 += vec2) *= 3.0f) -= vec2) *= vec2) /= 2.0f);
    ASSERT_EQ(vec1, DryPhys::Vector3D(7.0f, 40.0f, 19.0f));
}

GTEST_TEST(testVector3DFunctions, clearFunctionSetsAllElementsToZero)
{
    DryPhys::Vector3D vec1 {1.0f, -3.0f, 5.0f};

    testing::internal::CaptureStdout();

    for (unsigned i {}; i < 3; ++i)
    {
        std::cout << (vec1.clear())[i] << std::endl;
    }

    ASSERT_EQ(testing::internal::GetCapturedStdout(), "0\n0\n0\n");

    DryPhys::Vector3D vec2 {1.0f, -3.0f, 5.0f};

    vec2.clear();

    ASSERT_EQ(0.0f, vec2[0]);
    ASSERT_EQ(0.0f, vec2[1]);
    ASSERT_EQ(0.0f, vec2[2]);
}

GTEST_TEST(testVector3DFunctions, invertFunctionSwapsTheSignOfAllElements)
{
    DryPhys::Vector3D vec1 {1.0f, -3.0f, 5.0f};

    testing::internal::CaptureStdout();

    for (unsigned i {}; i < 3; ++i)
    {
        std::cout << (vec1.invert())[i] << std::endl;
    }

    ASSERT_EQ(testing::internal::GetCapturedStdout(), "-1\n-3\n-5\n");

    DryPhys::Vector3D vec2 {1.0f, -3.0f, 5.0f};

    vec2.invert();

    ASSERT_FLOAT_EQ(-1.0f, vec2[0]);
    ASSERT_FLOAT_EQ(3.0f, vec2[1]);
    ASSERT_FLOAT_EQ(-5.0f, vec2[2]);
}

GTEST_TEST(testVector3DFunctions, magnitudeFunctionsCalculateTheLengthOfAVector)
{
    DryPhys::Vector3D vec {1.0f, -2.0f, 4.0f};

    float value        = vec.magnitude();
    float valueSquared = vec.magnitudeSquared();

    ASSERT_FLOAT_EQ(value, std::sqrt(valueSquared));

    ASSERT_FLOAT_EQ(21.0f, valueSquared);
}

GTEST_TEST(testVector3DFunctions, normalizeDividesTheElementsByTheMagnitude)
{
    DryPhys::Vector3D vec {2.0f, -2.0f, -2.0f};

    vec.normalize();

    ASSERT_FLOAT_EQ(1.0f / std::sqrt(3.0f), vec[0]);
    ASSERT_FLOAT_EQ(-1.0f / std::sqrt(3.0f), vec[1]);
    ASSERT_FLOAT_EQ(-1.0f / std::sqrt(3.0f), vec[2]);
}

GTEST_TEST(testVector3DFunctions, vectorMultiplicationHasThreeUniqueForms)
{
    DryPhys::Vector3D vec1 {1.0f, 2.0f, 5.0f};
    DryPhys::Vector3D vec2 {2.0f, -8.0f, 2.0f};

    // Component Product
    vec1 *= vec2;
    ASSERT_EQ(vec1, DryPhys::Vector3D(2.0f, -16.0f, 10.0f));

    DryPhys::Vector3D prod = vec2 * vec2;
    ASSERT_EQ(prod, DryPhys::Vector3D(4.0f, 64.0f, 4.0f));

    // Dot Product
    ASSERT_FLOAT_EQ(152.0f, vec1.dot(vec2));

    // Cross Product
    ASSERT_EQ(vec1.cross(vec2), DryPhys::Vector3D(48.0f, 16.0f, 16.0f));
}

GTEST_TEST(testVector3DFunctions, ourTupleLikeAPIProvidesReadAccessToTheElementsViaStructuredBindings)
{
    DryPhys::Vector3D vec {0.0f, 1.2f, 3.14f};
    const auto& [x, y, z] = vec;

    ASSERT_FLOAT_EQ(x, vec[0]);
    ASSERT_FLOAT_EQ(y, vec[1]);
    ASSERT_FLOAT_EQ(z, vec[2]);
}

GTEST_TEST(testVector3DFunctions, ourTupleLikeAPIProvidesWriteAccessToTheElementsViaStructuredBindings)
{
    DryPhys::Vector3D vec {0.0f, 1.2f, 3.14f};

    auto&& [x, y, z] = vec;

    float value {std::move(z)};

    ASSERT_FLOAT_EQ(value, 3.14f);

    z = 4.2f;
    y += 3.8f;

    ASSERT_FLOAT_EQ(x, 0.0f);
    ASSERT_FLOAT_EQ(y, 5.0f);
    ASSERT_FLOAT_EQ(z, 4.2f);
}

#endif
