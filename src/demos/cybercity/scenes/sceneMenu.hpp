// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: sceneMenu.hpp
// Author: crdrisko
// Date: 06/19/2024-08:47:37
// Description:

#ifndef SCENEMENU_HPP
#define SCENEMENU_HPP

#include <string>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <engine2d/action.hpp>
#include <engine2d/engine.hpp>
#include <engine2d/scene.hpp>

class SceneMenu : public Engine2D::Scene
{
private:
    std::string title_;
    std::vector<std::string> menuStrings_;
    std::vector<std::string> levelPaths_;
    sf::Text menuText_;
    std::size_t selectedMenuIndex_ = 0;

    std::unique_ptr<sf::Music> music_;

    void init();
    void update() override;
    void sDoAction(const Engine2D::Action& action) override;
    void sRender() override;
    void onEnd() override;

public:
    explicit SceneMenu(Engine2D::Engine* gameEngine = nullptr);
};

#endif
