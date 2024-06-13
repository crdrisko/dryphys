### Top Level CMake Script for DryPhys ###
cmake_minimum_required(VERSION 3.12.0)
project(DryPhys VERSION 1.0.0)


### Options ###
option(phys_build_all           "Builds all libraries, unit tests, and code samples." OFF)
option(phys_build_tests         "Builds all libraries and unit tests." OFF)
option(phys_build_samples       "Builds all libraries and code samples." OFF)
option(phys_configure_docs      "Builds all libraries while configuring the Doxygen file for documentation generation." OFF)
option(phys_enable_coverage     "Builds all libraries, unit tests, and enables coverage options." OFF)
option(phys_format_codebase     "Builds all libraries while also formatting the code." OFF)
option(phys_double_precision    "Builds all libraries using double-precision floating point arithmetic enabled." OFF)
option(phys_four_word_alignment "Builds all libraries while padding certain classes to make use of four-word alignment." OFF)

if (phys_format_codebase AND NOT WIN32)
    ## Run clang-format on all code except the files passed as arguments with the '-i' flag ##
    execute_process(COMMAND bash cmake/scripts/format-codebase.sh
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    OUTPUT_QUIET)
endif()

if (phys_enable_coverage)
    set(phys_build_tests ON)
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} --coverage)
endif()

if (phys_configure_docs)
    configure_file(${PROJECT_SOURCE_DIR}/docs/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile)
endif()

if (phys_build_all)
    set(phys_build_tests ON)
    set(phys_build_samples ON)
endif()


### Compilation Options ###
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (WIN32)
    add_compile_options(/MD /WX)
else()
    add_compile_options(-pedantic-errors -Wall -Werror -Wextra)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wno-psabi)
endif()


### Functions ###
include(cmake/utility-functions.cmake)


### External Modules ###
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

find_package(DryChem REQUIRED)

if (NOT ${DryChem_FOUND})
    ## If DryChem isn't installed or can't be found, download it from github ##
    DryPhysExternalDownload(PROJECT DryChem
                            REPOSITORY "https://github.com/crdrisko/drychem.git"
                            TAG main)

    set(COMMON_UTILS_INCLUDE_DIR "${DryChem_SOURCE_DIR}/common-utilities/include")
    set(CPP_UNITS_INCLUDE_DIR "${DryChem_SOURCE_DIR}/cpp-units/include")
    set(DryChem_FOUND TRUE)
endif()

include_directories(${COMMON_UTILS_INCLUDE_DIR})


### Files / Directories ###
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    CACHE PATH "Single directory for all executables.")

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
    CACHE PATH "Single directory for all static libraries.")


### Libraries / Executables ###
configure_file (
  "${PROJECT_SOURCE_DIR}/include/config.h.cmake"
  "${PROJECT_BINARY_DIR}/config.h"
)

include_directories("${PROJECT_BINARY_DIR}" include)


### Installation ###
install(DIRECTORY ${LIBRARIES}
        DESTINATION include)


### Unit Testing ###
if (phys_build_tests)
    find_package(GTest)
    find_package(Threads)

    if (NOT ${GTest_FOUND})
        ## If googletest isn't installed or can't be found, download it from github ##
        DryPhysExternalDownload(PROJECT googletest
                                REPOSITORY "https://github.com/google/googletest.git"
                                TAG main)

        set(GTEST_LIBRARIES gtest)
        set(GTEST_INCLUDE_DIRS "${gtest_SOURCE_DIR}/include")
    endif()

    include_directories(${GTEST_INCLUDE_DIRS})

    include(CTest)
    include(GoogleTest)

    enable_testing()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/tests)

    ## Use a seperate directory for tests to declutter build directory
    add_subdirectory(cmake/Testing ${PROJECT_BINARY_DIR}/Testing)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()
