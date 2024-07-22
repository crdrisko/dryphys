// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scenePlay.hpp
// Author: crdrisko
// Date: 07/01/2024-07:35:52
// Description:

#ifndef DRYPHYS_SRC_DEMOS_LIGHTCYCLE_SCENES_SCENEPLAY_HPP
#define DRYPHYS_SRC_DEMOS_LIGHTCYCLE_SCENES_SCENEPLAY_HPP

#include <string>

#include <SFML/Graphics.hpp>
#include <dryphys/math.hpp>
#include <engine2d/action.hpp>
#include <engine2d/engine.hpp>
#include <engine2d/entityManager.hpp>
#include <engine2d/scene.hpp>

#include "lightCycle/forwardDeclare.hpp"

namespace LightCycle
{
    struct Direction
    {
        int x = 0;
        int y = 0;
    };

    class ScenePlay : public Engine2D::Scene
    {
    private:
        std::string path_;
        ConcreteEntityManager entityManager_;

        std::size_t gridWidth_  = 0;
        std::size_t gridHeight_ = 0;

        std::vector<std::vector<int>> level_;

        // Legal Directions                      UP      DOWN     LEFT    RIGHT
        std::vector<Direction> directions_ = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        sf::Text text_;
        sf::Clock clock_;
        std::size_t gridSize_ = 16;

        void setMap();
        void init();

        void update() override;
        void doActionImpl(const Engine2D::Action& action) override;
        void render() override;
        void onEnd() override { game_->quit(); }

        void sAI();
        void sMovement();
        void sStatus();

        std::size_t battleWidth() const { return gridSize_ * gridWidth_; }
        std::size_t battleHeight() const { return gridSize_ * gridHeight_; }

    public:
        ScenePlay(Engine2D::Engine* gameEngine, const std::string& config);

        bool isLegalAction(ConcreteEntityPtr entity, std::size_t action) const;
        std::vector<std::size_t> getLegalActions(ConcreteEntityPtr entity) const;
        bool isSafe(std::size_t x, std::size_t y) const;

        const Direction& getDirection(std::size_t index) const { return directions_[index]; }
    };
}   // namespace LightCycle

#endif
