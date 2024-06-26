// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scenePlay.cpp
// Author: crdrisko
// Date: 06/19/2024-08:53:49
// Description:

#include "cybercity/scenes/scenePlay.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

#include <common-utils/files.hpp>
#include <common-utils/strings.hpp>
#include <dryphys/vector3d.hpp>
#include <engine2d/action.hpp>
#include <engine2d/animation.hpp>
#include <engine2d/engine.hpp>
#include <engine2d/scene.hpp>

#include "cybercity/entity.hpp"
#include "cybercity/physics.hpp"
#include "cybercity/scenes/sceneMenu.hpp"

namespace CyberCity
{
    ScenePlay::ScenePlay(Engine2D::Engine* gameEngine, const std::string& levelPath)
        : Engine2D::Scene {gameEngine}, levelPath_ {levelPath}
    {
        init(levelPath_);
    }

    void ScenePlay::init(const std::string& levelPath)
    {
        registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
        registerAction(sf::Keyboard::C, "TOGGLE_COLLISIONS");
        registerAction(sf::Keyboard::G, "TOGGLE_GRID");
        registerAction(sf::Keyboard::P, "PAUSE");
        registerAction(sf::Keyboard::Escape, "QUIT");
        registerAction(sf::Keyboard::W, "JUMP");
        registerAction(sf::Keyboard::S, "DEFEND");
        registerAction(sf::Keyboard::A, "LEFT");
        registerAction(sf::Keyboard::D, "RIGHT");
        registerAction(sf::Keyboard::Space, "EVADE");
        registerAction(sf::Keyboard::LShift, "SPRINT");
        registerAction(sf::Keyboard::Enter, "ATTACK");

        // Set display text parameters
        pauseText_.setCharacterSize(24);
        pauseText_.setFont(game_->assets().getFont("VinerHand"));

        gridText_.setCharacterSize(12);
        gridText_.setFont(game_->assets().getFont("Arial"));

        loadLevel(levelPath);
    }

    DryPhys::Vector3D ScenePlay::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
    {
        DryPhys::Vector3D result {gridX * gridSize_[0], height() - (gridY * gridSize_[1]), 0};
        DryPhys::Vector3D size {entity->getComponent<CAnimation>().animation.getSize()};

        float scale = gridSize_[0] / std::min(size[0], size[1]);

        result[0] += size[0] * scale / 2.0f;
        result[1] -= size[1] * scale / 2.0f;

        return result;
    }

