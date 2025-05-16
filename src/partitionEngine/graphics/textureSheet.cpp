// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: textureSheet.cpp
// Author: crdrisko
// Date: 01/15/2025-04:29:38
// Description:

#include "partitionEngine/graphics/textureSheet.hpp"

#include <iostream>
#include <string>

#include <drychem/drychem.hpp>

#include "partitionEngine/graphics/texture.hpp"

namespace PartitionEngine
{
    TextureSheet::TextureSheet(const std::string& name, const std::string& path, bool flip) : name_ {name}, path_ {path}
    {
        if (!texture_.loadTextureFromFile(path_, flip))
        {
            DryChem::printErrorMessage(DryChem::ErrorMessage {"TextureSheet", "Could not find texture: " + name_});
        }
    }

    TextureSheet::TextureSheet(const std::string& name, const std::string& path, std::size_t perRow, std::size_t perCol)
        : name_ {name}, path_ {path}, perRow_ {perRow}, perColumn_ {perCol}
    {
        if (!texture_.loadTextureFromFile(path_))
        {
            DryChem::printErrorMessage(DryChem::ErrorMessage {"TextureSheet", "Could not find texture: " + name_});
        }
    }
}   // namespace PartitionEngine
