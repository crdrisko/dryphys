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

#include <dryphys/dryphys.hpp>
#include <gtest/gtest.h>

GTEST_TEST(testParticleFunctions, settersSetAndGettersGet)
{
    DryPhys::Vector3D pos {10.0, 5.0, 12.0};
    DryPhys::Vector3D vel {3.0, 2.0, 1.0};
    DryPhys::Vector3D acc {0.0, 0.5, 0.0};

    DryPhys::Particle particle;

    particle.setPosition(pos);
    particle.setVelocity(vel);
    particle.setAcceleration(acc);
    particle.setDamping(0.99);
    particle.setInverseMass(15.0);

    ASSERT_EQ(particle.getPosition(), DryPhys::Vector3D(10.0, 5.0, 12.0));
    ASSERT_EQ(particle.getVelocity(), DryPhys::Vector3D(3.0, 2.0, 1.0));
    ASSERT_EQ(particle.getAcceleration(), DryPhys::Vector3D(0.0, 0.5, 0.0));

    ASSERT_EQ(0.99, particle.getDamping());
    ASSERT_EQ(15.0, particle.getInverseMass());
}

GTEST_TEST(testParticleFunctions, additionalElementWiseSettersExistForVectorMembers)
{
    DryPhys::real p_x {10.0}, p_y {5.0}, p_z {12.0};
    DryPhys::real v_x {3.0}, v_y {2.0}, v_z {1.0};
    DryPhys::real a_x {0.0}, a_y {0.5}, a_z {0.0};

    DryPhys::Particle particle;

    particle.setPosition(p_x, p_y, p_z);
    particle.setVelocity(v_x, v_y, v_z);
    particle.setAcceleration(a_x, a_y, a_z);

    ASSERT_EQ(particle.getPosition(), DryPhys::Vector3D(10.0, 5.0, 12.0));
    ASSERT_EQ(particle.getVelocity(), DryPhys::Vector3D(3.0, 2.0, 1.0));
    ASSERT_EQ(particle.getAcceleration(), DryPhys::Vector3D(0.0, 0.5, 0.0));
}

GTEST_TEST(testParticleFunctions, particleMovementCanBePerformedInOneFunctionCall)
{
    DryPhys::Vector3D pos {10.0, 5.0, 12.0};
    DryPhys::Vector3D offset {3.0, -2.0, 1.0};
    DryPhys::real duration {2.0};

    DryPhys::Particle particle1;
    DryPhys::Particle particle2;

    particle1.setPosition(pos);
    particle2.setPosition(pos);

    particle1.drift(offset);
    particle2.setPosition(particle2.getPosition() + offset);

    ASSERT_EQ(particle1.getPosition(), particle2.getPosition());

    particle1.drift(offset, duration);
    particle2.setPosition(particle2.getPosition() + offset * duration);

    ASSERT_EQ(particle1.getPosition(), particle2.getPosition());
}

GTEST_TEST(testParticleFunctions, particleVelocityKicksCanBePerformedInOneFunctionCall)
{
    DryPhys::Vector3D vel {10.0, 5.0, 12.0};
    DryPhys::Vector3D offset {0.0, 0.5, 0.0};
    DryPhys::real duration {2.0};

    DryPhys::Particle particle1;
    DryPhys::Particle particle2;

    particle1.setVelocity(vel);
    particle2.setVelocity(vel);

    particle1.kick(offset);
    particle2.setVelocity(particle2.getVelocity() + offset);

    ASSERT_EQ(particle1.getVelocity(), particle2.getVelocity());

    particle1.kick(offset, duration);
    particle2.setVelocity(particle2.getVelocity() + offset * duration);

    ASSERT_EQ(particle1.getVelocity(), particle2.getVelocity());
}

GTEST_TEST(testParticleFunctions, particleDragCanBePerformedInOneFunctionCall)
{
    DryPhys::Vector3D vel {10.0, 5.0, 12.0};
    DryPhys::real damping {0.8}, timestep {0.5};

    DryPhys::Particle particle1;
    DryPhys::Particle particle2;

    particle1.setVelocity(vel);
    particle2.setVelocity(vel);

    particle1.drag(std::pow(damping, timestep));
    particle2.setVelocity(particle2.getVelocity() * std::pow(damping, timestep));

    ASSERT_EQ(particle1.getVelocity(), particle2.getVelocity());
}