    void ScenePlay::loadLevel(const std::string& filename)
    {
        // Reset the entity manager every time we load a level
        entityManager_ = EntityManager();

        DryChem::FileParser parser {filename};
        auto rows = parser.parseDataFile(DryChem::AsRows());

        for (const auto& row : rows)
        {
            DryChem::Tokenizer tok {row};
            std::vector<std::string> splitRow = tok.split();

            if (splitRow[0] == "Tile")
            {
                /*
             * Tile N GX GY
             *  Animation Name:     N       std::string (Animation asset name for this tile)
             *  Grid X Pos:         GX      float
             *  Grid Y Pos:         GY      float
             */
                std::shared_ptr<Entity> tileEntity = entityManager_.addEntity("tile");

                Engine2D::Animation& anim  = game_->assets().getAnimation(splitRow[1]);
                DryPhys::Vector3D animSize = anim.getSize();

                float scale = gridSize_[0] / std::min(animSize[0], animSize[1]);

                tileEntity->addComponent<CAnimation>(anim);
                tileEntity->addComponent<CTransform>(
                    gridToMidPixel(std::stof(splitRow[2]), std::stof(splitRow[3]), tileEntity),
                    DryPhys::Vector3D {},
                    DryPhys::Vector3D {scale, scale, 0},
                    0.0f);

                tileEntity->addComponent<CBoundingBox>(game_->assets().getAnimation(splitRow[1]).getSize() * scale);
            }
            else if (splitRow[0] == "Dec")
            {
                /*
             * Dec N X Y
             *  Animation Name:     N       std::string (Animation asset name for this decoration)
             *  Grid X Pos:         GX      float
             *  Grid Y Pos:         GY      float
             */
                std::shared_ptr<Entity> tileEntity = entityManager_.addEntity("dec");

                tileEntity->addComponent<CAnimation>(game_->assets().getAnimation(splitRow[1]));
                tileEntity->addComponent<CTransform>(
                    gridToMidPixel(std::stof(splitRow[2]), std::stof(splitRow[3]), tileEntity));
            }
            else if (splitRow[0] == "Background" || splitRow[0] == "Middleground" || splitRow[0] == "Foreground")
            {
                std::shared_ptr<Entity> envEntity = entityManager_.addEntity("env");

                envEntity->addComponent<CAnimation>(game_->assets().getAnimation(splitRow[1]));

                auto& anim = envEntity->getComponent<CAnimation>().animation;

                envViews[anim.getName()] = game_->window().getDefaultView();

                DryPhys::Vector3D size = anim.getSize();

                float scaleX = static_cast<float>(width()) / size[0] / std::stof(splitRow[2]);
                float scaleY = static_cast<float>(height()) / size[1];

                const sf::IntRect sizes {0, 0, 1800, 1000};

                anim.getSprite().setTextureRect(sizes);
                anim.getSprite().setScale(scaleX, scaleY);
            }
            else if (splitRow[0] == "Player")
            {
                /*
             * Player X Y CX CY SX SY SM G A B
             *  Grid X Pos:             X       float (starting position of player)
             *  Grid Y Pos:             Y       float (starting position of player)
             *  Bounding Box Width:     CX      float
             *  Bounding Box Height:    CY      float
             *  Left/Right Speed:       SX      float
             *  Jump Speed:             SY      float
             *  Max Speed:              SM      float
             *  Gravity:                G       float
             *  Avatar Animation:       A       std::string (Animation asset name to use for player character)
             *  Bullet Animation:       B       std::string (Animation asset name to use for player bullet)
             */
                playerConfig_.X        = std::stof(splitRow[1]);
                playerConfig_.Y        = std::stof(splitRow[2]);
                playerConfig_.CX       = std::stof(splitRow[3]);
                playerConfig_.CY       = std::stof(splitRow[4]);
                playerConfig_.SPEED    = std::stof(splitRow[5]);
                playerConfig_.JUMP     = std::stof(splitRow[6]);
                playerConfig_.MAXSPEED = std::stof(splitRow[7]);
                playerConfig_.GRAVITY  = std::stof(splitRow[8]);
                playerConfig_.AVATAR   = splitRow[9];
                playerConfig_.WEAPON   = splitRow[10];
            }
            else if (splitRow[0] == "Enemy")
            {
                /*
             * Enemy X Y CX CY SX SY SM G A B
             *  Grid X Pos:             X       float (starting position of enemy)
             *  Grid Y Pos:             Y       float (starting position of enemy)
             *  Bounding Box Width:     CX      float
             *  Bounding Box Height:    CY      float
             *  Left/Right Speed:       SX      float
             *  Jump Speed:             SY      float
             *  Max Speed:              SM      float
             *  Gravity:                G       float
             *  Avatar Animation:       A       std::string (Animation asset name to use for enemy character)
             *  Bullet Animation:       B       std::string (Animation asset name to use for enemy bullet)
             */
                ConfigData enemyConfig {};

                enemyConfig.X        = std::stof(splitRow[1]);
                enemyConfig.Y        = std::stof(splitRow[2]);
                enemyConfig.CX       = std::stof(splitRow[3]);
                enemyConfig.CY       = std::stof(splitRow[4]);
                enemyConfig.SPEED    = std::stof(splitRow[5]);
                enemyConfig.JUMP     = std::stof(splitRow[6]);
                enemyConfig.MAXSPEED = std::stof(splitRow[7]);
                enemyConfig.GRAVITY  = std::stof(splitRow[8]);
                enemyConfig.AVATAR   = splitRow[9];
                enemyConfig.WEAPON   = splitRow[10];

                spawnEnemy(enemyConfig);
            }
            else if (splitRow[0] == "Music")
            {
                music_ = game_->assets().getMusic(splitRow[1]);

                music_->setVolume(std::stof(splitRow[2]));
                music_->play();
            }
        }

        spawnPlayer();
    }

