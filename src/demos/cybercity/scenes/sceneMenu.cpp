// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: sceneMenu.cpp
// Author: crdrisko
// Date: 06/19/2024-08:47:07
// Description:

#include "cybercity/scenes/sceneMenu.hpp"

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>
#include <dryphys/utilities/utils.hpp>
#include <engine2d/action.hpp>
#include <engine2d/engine.hpp>
#include <engine2d/scene.hpp>

#include "cybercity/scenes/scenePlay.hpp"

using namespace DryPhys::Literals;

namespace fs = std::filesystem;

namespace CyberCity
{
    SceneMenu::SceneMenu(Engine2D::Engine* gameEngine, const std::string& path) : Engine2D::Scene {gameEngine}
    {
        fs::path fullPath {path};

        init(fullPath.remove_filename().string());
    }

    void SceneMenu::init(const std::string& path)
    {
        registerAction(sf::Keyboard::W, "UP");
        registerAction(sf::Keyboard::S, "DOWN");
        registerAction(sf::Keyboard::Enter, "PLAY");
        registerAction(sf::Keyboard::Escape, "QUIT");

        title_ = "CyberCity";
        menuStrings_.push_back("Level 1");
        menuStrings_.push_back("Level 2");
        menuStrings_.push_back("Level 3");

        levelPaths_.push_back(path + "CP-Level1.txt");
        levelPaths_.push_back(path + "CP-Level2.txt");
        levelPaths_.push_back(path + "CP-Level3.txt");

        menuText_.setFont(game_->assets().getFont("VinerHand"));
        menuText_.setCharacterSize(60);
        menuText_.setFillColor(sf::Color(0, 0, 0));

        if (!music_)
            music_ = game_->assets().getMusic("LastArea");

        music_->setVolume(20);
        music_->play();
    }

    void SceneMenu::update() {}

    void SceneMenu::sDoAction(const Engine2D::Action& action)
    {
        if (auto atype = action.type(); atype == Engine2D::Action::START)
        {
            switch (action.sid())
            {
            case "UP"_sid:
                if (selectedMenuIndex_ > 0)
                    selectedMenuIndex_--;
                else
                    selectedMenuIndex_ = menuStrings_.size() - 1;
                break;
            case "DOWN"_sid:
                selectedMenuIndex_ = (selectedMenuIndex_ + 1) % menuStrings_.size();
                break;
            case "PLAY"_sid:
                if (music_)
                    music_->stop();

                game_->changeScene("PLAY", std::make_shared<ScenePlay>(game_, levelPaths_[selectedMenuIndex_]));
                break;
            case "QUIT"_sid:
                onEnd();
                break;
            default:
                break;
            }
        }
    }

    void SceneMenu::sRender()
    {
        game_->window().clear(sf::Color(51, 51, 255));

        sf::View view {game_->window().getDefaultView()};
        game_->window().setView(view);

        menuText_.setString(title_);
        menuText_.setCharacterSize(80);
        menuText_.setPosition(10, 8);
        menuText_.setFont(game_->assets().getFont("VinerHand"));

        game_->window().draw(menuText_);

        menuText_.setPosition(50, 50);
        menuText_.setFont(game_->assets().getFont("AniMeMatrix"));

        for (std::size_t i {}; i < menuStrings_.size(); ++i)
        {
            menuText_.setString(menuStrings_[i]);
            menuText_.setPosition(menuText_.getPosition().x, menuText_.getPosition().y + 75);

            if (i == selectedMenuIndex_)
            {
                menuText_.setFillColor(sf::Color(255, 255, 255));
            }
            else
            {
                menuText_.setFillColor(sf::Color(0, 0, 0));
            }

            game_->window().draw(menuText_);
        }

        menuText_.setString("UP:W  DOWN:S  PLAY:ENTER  BACK:ESC");
        menuText_.setFillColor(sf::Color(0, 0, 0));
        menuText_.setCharacterSize(32);
        menuText_.setPosition(5.0f, static_cast<float>(game_->window().getSize().y - 64.0f));

        game_->window().draw(menuText_);
    }

    void SceneMenu::onEnd()
    {
        music_->stop();
        game_->quit();
    }
}   // namespace CyberCity
