### Top Level CMake Script for DryPhys ###
cmake_minimum_required(VERSION 3.12.0)
project(DryPhys VERSION 1.0.0)


### Options ###
option(phys_build_all           "Builds all libraries, unit tests, and code samples." OFF)
option(phys_build_tests         "Builds all libraries and unit tests." OFF)
option(phys_build_demos         "Builds all libraries and code demos/samples." OFF)
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
    set(phys_build_demos ON)
endif()


### Compilation Options ###
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (WIN32)
    # add_compile_options(/MD /WX)
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
    set(DryChem_FOUND TRUE)
endif()

include_directories(${COMMON_UTILS_INCLUDE_DIR})

include(FetchContent)

## ImGui
message(STATUS "Fetching ImGUI...")
FetchContent_Declare(imgui          # v1.90.5
                     GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
                     GIT_TAG "231cbee0fc4f59dbe5b8b853a11b08dc84e57c65")

FetchContent_MakeAvailable(imgui)
include_directories(${imgui_SOURCE_DIR})

## SFML
message(STATUS "Fetching SFML...")
FetchContent_Declare(sfml           # v2.6.1
                     GIT_REPOSITORY "https://github.com/SFML/SFML.git"
                     GIT_TAG "69ea0cd863aed1d4092b970b676924a716ff718b")

set(SFML_BUILD_AUDIO ON)
set(SFML_BUILD_NETWORK OFF)

FetchContent_MakeAvailable(sfml)
include_directories(${sfml_SOURCE_DIR}/include)

## ImGui-SFML
message(STATUS "Fetching ImGui-SFML...")
FetchContent_Declare(imgui_sfml     # v2.6
                     GIT_REPOSITORY "https://github.com/SFML/imgui-sfml.git"
                     GIT_TAG "de565ac8f2b795dedc0307b60830cb006afd2ecd")

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

FetchContent_MakeAvailable(imgui_sfml)
include_directories(${imgui_sfml_SOURCE_DIR})

set(IMGUI_SOURCE ${imgui_SOURCE_DIR}/imgui.cpp
                 ${imgui_SOURCE_DIR}/imgui_demo.cpp
                 ${imgui_SOURCE_DIR}/imgui_draw.cpp
                 ${imgui_SOURCE_DIR}/imgui_tables.cpp
                 ${imgui_SOURCE_DIR}/imgui_widgets.cpp
                 ${imgui_sfml_SOURCE_DIR}/imgui-SFML.cpp)


### Files / Directories ###
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    CACHE PATH "Single directory for all executables.")

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
    CACHE PATH "Single directory for all static libraries.")


### Libraries / Executables ###
configure_file("${PROJECT_SOURCE_DIR}/include/config.h.cmake" "${PROJECT_BINARY_DIR}/config.h")

include_directories("${PROJECT_BINARY_DIR}" include)

set(DRYPHYS_SOURCE src/dryphys/eulersMethod.cpp
                   src/dryphys/integrator.cpp
                   src/dryphys/particle.cpp
                   src/dryphys/particleBuoyancy.cpp
                   src/dryphys/particleDrag.cpp
                   src/dryphys/particleGravity.cpp
                   src/dryphys/particleSprings.cpp
                   src/dryphys/pfgen.cpp)

set(ENGINE2D_SOURCE src/engine2d/animation.cpp
                    src/engine2d/assets.cpp
                    src/engine2d/engine.cpp
                    src/engine2d/scene.cpp
                    src/engine2d/textureSheet.cpp)

add_library(engine2d STATIC ${ENGINE2D_SOURCE})
add_library(dryphys STATIC ${DRYPHYS_SOURCE})

target_link_libraries(engine2d PRIVATE dryphys sfml-audio)

set(LIBRARIES include/dryphys
              include/engine2d)

if (phys_build_demos)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/demos)

    ## CyberCity Demo
    set(CYBERCITY_SOURCE src/demos/cybercity/main.cpp
                         src/demos/cybercity/physics.cpp
                         src/demos/cybercity/scenes/sceneMenu.cpp
                         src/demos/cybercity/scenes/scenePlay.cpp
                         src/demos/cybercity/states/playerStates.cpp
                         src/demos/cybercity/states/states.cpp)

    add_executable(cyberCity ${CYBERCITY_SOURCE} ${IMGUI_SOURCE})
    target_include_directories(cyberCity PRIVATE ${PROJECT_SOURCE_DIR}/src/demos)
    target_link_libraries(cyberCity PRIVATE ImGui-SFML::ImGui-SFML engine2d)

    set(CYBERCITY_CONFIG_FILES CP-Assets.txt
                               CP-Level1.txt
                               CP-Level2.txt
                               CP-Level3.txt)

    foreach (FILE ${CYBERCITY_CONFIG_FILES})
        configure_file(src/demos/cybercity/config/${FILE} ${PROJECT_BINARY_DIR}/etc/${FILE})
    endforeach()

    ## Geometry Waves Demo
    set(GEOMETRYWAVES_SOURCE src/demos/geometryWaves/main.cpp
                             src/demos/geometryWaves/entityManager.cpp
                             src/demos/geometryWaves/scenes/scenePlay.cpp)

    add_executable(geometryWaves ${GEOMETRYWAVES_SOURCE} ${IMGUI_SOURCE})
    target_include_directories(geometryWaves PRIVATE ${PROJECT_SOURCE_DIR}/src/demos)
    target_link_libraries(geometryWaves PRIVATE ImGui-SFML::ImGui-SFML engine2d)

    configure_file(src/demos/geometryWaves/config/GW-Config.txt ${PROJECT_BINARY_DIR}/etc/GW-Config.txt)

    ## Light Cycle Demo

    ## Particle System Demo
    set(PARTICLESYSTEM_SOURCE src/demos/particleSystem/main.cpp
                              src/demos/particleSystem/particleSystem.cpp)

    add_executable(particleSystem ${PARTICLESYSTEM_SOURCE} ${IMGUI_SOURCE})
    target_include_directories(particleSystem PRIVATE ${PROJECT_SOURCE_DIR}/src/demos)
    target_link_libraries(particleSystem PRIVATE ImGui-SFML::ImGui-SFML)

    ## Shape Watcher Demo
    set(SHAPEWATCHER_SOURCE src/demos/shapeWatcher/main.cpp
                            src/demos/shapeWatcher/entityManager.cpp
                            src/demos/shapeWatcher/scenes/scenePlay.cpp)

    add_executable(shapeWatcher ${SHAPEWATCHER_SOURCE} ${IMGUI_SOURCE})
    target_include_directories(shapeWatcher PRIVATE ${PROJECT_SOURCE_DIR}/src/demos)
    target_link_libraries(shapeWatcher PRIVATE ImGui-SFML::ImGui-SFML engine2d)

    configure_file(src/demos/shapeWatcher/config/SW-Config.txt ${PROJECT_BINARY_DIR}/etc/SW-Config.txt)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()


### Installation ###
install(DIRECTORY ${LIBRARIES}
        DESTINATION include)

install(TARGETS dryphys engine2d
        DESTINATION lib)


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