    void ScenePlay::spawnPlayer()
    {
        if (player_)
            player_->destroy();

        player_ = entityManager_.addEntity("player");

        player_->addComponent<CAnimation>(game_->assets().getAnimation(playerConfig_.AVATAR + "Idle"));
        player_->addComponent<CTransform>(gridToMidPixel(playerConfig_.X, playerConfig_.Y, player_));

        float scale = gridSize_[0] / 50.0f;

        player_->getComponent<CTransform>().scale = DryPhys::Vector3D {scale, scale, 0};

        player_->addComponent<CBoundingBox>(DryPhys::Vector3D {25, 50, 0} * scale);   // Needs to shift down
        player_->addComponent<CInput>();
        player_->addComponent<CState>();
        player_->addComponent<CGravity>(playerConfig_.GRAVITY);
    }

    void ScenePlay::spawnEnemy(const ConfigData& enemyConfig)
    {
        std::shared_ptr<Entity> enemyEntity = entityManager_.addEntity("enemy");

        // Engine2D::Animation& anim  = game_->assets().getAnimation(enemyConfig.AVATAR);
        // DryPhys::Vector3D animSize = anim.getSize();

        enemyEntity->addComponent<CAnimation>(game_->assets().getAnimation(enemyConfig.AVATAR + "Idle"));
        enemyEntity->addComponent<CTransform>(gridToMidPixel(enemyConfig.X, enemyConfig.Y, enemyEntity));

        float scale = gridSize_[0] / 50.0f;

        enemyEntity->getComponent<CTransform>().scale = DryPhys::Vector3D {scale, scale, 0};

        enemyEntity->addComponent<CBoundingBox>(DryPhys::Vector3D {25, 50, 0} * scale);   // Needs to shift down
        //enemyEntity->addComponent<CGravity>(enemyConfig.GRAVITY);
    }

    void ScenePlay::spawnBullet(std::shared_ptr<Entity>) {}

    void ScenePlay::update()
    {
        entityManager_.update();

        if (!paused_)
        {
            sMovement();
            sLifespan();
            sCollision();
            sAnimation();

            currentFrame_++;
        }
    }

    void ScenePlay::sMovement()
    {
        auto& pInput     = player_->getComponent<CInput>();
        auto& pTransform = player_->getComponent<CTransform>();

        // Maintain y velocity from previous frame so gravity acts as an acceleration
        pTransform.vel[0] = 0.0f;

        float horizontalMoveSpeed {playerConfig_.SPEED};

        if (pInput.inputs[CInput::SPRINT])
            horizontalMoveSpeed *= 1.5;

        if (pInput.inputs[CInput::LEFT])
        {
            if (pInput.facingRight)
            {
                pTransform.scale[0] *= -1.0f;
                pInput.facingRight = false;
            }

            if (pInput.canMove)
                pTransform.vel[0] = -horizontalMoveSpeed;
        }
        else if (pInput.inputs[CInput::RIGHT])
        {
            if (!pInput.facingRight)
            {
                pTransform.scale[0] *= -1.0f;
                pInput.facingRight = true;
            }

            if (pInput.canMove)
                pTransform.vel[0] = horizontalMoveSpeed;
        }

        if (pInput.canJump && pInput.inputs[CInput::JUMP])
        {
            pTransform.vel[1] += playerConfig_.JUMP;
        }

        player_->getComponent<CState>().state->handleInput(player_);

        for (std::shared_ptr<Entity> entity : entityManager_.getEntities())
        {
            auto& transform    = entity->getComponent<CTransform>();
            auto& [vx, vy, vz] = transform.vel;
            assert(vz == static_cast<DryPhys::real>(0));

            if (entity->hasComponent<CGravity>())
                vy += entity->getComponent<CGravity>().gravity;

            // Cap the entity's speed so it doesn't go infinite
            if (std::fabs(vx) > playerConfig_.MAXSPEED)
            {
                if (vx > 0)
                    vx = playerConfig_.MAXSPEED;
                else
                    vx = -playerConfig_.MAXSPEED;
            }

            if (std::fabs(vy) > playerConfig_.MAXSPEED)
            {
                if (vy > 0)
                    vy = playerConfig_.MAXSPEED;
                else
                    vy = -playerConfig_.MAXSPEED;
            }

            transform.prevPos = transform.pos;
            transform.pos += transform.vel;
        }
    }

