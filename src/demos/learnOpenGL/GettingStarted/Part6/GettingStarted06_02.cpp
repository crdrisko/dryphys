// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: GettingStarted06_02.cpp
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

    Window window {800, 600, "Getting Started: Coordinate Systems Depth"};

    Assets assets {};
    assets.loadFromFile(argv[1]);

    const DryPhys::Transform4D ident {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};

    ShaderProgram* shaderProgram = assets.getShaderProgram("Example6_1");

    shaderProgram->use();
    shaderProgram->setUniform("texture1", assets.getTextureSheet("Container").getTextureID());
    shaderProgram->setUniform("texture2", assets.getTextureSheet("AwesomeFace").getTextureID());

    DryPhys::Transform4D view {ident};
    view = view * translate(DryPhys::Vector3D {0, 0, -3});

    DryPhys::Matrix4D projection
        = makeProjection(45.0 * DryPhys::Constants::pi / 180.0, (float)800 / (float)600, 0.1, 100.0);

    shaderProgram->setUniformMatrix("view", view);
    shaderProgram->setUniformMatrix("projection", projection);

    auto meshes = parseMeshFile("cube.mesh");

    while (window.isOpen())
    {
        window.pollEvents();
        processInput(window);

        window.clear(ColorRGBA {51, 76, 76, 255});

        DryPhys::Transform4D model {ident};
        model = model * rotate((float)glfwGetTime(), DryPhys::Vector3D {0.5, 1, 0});

        shaderProgram->use();
        shaderProgram->setUniformMatrix("model", model);

        meshes["Cube"]->draw();

        window.display();
    }
}

void processInput(Window& window)
{
    GLFWwindow* win = window.window();

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
}
