// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: assets.cpp
// Author: crdrisko
// Date: 06/23/2024-14:07:15
// Description:

#include "engine2d/assets.hpp"

#include <cassert>
#include <string>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <common-utils/files.hpp>
#include <common-utils/strings.hpp>

#include "engine2d/animation.hpp"
#include "engine2d/textureSheet.hpp"

namespace Engine2D
{
    void Assets::loadFromFile(const std::string& path)
    {
        DryChem::FileParser parser {path};
        auto rows = parser.parseDataFile(DryChem::AsRows());

        for (const auto& row : rows)
        {
            DryChem::Tokenizer tok {row};
            std::vector<std::string> splitRow = tok.split();

            if (splitRow[0] == "TextureSheet")
            {
                /*
                 * TextureSheet N P
                 *  Name:                   N       std::string (no spaces)
                 *  FilePath:               P       std::string (no spaces)
                 */
                addTextureSheet(splitRow[1], splitRow[2]);
            }
            else if (splitRow[0] == "Texture")
            {
                addTextureSheet(splitRow[1], splitRow[2]);
                getTextureSheet(splitRow[1]).getTexture().setRepeated(true);

                addAnimation(splitRow[1], getTextureSheet(splitRow[1]));
            }
            else if (splitRow[0] == "TextureSheetWrapped")
            {
                /*
                 * TextureSheetWrapped N P R C
                 *  Name:                   N       std::string (no spaces)
                 *  FilePath:               P       std::string (no spaces)
                 *  Animations per Row:     R       int
                 *  Animations per Column:  C       int
                 */
                addTextureSheet(splitRow[1], splitRow[2], std::stoul(splitRow[3]), std::stoul(splitRow[4]));
            }
            else if (splitRow[0] == "Animation")
            {
                /*
                 * Animation N TS F S X Y W H
                 *  Name:           N       std::string (no spaces)
                 *  Texture Sheet:  TS      std::string (refers to the existing TextureSheet)
                 *  Frame Count:    F       int (number of game frames in the Animation)
                 *  Speed:          S       int (number of game frames between Animation frames)
                 *  X Position:     X       int (starting frame's x position)
                 *  Y Position:     Y       int (starting frame's y position)
                 *  Width:          W       int (starting frame's width)
                 *  Height:         H       int (starting frame's height)
                 */
                addAnimation(splitRow[1],
                    getTextureSheet(splitRow[2]),
                    std::stoul(splitRow[3]),
                    std::stoul(splitRow[4]),
                    sf::IntRect {
                        std::stoi(splitRow[5]), std::stoi(splitRow[6]), std::stoi(splitRow[7]), std::stoi(splitRow[8])});
            }
            else if (splitRow[0] == "AnimationWrapped")
            {
                /*
                 * AnimationWrapped N TS F S
                 *  Name:           N       std::string (no spaces)
                 *  Texture Sheet:  TS      std::string (refers to the existing TextureSheet)
                 *  Frame Count:    F       int (number of game frames in the Animation)
                 *  Speed:          S       int (number of game frames between Animation frames)
                 */
                addAnimation(splitRow[1], getTextureSheet(splitRow[2]), std::stoul(splitRow[3]), std::stoul(splitRow[4]));
            }
            else if (splitRow[0] == "Font")
            {
                /*
                 * Font N F
                 *  Name:       N       std::string (no spaces)
                 *  File Path:  P       std::string (no spaces)
                 */
                addFont(splitRow[1], splitRow[2]);
            }
            else if (splitRow[0] == "Sound")
            {
                /*
                 * Sound N F V
                 *  Name:       N       std::string (no spaces)
                 *  File Path:  P       std::string (no spaces)
                 *  Volume:     V       float
                 */
                addSound(splitRow[1], splitRow[2]);
            }
            else if (splitRow[0] == "Music")
            {
                /*
                 * Music N F V
                 *  Name:       N       std::string (no spaces)
                 *  File Path:  P       std::string (no spaces)
                 *  Volume:     V       float
                 */
                addMusic(splitRow[1], splitRow[2]);
            }
            else
            {
                std::cerr << "Unknown asset type: " << splitRow[0] << "\n";
                std::exit(EXIT_FAILURE);
            }
        }
    }

    void Assets::addTextureSheet(const std::string& name, const std::string& path, std::size_t perRow, std::size_t perCol)
    {
        textureSheetMap_[name] = TextureSheet {name, path, perRow, perCol};
    }

    void Assets::addAnimation(const std::string& name, TextureSheet& textureSheet)
    {
        animationMap_[name] = Animation {name, textureSheet};
    }

    void Assets::addAnimation(const std::string& name, TextureSheet& textureSheet, std::size_t frameCount, std::size_t speed)
    {
        animationMap_[name] = Animation {name, textureSheet, frameCount, speed};
    }

    void Assets::addAnimation(const std::string& name,
        TextureSheet& textureSheet,
        std::size_t frameCount,
        std::size_t speed,
        const sf::IntRect& startingFrame)
    {
        animationMap_[name] = Animation {name, textureSheet, frameCount, speed, startingFrame};
    }

    void Assets::addFont(const std::string& name, const std::string& path)
    {
        if (!fontMap_[name].loadFromFile(path))
        {
            std::cerr << "Could not load font file: " << name << '\n';
            std::exit(EXIT_FAILURE);
        }
    }

    void Assets::addSound(const std::string& name, const std::string& path)
    {
        soundBufferMap_[name] = sf::SoundBuffer();

        if (!soundBufferMap_[name].loadFromFile(path))
        {
            std::cerr << "Could not load sound file: " << name << '\n';
            std::exit(EXIT_FAILURE);
        }
        else
        {
            soundMap_[name] = sf::Sound(soundBufferMap_[name]);
            soundMap_[name].setVolume(25);
        }
    }

    void Assets::addMusic(const std::string& name, const std::string& path) { musicMap_[name] = path; }

    TextureSheet& Assets::getTextureSheet(const std::string& name)
    {
        assert(textureSheetMap_.find(name) != textureSheetMap_.end());
        return textureSheetMap_[name];
    }

    Animation& Assets::getAnimation(const std::string& name)
    {
        assert(animationMap_.find(name) != animationMap_.end());
        return animationMap_[name];
    }

    sf::Font& Assets::getFont(const std::string& name)
    {
        assert(fontMap_.find(name) != fontMap_.end());
        return fontMap_[name];
    }

    sf::Sound& Assets::getSound(const std::string& name)
    {
        assert(soundMap_.find(name) != soundMap_.end());
        return soundMap_[name];
    }

    std::unique_ptr<sf::Music> Assets::getMusic(const std::string& name)
    {
        auto music = std::make_unique<sf::Music>();

        if (!music->openFromFile(musicMap_[name]))
        {
            std::cerr << "Could not load music file: " << name << '\n';
            std::exit(EXIT_FAILURE);
        }

        music->setLoop(true);

        return music;
    }
}   // namespace Engine2D
