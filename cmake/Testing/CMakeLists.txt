### Lower Level CMake Script for DryPhys Tests ###
cmake_minimum_required(VERSION 3.20.0)
project(DryPhysUnitTesting VERSION 1.0.0)


### Unit Tests ###
if (phys_build_tests)
    # if (NOT phys_exclude_engine)
    #     DryPhysNewTest(TESTNAME AllEngine2DFunctions)
    # endif()

    DryPhysNewTest(TESTNAME AllMathFunctions)
    DryPhysNewTest(TESTNAME AllParticleSystemFunctions)
    DryPhysNewTest(TESTNAME AllUtilityFunctions)
endif()
