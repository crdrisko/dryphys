### Lower Level CMake Script for DryChem ###
cmake_minimum_required(VERSION 3.12.0)
project(UnitTesting VERSION 1.0.0)


### Unit Tests ###
if (phys_build_tests)
    if (NOT phys_exclude_engine)
        DryPhysNewTest(TESTNAME AllEngine2DFunctions)
    endif()

    DryPhysNewTest(TESTNAME AllMathFunctions)
    DryPhysNewTest(TESTNAME AllParticleSystemFunctions)
    DryPhysNewTest(TESTNAME AllUtilityFunctions)
endif()
