// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: GettingStarted07_04.cpp
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

void processInput(Window& window, Camera& camera);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <config_file>\n";
        return EXIT_FAILURE;
    }

    Window window {800, 600, "Getting Started: Camera Class"};

    glfwSetInputMode(window.window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Camera camera {DryPhys::Vector3D {0, 0, 3}};

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
        processInput(window, camera);

        window.clear(ColorRGBA {51, 76, 76, 255});

        const auto& [speed, fov, sensitivity, up] = camera.getConfig();

        shaderProgram->setUniformMatrix("view", camera.getView());
        shaderProgram->setUniformMatrix("projection", makeProjection(DryPhys::radians(fov), 800.0 / 600.0, 0.1, 100.0));

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

void processInput(Window& window, Camera& camera)
{
    GLFWwindow* win = window.window();

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);

    static bool firstMouse     = true;
    static DryPhys::real lastX = 800.0 / 2.0;
    static DryPhys::real lastY = 600.0 / 2.0;

    static double deltaTime {};
    static double lastFrame {};

    double currentFrame = glfwGetTime();
    deltaTime           = currentFrame - lastFrame;
    lastFrame           = currentFrame;

    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        camera.processMovement(Camera::FORWARD, deltaTime);

    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        camera.processMovement(Camera::BACKWARD, deltaTime);

    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        camera.processMovement(Camera::LEFT, deltaTime);

    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        camera.processMovement(Camera::RIGHT, deltaTime);

    auto& events = window.getEvents();

    while (!events.empty())
    {
        auto& event = events.front();

        if (auto* closeEvent = event.getEvent<Event::Closed>())
        {
            glfwSetWindowShouldClose(win, true);
        }

        if (auto* mouseEvent = event.getEvent<Event::MouseMoved>())
        {
            if (firstMouse)
            {
                lastX      = mouseEvent->xpos;
                lastY      = mouseEvent->ypos;
                firstMouse = false;
            }

            DryPhys::real xoffset = mouseEvent->xpos - lastX;
            DryPhys::real yoffset = lastY - mouseEvent->ypos;
            lastX                 = mouseEvent->xpos;
            lastY                 = mouseEvent->ypos;

            camera.processMouseMovement(xoffset, yoffset);
        }

        if (auto* scrollEvent = event.getEvent<Event::Scroll>())
        {
            camera.processMouseScroll(scrollEvent->y);
        }

        events.pop();
    }
}
