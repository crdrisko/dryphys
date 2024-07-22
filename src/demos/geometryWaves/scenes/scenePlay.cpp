// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scenePlay.cpp
// Author: crdrisko
// Date: 06/19/2024-05:30:03
// Description:

#include "geometryWaves/scenes/scenePlay.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <random>

#include <common-utils/files.hpp>
#include <common-utils/strings.hpp>
#include <dryphys/utilities.hpp>
#include <engine2d/action.hpp>
#include <engine2d/engine.hpp>

#include "geometryWaves/components.hpp"
#include "geometryWaves/forwardDeclare.hpp"

using namespace DryPhys::Literals;

namespace GeometryWaves
{
    ScenePlay::ScenePlay(Engine2D::Engine* gameEngine, const std::string& config) : Engine2D::Scene {gameEngine}
    {
        init(config);
    }

    void ScenePlay::init(const std::string& config)
    {
        registerAction(sf::Keyboard::W, "UP");
        registerAction(sf::Keyboard::S, "DOWN");
        registerAction(sf::Keyboard::A, "LEFT");
        registerAction(sf::Keyboard::D, "RIGHT");
        registerAction(sf::Keyboard::P, "PAUSE");
        registerAction(sf::Keyboard::Escape, "QUIT");

        // Parse input file
        DryChem::FileParser parser {config};
        auto rows = parser.parseDataFile(DryChem::AsRows());

        for (const auto& row : rows)
        {
            DryChem::Tokenizer tok {row};
            std::vector<std::string> splitRow = tok.split();

            if (splitRow[0] == "Window")
            {
                /*
                 * Window N W H FL FS
                 *  Name:           N       std::string
                 *  Width:          W       int
                 *  Height:         H       int
                 *  Frame Limit:    FL      int
                 *  Full Screen:    FS      int (1 or 0)
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
            else if (splitRow[0] == "Player")
            {
                /*
                 * Player SR CR S FR FG FB OR OG OB OT V
                 *  Shape Radius:       SR              int
                 *  Collision Radius:   CR              int
                 *  Speed:              S               float
                 *  Fill Color:         (FR, FG, FB)    int, int, int
                 *  Outline Color:      (OR, OG, OB)    int, int, int
                 *  Outline Thickness:  OT              int
                 *  Shape Vertices:     V               int
                 */
                playerConfig_.SR = std::stoi(splitRow[1]);
                playerConfig_.CR = std::stoi(splitRow[2]);
                playerConfig_.S  = std::stof(splitRow[3]);
                playerConfig_.FR = std::stoi(splitRow[4]);
                playerConfig_.FG = std::stoi(splitRow[5]);
                playerConfig_.FB = std::stoi(splitRow[6]);
                playerConfig_.OR = std::stoi(splitRow[7]);
                playerConfig_.OG = std::stoi(splitRow[8]);
                playerConfig_.OB = std::stoi(splitRow[9]);
                playerConfig_.OT = std::stoi(splitRow[10]);
                playerConfig_.V  = std::stoi(splitRow[11]);
            }
            else if (splitRow[0] == "Enemy")
            {
                /*
                 * Enemy SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SI
                 *  Shape Radius:       SR              int
                 *  Collision Radius:   CR              int
                 *  Min / Max Speed:    (SMIN, SMAX)    float, float
                 *  Outline Color:      (OR, OG, OB)    int, int, int
                 *  Outline Thickness:  OT              int
                 *  Min / Max Vertices: (VMIN, VMAX)    int, int
                 *  Small Lifespan:     L               int
                 *  Spawn Interval:     SI              int
                 */
                enemyConfig_.SR   = std::stoi(splitRow[1]);
                enemyConfig_.CR   = std::stoi(splitRow[2]);
                enemyConfig_.SMIN = std::stof(splitRow[3]);
                enemyConfig_.SMAX = std::stof(splitRow[4]);
                enemyConfig_.OR   = std::stoi(splitRow[5]);
                enemyConfig_.OG   = std::stoi(splitRow[6]);
                enemyConfig_.OB   = std::stoi(splitRow[7]);
                enemyConfig_.OT   = std::stoi(splitRow[8]);
                enemyConfig_.VMIN = std::stoi(splitRow[9]);
                enemyConfig_.VMAX = std::stoi(splitRow[10]);
                enemyConfig_.L    = std::stoi(splitRow[11]);
                enemyConfig_.SI   = std::stoi(splitRow[12]);
            }
            else if (splitRow[0] == "Bullet")
            {
                /*
                 * Bullet SR CR S FR FG FB OR OG OB OT V L
                 *  Shape Radius:       SR              int
                 *  Collision Radius:   CR              int
                 *  Speed:              S               float
                 *  Fill Color:         (FR, FG, FB)    int, int, int
                 *  Outline Color:      (OR, OG, OB)    int, int, int
                 *  Outline Thickness:  OT              int
                 *  Shape Vertices:     V               int
                 *  Lifespan:           L               int
                 */
                bulletConfig_.SR = std::stoi(splitRow[1]);
                bulletConfig_.CR = std::stoi(splitRow[2]);
                bulletConfig_.S  = std::stof(splitRow[3]);
                bulletConfig_.FR = std::stoi(splitRow[4]);
                bulletConfig_.FG = std::stoi(splitRow[5]);
                bulletConfig_.FB = std::stoi(splitRow[6]);
                bulletConfig_.OR = std::stoi(splitRow[7]);
                bulletConfig_.OG = std::stoi(splitRow[8]);
                bulletConfig_.OB = std::stoi(splitRow[9]);
                bulletConfig_.OT = std::stoi(splitRow[10]);
                bulletConfig_.V  = std::stoi(splitRow[11]);
                bulletConfig_.L  = std::stoi(splitRow[12]);
            }
            else if (splitRow[0] == "Music")
            {
                if (!music_)
                    music_ = game_->assets().getMusic("GeoMusic");

                music_->setVolume(20);
                music_->play();
            }
        }

        std::random_device rd;
        gen_ = std::mt19937(rd());

        entityManager_ = ConcreteEntityManager {};

        spawnPlayer();
    }

