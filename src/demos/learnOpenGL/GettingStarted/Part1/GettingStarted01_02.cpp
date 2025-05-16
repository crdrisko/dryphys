// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: GettingStarted01_02.cpp
// Author: crdrisko
// Date: 04/21/2025-07:00:25
// Description:

#include <partitionEngine/partitionEngine.hpp>

#include "learnOpenGL/learnOpenGL.hpp"

using namespace PartitionEngine;

void processInput(Window& window);

int main()
{
    Window window {800, 600, "Getting Started: Hello Window Clear"};

    while (window.isOpen())
    {
        window.pollEvents();
        processInput(window);

        window.clear(ColorRGBA {51, 76, 76, 255});

        window.display();
    }
}

void processInput(Window& window)
{
    GLFWwindow* win = window.window();

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
}
