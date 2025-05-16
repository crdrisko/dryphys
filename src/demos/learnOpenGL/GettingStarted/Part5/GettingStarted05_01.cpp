// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: GettingStarted05_01.cpp
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

    Window window {800, 600, "Getting Started: Transformations"};

    Assets assets {};
    assets.loadFromFile(argv[1]);

    ShaderProgram* shaderProgram = assets.getShaderProgram("Example5_1");

    shaderProgram->use();
    shaderProgram->setUniform("texture1", assets.getTextureSheet("Container").getTextureID());
    shaderProgram->setUniform("texture2", assets.getTextureSheet("AwesomeFace").getTextureID());

    auto meshes = parseMeshFile("rectangle.mesh");

    while (window.isOpen())
    {
        window.pollEvents();
        processInput(window);

        window.clear(ColorRGBA {51, 76, 76, 255});

        DryPhys::Transform4D transform {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};

        transform = transform * translate(DryPhys::Vector3D {0.5f, -0.5f, 0.0f});
        transform = transform * rotate((float)glfwGetTime(), DryPhys::Vector3D {0.0f, 0.0f, 1.0f});

        shaderProgram->use();
        shaderProgram->setUniformMatrix("transform", transform);

        meshes["Rectangle"]->draw();

        window.display();
    }
}

void processInput(Window& window)
{
    GLFWwindow* win = window.window();

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
}