    void ScenePlay::update()
    {
        entityManager_.update();

        if (!paused_)
        {
            currentFrame_++;

            sEnemySpawner();
            sCollision();
            sMovement();
            sLifespan();
        }
    }

    void ScenePlay::doActionImpl(const Engine2D::Action& action)
    {
        auto& pInput = player_->getComponent<CInput>();

        if (auto atype = action.type(); atype == Engine2D::Action::START)
        {
            switch (action.sid())
            {
            case "LEFT_CLICK"_sID:
                if (!paused_)
                    spawnBullet(player_, action.pos());
                break;
            case "RIGHT_CLICK"_sID:
                if (!paused_)
                    spawnSpecialWeapon(player_, action.pos());
                break;
            case "UP"_sID:
                pInput.up = true;
                break;
            case "DOWN"_sID:
                pInput.down = true;
                break;
            case "LEFT"_sID:
                pInput.left = true;
                break;
            case "RIGHT"_sID:
                pInput.right = true;
                break;
            case "PAUSE"_sID:
                setPaused(!paused_);
                break;
            case "QUIT"_sID:
                onEnd();
                break;
            default:
                break;
            }
        }
        else if (atype == Engine2D::Action::END)
        {
            switch (action.sid())
            {
            case "UP"_sID:
                pInput.up = false;
                break;
            case "DOWN"_sID:
                pInput.down = false;
                break;
            case "LEFT"_sID:
                pInput.left = false;
                break;
            case "RIGHT"_sID:
                pInput.right = false;
                break;
            default:
                break;
            }
        }
    }

    void ScenePlay::render()
    {
        game_->window().clear();

        text_.setString("Score: " + std::to_string(score_));
        game_->window().draw(text_);

        for (auto entity : entityManager_.getEntities())
        {
            auto& eTrans = entity->getComponent<CTransform>();
            auto& eShape = entity->getComponent<CShape>();

            eShape.circle.setPosition(eTrans.position[0], eTrans.position[1]);

            eTrans.angle += 1.0f;

            eShape.circle.setRotation(eTrans.angle);

            game_->window().draw(eShape.circle);
        }
    }

    void ScenePlay::onEnd()
    {
        if (music_)
            music_->stop();

        game_->quit();
    }

    void ScenePlay::sEnemySpawner()
    {
        if (!((currentFrame_ - lastEnemySpawnTime_) % enemyConfig_.SI))
        {
            spawnEnemy();
        }
    }

