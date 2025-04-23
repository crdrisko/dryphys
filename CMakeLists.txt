### Top Level CMake Script for DryPhys ###
cmake_minimum_required(VERSION 3.20.0)
project(DryPhys VERSION 1.0.0)


### Options ###
option(phys_build_all           "Builds all libraries, unit tests, and code samples." OFF)
option(phys_build_tests         "Builds all libraries and unit tests." OFF)
option(phys_build_demos         "Builds all libraries and code demos/samples." OFF)
option(phys_configure_docs      "Builds all libraries while configuring the Doxygen file for documentation generation." OFF)
option(phys_enable_coverage     "Builds all libraries, unit tests, and enables coverage options." OFF)
option(phys_format_codebase     "Builds all libraries while also formatting the code." OFF)
option(phys_single_precision    "Builds all libraries using single-precision floating point arithmetic enabled." OFF)
option(phys_exclude_engine      "Builds dryphys libraries while ignoring the partitionEngine library." OFF)

if (phys_format_codebase AND NOT WIN32)
    find_program(formatter clang-format HINTS /opt/homebrew/bin/)

    if (NOT formatter_NOTFOUND)
        message(STATUS "Formatting DryPhys")

        ## Run clang-format on all code except the files passed as arguments with the '-i' flag ##
        execute_process(COMMAND bash cmake/scripts/format-codebase.sh
                                        -f ${formatter}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        OUTPUT_QUIET)
    endif()
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

if (NOT WIN32)
    # add_compile_options(-pedantic-errors -Wall -Werror -Wextra)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13.2)
        message(FATAL_ERROR "GCC version must be at least 13.2! " ${CMAKE_CXX_COMPILER_VERSION})
    endif()

    add_compile_options(-Wno-psabi)
endif()


### Functions ###
include(cmake/utility-functions.cmake)


### External Modules ###
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(FetchContent)
find_package(DryChem)

if (NOT DryChem_FOUND)
    ## If DryChem isn't installed or can't be found, download it from github ##
    message(STATUS "Fetching DryChem...")
    FetchContent_Declare(drychem        # v1.0.0
                         GIT_REPOSITORY "https://github.com/crdrisko/drychem.git"
                         GIT_TAG        "714dded492aca9a59c7d3f167839abe57daec12e")

    FetchContent_MakeAvailable(drychem)

    set(DryChem_FOUND TRUE)
    set(COMMON_UTILS_INCLUDE_DIR "${drychem_SOURCE_DIR}/common-utilities/include")
endif()

include_directories(${COMMON_UTILS_INCLUDE_DIR})

if (NOT phys_exclude_engine)
    ## ImGui ##
    message(STATUS "Fetching ImGui...")
    FetchContent_Declare(imgui          # v1.90.5
                         GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
                         GIT_TAG        "231cbee0fc4f59dbe5b8b853a11b08dc84e57c65")

    FetchContent_MakeAvailable(imgui)
    include_directories(${imgui_SOURCE_DIR})

    set(IMGUI_SOURCE ${imgui_SOURCE_DIR}/imgui.cpp
                     ${imgui_SOURCE_DIR}/imgui_demo.cpp
                     ${imgui_SOURCE_DIR}/imgui_draw.cpp
                     ${imgui_SOURCE_DIR}/imgui_tables.cpp
                     ${imgui_SOURCE_DIR}/imgui_widgets.cpp)

    # ## SFML ##
    # message(STATUS "Fetching SFML...")
    # FetchContent_Declare(sfml           # v2.6.1
    #                      GIT_REPOSITORY "https://github.com/SFML/SFML.git"
    #                      GIT_TAG        "69ea0cd863aed1d4092b970b676924a716ff718b")

    # set(SFML_BUILD_AUDIO ON)
    # set(SFML_BUILD_NETWORK OFF)

    # FetchContent_MakeAvailable(sfml)

    # ## ImGui-SFML ##
    # message(STATUS "Fetching ImGui-SFML...")
    # FetchContent_Declare(imgui_sfml     # v2.6
    #                      GIT_REPOSITORY "https://github.com/SFML/imgui-sfml.git"
    #                      GIT_TAG        "de565ac8f2b795dedc0307b60830cb006afd2ecd")

    # set(IMGUI_DIR ${imgui_SOURCE_DIR})
    # set(IMGUI_SFML_FIND_SFML OFF)
    # set(IMGUI_SFML_IMGUI_DEMO ON)

    # FetchContent_MakeAvailable(imgui_sfml)
    # include_directories(${imgui_sfml_SOURCE_DIR})

    # set(IMGUI_SFML_SOURCE ${IMGUI_SOURCE}
    #                       ${imgui_sfml_SOURCE_DIR}/imgui-SFML.cpp)

    ## GLFW ##
    find_package(glfw3 REQUIRED)
    # find_package(glm REQUIRED)

    include_directories(${imgui_SOURCE_DIR}/backends)
    include_directories(${PROJECT_SOURCE_DIR}/extern)
    include_directories(/opt/homebrew/include/glm)

    set(IMGUI_GLFW_SOURCE ${IMGUI_SOURCE}
                          ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp 
                          ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
    
    if(APPLE)
        find_package(Freetype REQUIRED)
        set(FREETYPE_LIBRARIES /opt/homebrew/opt/freetype/lib/libfreetype.dylib)
        include_directories(${FREETYPE_INCLUDE_DIRS})
    endif()
endif()


### Files / Directories ###
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    CACHE PATH "Single directory for all executables.")

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
    CACHE PATH "Single directory for all static libraries.")


