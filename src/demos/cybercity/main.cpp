// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: main.cpp
// Author: crdrisko
// Date: 06/19/2024-08:43:55
// Description:

#include <iostream>

#include <engine2d/engine.hpp>

#include "cybercity/scenes/sceneMenu.hpp"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <config_file>\n";
        return EXIT_FAILURE;
    }

    Engine2D::Engine game {argv[1]};

    // Set the default scene our-self so the game engine doesn't need to know about it
    game.setDefaultScene<CyberCity::SceneMenu>("MENU");
    game.run();
}