    void ScenePlay::sLifespan()
    {
        for (std::shared_ptr<Entity> entity : entityManager_.getEntities())
        {
            if (entity->hasComponent<CLifespan>())
            {
                auto& lifespan  = entity->getComponent<CLifespan>();
                int framesAlive = static_cast<int>(currentFrame_) - lifespan.frameCreated;

                if (framesAlive >= lifespan.lifespan)
                {
                    entity->destroy();
                }
            }
        }
    }

    void ScenePlay::sCollision()
    {
        // TODO: Implement bullet / tile collisions
        //       Destroy the tile if it has a Brick animation
        // TODO: Implement player / tile collisions and resolutions
        //       Update the CState component of the player to store whether
        //       it is currently on the ground or in the air. This will be
        //       used by the Animation system

        auto& pTransform   = player_->getComponent<CTransform>();
        auto& pBoundingBox = player_->getComponent<CBoundingBox>();
        auto& pInput       = player_->getComponent<CInput>();

        auto& [px, py, pz]                = pTransform.pos;
        auto& [prev_px, prev_py, prev_pz] = pTransform.prevPos;

        // Nothing should have set the z component of these "2d"-vectors
        assert(pz == static_cast<DryPhys::real>(0) && prev_pz == static_cast<DryPhys::real>(0));

        if (py > height())
            spawnPlayer();

        if (px < pBoundingBox.halfSize[0])
            px = pBoundingBox.halfSize[0];

        for (std::shared_ptr<Entity> tile : entityManager_.getEntities("tile"))
        {
            DryPhys::Vector3D overlap = Physics::GetOverlap(tile, player_);

            if (overlap[0] > 0 && overlap[1] > 0)
            {
                // Overlap Detected...
                DryPhys::Vector3D prevOverlap = Physics::GetPreviousOverlap(tile, player_);

                if (prevOverlap[0] > 0)
                {
                    if (py > prev_py)
                    {
                        // Collision from above
                        py -= overlap[1];
                        pTransform.vel[1] = 0.0f;

                        // Floor collisions determine whether or not we can jump
                        pInput.canJump = true;
                    }
                    else if (py < prev_py)
                    {
                        // Collision from below
                        py += overlap[1];
                    }
                }

                if (prevOverlap[1] > 0)
                {
                    if (px > prev_px)
                    {
                        // Collision from left
                        px -= overlap[0];
                    }
                    else if (px < prev_px)
                    {
                        // Collision from right
                        px += overlap[0];
                    }
                }
            }
        }
    }

