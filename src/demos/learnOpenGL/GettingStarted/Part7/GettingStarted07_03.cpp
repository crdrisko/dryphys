// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: GettingStarted07_03.cpp
// Author: crdrisko
// Date: 04/21/2025-07:00:25
// Description:

#include <cmath>
#include <map>
#include <memory>
#include <string>

#include <dryphys/dryphys.hpp>
#include <partitionEngine/partitionEngine.hpp>

#include "learnOpenGL/learnOpenGL.hpp"

using namespace PartitionEngine;

struct
{
    DryPhys::Vector3D position {0.0, 0.0, 3.0};
    DryPhys::Vector3D front {0.0, 0.0, -1.0};
    DryPhys::Vector3D up {0.0, 1.0, 0.0};

    DryPhys::real speed {0.05};
    DryPhys::real fov {45.0};
    DryPhys::real sensitivity {0.1};
} cameraData;

void processInput(Window& window);
void processMouseScroll(DryPhys::real offset);
void processMouseMovement(DryPhys::real xposIn, DryPhys::real yposIn);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <config_file>\n";
        return EXIT_FAILURE;
    }

    Window window {800, 600, "Getting Started: Camera Mouse Zoom"};

    glfwSetInputMode(window.window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Assets assets {};
    assets.loadFromFile(argv[1]);

    const DryPhys::Transform4D ident {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};

    ShaderProgram* shaderProgram = assets.getShaderProgram("Example6_1");

    shaderProgram->use();
    shaderProgram->setUniform("texture1", assets.getTextureSheet("Container").getTextureID());
    shaderProgram->setUniform("texture2", assets.getTextureSheet("AwesomeFace").getTextureID());

    auto meshes = parseMeshFile("cube.mesh");

    std::vector<DryPhys::Vector3D> cubePositions(10);

    cubePositions[0] = DryPhys::Vector3D {0.0f, 0.0f, 0.0f};
    cubePositions[1] = DryPhys::Vector3D {2.0f, 5.0f, -15.0f};
    cubePositions[2] = DryPhys::Vector3D {-1.5f, -2.2f, -2.5f};
    cubePositions[3] = DryPhys::Vector3D {-3.8f, -2.0f, -12.3f};
    cubePositions[4] = DryPhys::Vector3D {2.4f, -0.4f, -3.5f};
    cubePositions[5] = DryPhys::Vector3D {-1.7f, 3.0f, -7.5f};
    cubePositions[6] = DryPhys::Vector3D {1.3f, -2.0f, -2.5f};
    cubePositions[7] = DryPhys::Vector3D {1.5f, 2.0f, -2.5f};
    cubePositions[8] = DryPhys::Vector3D {1.5f, 0.2f, -1.5f};
    cubePositions[9] = DryPhys::Vector3D {-1.3f, 1.0f, -1.5f};

    while (window.isOpen())
    {
        window.pollEvents();
        processInput(window);

        window.clear(ColorRGBA {51, 76, 76, 255});

        shaderProgram->setUniformMatrix("view", lookAt(cameraData.position, cameraData.position + cameraData.front));
        shaderProgram->setUniformMatrix(
            "projection", makeProjection(DryPhys::radians(cameraData.fov), 800.0 / 600.0, 0.1, 100.0));

        for (std::size_t i {}; i < 10; ++i)
        {
            DryPhys::real angle = 20.0 * i;

            DryPhys::Transform4D model {ident};
            model = model * translate(cubePositions[i]);
            model = model * rotate(DryPhys::radians(angle), DryPhys::Vector3D {1.0f, 0.3f, 0.5f});
            shaderProgram->setUniformMatrix("model", model);

            meshes["Cube"]->draw();
        }

        window.display();
    }
}

void processInput(Window& window)
{
    GLFWwindow* win = window.window();

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);

    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;

    float currentFrame = glfwGetTime();
    deltaTime          = currentFrame - lastFrame;
    lastFrame          = currentFrame;

    cameraData.speed = 2.5f * deltaTime;

    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        cameraData.position += cameraData.front * cameraData.speed;

    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        cameraData.position -= cameraData.front * cameraData.speed;

    auto newVec = cameraData.front.cross(cameraData.up);
    newVec.normalize();

    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        cameraData.position -= newVec * cameraData.speed;

    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        cameraData.position += newVec * cameraData.speed;

    auto& events = window.getEvents();

    while (!events.empty())
    {
        auto& event = events.front();

        if (auto* mouseEvent = event.getEvent<Event::Closed>())
        {
            glfwSetWindowShouldClose(win, true);
        }

        if (auto* mouseEvent = event.getEvent<Event::MouseMoved>())
        {
            processMouseMovement(mouseEvent->xpos, mouseEvent->ypos);
        }

        if (auto* scrollEvent = event.getEvent<Event::Scroll>())
        {
            processMouseScroll(scrollEvent->y);
        }

        events.pop();
    }

    // for (auto& event : window.getEvents())
    // {
    //     if (auto* mouseEvent = event.getEvent<Event::Closed>())
    //     {
    //         glfwSetWindowShouldClose(win, true);
    //     }

    //     if (auto* mouseEvent = event.getEvent<Event::MouseMoved>())
    //     {
    //         processMouseMovement(mouseEvent->xpos, mouseEvent->ypos);
    //     }

    //     if (auto* scrollEvent = event.getEvent<Event::Scroll>())
    //     {
    //         processMouseScroll(scrollEvent->y);
    //     }
    // }
}

void processMouseScroll(DryPhys::real offset) { cameraData.fov = std::clamp(cameraData.fov - offset, 1.0, 45.0); }

void processMouseMovement(DryPhys::real xpos, DryPhys::real ypos)
{
    static bool firstMouse     = true;
    static DryPhys::real yaw   = -90.0;
    static DryPhys::real pitch = 0.0;
    static DryPhys::real lastX = 800.0 / 2.0;
    static DryPhys::real lastY = 600.0 / 2.0;

    if (firstMouse)
    {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    DryPhys::real xoffset = xpos - lastX;
    DryPhys::real yoffset = lastY - ypos;   // reversed since y-coordinates go from bottom to top
    lastX                 = xpos;
    lastY                 = ypos;

    xoffset *= cameraData.sensitivity;
    yoffset *= cameraData.sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0)
        pitch = 89.0;
    if (pitch < -89.0)
        pitch = -89.0;

    DryPhys::real yaw_rads   = DryPhys::radians(yaw);
    DryPhys::real pitch_rads = DryPhys::radians(pitch);

    cameraData.front.x = std::cos(yaw_rads) * std::cos(pitch_rads);
    cameraData.front.y = std::sin(pitch_rads);
    cameraData.front.z = std::sin(yaw_rads) * std::cos(pitch_rads);
    cameraData.front.normalize();
}
