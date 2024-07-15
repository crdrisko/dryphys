// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scenePlay.cpp
// Author: crdrisko
// Date: 07/02/2024-06:01:21
// Description:

#include "lightCycle/scenes/scenePlay.hpp"

#include <sstream>
#include <string>

#include <SFML/Graphics.hpp>
#include <common-utils/files.hpp>
#include <common-utils/strings.hpp>

#include "lightCycle/ai.hpp"
#include "lightCycle/components.hpp"

namespace LightCycle
{
    ScenePlay::ScenePlay(Engine2D::Engine* gameEngine, const std::string& config)
        : Engine2D::Scene {gameEngine}, path_ {config}
    {
        init();
        setMap();
    }

    void ScenePlay::init()
    {
        auto& [name, width, height, fps, fullscreen] = game_->assets().getWindowConfig();

        gridWidth_  = width;
        gridHeight_ = height;

        game_->window().create(sf::VideoMode(battleWidth() + 200, battleHeight()), name, (fullscreen + sf::Style::Default));
        game_->window().setFramerateLimit(fps);

        registerAction(sf::Keyboard::Escape, "QUIT");
        registerAction(sf::Keyboard::P, "PAUSE");
        registerAction(sf::Keyboard::R, "RESET");

        text_.setFont(game_->assets().getFont("Arial"));
        text_.setPosition(10, 5);
        text_.setCharacterSize(20);
    }

    void ScenePlay::setMap()
    {
        // Reset the entity manager every time we load a level
        entityManager_ = ConcreteEntityManager();

        level_ = std::vector<std::vector<int>>(gridWidth_, std::vector<int>(gridHeight_, -1));

        // Parse input file - will only be opened once with static here
        static DryChem::FileParser parser {path_};
        auto rows = parser.parseDataFile(DryChem::AsRows());

        for (const auto& row : rows)
        {
            DryChem::Tokenizer tok {row};
            std::vector<std::string> splitRow = tok.split();

            if (splitRow[0] == "Player")
            {
                std::string name = splitRow[1];

                float x = std::stof(splitRow[2]);
                float y = std::stof(splitRow[3]);

                int r = std::stoi(splitRow[4]);
                int g = std::stoi(splitRow[5]);
                int b = std::stoi(splitRow[6]);

                AIFactory factory {name};
                std::size_t randomStart = rand() % directions_.size();

                auto entity = entityManager_.addEntity("LightCycle");

                entity->addComponent<CTransform>(DryPhys::Vector3D {x, y, 0}, randomStart);
                entity->addComponent<CCycle>(sf::Color(r, g, b));
                entity->addComponent<CAI>(factory.create());

                // set the grid cell to its value
                level_[x][y] = entity->id();
            }
        }
    }

    void ScenePlay::update()
    {
        entityManager_.update();

        if (!paused_)
        {
            sAI();
            sMovement();
            sStatus();

            currentFrame_++;
        }
    }

    void ScenePlay::sAI()
    {
        for (auto entity : entityManager_.getEntities())
        {
            if (!entity->getComponent<CCycle>().isAlive())
                continue;

            auto& eTrans = entity->getComponent<CTransform>();
            auto& eAI    = entity->getComponent<CAI>();

            eTrans.dir = eAI.ai->getAction(*this, entity);
        }
    }

    void ScenePlay::sMovement()
    {
        for (auto entity : entityManager_.getEntities())
        {
            auto& eStatus = entity->getComponent<CCycle>();

            if (!eStatus.isAlive())
                continue;

            auto& eTrans = entity->getComponent<CTransform>();

            auto& dir = directions_[eTrans.dir];

            eTrans.pos[0] += dir.x;
            eTrans.pos[1] += dir.y;
        }
    }

    void ScenePlay::sStatus()
    {
        for (auto entity : entityManager_.getEntities())
        {
            auto& eStatus = entity->getComponent<CCycle>();
            auto& eTrans  = entity->getComponent<CTransform>();

            if (!eStatus.isAlive())
                continue;

            eStatus.score += 1;

            // if the player is not on a blank tile, it dies
            if (!isSafe(eTrans.pos[0], eTrans.pos[1]))
                eStatus.kill();

            // if the player is still alive, set the grid cell to its value
            if (eStatus.isAlive())
                level_[eTrans.pos[0]][eTrans.pos[1]] = entity->id();
        }
    }

    void ScenePlay::sDoAction(const Engine2D::Action& action)
    {
        if (action.type() == "START")
        {
            if (action.name() == "QUIT")
            {
                onEnd();
            }
            else if (action.name() == "PAUSE")
            {
                paused_ = !paused_;
            }
            else if (action.name() == "RESET")
            {
                setMap();
            }
        }
    }

    void ScenePlay::sRender()
    {
        game_->window().clear();

        // draw background
        float g = static_cast<float>(gridSize_);

        // the rectangle we will use to draw the filled cells
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(g, g));

        // loop over every tile in the grid
        for (std::size_t x {}; x < gridWidth_; x++)
        {
            for (std::size_t y {}; y < gridHeight_; y++)
            {
                // blank tiles are -1, so skip those
                int val = level_[x][y];
                if (val == -1)
                    continue;

                // look up the color from the cycle with the given id
                for (auto entity : entityManager_.getEntities())
                {
                    if (entity->id() == static_cast<std::size_t>(val))
                        rect.setFillColor(entity->getComponent<CCycle>().color);
                }

                rect.setPosition(x * g, y * g);

                game_->window().draw(rect);
            }
        }

        // draw grid lines
        for (std::size_t i {}; i <= gridWidth_; i++)
            drawLine(i * g, 0, i * g, battleHeight(), sf::Color(30, 30, 30));

        for (std::size_t j {}; j <= gridHeight_; j++)
            drawLine(0, j * g, battleWidth(), j * g, sf::Color(30, 30, 30));

        // draw score
        for (auto entity : entityManager_.getEntities())
        {
            auto eAI    = entity->getComponent<CAI>();
            auto eCycle = entity->getComponent<CCycle>();

            std::stringstream ss;
            ss << eAI.ai->name() << '\n' << eCycle.score;

            text_.setFillColor(eCycle.color);
            text_.setString(ss.str().c_str());
            text_.setPosition(battleWidth() + 5, static_cast<float>(5 + entity->id() * 64));

            game_->window().draw(text_);
        }
    }

    bool ScenePlay::isLegalAction(ConcreteEntityPtr entity, std::size_t action) const
    {
        const Direction& oldDir = directions_[entity->getComponent<CTransform>().dir];
        const Direction& newDir = directions_[action];

        // it's not a legal action if it goes backward onto itself
        if ((oldDir.x != 0) && (oldDir.x + newDir.x == 0))
        {
            return false;
        }
        if ((oldDir.y != 0) && (oldDir.y + newDir.y == 0))
        {
            return false;
        }

        return true;
    }

    std::vector<std::size_t> ScenePlay::getLegalActions(ConcreteEntityPtr entity) const
    {
        std::vector<std::size_t> legalActions;

        for (std::size_t action {}; action < directions_.size(); action++)
        {
            if (isLegalAction(entity, action))
            {
                legalActions.push_back(action);
            }
        }

        return legalActions;
    }

    bool ScenePlay::isSafe(std::size_t x, std::size_t y) const
    {
        return x >= 0 && x < level_.size() && y >= 0 && y < level_[x].size() && level_[x][y] == -1;
    }
}   // namespace LightCycle