GTEST_TEST(testParticleFunctions, additionalGettersExistForVectorMembersForInPlacePointerAssignment)
{
    DryPhys::real p_x {10.0}, p_y {5.0}, p_z {12.0};
    DryPhys::real v_x {3.0}, v_y {2.0}, v_z {1.0};
    DryPhys::real a_x {0.0}, a_y {0.5}, a_z {0.0};

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

    ASSERT_EQ(pos, DryPhys::Vector3D(10.0, 5.0, 12.0));
    ASSERT_EQ(vel, DryPhys::Vector3D(3.0, 2.0, 1.0));
    ASSERT_EQ(acc, DryPhys::Vector3D(0.0, 0.5, 0.0));
}

GTEST_TEST(testParticleFunctions, theInverseMassCanBeSetWithAMassDirectlyButHasExtraConditions)
{
    DryPhys::Particle particle;

    particle.setMass(15.0);

    ASSERT_FLOAT_EQ(15.0, particle.getMass());

    ASSERT_DEBUG_DEATH({ particle.setMass(0.0); }, "Assertion failed");

    particle.setMass(-2.0);
    ASSERT_FLOAT_EQ(std::numeric_limits<DryPhys::real>::max(), particle.getMass());
}

GTEST_TEST(testParticleFunctions, moveADoesNotActOnZeroAndNonPositiveTime)
{
    DryPhys::Particle particle;

    ASSERT_DEBUG_DEATH({ particle.moveA(0.0); }, "Assertion failed");
    ASSERT_DEBUG_DEATH({ particle.moveA(-2.0); }, "Assertion failed");
}

GTEST_TEST(testParticleFunctions, moveBDoesNotActOnZeroAndNonPositiveTime)
{
    DryPhys::Particle particle;

    ASSERT_DEBUG_DEATH({ particle.moveB(0.0); }, "Assertion failed");
    ASSERT_DEBUG_DEATH({ particle.moveB(-2.0); }, "Assertion failed");
}

GTEST_TEST(testParticleFunctions, moveAandBPropogatePositionsAndVelocitiesAccordingToVelocityVerletMethod)
{
    DryPhys::Vector3D pos {10.0, 5.0, 12.0};
    DryPhys::Vector3D vel {3.0, 2.0, 1.0};
    DryPhys::Vector3D acc {0.0, -10.0, 0.0};

    DryPhys::Particle particle;

    particle.setPosition(pos);
    particle.setVelocity(vel);
    particle.setAcceleration(acc);
    particle.setInverseMass(0.5);

    particle.moveA(2.0);

    ASSERT_EQ(particle.getVelocity(), DryPhys::Vector3D(3.0, -8.0, 1.0));
    ASSERT_EQ(particle.getPosition(), DryPhys::Vector3D(16.0, -11.0, 14.0));

    particle.addForce(acc * particle.getMass());
    particle.moveB(2.0);

    ASSERT_EQ(particle.getAcceleration(), DryPhys::Vector3D(0.0, -10.0, 0.0));
    ASSERT_EQ(particle.getVelocity(), DryPhys::Vector3D(3.0, -18.0, 1.0));
}

GTEST_TEST(testParticleFunctions, dampingCanBeUsedToAffectTheVelocityDuringIntegration)
{
    DryPhys::Vector3D pos {10.0, 5.0, 12.0};
    DryPhys::Vector3D vel {3.0, 2.0, 1.0};
    DryPhys::Vector3D acc {0.0, -10.0, 0.0};

    DryPhys::Particle particle;

    particle.setPosition(pos);
    particle.setVelocity(vel);
    particle.setAcceleration(acc);
    particle.setDamping(0.5);
    particle.setInverseMass(0.5);

    particle.moveA(2.0);

    ASSERT_EQ(particle.getPosition(), DryPhys::Vector3D(16.0, -11.0, 14.0));
    ASSERT_EQ(particle.getVelocity(), DryPhys::Vector3D(3.0, -8.0, 1.0));

    particle.addForce(acc * particle.getMass());
    particle.moveB(2.0);

    ASSERT_EQ(particle.getAcceleration(), DryPhys::Vector3D(0.0, -10.0, 0.0));
    ASSERT_EQ(particle.getVelocity(), DryPhys::Vector3D(3.0, -18.0, 1.0));

    particle.drag(std::pow(particle.getDamping(), 2.0));

    ASSERT_EQ(particle.getVelocity(), DryPhys::Vector3D(0.75, -4.5, 0.25));
}

GTEST_TEST(testParticleFunctions, hasFiniteMassReturnsTrueForInverseMassesThatAreNonZeroAndPositive)
{
    DryPhys::Particle particle;

    particle.setInverseMass(5.0);
    ASSERT_TRUE(particle.hasFiniteMass());

    particle.setInverseMass(0.0);
    ASSERT_FALSE(particle.hasFiniteMass());

    particle.setMass(-2.0);
    ASSERT_FALSE(particle.hasFiniteMass());
}

#endif
