// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: main.cpp
// Author: crdrisko
// Date: 06/19/2024-05:28:03
// Description:

#include <cstddef>
#include <iostream>

#include <engine2d/engine.hpp>

#include "geometryWaves/scenes/scenePlay.hpp"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <config_file>\n";
        return EXIT_FAILURE;
    }

    Engine2D::Engine game {argv[1]};

    // Set the default scene our-self so the game engine doesn't need to know about it
    game.setDefaultScene<GeometryWaves::ScenePlay>("GAME", argv[1]);
    game.run();
}
