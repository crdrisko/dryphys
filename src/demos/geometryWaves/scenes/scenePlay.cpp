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
#include <engine2d/action.hpp>
#include <engine2d/engine.hpp>

namespace GeometryWaves
{
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

        spawnPlayer();
    }

    void ScenePlay::update()
    {
        entities_.update();

        if (!paused_)
        {
            currentFrame_++;

            sEnemySpawner();
            sCollision();
            sMovement();
            sLifespan();
        }
    }

    void ScenePlay::sDoAction(const Engine2D::Action& action)
    {
        if (action.type() == "START")
        {
            if (action.name() == "LEFT_CLICK")
            {
                if (!paused_)
                    spawnBullet(player_, action.pos());
            }
            else if (action.name() == "UP")
            {
                player_->cInput->up = true;
            }
            else if (action.name() == "DOWN")
            {
                player_->cInput->down = true;
            }
            else if (action.name() == "LEFT")
            {
                player_->cInput->left = true;
            }
            else if (action.name() == "RIGHT")
            {
                player_->cInput->right = true;
            }
            else if (action.name() == "PAUSE")
            {
                setPaused(!paused_);
            }
            else if (action.name() == "QUIT")
            {
                onEnd();
            }
        }

        if (action.type() == "END")
        {
            if (action.name() == "UP")
            {
                player_->cInput->up = false;
            }
            else if (action.name() == "DOWN")
            {
                player_->cInput->down = false;
            }
            else if (action.name() == "LEFT")
            {
                player_->cInput->left = false;
            }
            else if (action.name() == "RIGHT")
            {
                player_->cInput->right = false;
            }
        }
    }

    void ScenePlay::sRender()
    {
        game_->window().clear();

        text_.setString("Score: " + std::to_string(score_));
        game_->window().draw(text_);

        for (auto entity : entities_.getEntities())
        {
            entity->cShape->circle.setPosition(entity->cTransform->position[0], entity->cTransform->position[1]);

            entity->cTransform->angle += 1.0f;

            entity->cShape->circle.setRotation(entity->cTransform->angle);

            game_->window().draw(entity->cShape->circle);
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

        DryPhys::Vector3D tempVec;

        // Enemy Collisions
        for (auto enemy : entities_.getEntities("enemy"))
        {
            DryPhys::Vector3D ePos = enemy->cTransform->position;

            // Enemy-Bullet Collisions
            for (auto bullet : entities_.getEntities("bullet"))
            {
                float radiusSum = enemy->cCollision->radius + bullet->cCollision->radius;

                tempVec = bullet->cTransform->position - ePos;

                if (tempVec.magnitudeSquared() < radiusSum * radiusSum)
                {
                    score_ += enemy->cScore->score;

                    bullet->destroy();
                    enemy->destroy();

                    // Don't spawn small enemies from small enemies
                    if (!enemy->cLifespan)
                        spawnSmallEnemies(enemy);
                }
            }

            // Enemy-Player Collisions
            float radiusSum = enemy->cCollision->radius + player_->cCollision->radius;

            tempVec = player_->cTransform->position - ePos;

            if (tempVec.magnitudeSquared() < radiusSum * radiusSum)
            {
                // Should this count towards score??
                //score_ += enemy->cScore->score;

                enemy->destroy();

                float mx = windowSize.x / 2.0f;
                float my = windowSize.y / 2.0f;

                player_->cTransform->position = DryPhys::Vector3D(mx, my, 0);

                // Don't spawn small enemies from small enemies
                if (!enemy->cLifespan)
                    spawnSmallEnemies(enemy);
            }

            // Enemy-World Collisions
            if (enemy->cTransform->position[0] < enemy->cCollision->radius
                || enemy->cTransform->position[0] > width - enemy->cCollision->radius)
            {
                enemy->cTransform->velocity[0] *= -1.0F;
            }

            if (enemy->cTransform->position[1] < enemy->cCollision->radius
                || enemy->cTransform->position[1] > height - enemy->cCollision->radius)
            {
                enemy->cTransform->velocity[1] *= -1.0F;
            }
        }

        // Bullet Collisions
        for (auto bullet : entities_.getEntities("bullet"))
        {
            if (bullet->cTransform->position[0] < bullet->cCollision->radius
                || bullet->cTransform->position[1] < bullet->cCollision->radius
                || bullet->cTransform->position[0] > width - bullet->cCollision->radius
                || bullet->cTransform->position[1] > height - bullet->cCollision->radius)
            {
                bullet->destroy();
            }
        }

        // Player Collisions
        if (player_->cTransform->position[0] < player_->cCollision->radius)
        {
            player_->cInput->left = false;
        }

        if (player_->cTransform->position[1] < player_->cCollision->radius)
        {
            player_->cInput->up = false;
        }

        if (player_->cTransform->position[0] > width - player_->cCollision->radius)
        {
            player_->cInput->right = false;
        }

        if (player_->cTransform->position[1] > height - player_->cCollision->radius)
        {
            player_->cInput->down = false;
        }
    }

    void ScenePlay::sMovement()
    {
        player_->cTransform->velocity = {0.0f, 0.0f, 0.0f};

        if (player_->cInput->up)
        {
            player_->cTransform->velocity[1] = -playerConfig_.S;
        }
        if (player_->cInput->left)
        {
            player_->cTransform->velocity[0] = -playerConfig_.S;
        }
        if (player_->cInput->down)
        {
            player_->cTransform->velocity[1] = playerConfig_.S;
        }
        if (player_->cInput->right)
        {
            player_->cTransform->velocity[0] = playerConfig_.S;
        }

        for (auto entity : entities_.getEntities())
        {
            entity->cTransform->position += entity->cTransform->velocity;
        }
    }

    void ScenePlay::sLifespan()
    {
        for (auto entity : entities_.getEntities())
        {
            if (entity->cLifespan)
            {
                sf::Color fill    = entity->cShape->circle.getFillColor();
                sf::Color outline = entity->cShape->circle.getOutlineColor();

                sf::Uint8 alpha = static_cast<sf::Uint8>(255.0f * static_cast<float>(entity->cLifespan->remaining)
                                                         / static_cast<float>(entity->cLifespan->total));

                fill.a    = alpha;
                outline.a = alpha;

                entity->cShape->circle.setFillColor(fill);
                entity->cShape->circle.setOutlineColor(outline);

                if (!entity->cLifespan->remaining--)
                {
                    entity->destroy();
                }
            }
        }
    }

    void ScenePlay::spawnPlayer()
    {
        auto windowSize = game_->window().getSize();
        auto player     = entities_.addEntity("player");

        sf::Color fill {playerConfig_.FR, playerConfig_.FG, playerConfig_.FB};
        sf::Color outline {playerConfig_.OR, playerConfig_.OG, playerConfig_.OB};

        DryPhys::Vector3D position {windowSize.x / 2.0f, windowSize.y / 2.0f, 0};

        player->cTransform = std::make_shared<CTransform>(position, DryPhys::Vector3D {}, 0.0);
        player->cShape     = std::make_shared<CShape>(playerConfig_.SR, playerConfig_.V, fill, outline, playerConfig_.OT);
        player->cCollision = std::make_shared<CCollision>(playerConfig_.CR);
        player->cInput     = std::make_shared<CInput>();

        player_ = player;
    }

    void ScenePlay::spawnEnemy()
    {
        auto windowSize = game_->window().getSize();
        auto enemy      = entities_.addEntity("enemy");

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

        enemy->cTransform = std::make_shared<CTransform>(position, velocity, angle);
        enemy->cShape     = std::make_shared<CShape>(enemyConfig_.SR, verticies, fill, outline, enemyConfig_.OT);
        enemy->cCollision = std::make_shared<CCollision>(enemyConfig_.CR);
        enemy->cScore     = std::make_shared<CScore>(100 * verticies);

        lastEnemySpawnTime_ = currentFrame_;
    }

    void ScenePlay::spawnSmallEnemies(std::shared_ptr<Entity> entity)
    {
        DryPhys::Vector3D position = entity->cTransform->position;

        int verticies  = static_cast<int>(entity->cShape->circle.getPointCount());
        sf::Color fill = entity->cShape->circle.getFillColor();
        sf::Color outline {enemyConfig_.OR, enemyConfig_.OG, enemyConfig_.OB};

        float angle = 6.28 / static_cast<float>(verticies);

        for (int i {}; i < verticies; ++i)
        {
            auto smallEnemy = entities_.addEntity("enemy");

            DryPhys::Vector3D velocity {enemyConfig_.SMAX * std::cos(i * angle), enemyConfig_.SMAX * std::sin(i * angle), 0};

            smallEnemy->cTransform = std::make_shared<CTransform>(position, velocity, angle);
            smallEnemy->cShape = std::make_shared<CShape>(enemyConfig_.SR / 2.0f, verticies, fill, outline, enemyConfig_.OT);
            smallEnemy->cCollision = std::make_shared<CCollision>(enemyConfig_.CR / 2.0f);
            smallEnemy->cScore     = std::make_shared<CScore>(200 * verticies);
            smallEnemy->cLifespan  = std::make_shared<CLifespan>(enemyConfig_.L);
        }
    }

    void ScenePlay::spawnBullet(std::shared_ptr<Entity> entity, const DryPhys::Vector3D& mousePos)
    {
        auto bullet = entities_.addEntity("bullet");

        sf::Color fill {bulletConfig_.FR, bulletConfig_.FG, bulletConfig_.FB};
        sf::Color outline {bulletConfig_.OR, bulletConfig_.OG, bulletConfig_.OB};

        DryPhys::Vector3D position = mousePos - entity->cTransform->position;

        float angle = std::atan2(position[1], position[0]);

        DryPhys::Vector3D velocity {bulletConfig_.S * std::cos(angle), bulletConfig_.S * std::sin(angle), 0};

        bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->position, velocity, angle);
        bullet->cShape     = std::make_shared<CShape>(bulletConfig_.SR, bulletConfig_.V, fill, outline, bulletConfig_.OT);
        bullet->cCollision = std::make_shared<CCollision>(bulletConfig_.CR);
        bullet->cLifespan  = std::make_shared<CLifespan>(bulletConfig_.L);
    }

    void ScenePlay::spawnSpecialWeapon(std::shared_ptr<Entity>) {}
}   // namespace GeometryWaves
