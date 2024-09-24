// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: animation.hpp
// Author: crdrisko
// Date: 06/18/2024-05:23:24
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE2D_ANIMATION_HPP
#define DRYPHYS_INCLUDE_ENGINE2D_ANIMATION_HPP

#include <map>
#include <string>

#include <SFML/Graphics.hpp>
#include <dryphys/math.hpp>

#include "engine2d/forwardDeclare.hpp"

namespace Engine2D
{
    class Animation
    {
        struct Configurations
        {
            std::size_t horizontalOffset {};
            std::size_t verticalOffset {};
            std::size_t lineTotal {};
        };

    private:
        std::string name_;
        sf::Sprite sprite_;

        std::size_t perRow_ {};
        std::size_t perColumn_ {};

        Configurations startingPosition {};
        Configurations framePosition {};

        std::size_t frameCount_ {1};    //!< Total number of frames of animation
        std::size_t currentFrame_ {};   //!< The current frame of animation being played
        std::size_t gameFrame_ {};      //!< Total number of frames the animation has been alive
        std::size_t speed_ {};          //!< The speed to play this animation

        sf::IntRect startingFrame_ {};

        DryPhys::Vector3D size_ {1.0f, 1.0f, 0.0f};

    public:
        //! Constructors
        Animation() = default;
        Animation(const std::string&, TextureSheet&);
        Animation(const std::string&, TextureSheet&, std::size_t, std::size_t);
        Animation(const std::string&, TextureSheet&, std::size_t, std::size_t, const sf::IntRect&);

        void update();
        bool hasEnded();

        //! Accessors
        const std::string& getName() const { return name_; }
        const DryPhys::Vector3D& getSize() const { return size_; }
        sf::Sprite& getSprite() { return sprite_; }
        const sf::Sprite& getSprite() const { return sprite_; }
    };
}   // namespace Engine2D

#endif
