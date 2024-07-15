// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scenePlay.cpp
// Author: crdrisko
// Date: 06/29/2024-13:27:44
// Description:

#include "shapeWatcher/scenes/scenePlay.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

#include <common-utils/files.hpp>
#include <common-utils/strings.hpp>

#include "shapeWatcher/components.hpp"
#include "shapeWatcher/forwardDeclare.hpp"

namespace ShapeWatcher
{
    ScenePlay::ScenePlay(Engine2D::Engine* gameEngine, const std::string& config) : Engine2D::Scene {gameEngine}
    {
        init(config);
    }

    void ScenePlay::init(const std::string& config)
    {
        registerAction(sf::Keyboard::Escape, "QUIT");

        DryChem::FileParser parser {config};
        auto rows = parser.parseDataFile(DryChem::AsRows());

        for (const auto& row : rows)
        {
            DryChem::Tokenizer tok {row};
            std::vector<std::string> splitRow = tok.split();

            if (splitRow[0] == "Window")
            {
                /*
                 * Window W H(first line)
                 *  Width: W - int
                 *  Height : H - int
                 */
            }
            else if (splitRow[0] == "Font")
            {
                /*
                 * Font N F S R G B
                 *  Name:   N           std::string
                 *	File:   F           std::string
                 *  Size:   S           int
                 *  Color:  (R, G, B)   int, int, int
                 */
                text_.setFont(game_->assets().getFont(splitRow[1]));
                text_.setCharacterSize(std::stoi(splitRow[3]));

                int r = std::stoi(splitRow[4]);
                int g = std::stoi(splitRow[5]);
                int b = std::stoi(splitRow[6]);

                text_.setFillColor(sf::Color(r, g, b));
            }
            else if (splitRow[0] == "Rectangle")
            {
                /*
                 * Rectangle N X Y SX SY R G B W H
                 *  Name : N - std::string
                 *  Position : (X, Y) - float, float
                 *  Speed : (SX, SY) - float, float
                 *  Color : (R, G, B) - int, int, int
                 *  Size : (W, H) - float, float
                 */
                RectangleConfig config;

                config.N  = splitRow[1];
                config.X  = std::stof(splitRow[2]);
                config.Y  = std::stof(splitRow[3]);
                config.SX = std::stof(splitRow[4]);
                config.SY = std::stof(splitRow[5]);
                config.OR = std::stoi(splitRow[6]);
                config.OG = std::stoi(splitRow[7]);
                config.OB = std::stoi(splitRow[8]);
                config.W  = std::stof(splitRow[9]);
                config.H  = std::stof(splitRow[10]);

                DryPhys::Vector3D pos {config.X, config.Y, 0};
                DryPhys::Vector3D vel {config.SX, config.SY, 0};

                sf::Color fillColor = sf::Color {config.OR, config.OG, config.OB};

                sf::Text text = text_;
                text.setString(config.N);

                auto shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(config.W, config.H));

                auto entity = entityManager_.addEntity("Rectangle");

                entity->addComponent<CTransform>(pos, vel);
                entity->addComponent<CShape>(text, shape, fillColor);
            }
            else if (splitRow[0] == "Circle")
            {
                /*
                 * Circle N X Y SX SY R G B R
                 *  Name : N - std::string
                 *  Position : (X, Y) - float, float
                 *  Speed : (SX, SY) - float, float
                 *  Color : (R, G, B) - int, int, int
                 *  Radius : R - float
                 */
                CircleConfig config;

                config.N  = splitRow[1];
                config.X  = std::stof(splitRow[2]);
                config.Y  = std::stof(splitRow[3]);
                config.SX = std::stof(splitRow[4]);
                config.SY = std::stof(splitRow[5]);
                config.OR = std::stoi(splitRow[6]);
                config.OG = std::stoi(splitRow[7]);
                config.OB = std::stoi(splitRow[8]);
                config.R  = std::stof(splitRow[9]);

                DryPhys::Vector3D pos {config.X, config.Y, 0};
                DryPhys::Vector3D vel {config.SX, config.SY, 0};

                sf::Color fillColor = sf::Color {config.OR, config.OG, config.OB};

                auto shape = std::make_shared<sf::CircleShape>(config.R);

                sf::Text text = text_;
                text.setString(config.N);

                auto entity = entityManager_.addEntity("Circle");

                entity->addComponent<CTransform>(pos, vel);
                entity->addComponent<CShape>(text, shape, fillColor);
            }
        }
    }

    void ScenePlay::update()
    {
        entityManager_.update();

        if (!paused_)
        {
            currentFrame_++;

            sCollision();
            sMovement();
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
        }
    }

    void ScenePlay::sRender()
    {
        game_->window().clear();

        for (auto entity : entityManager_.getEntities())
        {
            auto& [px, py, pz] = entity->getComponent<CTransform>().position;
            assert(pz == static_cast<DryPhys::real>(0));

            auto& eShape = entity->getComponent<CShape>();

            eShape.shape->setPosition(px, py);

            game_->window().draw(*eShape.shape);

            sf::FloatRect shapeBoundingBox = eShape.shape->getGlobalBounds();
            sf::FloatRect textBoundingBox  = eShape.text.getGlobalBounds();

            eShape.text.setPosition(px + 0.5f * (shapeBoundingBox.width - textBoundingBox.width),
                py + 0.5f * (shapeBoundingBox.height - textBoundingBox.height));

            game_->window().draw(eShape.text);
        }
    }

    void ScenePlay::onEnd() { game_->quit(); }

    void ScenePlay::sCollision()
    {
        const auto windowSize = game_->window().getSize();

        for (auto entity : entityManager_.getEntities())
        {
            auto& [vx, vy, vz] = entity->getComponent<CTransform>().velocity;
            assert(vz == static_cast<DryPhys::real>(0));

            auto boundingBox = entity->getComponent<CShape>().shape->getGlobalBounds();

            auto bb_pos = boundingBox.getPosition();

            if (bb_pos.x <= 0.0f || bb_pos.x + boundingBox.width > static_cast<float>(windowSize.x))
            {
                vx *= -1.0f;
            }

            if (bb_pos.y <= 0.0f || bb_pos.y + boundingBox.height > static_cast<float>(windowSize.y))
            {
                vy *= -1.0f;
            }
        }
    }

    void ScenePlay::sMovement()
    {
        for (auto entity : entityManager_.getEntities())
        {
            auto& eTrans = entity->getComponent<CTransform>();

            eTrans.position += eTrans.velocity;
        }
    }
}   // namespace ShapeWatcher