    void ScenePlay::sCollision()
    {
        auto windowSize = game_->window().getSize();

        float width  = static_cast<float>(windowSize.x);
        float height = static_cast<float>(windowSize.y);

        auto& pPos    = player_->getComponent<CTransform>().position;
        auto& pRadius = player_->getComponent<CCollision>().radius;
        auto& pInput  = player_->getComponent<CInput>();


        DryPhys::Vector3D tempVec;

        // Enemy Collisions
        for (auto enemy : entityManager_.getEntities("enemy"))
        {
            auto& eTrans  = enemy->getComponent<CTransform>();
            auto& eRadius = enemy->getComponent<CCollision>().radius;

            // Enemy-Bullet Collisions
            for (auto bullet : entityManager_.getEntities("bullet"))
            {
                auto& bPos    = bullet->getComponent<CTransform>().position;
                auto& bRadius = bullet->getComponent<CCollision>().radius;

                float radiusSum = eRadius + bRadius;

                tempVec = bPos - eTrans.position;

                if (tempVec.magnitudeSquared() < radiusSum * radiusSum)
                {
                    score_ += enemy->getComponent<CScore>().score;

                    bullet->destroy();
                    enemy->destroy();

                    // Don't spawn small enemies from small enemies
                    if (!enemy->hasComponent<CLifespan>())
                        spawnSmallEnemies(enemy);
                }
            }

            // Enemy-Player Collisions
            float radiusSum = eRadius + pRadius;

            tempVec = pPos - eTrans.position;

            if (tempVec.magnitudeSquared() < radiusSum * radiusSum)
            {
                score_ -= enemy->getComponent<CScore>().score;

                enemy->destroy();

                float mx = windowSize.x / 2.0f;
                float my = windowSize.y / 2.0f;

                pPos = DryPhys::Vector3D(mx, my, 0);

                // Don't spawn small enemies from small enemies
                if (!enemy->hasComponent<CLifespan>())
                    spawnSmallEnemies(enemy);
            }

            // Enemy-World Collisions
            if (eTrans.position[0] < eRadius || eTrans.position[0] > width - eRadius)
            {
                eTrans.velocity[0] *= -1.0F;
            }

            if (eTrans.position[1] < eRadius || eTrans.position[1] > height - eRadius)
            {
                eTrans.velocity[1] *= -1.0F;
            }
        }

        // Bullet Collisions
        for (auto bullet : entityManager_.getEntities("bullet"))
        {
            auto& bPos    = bullet->getComponent<CTransform>().position;
            auto& bRadius = bullet->getComponent<CCollision>().radius;

            if (bPos[0] < bRadius || bPos[1] < bRadius || bPos[0] > width - bRadius || bPos[1] > height - bRadius)
            {
                bullet->destroy();
            }
        }

        // Player Collisions
        if (pPos[0] < pRadius)
        {
            pInput.left = false;
        }
        if (pPos[1] < pRadius)
        {
            pInput.up = false;
        }
        if (pPos[0] > width - pRadius)
        {
            pInput.right = false;
        }
        if (pPos[1] > height - pRadius)
        {
            pInput.down = false;
        }
    }

    void ScenePlay::sMovement()
    {
        auto& pTrans = player_->getComponent<CTransform>();
        auto& pInput = player_->getComponent<CInput>();

        pTrans.velocity = {0.0f, 0.0f, 0.0f};

        if (pInput.up)
        {
            pTrans.velocity[1] = -playerConfig_.S;
        }
        if (pInput.left)
        {
            pTrans.velocity[0] = -playerConfig_.S;
        }
        if (pInput.down)
        {
            pTrans.velocity[1] = playerConfig_.S;
        }
        if (pInput.right)
        {
            pTrans.velocity[0] = playerConfig_.S;
        }

        for (auto entity : entityManager_.getEntities())
        {
            auto& eTrans = entity->getComponent<CTransform>();

            eTrans.position += eTrans.velocity;
        }
    }

    void ScenePlay::sLifespan()
    {
        for (auto entity : entityManager_.getEntities())
        {
            if (entity->hasComponent<CLifespan>())
            {
                auto& eShape    = entity->getComponent<CShape>().circle;
                auto& eLifespan = entity->getComponent<CLifespan>();

                sf::Color fill    = eShape.getFillColor();
                sf::Color outline = eShape.getOutlineColor();

                sf::Uint8 alpha = static_cast<sf::Uint8>(
                    255.0f * static_cast<float>(eLifespan.remaining) / static_cast<float>(eLifespan.total));

                fill.a    = alpha;
                outline.a = alpha;

                eShape.setFillColor(fill);
                eShape.setOutlineColor(outline);

                if (!eLifespan.remaining--)
                {
                    entity->destroy();
                }
            }
        }
    }

    void ScenePlay::spawnPlayer()
    {
        auto windowSize = game_->window().getSize();
        player_         = entityManager_.addEntity("player");

        sf::Color fill {playerConfig_.FR, playerConfig_.FG, playerConfig_.FB};
        sf::Color outline {playerConfig_.OR, playerConfig_.OG, playerConfig_.OB};

        DryPhys::Vector3D position {windowSize.x / 2.0f, windowSize.y / 2.0f, 0};

        player_->addComponent<CTransform>(position, DryPhys::Vector3D {}, 0.0);
        player_->addComponent<CShape>(playerConfig_.SR, playerConfig_.V, fill, outline, playerConfig_.OT);
        player_->addComponent<CCollision>(playerConfig_.CR);
        player_->addComponent<CInput>();
    }

