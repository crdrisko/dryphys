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
    Scene::Scene() : currentFrame_ {0} {}

    Scene::Scene(Engine* gameEngine) : game_ {gameEngine}, currentFrame_ {0} {}

    void Scene::doAction(const Action& action) { sDoAction(action); }

    void Scene::simulate(const std::size_t frames)
    {
        for (std::size_t i {}; i < frames; ++i)
        {
            update();
        }
    }

    void Scene::registerAction(int inputKeyCode, const std::string& actionName) { actionMap_[inputKeyCode] = actionName; }

    void Scene::registerMouseAction(int inputMouseCode, const std::string& actionName)
    {
        registerAction(inputMouseCode + sf::Keyboard::KeyCount, actionName);
    }

    void Scene::drawLine(const DryPhys::Vector3D& p1, const DryPhys::Vector3D& p2)
    {
        sf::Vertex line[] = {sf::Vector2f(p1[0], p1[1]), sf::Vector2f(p2[0], p2[1])};
        game_->window().draw(line, 2, sf::Lines);
    }

    std::size_t Scene::width() const { return game_->window().getSize().x; }

    std::size_t Scene::height() const { return game_->window().getSize().y; }
}   // namespace Engine2D