### Libraries / Executables ###
configure_file("${PROJECT_SOURCE_DIR}/include/config.hpp.cmake" "${PROJECT_SOURCE_DIR}/include/dryphys/utilities/config.hpp")

include_directories("${PROJECT_SOURCE_DIR}/include")

set(DRYPHYS_SOURCE src/dryphys/particleSystems/constraints.cpp
                   src/dryphys/particleSystems/constraintGenerators/groundConstraints.cpp
                   src/dryphys/particleSystems/constraintGenerators/links.cpp
                   src/dryphys/particleSystems/forceGenerator.cpp
                   src/dryphys/particleSystems/forceGenerators/buoyancy.cpp
                   src/dryphys/particleSystems/forceGenerators/drag.cpp
                   src/dryphys/particleSystems/forceGenerators/gravity.cpp
                   src/dryphys/particleSystems/forceGenerators/springs.cpp
                   src/dryphys/particleSystems/world.cpp
                   src/dryphys/rigidBodySystems/forceGenerator.cpp
                   src/dryphys/rigidBodySystems/forceGenerators/aero.cpp
                   src/dryphys/rigidBodySystems/forceGenerators/buoyancy.cpp
                   src/dryphys/rigidBodySystems/forceGenerators/gravity.cpp
                   src/dryphys/rigidBodySystems/forceGenerators/springs.cpp
                   src/dryphys/rigidBodySystems/world.cpp
                   src/dryphys/types/particle.cpp
                   src/dryphys/types/rigidBody.cpp)

add_library(dryphys STATIC ${DRYPHYS_SOURCE})

set(LIBRARIES include/dryphys)

if (NOT phys_exclude_engine)
    ## 2D-SFML Engine ##
    # set(ENGINE2D_SOURCE src/engine2d/animation.cpp
    #                     src/engine2d/assets.cpp
    #                     src/engine2d/engine.cpp
    #                     src/engine2d/physics.cpp
    #                     src/engine2d/scene.cpp
    #                     src/engine2d/textureSheet.cpp)

    # add_library(engine2d STATIC ${ENGINE2D_SOURCE})
    # add_library(engine2d_gui STATIC ${ENGINE2D_SOURCE})

    # target_link_libraries(engine2d PRIVATE dryphys ImGui-SFML::ImGui-SFML sfml-audio)
    # target_link_libraries(engine2d_gui PRIVATE dryphys ImGui-SFML::ImGui-SFML sfml-audio)
    # set_target_properties(engine2d_gui PROPERTIES COMPILE_DEFINITIONS USE_IMGUI)

    ## 2D/3D PartitionEngine ##
    set(PARTITIONENGINE_SOURCE src/partitionEngine/window/window.cpp)
                            #    src/partitionEngine/engine/assets.cpp
                            #    src/partitionEngine/engine/engine.cpp
                            #    src/partitionEngine/engine/scene.cpp
                            #    src/partitionEngine/graphics/shaders.cpp
                            #    src/partitionEngine/graphics/textureSheet.cpp

    set(EXTERNAL_SOURCE ${PROJECT_SOURCE_DIR}/extern/glad/glad.c
                        ${PROJECT_SOURCE_DIR}/extern/stb_image/stb_image.cpp)

    add_library(partitionEngine STATIC ${PARTITIONENGINE_SOURCE} ${EXTERNAL_SOURCE} ${IMGUI_GLFW_SOURCE})

    target_link_libraries(partitionEngine PUBLIC dryphys ${FREETYPE_LIBRARIES})
    set_target_properties(partitionEngine PROPERTIES COMPILE_DEFINITIONS USE_IMGUI)

    set(LIBRARIES ${LIBRARIES} include/partitionEngine) # include/engine2d

    if (phys_build_demos)
        add_subdirectory(src/demos)
    endif()
endif()


### Installation ###
install(DIRECTORY ${LIBRARIES}
        DESTINATION include)

install(TARGETS dryphys
        DESTINATION lib)

if (NOT phys_exclude_engine)
    install(TARGETS partitionEngine #engine2d engine2d_gui 
            DESTINATION lib)
endif()


### Unit Testing ###
if (phys_build_tests)
    # find_package(GTest 1.15.2)
    find_package(Threads)

    if (NOT GTEST_FOUND)
        ## If googletest isn't installed or can't be found, download it from github ##
        message(STATUS "Fetching GoogleTest...")
        FetchContent_Declare(googletest     # v1.15.2
                             GIT_REPOSITORY "https://github.com/google/googletest.git"
                             GIT_TAG        "b514bdc898e2951020cbdca1304b75f5950d1f59")
                            
        FetchContent_MakeAvailable(googletest)

        set(GTEST_FOUND TRUE)
        set(GTEST_LIBRARIES gtest)
        set(GTEST_INCLUDE_DIRS "${gtest_SOURCE_DIR}/include")
    else()
        set(GTEST_INCLUDE_DIRS "${googletest_SOURCE_DIR}/include")
    endif()

    include_directories(${GTEST_INCLUDE_DIRS})

    include(CTest)
    include(GoogleTest)

    enable_testing()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/tests)

    ## Use a seperate directory for tests to declutter build directory ##
    add_subdirectory(cmake/Testing ${PROJECT_BINARY_DIR}/Testing)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()
