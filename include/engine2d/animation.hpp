// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: animation.hpp
// Author: crdrisko
// Date: 06/18/2024-05:23:24
// Description:

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "dryphys/vector3d.hpp"
#include "engine2d/textureSheet.hpp"

namespace Engine2D
{
    class Animation
    {
    private:
        struct Configurations
        {
            std::size_t horizontalOffset {};
            std::size_t verticalOffset {};
            std::size_t lineTotal {};
        };

        std::string name_ = "none";
        sf::Sprite sprite_;

        std::size_t perRow_ {};
        std::size_t perColumn_ {};

        Configurations startingPosition {};
        Configurations framePosition {};

        std::size_t frameCount_ {1};    // Total number of frames of animation
        std::size_t currentFrame_ {};   // The current frame of animation being played
        std::size_t gameFrame_ {};      // Total number of frames the animation has been alive
        std::size_t speed_ {};          // The speed to play this animation

        sf::IntRect startingFrame_ {};

        DryPhys::Vector3D size_ {1.0f, 1.0f, 0.0f};

    public:
        Animation() = default;
        Animation(const std::string& name, TextureSheet& t);
        Animation(const std::string& name, TextureSheet& t, std::size_t frameCount, std::size_t speed);
        Animation(const std::string& name,
            TextureSheet& t,
            std::size_t frameCount,
            std::size_t speed,
            const sf::IntRect& startingFrame);

        void update();
        bool hasEnded();

        const std::string& getName() const { return name_; }
        const DryPhys::Vector3D& getSize() const { return size_; }

        sf::Sprite& getSprite() { return sprite_; }
        const sf::Sprite& getSprite() const { return sprite_; }
    };
}   // namespace Engine2D

#endif