    void ScenePlay::spawnEnemy()
    {
        auto windowSize = game_->window().getSize();
        auto enemy      = entityManager_.addEntity("enemy");

        std::uniform_int_distribution<> verticiesDistr(enemyConfig_.VMIN, enemyConfig_.VMAX);
        std::uniform_int_distribution<> colorDistr(0, 255);

        std::uniform_real_distribution<float> xDistr(enemyConfig_.SR, static_cast<float>(windowSize.x) - enemyConfig_.SR);
        std::uniform_real_distribution<float> yDistr(enemyConfig_.SR, static_cast<float>(windowSize.y) - enemyConfig_.SR);

        std::uniform_real_distribution<float> speedDistr(enemyConfig_.SMIN, enemyConfig_.SMAX);

        int verticies = verticiesDistr(gen_);

        sf::Uint8 r = static_cast<sf::Uint8>(colorDistr(gen_));
        sf::Uint8 g = static_cast<sf::Uint8>(colorDistr(gen_));
        sf::Uint8 b = static_cast<sf::Uint8>(colorDistr(gen_));

        sf::Color fill {r, g, b};
        sf::Color outline {enemyConfig_.OR, enemyConfig_.OG, enemyConfig_.OB};

        DryPhys::Vector3D position {xDistr(gen_), yDistr(gen_), 0};

        float angle = std::atan2(position[1], position[0]);
        float speed = speedDistr(gen_);

        DryPhys::Vector3D velocity {speed * std::cos(angle), speed * std::sin(angle), 0};

        enemy->addComponent<CTransform>(position, velocity, angle);
        enemy->addComponent<CShape>(enemyConfig_.SR, verticies, fill, outline, enemyConfig_.OT);
        enemy->addComponent<CCollision>(enemyConfig_.CR);
        enemy->addComponent<CScore>(100 * verticies);

        lastEnemySpawnTime_ = currentFrame_;
    }

    void ScenePlay::spawnSmallEnemies(ConcreteEntityPtr entity)
    {
        DryPhys::Vector3D position = entity->getComponent<CTransform>().position;

        int verticies  = static_cast<int>(entity->getComponent<CShape>().circle.getPointCount());
        sf::Color fill = entity->getComponent<CShape>().circle.getFillColor();
        sf::Color outline {enemyConfig_.OR, enemyConfig_.OG, enemyConfig_.OB};

        float angle = 6.28 / static_cast<float>(verticies);

        for (int i {}; i < verticies; ++i)
        {
            auto smallEnemy = entityManager_.addEntity("enemy");

            DryPhys::Vector3D velocity {enemyConfig_.SMAX * std::cos(i * angle), enemyConfig_.SMAX * std::sin(i * angle), 0};

            smallEnemy->addComponent<CTransform>(position, velocity, angle);
            smallEnemy->addComponent<CShape>(enemyConfig_.SR / 2.0f, verticies, fill, outline, enemyConfig_.OT);
            smallEnemy->addComponent<CCollision>(enemyConfig_.CR / 2.0f);
            smallEnemy->addComponent<CScore>(200 * verticies);
            smallEnemy->addComponent<CLifespan>(enemyConfig_.L);
        }
    }

    void ScenePlay::spawnBullet(ConcreteEntityPtr entity, const DryPhys::Vector3D& mousePos)
    {
        auto& ePos = entity->getComponent<CTransform>().position;

        auto bullet = entityManager_.addEntity("bullet");

        sf::Color fill {bulletConfig_.FR, bulletConfig_.FG, bulletConfig_.FB};
        sf::Color outline {bulletConfig_.OR, bulletConfig_.OG, bulletConfig_.OB};

        DryPhys::Vector3D position = mousePos - ePos;

        float angle = std::atan2(position[1], position[0]);

        DryPhys::Vector3D velocity {bulletConfig_.S * std::cos(angle), bulletConfig_.S * std::sin(angle), 0};

        bullet->addComponent<CTransform>(ePos, velocity, angle);
        bullet->addComponent<CShape>(bulletConfig_.SR, bulletConfig_.V, fill, outline, bulletConfig_.OT);
        bullet->addComponent<CCollision>(bulletConfig_.CR);
        bullet->addComponent<CLifespan>(bulletConfig_.L);
    }

    void ScenePlay::spawnSpecialWeapon(ConcreteEntityPtr, const DryPhys::Vector3D&) {}
}   // namespace GeometryWaves
