// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: animation.cpp
// Author: crdrisko
// Date: 06/18/2024-06:25:46
// Description:

#include "engine2d/animation.hpp"

#include <config.h>

#include <cassert>
#include <cmath>
#include <iostream>

#include "dryphys/vector3d.hpp"
#include "engine2d/textureSheet.hpp"

using namespace DryPhys;

namespace Engine2D
{
    Animation::Animation(const std::string& name, TextureSheet& t)
        : name_ {name}, sprite_ {t.getTexture()}, frameCount_ {1}, currentFrame_ {0}, gameFrame_ {0}, speed_ {0}
    {
        sf::Vector2u texSize = t.getTexture().getSize();

        size_ = Vector3D {static_cast<real>(texSize.x), static_cast<real>(texSize.y), 0};

        startingFrame_ = sf::IntRect {0, 0, static_cast<int>(texSize.x), static_cast<int>(texSize.y)};
    }

    Animation::Animation(const std::string& name, TextureSheet& t, std::size_t frameCount, std::size_t speed)
        : name_ {name}, sprite_ {t.getTexture()}, perRow_ {t.perRow}, perColumn_ {t.perColumn},
          startingPosition {t.rowOffset, t.columnOffset, t.lineTotal}, framePosition {startingPosition},
          frameCount_ {frameCount}, currentFrame_ {0}, gameFrame_ {0}, speed_ {speed}
    {
        sf::Vector2u texSize = t.getTexture().getSize();

        size_ = Vector3D {static_cast<float>(texSize.x) / perRow_, static_cast<float>(texSize.y) / perColumn_, 0};

        sprite_.setOrigin(size_[0] / 2.0f, size_[1] / 2.0f);
        sprite_.setTextureRect(sf::IntRect(static_cast<int>(size_[0] * (startingPosition.horizontalOffset % perRow_)),
            static_cast<int>(size_[1] * startingPosition.verticalOffset),
            static_cast<int>(size_[0]),
            static_cast<int>(size_[1])));

        // Modify offsets for next animation in the same texture sheet
        t.rowOffset += frameCount;
        t.lineTotal += frameCount;

        if (t.lineTotal >= perRow_)
        {
            t.columnOffset++;
            t.lineTotal -= t.perRow;
        }
    }

    Animation::Animation(const std::string& name,
        TextureSheet& t,
        std::size_t frameCount,
        std::size_t speed,
        const sf::IntRect& startingFrame)
        : name_ {name}, sprite_ {t.getTexture()}, frameCount_ {frameCount}, currentFrame_ {0},
          gameFrame_ {0}, speed_ {speed}, startingFrame_ {startingFrame}
    {
        size_ = Vector3D {static_cast<float>(startingFrame_.width), static_cast<float>(startingFrame_.height), 0};

        sprite_.setOrigin(size_[0] / 2.0f, size_[1] / 2.0f);
        sprite_.setTextureRect(startingFrame_);
    }

    void Animation::update()
    {
        std::size_t previousFrame {currentFrame_};

        gameFrame_++;

        if (speed_)
            currentFrame_ = (gameFrame_ / speed_) % frameCount_;

        if (perRow_)
        {
            if (framePosition.lineTotal >= perRow_)
            {
                framePosition.verticalOffset++;
                framePosition.lineTotal -= perRow_;
            }

            sprite_.setTextureRect(
                sf::IntRect(static_cast<int>(size_[0] * ((previousFrame + framePosition.horizontalOffset) % perRow_)),
                    static_cast<int>(size_[1] * framePosition.verticalOffset),
                    static_cast<int>(size_[0]),
                    static_cast<int>(size_[1])));

            framePosition.lineTotal += currentFrame_ - previousFrame;
        }
        else
        {
            sprite_.setTextureRect(sf::IntRect(static_cast<int>(startingFrame_.getPosition().x + (size_[0] * currentFrame_)),
                static_cast<int>(startingFrame_.getPosition().y),
                static_cast<int>(startingFrame_.width),
                static_cast<int>(startingFrame_.height)));
        }
    }

    bool Animation::hasEnded()
    {
        if (gameFrame_ >= frameCount_ * speed_)
        {
            gameFrame_    = 0;
            framePosition = startingPosition;

            return true;
        }

        return false;
    }
}   // namespace Engine2D
