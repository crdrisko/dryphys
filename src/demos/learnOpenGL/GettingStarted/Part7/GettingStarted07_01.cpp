// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: GettingStarted07_01.cpp
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

void processInput(Window& window);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <config_file>\n";
        return EXIT_FAILURE;
    }

    Window window {800, 600, "Getting Started: Camera Circle"};

    Assets assets {};
    assets.loadFromFile(argv[1]);

    const DryPhys::Transform4D ident {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};

    ShaderProgram* shaderProgram = assets.getShaderProgram("Example6_1");

    shaderProgram->use();
    shaderProgram->setUniform("texture1", assets.getTextureSheet("Container").getTextureID());
    shaderProgram->setUniform("texture2", assets.getTextureSheet("AwesomeFace").getTextureID());

    DryPhys::Matrix4D projection = makeProjection(DryPhys::radians(45.0), 800.0 / 600.0, 0.1, 100.0);
    shaderProgram->setUniformMatrix("projection", projection);

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

        const double radius = 10.0f;
        const double time   = glfwGetTime();

        double camX = std::sin(time) * radius;
        double camZ = std::cos(time) * radius;

        DryPhys::Transform4D view = lookAt(DryPhys::Vector3D {camX, 0.0, camZ});
        shaderProgram->setUniformMatrix("view", view);

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
}
