// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: testParticleFunctions.hpp
// Author: crdrisko
// Date: 07/18/2024-06:37:44
// Description: Provides ~100% unit test coverage over all Particle functions

#ifndef DRYPHYS_TESTS_TESTPARTICLESYSTEMS_TESTPARTICLEFUNCTIONS_HPP
#define DRYPHYS_TESTS_TESTPARTICLESYSTEMS_TESTPARTICLEFUNCTIONS_HPP

#include <limits>

#include <dryphys/math.hpp>
#include <dryphys/particle.hpp>
#include <gtest/gtest.h>

GTEST_TEST(testParticleFunctions, settersSetAndGettersGet)
{
    DryPhys::Vector3D pos {10.0f, 5.0f, 12.0f};
    DryPhys::Vector3D vel {3.0f, 2.0f, 1.0f};
    DryPhys::Vector3D acc {0.0f, 0.5f, 0.0f};

    DryPhys::Particle particle;

    particle.setPosition(pos);
    particle.setVelocity(vel);
    particle.setAcceleration(acc);
    particle.setDamping(0.99f);
    particle.setInverseMass(15.0f);

    ASSERT_EQ(particle.getPosition(), DryPhys::Vector3D(10.0f, 5.0f, 12.0f));
    ASSERT_EQ(particle.getVelocity(), DryPhys::Vector3D(3.0f, 2.0f, 1.0f));
    ASSERT_EQ(particle.getAcceleration(), DryPhys::Vector3D(0.0f, 0.5f, 0.0f));

    ASSERT_EQ(0.99f, particle.getDamping());
    ASSERT_EQ(15.0f, particle.getInverseMass());
}

GTEST_TEST(testParticleFunctions, additionalElementWiseSettersExistForVectorMembers)
{
    DryPhys::real p_x {10.0f}, p_y {5.0f}, p_z {12.0f};
    DryPhys::real v_x {3.0f}, v_y {2.0f}, v_z {1.0f};
    DryPhys::real a_x {0.0f}, a_y {0.5f}, a_z {0.0f};

    DryPhys::Particle particle;

    particle.setPosition(p_x, p_y, p_z);
    particle.setVelocity(v_x, v_y, v_z);
    particle.setAcceleration(a_x, a_y, a_z);

    ASSERT_EQ(particle.getPosition(), DryPhys::Vector3D(10.0f, 5.0f, 12.0f));
    ASSERT_EQ(particle.getVelocity(), DryPhys::Vector3D(3.0f, 2.0f, 1.0f));
    ASSERT_EQ(particle.getAcceleration(), DryPhys::Vector3D(0.0f, 0.5f, 0.0f));
}

GTEST_TEST(testParticleFunctions, particleMovementCanBePerformedInOneFunctionCall)
{
    DryPhys::Vector3D pos {10.0f, 5.0f, 12.0f};
    DryPhys::Vector3D offset {3.0f, -2.0f, 1.0f};

    DryPhys::Particle particle1;
    DryPhys::Particle particle2;

    particle1.setPosition(pos);
    particle2.setPosition(pos);

    particle1.move(offset);
    particle2.setPosition(particle2.getPosition() + offset);

    ASSERT_EQ(particle1.getPosition(), particle2.getPosition());

    particle1.move(-4.0f, 8.0f, 3.7f);

    ASSERT_EQ(particle1.getPosition(), DryPhys::Vector3D(9.0f, 11.0f, 16.7f));
}

GTEST_TEST(testParticleFunctions, particleVelocityKicksCanBePerformedInOneFunctionCall)
{
    DryPhys::Vector3D vel {10.0f, 5.0f, 12.0f};
    DryPhys::Vector3D offset {0.0f, 0.5f, 0.0f};

    DryPhys::Particle particle1;
    DryPhys::Particle particle2;

    particle1.setVelocity(vel);
    particle2.setVelocity(vel);

    particle1.kick(offset);
    particle2.setVelocity(particle2.getVelocity() + offset);

    ASSERT_EQ(particle1.getVelocity(), particle2.getVelocity());

    particle1.kick(3.0f, 2.0f, 1.0f);

    ASSERT_EQ(particle1.getVelocity(), DryPhys::Vector3D(13.0f, 7.5f, 13.0f));
}

