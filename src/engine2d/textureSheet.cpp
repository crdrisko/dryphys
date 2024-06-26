// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: textureSheet.cpp
// Author: crdrisko
// Date: 06/23/2024-14:07:26
// Description:

#include "engine2d/textureSheet.hpp"

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

namespace Engine2D
{
    TextureSheet::TextureSheet(const std::string& name, const std::string& path, std::size_t perRow, std::size_t perCol)
        : name_ {name}, path_ {path}, perRow {perRow}, perColumn {perCol}
    {
        if (!texture_.loadFromFile(path_))
        {
            std::cerr << "Could not find texture: " << name_ << '\n';
            std::exit(EXIT_FAILURE);
        }
    }
}   // namespace Engine2D
