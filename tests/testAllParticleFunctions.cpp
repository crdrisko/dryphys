// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: testAllParticleFunctions.cpp
// Author: crdrisko
// Date: 06/14/2024-06:19:00
// Description: Provides ~100% unit test coverage over all Particle functions

#include <limits>

#include <dryphys/integrators/eulersMethod.hpp>
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

    // ASSERT_DEBUG_DEATH({ particle.setMass(0.0f); }, "Assertion failed");

    particle.setMass(-2.0f);
    ASSERT_FLOAT_EQ(std::numeric_limits<DryPhys::real>::max(), particle.getMass());
}

GTEST_TEST(testParticleFunctions, integrateDoesNotActOnZeroAndNonPositiveTime)
{
    DryPhys::Particle particle;

    // ASSERT_DEBUG_DEATH({ particle.integrate(0.0f); }, "Assertion failed");
    // ASSERT_DEBUG_DEATH({ particle.integrate(-2.0f); }, "Assertion failed");
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

    // particle.integrate(2.0f);

    DryPhys::IntegratorRegistry reg;
    DryPhys::Integrator* integrator = new DryPhys::EulersMethod {};

    reg.add(&particle, integrator);

    reg.integrate(2.0f);

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

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
