// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: textureSheet.hpp
// Author: crdrisko
// Date: 06/18/2024-05:23:55
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE2D_TEXTURESHEET_HPP
#define DRYPHYS_INCLUDE_ENGINE2D_TEXTURESHEET_HPP

#include <string>

#include <SFML/Graphics.hpp>

namespace Engine2D
{
    class TextureSheet
    {
    private:
        std::string name_ {};
        std::string path_ {};
        sf::Texture texture_ {};

    public:
        std::size_t perRow {};
        std::size_t perColumn {};
        std::size_t rowOffset {};
        std::size_t columnOffset {};
        std::size_t lineTotal {};

        TextureSheet() = default;
        TextureSheet(const std::string& name, const std::string& path, std::size_t perRow = 0, std::size_t perCol = 0);

        sf::Texture& getTexture() { return texture_; }
    };
}   // namespace Engine2D

#endif
