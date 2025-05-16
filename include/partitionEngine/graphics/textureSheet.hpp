// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: textureSheet.hpp
// Author: crdrisko
// Date: 01/15/2025-04:25:59
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE3D_TEXTURESHEET_HPP
#define DRYPHYS_INCLUDE_ENGINE3D_TEXTURESHEET_HPP

#include <cstddef>
#include <string>

#include "partitionEngine/graphics/texture.hpp"

namespace PartitionEngine
{
    class TextureSheet
    {
    private:
        std::string name_ {};
        std::string path_ {};
        Texture texture_;

    public:
        std::size_t perRow_ {};
        std::size_t perColumn_ {};
        std::size_t rowOffset_ {};
        std::size_t columnOffset_ {};
        std::size_t lineTotal_ {};

        TextureSheet() = default;
        TextureSheet(const std::string& name, const std::string& path, bool flip);
        TextureSheet(const std::string& name, const std::string& path, std::size_t perRow = 0, std::size_t perCol = 0);

        Texture& getTexture() { return texture_; }
        int getTextureID() const { return static_cast<int>(texture_.getID()); }
    };
}   // namespace PartitionEngine

#endif
