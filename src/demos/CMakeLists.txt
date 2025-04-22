### Lower Level CMake Script for DryPhys Demos ###
cmake_minimum_required(VERSION 3.20.0)
project(DryPhysDemos VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DryPhys_BINARY_DIR}/bin/demos)

## CyberCity Demo ##
# set(CYBERCITY_SOURCE src/demos/cybercity/main.cpp
#                      src/demos/cybercity/ai.cpp
#                      src/demos/cybercity/scenes/sceneMenu.cpp
#                      src/demos/cybercity/scenes/scenePlay.cpp
#                      src/demos/cybercity/states/playerStates.cpp
#                      src/demos/cybercity/states/states.cpp)

# add_executable(cyberCity ${CYBERCITY_SOURCE})
# target_include_directories(cyberCity PRIVATE ${PROJECT_SOURCE_DIR}/src/demos)
# target_link_libraries(cyberCity PRIVATE engine2d_gui)
# set_target_properties(cyberCity PROPERTIES COMPILE_DEFINITIONS USE_IMGUI)

# set(CYBERCITY_CONFIG_FILES CP-Assets.txt
#                            CP-Level1.txt
#                            CP-Level2.txt
#                            CP-Level3.txt)

# foreach (FILE ${CYBERCITY_CONFIG_FILES})
#     configure_file(src/demos/cybercity/config/${FILE} ${PROJECT_BINARY_DIR}/etc/${FILE})
# endforeach()

## Geometry Waves Demo ##
# set(GEOMETRYWAVES_SOURCE src/demos/geometryWaves/main.cpp
#                          src/demos/geometryWaves/scenes/scenePlay.cpp)

# add_executable(geometryWaves ${GEOMETRYWAVES_SOURCE})
# target_include_directories(geometryWaves PRIVATE ${PROJECT_SOURCE_DIR}/src/demos)
# target_link_libraries(geometryWaves PRIVATE engine2d)

# configure_file(src/demos/geometryWaves/config/GW-Config.txt ${PROJECT_BINARY_DIR}/etc/GW-Config.txt)

## Light Cycle Demo ##
# set(LIGHTCYCLE_SOURCE src/demos/lightCycle/main.cpp
#                       src/demos/lightCycle/ai.cpp
#                       src/demos/lightCycle/scenes/scenePlay.cpp)

# add_executable(lightCycle ${LIGHTCYCLE_SOURCE})
# target_include_directories(lightCycle PRIVATE ${PROJECT_SOURCE_DIR}/src/demos)
# target_link_libraries(lightCycle PRIVATE engine2d)

# configure_file(src/demos/lightCycle/config/LC-Config.txt ${PROJECT_BINARY_DIR}/etc/LC-Config.txt)

## Shape Watcher Demo ##
# set(SHAPEWATCHER_SOURCE src/demos/shapeWatcher/main.cpp
#                         src/demos/shapeWatcher/scenes/scenePlay.cpp)

# add_executable(shapeWatcher ${SHAPEWATCHER_SOURCE})
# target_include_directories(shapeWatcher PRIVATE ${PROJECT_SOURCE_DIR}/src/demos)
# target_link_libraries(shapeWatcher PRIVATE engine2d)

# configure_file(src/demos/shapeWatcher/config/SW-Config.txt ${PROJECT_BINARY_DIR}/etc/SW-Config.txt)

## SpaceShooter Demo ##
# set(SPACESHOOTER_SOURCE src/demos/spaceShooter/main.cpp
#                         src/demos/spaceShooter/scenes/sceneSpace.cpp)

# add_executable(spaceShooter ${SPACESHOOTER_SOURCE})
# target_include_directories(spaceShooter PRIVATE ${PROJECT_SOURCE_DIR}/src/demos)
# target_link_libraries(spaceShooter PRIVATE engine2d_gui)
# set_target_properties(spaceShooter PROPERTIES COMPILE_DEFINITIONS USE_IMGUI)

# add_executable(spriteSheetMaker src/demos/spaceShooter/assets/spriteSheetMaker.cpp)
# target_include_directories(spriteSheetMaker PRIVATE ${PROJECT_SOURCE_DIR}/src/demos)
# target_link_libraries(spriteSheetMaker PRIVATE engine2d_gui)
# set_target_properties(spriteSheetMaker PROPERTIES COMPILE_DEFINITIONS USE_IMGUI)

# set(SPACESHOOTER_CONFIG_FILES SS-Config.txt)

# foreach (FILE ${SPACESHOOTER_CONFIG_FILES})
#     configure_file(src/demos/spaceShooter/config/${FILE} ${PROJECT_BINARY_DIR}/etc/${FILE})
# endforeach()

## Samples ##
# set(LIGHTINGSYSTEM_SOURCE src/demos/samples/lightingSystem/main.cpp)

# add_executable(lightingSystem ${LIGHTINGSYSTEM_SOURCE})
# target_include_directories(lightingSystem PRIVATE ${PROJECT_SOURCE_DIR}/src/demos/samples)
# target_link_libraries(lightingSystem PRIVATE ImGui-SFML::ImGui-SFML)

# set(PARTICLESYSTEM_SOURCE src/demos/samples/particleSystem/main.cpp
#                         src/demos/samples/particleSystem/particleSystem.cpp)

# add_executable(particleSystem ${PARTICLESYSTEM_SOURCE})
# target_include_directories(particleSystem PRIVATE ${PROJECT_SOURCE_DIR}/src/demos/samples)
# target_link_libraries(particleSystem PRIVATE ImGui-SFML::ImGui-SFML dryphys)

## GLFW ##
# add_executable(test_glfw glfw/test.cpp)
# target_link_libraries(test_glfw PRIVATE glfw partitionEngine "-framework opengl")
# # set_target_properties(test_glfw PROPERTIES COMPILE_DEFINITIONS USE_IMGUI)

# add_executable(learnopengl glfw/learnopengl.cpp)
# target_link_libraries(learnopengl PRIVATE glfw partitionEngine "-framework opengl")

# set(GLFW_DEMO_SOURCE glfw/demo3d.cpp
#                     #  glfw/scenes/scene.cpp
#                      glfw/scenes/sceneMenu.cpp)

# add_executable(demo3d ${GLFW_DEMO_SOURCE})
# target_include_directories(demo3d PRIVATE ${DryPhys_SOURCE_DIR}/src/demos)
# target_link_libraries(demo3d PRIVATE glfw partitionEngine "-framework opengl")
# set_target_properties(demo3d PROPERTIES COMPILE_DEFINITIONS USE_IMGUI)

# configure_file(glfw/config/GL-Config.txt ${DryPhys_BINARY_DIR}/etc/GL-Config.txt)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DryPhys_BINARY_DIR}/bin)
