// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scene.cpp
// Author: crdrisko
// Date: 06/23/2024-14:02:51
// Description:

#include "engine2d/scene.hpp"

#include <cstddef>
#include <string>

#include <SFML/Graphics.hpp>

#include "engine2d/action.hpp"
#include "engine2d/engine.hpp"

namespace Engine2D
{
    void Scene::doAction(const Action& action) { sDoAction(action); }

    void Scene::simulate(const std::size_t frames)
    {
        for (std::size_t i {}; i < frames; ++i)
        {
            update();
        }
    }

    void Scene::registerAction(int inputKeyCode, const std::string& actionName) { actionMap_[inputKeyCode] = actionName; }

    void Scene::drawLine(const DryPhys::Vector3D& p1, const DryPhys::Vector3D& p2)
    {
        sf::Vertex line[] = {sf::Vector2f(p1[0], p1[1]), sf::Vector2f(p2[0], p2[1])};
        game_->window().draw(line, 2, sf::Lines);
    }

    void Scene::drawLine(float x1, float y1, float x2, float y2, sf::Color color)
    {
        sf::VertexArray lines(sf::LinesStrip, 2);

        lines[0].position.x = x1;
        lines[0].position.y = y1;
        lines[0].color      = color;

        lines[1].position.x = x2;
        lines[1].position.y = y2;
        lines[1].color      = color;

        game_->window().draw(lines);
    }
}   // namespace Engine2D
