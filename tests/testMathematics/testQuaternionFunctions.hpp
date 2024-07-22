// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: testQuaternionFunctions.hpp
// Author: crdrisko
// Date: 07/18/2024-06:30:52
// Description: Provides ~100% unit test coverage over all Quaternion functions

#ifndef DRYPHYS_TESTS_TESTMATHEMATICS_TESTQUATERNIONFUNCTIONS_HPP
#define DRYPHYS_TESTS_TESTMATHEMATICS_TESTQUATERNIONFUNCTIONS_HPP

#include <dryphys/math.hpp>
#include <gtest/gtest.h>

GTEST_TEST(testQuaternionFunctions, differentConstructorsInitializeObjectsAsExpected)
{
    DryPhys::Vector3D vector {2.0f, -8.0f, 2.0f};
    DryPhys::real scalar {-7.0f};

    DryPhys::Quaternion defaultInitialized {};
    DryPhys::Quaternion valuesInitialized {1.0f, 3.0f, 5.0f, 2.0f};
    DryPhys::Quaternion partInitialized {vector, scalar};

    ASSERT_FLOAT_EQ(defaultInitialized.w, 1.0f);
    ASSERT_FLOAT_EQ(defaultInitialized.x, 0.0f);
    ASSERT_FLOAT_EQ(defaultInitialized.y, 0.0f);
    ASSERT_FLOAT_EQ(defaultInitialized.z, 0.0f);

    ASSERT_FLOAT_EQ(valuesInitialized.w, 1.0f);
    ASSERT_FLOAT_EQ(valuesInitialized.x, 3.0f);
    ASSERT_FLOAT_EQ(valuesInitialized.y, 5.0f);
    ASSERT_FLOAT_EQ(valuesInitialized.z, 2.0f);

    ASSERT_FLOAT_EQ(partInitialized.w, -7.0f);
    ASSERT_FLOAT_EQ(partInitialized.x, 2.0f);
    ASSERT_FLOAT_EQ(partInitialized.y, -8.0f);
    ASSERT_FLOAT_EQ(partInitialized.z, 2.0f);
}

GTEST_TEST(testQuaternionFunctions, overloadedComparsionOperatorsPerformElementwiseComparisons)
{
    DryPhys::Quaternion value1 {1.0f, 1.0f, 2.0f, 3.0f};
    DryPhys::Quaternion value2 {};
    DryPhys::Quaternion value3 {1.0f, 1.0f, 2.0f, 3.0f};

    ASSERT_TRUE(value1 == value3);
    ASSERT_FALSE(value1 == value2);

    ASSERT_TRUE(value3 != value2);
    ASSERT_FALSE(value3 != value1);
}

GTEST_TEST(testQuaternionFunctions, theInternalDataIsAccessibleWithoutFunctionCalls)
{
    DryPhys::Quaternion orientation {2.0f, 1.0f, -3.0f, 5.0f};

    ASSERT_FLOAT_EQ(2.0f, orientation.w);
    ASSERT_FLOAT_EQ(1.0f, orientation.x);
    ASSERT_FLOAT_EQ(-3.0f, orientation.y);
    ASSERT_FLOAT_EQ(5.0f, orientation.z);

    orientation.w += 4.0f;
    orientation.x += 4.0f;
    orientation.y += 4.0f;
    orientation.z += 4.0f;

    ASSERT_FLOAT_EQ(6.0f, orientation.w);
    ASSERT_FLOAT_EQ(5.0f, orientation.x);
    ASSERT_FLOAT_EQ(1.0f, orientation.y);
    ASSERT_FLOAT_EQ(9.0f, orientation.z);
}

GTEST_TEST(testQuaternionFunctions, partFunctionsCanReturnAndSetSpecificPartsTheInternalData)
{
    DryPhys::Quaternion orientation {2.0f, 1.0f, -3.0f, 5.0f};

    ASSERT_FLOAT_EQ(orientation.getRealPart(), orientation.w);
    ASSERT_EQ(orientation.getVectorPart(), DryPhys::Vector3D(1.0f, -3.0f, 5.0f));

    orientation.getRealPart() += 4.0f;
    orientation.getVectorPart().normalize();

    DryPhys::real len = std::sqrt(35.0f);

    ASSERT_FLOAT_EQ(6.0f, orientation.w);
    ASSERT_FLOAT_EQ(1.0f / len, orientation.x);
    ASSERT_FLOAT_EQ(-3.0f / len, orientation.y);
    ASSERT_FLOAT_EQ(5.0f / len, orientation.z);
}

