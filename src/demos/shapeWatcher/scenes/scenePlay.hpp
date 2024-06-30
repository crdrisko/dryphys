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
#include <engine2d/action.hpp>
#include <engine2d/engine.hpp>
#include <engine2d/scene.hpp>

#include "shapeWatcher/entityManager.hpp"

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
        EntityManager entityManager_;

        sf::Text text_;
        int currentFrame_ {};

        void init(const std::string& config);
        void update() override;
        void sDoAction(const Engine2D::Action& action) override;
        void sRender() override;
        void onEnd() override;

        // Systems
        void sMovement();
        void sCollision();

    public:
        ScenePlay(Engine2D::Engine* gameEngine, const std::string& config) : Engine2D::Scene {gameEngine} { init(config); }
    };
}   // namespace ShapeWatcher

#endif