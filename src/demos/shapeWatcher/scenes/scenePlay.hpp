// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scenePlay.hpp
// Author: crdrisko
// Date: 06/29/2024-13:27:39
// Description:

#ifndef DRYPHYS_SRC_DEMOS_SHAPEWATCHER_SCENES_SCENEPLAY_HPP
#define DRYPHYS_SRC_DEMOS_SHAPEWATCHER_SCENES_SCENEPLAY_HPP

#include <string>

#include <SFML/Graphics.hpp>
#include <dryphys/dryphys.hpp>
#include <engine2d/action.hpp>
#include <engine2d/engine.hpp>
#include <engine2d/scene.hpp>

#include "shapeWatcher/forwardDeclare.hpp"

namespace ShapeWatcher
{
    class ScenePlay : public Engine2D::Scene
    {
        struct CircleConfig
        {
            std::string N;
            float X, Y, SX, SY, R;
            sf::Uint8 OR, OG, OB;
        };

        struct RectangleConfig
        {
            std::string N;
            float X, Y, SX, SY, W, H;
            sf::Uint8 OR, OG, OB;
        };

    private:
        ConcreteEntityManager entityManager_;
        float width_ {}, height_ {};

        sf::Text text_;
        int currentFrame_ {};

        void init(const std::string& config);
        void update() override;
        void doActionImpl(const Engine2D::Action& action) override;
        void render() override;
        void onEnd() override;

        // Systems
        void sCollision();
        void sMovement();

    public:
        ScenePlay(Engine2D::Engine* gameEngine, const std::string& config);
    };
}   // namespace ShapeWatcher

#endif