GTEST_TEST(testParticleFunctions, additionalGettersExistForVectorMembersForInPlacePointerAssignment)
{
    DryPhys::real p_x {10.0f}, p_y {5.0f}, p_z {12.0f};
    DryPhys::real v_x {3.0f}, v_y {2.0f}, v_z {1.0f};
    DryPhys::real a_x {0.0f}, a_y {0.5f}, a_z {0.0f};

    DryPhys::Particle particle;

    particle.setPosition(p_x, p_y, p_z);
    particle.setVelocity(v_x, v_y, v_z);
    particle.setAcceleration(a_x, a_y, a_z);

    DryPhys::Vector3D pos;
    DryPhys::Vector3D vel;
    DryPhys::Vector3D acc;

    particle.getPosition(&pos);
    particle.getVelocity(&vel);
    particle.getAcceleration(&acc);

    ASSERT_EQ(pos, DryPhys::Vector3D(10.0f, 5.0f, 12.0f));
    ASSERT_EQ(vel, DryPhys::Vector3D(3.0f, 2.0f, 1.0f));
    ASSERT_EQ(acc, DryPhys::Vector3D(0.0f, 0.5f, 0.0f));
}

GTEST_TEST(testParticleFunctions, theInverseMassCanBeSetWithAMassDirectlyButHasExtraConditions)
{
    DryPhys::Particle particle;

    particle.setMass(15.0f);

    ASSERT_FLOAT_EQ(15.0f, particle.getMass());

    ASSERT_DEBUG_DEATH({ particle.setMass(0.0f); }, "Assertion failed");

    particle.setMass(-2.0f);
    ASSERT_FLOAT_EQ(std::numeric_limits<DryPhys::real>::max(), particle.getMass());
}

GTEST_TEST(testParticleFunctions, integrateDoesNotActOnZeroAndNonPositiveTime)
{
    DryPhys::Particle particle;

    ASSERT_DEBUG_DEATH({ particle.integrate(0.0f); }, "Assertion failed");
    ASSERT_DEBUG_DEATH({ particle.integrate(-2.0f); }, "Assertion failed");
}

GTEST_TEST(testParticleFunctions, integrateDoesNothingIfMassIsNotFinite)
{
    DryPhys::Particle before;
    DryPhys::Particle after {before};

    ASSERT_FALSE(after.hasFiniteMass());

    after.integrate(2.0f);

    ASSERT_EQ(before.getPosition(), after.getPosition());
    ASSERT_EQ(before.getVelocity(), after.getVelocity());
}

GTEST_TEST(testParticleFunctions, integratePropogatesPositionsAndVelocitiesAccordingToEulersMethod)
{
    DryPhys::Vector3D pos {10.0f, 5.0f, 12.0f};
    DryPhys::Vector3D vel {3.0f, 2.0f, 1.0f};
    DryPhys::Vector3D acc {0.0f, 0.5f, 0.0f};

    DryPhys::Particle particle;

    particle.setPosition(pos);
    particle.setVelocity(vel);
    particle.setAcceleration(acc);
    particle.setDamping(1.0f);
    particle.setInverseMass(15.0f);

    particle.integrate(2.0f);

    ASSERT_EQ(particle.getPosition(), DryPhys::Vector3D(16.0f, 9.0f, 14.0f));
    ASSERT_EQ(particle.getVelocity(), DryPhys::Vector3D(3.0f, 3.0f, 1.0f));
}

GTEST_TEST(testParticleFunctions, dampingAffectsTheVelocitiesDuringIntegration)
{
    DryPhys::Vector3D pos {16.0f, 9.0f, 14.0f};
    DryPhys::Vector3D vel {3.0f, 3.0f, 1.0f};
    DryPhys::Vector3D acc {0.0f, 0.5f, 0.0f};

    DryPhys::Particle particle;

    particle.setPosition(pos);
    particle.setVelocity(vel);
    particle.setAcceleration(acc);
    particle.setDamping(0.5f);
    particle.setInverseMass(15.0f);

    particle.integrate(2.0f);

    ASSERT_EQ(particle.getPosition(), DryPhys::Vector3D(22.0f, 15.0f, 16.0f));
    ASSERT_EQ(particle.getVelocity(), DryPhys::Vector3D(0.75f, 1.0f, 0.25f));
}

GTEST_TEST(testParticleFunctions, hasFiniteMassReturnsTrueForInverseMassesThatAreNonZeroAndPositive)
{
    DryPhys::Particle particle;

    particle.setInverseMass(5.0f);
    ASSERT_TRUE(particle.hasFiniteMass());

    particle.setInverseMass(0.0f);
    ASSERT_FALSE(particle.hasFiniteMass());

    particle.setMass(-2.0f);
    ASSERT_FALSE(particle.hasFiniteMass());
}

#endif
