// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: GettingStarted03_01.cpp
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

    Window window {800, 600, "Getting Started: Shaders Uniform"};

    Assets assets {};
    assets.loadFromFile(argv[1]);

    ShaderProgram* shaderProgram = assets.getShaderProgram("Example3_1");

    auto meshes = parseMeshFile("triangle.mesh");

    while (window.isOpen())
    {
        window.pollEvents();
        processInput(window);

        window.clear(ColorRGBA {51, 76, 76, 255});

        float timeValue  = glfwGetTime();
        float greenValue = std::sin(timeValue) / 2.0f + 0.5f;

        shaderProgram->use();
        shaderProgram->setUniform("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

        meshes["Triangle"]->draw();

        window.display();
    }
}

void processInput(Window& window)
{
    GLFWwindow* win = window.window();

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
}
