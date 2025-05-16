// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: GettingStarted02_02.cpp
// Author: crdrisko
// Date: 04/21/2025-07:00:25
// Description:

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

    Window window {800, 600, "Getting Started: Hello Triangle Indexed"};

    Assets assets {};
    assets.loadFromFile(argv[1]);

    ShaderProgram* shaderProgram = assets.getShaderProgram("Example2_1");

    auto meshes = parseMeshFile("rectangle.mesh");

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (window.isOpen())
    {
        window.pollEvents();
        processInput(window);

        window.clear(ColorRGBA {51, 76, 76, 255});

        meshes["Rectangle"]->draw(shaderProgram);

        window.display();
    }
}

void processInput(Window& window)
{
    GLFWwindow* win = window.window();

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
}