GTEST_TEST(testQuaternionFunctions, quaternionMultiplicationIsPreformedWithOverloadedMultiplicationOperators)
{
    DryPhys::Quaternion orientation1 {2.0f, 1.0f, -3.0f, 5.0f};
    DryPhys::Quaternion orientation2 {1.0f, 0.0f, 10.0f, -4.0f};

    DryPhys::Quaternion oreientation_result = orientation1 * orientation2;

    ASSERT_EQ(oreientation_result, DryPhys::Quaternion(52.0f, -37.0f, 21.0f, 7.0f));

    oreientation_result *= DryPhys::Quaternion {};

    ASSERT_EQ(oreientation_result, DryPhys::Quaternion(52.0f, -37.0f, 21.0f, 7.0f));
}

GTEST_TEST(testQuaternionFunctions, opEqualOperatorsCanBeChainedTogether)
{
    DryPhys::Quaternion orientation1 {2.0f, 1.0f, -3.0f, 5.0f};
    DryPhys::Quaternion orientation2 {1.0f, 0.0f, 10.0f, -4.0f};

    ((orientation1 *= orientation2) *= DryPhys::Quaternion {});

    ASSERT_EQ(orientation1, DryPhys::Quaternion(52.0f, -37.0f, 21.0f, 7.0f));
}

GTEST_TEST(testQuaternionFunctions, normalizeDividesTheElementsByTheMagnitude)
{
    DryPhys::Quaternion orientation {2.0f, 1.0f, -3.0f, 5.0f};
    DryPhys::real len = std::sqrt(39.0f);

    orientation.normalize();

    ASSERT_FLOAT_EQ(2.0f / len, orientation.w);
    ASSERT_FLOAT_EQ(1.0f / len, orientation.x);
    ASSERT_FLOAT_EQ(-3.0f / len, orientation.y);
    ASSERT_FLOAT_EQ(5.0f / len, orientation.z);
}

GTEST_TEST(testQuaternionFunctions, normalizeOnALengthZeroQuaternionSetsRealPartTo1)
{
    DryPhys::Quaternion zero {0.0f, 0.0f, 0.0f, 0.0f};
    zero.normalize();

    ASSERT_FLOAT_EQ(1.0f, zero.w);
    ASSERT_FLOAT_EQ(0.0f, zero.x);
    ASSERT_FLOAT_EQ(0.0f, zero.y);
    ASSERT_FLOAT_EQ(0.0f, zero.z);
}

GTEST_TEST(testQuaternionFunctions, ourTupleLikeAPIProvidesReadAccessToTheElementsViaStructuredBindings)
{
    DryPhys::Quaternion q {2.0f, 0.0f, 1.2f, 3.14f};

    const auto& [w, x, y, z] = q;

    ASSERT_FLOAT_EQ(w, q.w);
    ASSERT_FLOAT_EQ(x, q.x);
    ASSERT_FLOAT_EQ(y, q.y);
    ASSERT_FLOAT_EQ(z, q.z);
}

GTEST_TEST(testQuaternionFunctions, ourTupleLikeAPIProvidesWriteAccessToTheElementsViaStructuredBindings)
{
    DryPhys::Quaternion q {2.0f, 0.0f, 1.2f, 3.14f};

    auto&& [w, x, y, z] = q;

    float value {std::move(z)};

    ASSERT_FLOAT_EQ(value, 3.14f);

    z = 4.2f;
    y += 3.8f;

    ASSERT_FLOAT_EQ(w, 2.0f);
    ASSERT_FLOAT_EQ(x, 0.0f);
    ASSERT_FLOAT_EQ(y, 5.0f);
    ASSERT_FLOAT_EQ(z, 4.2f);
}

#endif