    void ScenePlay::sDoAction(const Engine2D::Action& action)
    {
        if (action.type() == "START")
        {
            if (action.name() == "TOGGLE_TEXTURE")
            {
                drawTextures_ = !drawTextures_;
            }
            else if (action.name() == "TOGGLE_COLLISIONS")
            {
                drawCollisions_ = !drawCollisions_;
            }
            else if (action.name() == "TOGGLE_GRID")
            {
                drawGrid_ = !drawGrid_;
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

        player_->getComponent<CState>().state->handleActions(player_, action);
    }

    void ScenePlay::sAnimation()
    {
        auto pState = player_->getComponent<CState>().state;
        pState->handleAnimations(player_, game_->assets(), playerConfig_.AVATAR);

        for (std::shared_ptr<Entity> entity : entityManager_.getEntities())
        {
            if (!entity->hasComponent<CAnimation>())
                continue;

            auto& anim = entity->getComponent<CAnimation>();

            if (anim.animation.hasEnded() && !anim.repeat)
                entity->destroy();
            else
                anim.animation.update();
        }
    }

    void ScenePlay::sRender()
    {
        // color the background darker so you know that the game is paused
        if (!paused_)
        {
            game_->window().clear(sf::Color(100, 100, 255));
        }
        else
        {
            game_->window().clear(sf::Color(50, 50, 150));

            pauseText_.setString("Paused");

            sf::FloatRect textBoundingBox = pauseText_.getGlobalBounds();
            pauseText_.setPosition(0.5f * (width() - textBoundingBox.width), 0.5f * (height() - textBoundingBox.height));

            game_->window().draw(pauseText_);
        }

        // set the viewport of the window to be centered on the player if it's far enough right
        auto& pPos          = player_->getComponent<CTransform>().pos;
        float windowCenterX = std::max(game_->window().getSize().x / 2.0f, pPos[0]);
        sf::View view       = game_->window().getView();
        view.setCenter(windowCenterX, game_->window().getSize().y - view.getCenter().y);
        game_->window().setView(view);

        for (auto& [key, value] : envViews)
        {
            value.setCenter(view.getCenter().x + windowCenterX / key.length(), view.getCenter().y);
        }

        // draw all Entity textures / animations
        if (drawTextures_)
        {
            for (std::shared_ptr<Entity> e : entityManager_.getEntities())
            {
                auto& transform = e->getComponent<CTransform>();

                if (e->hasComponent<CAnimation>())
                {
                    auto& animation = e->getComponent<CAnimation>().animation;

                    if (animation.getName() == "Back")
                    {
                        game_->window().setView(envViews[animation.getName()]);
                        envViews[animation.getName()] = view;
                    }
                    else if (animation.getName() == "Middle")
                    {
                        game_->window().setView(envViews[animation.getName()]);
                        envViews[animation.getName()] = view;
                    }
                    else if (animation.getName() == "Front")
                    {
                        game_->window().setView(envViews[animation.getName()]);
                        envViews[animation.getName()] = view;
                    }
                    else
                    {
                        animation.getSprite().setRotation(transform.angle);
                        animation.getSprite().setPosition(transform.pos[0], transform.pos[1]);
                        animation.getSprite().setScale(transform.scale[0], transform.scale[1]);

                        game_->window().setView(view);
                    }

                    game_->window().draw(animation.getSprite());
                }
            }
        }

        // draw all Entity collision bounding boxes with a rectangle shape
        if (drawCollisions_)
        {
            for (std::shared_ptr<Entity> e : entityManager_.getEntities())
            {
                if (e->hasComponent<CBoundingBox>())
                {
                    auto& box       = e->getComponent<CBoundingBox>();
                    auto& transform = e->getComponent<CTransform>();

                    sf::RectangleShape rect;
                    rect.setSize(sf::Vector2f(box.size[0] - 1, box.size[1] - 1));
                    rect.setOrigin(sf::Vector2f(box.halfSize[0], box.halfSize[1]));
                    rect.setPosition(transform.pos[0], transform.pos[1]);
                    rect.setFillColor(sf::Color(0, 0, 0, 0));
                    rect.setOutlineColor(sf::Color(255, 255, 255, 255));
                    rect.setOutlineThickness(1);

                    game_->window().draw(rect);
                }
            }
        }

        // draw the grid for easy debugging
        if (drawGrid_)
        {
            float leftX     = game_->window().getView().getCenter().x - width() / 2;
            float rightX    = leftX + width() + gridSize_[0];
            float nextGridX = leftX - ((int)leftX % (int)gridSize_[0]);

            for (float x = nextGridX; x < rightX; x += gridSize_[0])
            {
                drawLine(DryPhys::Vector3D(x, 0.0f, 0), DryPhys::Vector3D(x, static_cast<float>(height()), 0));
            }

            for (float y = 0; y < height(); y += gridSize_[1])
            {
                drawLine(DryPhys::Vector3D(leftX, height() - y, 0), DryPhys::Vector3D(rightX, height() - y, 0));

                for (float x = nextGridX; x < rightX; x += gridSize_[0])
                {
                    std::string xCell = std::to_string((int)x / (int)gridSize_[0]);
                    std::string yCell = std::to_string((int)y / (int)gridSize_[1]);

                    gridText_.setString("(" + xCell + "," + yCell + ")");
                    gridText_.setPosition(x + 3, height() - y - gridSize_[1] + 2);

                    game_->window().draw(gridText_);
                }
            }
        }
    }

    void ScenePlay::onEnd()
    {
        music_->stop();
        game_->changeScene("MENU", std::make_shared<SceneMenu>(game_));
    }
}   // namespace CyberCity
