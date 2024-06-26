// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: assets.hpp
// Author: crdrisko
// Date: 06/18/2024-05:23:32
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE2D_ASSETS_HPP
#define DRYPHYS_INCLUDE_ENGINE2D_ASSETS_HPP

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "engine2d/animation.hpp"
#include "engine2d/textureSheet.hpp"

namespace Engine2D
{
    class Assets
    {
    private:
        std::map<std::string, TextureSheet> textureSheetMap_;
        std::map<std::string, Animation> animationMap_;
        std::map<std::string, sf::Font> fontMap_;
        std::map<std::string, sf::SoundBuffer> soundBufferMap_;
        std::map<std::string, sf::Sound> soundMap_;
        std::map<std::string, std::string> musicMap_;

        void addTextureSheet(
            const std::string& name, const std::string& path, std::size_t perRow = 0, std::size_t perCol = 0);
        void addAnimation(const std::string& name, TextureSheet& textureSheet);
        void addAnimation(const std::string& name, TextureSheet& textureSheet, std::size_t frameCount, std::size_t speed);
        void addAnimation(const std::string& name,
            TextureSheet& textureSheet,
            std::size_t frameCount,
            std::size_t speed,
            const sf::IntRect& startingFrame);
        void addFont(const std::string& name, const std::string& path);
        void addSound(const std::string& name, const std::string& path);
        void addMusic(const std::string& name, const std::string& path);

    public:
        Assets() = default;

        void loadFromFile(const std::string& path);

        TextureSheet& getTextureSheet(const std::string& name);
        Animation& getAnimation(const std::string& name);
        sf::Font& getFont(const std::string& name);
        sf::Sound& getSound(const std::string& name);
        std::unique_ptr<sf::Music> getMusic(const std::string& name);
    };
}   // namespace Engine2D

#endif
