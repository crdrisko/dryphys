// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scenePlay.cpp
// Author: crdrisko
// Date: 06/19/2024-08:53:49
// Description:

#include "cybercity/scenes/scenePlay.hpp"

#ifdef USE_IMGUI
    #include <imgui-SFML.h>
    #include <imgui.h>
#endif

#include <algorithm>
#include <cassert>
#include <iostream>

#include <common-utils/files.hpp>
#include <common-utils/strings.hpp>
#include <dryphys/math.hpp>
#include <dryphys/utilities.hpp>
#include <engine2d/action.hpp>
#include <engine2d/animation.hpp>
#include <engine2d/engine.hpp>
#include <engine2d/physics.hpp>
#include <engine2d/scene.hpp>

#include "cybercity/components.hpp"
#include "cybercity/forwardDeclare.hpp"
#include "cybercity/scenes/sceneMenu.hpp"

using namespace DryPhys::Literals;

namespace CyberCity
{
    ScenePlay::ScenePlay(Engine2D::Engine* gameEngine, const std::string& levelPath)
        : Engine2D::Scene {gameEngine}, levelPath_ {levelPath}
    {
        auto windowSize = game_->window().getSize();

        width_  = static_cast<float>(windowSize.x);
        height_ = static_cast<float>(windowSize.y);

        init();
    }

    void ScenePlay::init()
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
        registerAction(sf::Keyboard::Up, "JUMP");
        registerAction(sf::Keyboard::Down, "DEFEND");
        registerAction(sf::Keyboard::Left, "LEFT");
        registerAction(sf::Keyboard::Right, "RIGHT");
        registerAction(sf::Keyboard::Space, "EVADE");
        registerAction(sf::Keyboard::LShift, "SPRINT");
        registerAction(sf::Keyboard::Enter, "ATTACK");

        // Set display text parameters
        pauseText_.setCharacterSize(24);
        pauseText_.setFont(game_->assets().getFont("VinerHand"));

        gridText_.setCharacterSize(12);
        gridText_.setFont(game_->assets().getFont("Arial"));

        loadLevel(levelPath_);
    }

    void ScenePlay::loadLevel(const std::string& filename)
    {
        // Reset the entity manager every time we load a level
        entityManager_ = ConcreteEntityManager();

        DryChem::FileParser parser {filename};
        auto rows = parser.parseDataFile(DryChem::AsRows());

        for (const auto& row : rows)
        {
            DryChem::Tokenizer tok {row};
            std::vector<std::string> splitRow = tok.split();

            switch (DryPhys::djb2Hash(splitRow[0].c_str()))
            {
            case "Camera"_sID:
            {
                cameraBounds_.leftExtreme   = width_ * std::stod(splitRow[2]);
                cameraBounds_.topExtreme    = height_ * std::stod(splitRow[3]);
                cameraBounds_.rightExtreme  = width_ * std::stod(splitRow[4]);
                cameraBounds_.bottomExtreme = height_ * std::stod(splitRow[5]);
                break;
            }
            case "Env"_sID:
            {
                /*
                 * Env N X Y
                 *  Animation Name:     N       std::string (Animation asset name for this environment)
                 *  World X Pos:        X       float
                 *  World Y Pos:        Y       float
                 *  Scale:              S       float
                 *  Scrolling Speed:    SX      float
                 */
                auto envEntity = entityManager_.addEntity("env");

                auto& envAnim = envEntity.addComponent<CAnimation>(game_->assets().getAnimation(splitRow[1]));
                envEntity.addComponent<CCamera>(std::stoi(splitRow[5]), game_->window().getDefaultView());

                auto size = envAnim.animation.getSize();

                int multiplier {static_cast<int>(width_) * 2};
                if (splitRow[2] != "0")
                    multiplier = size.x;

                const sf::IntRect sizes {0, 0, multiplier, static_cast<int>(size.y)};

                float scaleX = width_ / size.x / std::stoi(splitRow[4]);
                float scaleY = height_ * ((cameraBounds_.bottomExtreme - cameraBounds_.topExtreme) / height_ + 1) / size.y;

                sf::Sprite& sprite = envAnim.animation.getSprite();

                sprite.setPosition(std::stof(splitRow[2]), std::stof(splitRow[3]));
                sprite.setScale(scaleX, scaleY);
                sprite.setTextureRect(sizes);
                break;
            }
            case "Tile"_sID:
            {
                /*
                 * Tile N GX GY
                 *  Animation Name:     N       std::string (Animation asset name for this tile)
                 *  Grid X Pos:         GX      float
                 *  Grid Y Pos:         GY      float
                 */
                auto tileEntity = entityManager_.addEntity("tile");

                Engine2D::Animation& anim  = game_->assets().getAnimation(splitRow[1]);
                DryPhys::Vector3D animSize = anim.getSize();

                float scale    = gridSize_ / std::min(animSize.x, animSize.y);
                auto animScale = anim.getSprite().getScale();

                tileEntity.addComponent<CAnimation>(anim);
                tileEntity.addComponent<CTransform>(
                    gridToMidPixel(std::stof(splitRow[2]), std::stof(splitRow[3]), tileEntity),
                    DryPhys::Vector3D {},
                    DryPhys::Vector3D {scale * animScale.x, scale * animScale.y, 0},
                    anim.getSprite().getRotation());
                tileEntity.addComponent<CDraggable>();
                tileEntity.addComponent<CBoundingBox>(game_->assets().getAnimation(splitRow[1]).getSize() * scale);
                break;
            }
            case "Dec"_sID:
            {
                /*
                 * Dec N GX GY
                 *  Animation Name:     N       std::string (Animation asset name for this decoration)
                 *  Grid X Pos:         GX      float
                 *  Grid Y Pos:         GY      float
                 */
                auto decEntity = entityManager_.addEntity("dec");

                decEntity.addComponent<CAnimation>(game_->assets().getAnimation(splitRow[1]));
                decEntity.addComponent<CTransform>(
                    gridToMidPixel(std::stof(splitRow[2]), std::stof(splitRow[3]), decEntity));
                break;
            }
            case "Player"_sID:
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
                break;
            }
            case "Enemy"_sID:
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
                break;
            }
            case "Vehicle"_sID:
            {
                AIFactory factory {"Drive"};

                auto vehicleEntity = entityManager_.addEntity("vehicle");

                Engine2D::Animation& anim  = game_->assets().getAnimation(splitRow[1]);
                DryPhys::Vector3D animSize = anim.getSize();

                float scale    = gridSize_ / std::min(animSize.x, animSize.y);
                auto animScale = anim.getSprite().getScale();

                if (splitRow[2] == "DriveRight")
                    animScale.x *= -1;

                vehicleEntity.addComponent<CAnimation>(anim);
                vehicleEntity.addComponent<CTransform>(
                    gridToMidPixel(std::stof(splitRow[3]), std::stof(splitRow[4]), vehicleEntity),
                    DryPhys::Vector3D {},
                    DryPhys::Vector3D {scale * animScale.x, scale * animScale.y, 0},
                    anim.getSprite().getRotation());

                vehicleEntity.addComponent<CAI>(factory.create());
                vehicleEntity.addComponent<CBoundingBox>(animSize * scale);
                break;
            }
            case "Music"_sID:
            {
                music_ = game_->assets().getMusic(splitRow[1]);

                music_->setVolume(std::stof(splitRow[2]));
                music_->play();
                break;
            }
            default:
                break;
            }
        }

        spawnPlayer();
    }

    void ScenePlay::update()
    {
        // These systems should execute once every update
        entityManager_.update();

        if (!paused_)
        {
            sAI();
            sMovement();
            sLifespan();
            sCollision();
            sDragAndDrop();

            currentFrame_++;
        }
    }

    void ScenePlay::postUpdate()
    {
        // These systems should execute once every render
        sAnimation();
        sGui();
        sCamera();
    }

    void ScenePlay::doActionImpl(const Engine2D::Action& action)
    {
        if (auto atype = action.type(); atype == Engine2D::Action::START)
        {
            switch (action.sid())
            {
            case "TOGGLE_TEXTURE"_sID:
                drawTextures_ = !drawTextures_;
                break;
            case "TOGGLE_COLLISIONS"_sID:
                drawCollisions_ = !drawCollisions_;
                break;
            case "TOGGLE_GRID"_sID:
                drawGrid_ = !drawGrid_;
                break;
            case "PAUSE"_sID:
                setPaused(!paused_);
                break;
            case "QUIT"_sID:
                onEnd();
                break;
            case "ATTACK"_sID:
                spawnBullet(*player_);
                break;
            case "LEFT_CLICK"_sID:
            {
                auto worldPos = windowToWorld(action.pos());

                for (auto& entity : entityManager_.getEntities())
                {
                    if (entity.hasComponent<CDraggable>() && isInside(worldPos, entity))
                    {
                        auto& drag = entity.getComponent<CDraggable>().dragging;

                        drag = !drag;
                        break;
                    }
                }
                break;
            }
            case "MOUSE_MOVE"_sID:
                mPos_ = action.pos();
                break;
            default:
                break;
            }
        }

        player_->getComponent<CState>().state->handleActions(player_, action);
    }

    void ScenePlay::render()
    {
        if (!paused_)
        {
            game_->window().clear(sf::Color(100, 100, 255));
        }
        else
        {
            game_->window().clear(sf::Color(50, 50, 150));

            pauseText_.setString("Paused");

            sf::FloatRect textBoundingBox = pauseText_.getGlobalBounds();
            pauseText_.setPosition(0.5f * (width_ - textBoundingBox.width), 0.5f * (height_ - textBoundingBox.height));

            game_->window().draw(pauseText_);
        }

        sf::View view = game_->window().getView();

        if (drawTextures_)
        {
            for (auto& entity : entityManager_.getEntities("env"))
            {
                auto& animation  = entity.getComponent<CAnimation>().animation;
                auto& cameraView = entity.getComponent<CCamera>().view;

                game_->window().setView(cameraView);
                cameraView = view;

                game_->window().draw(animation.getSprite());
            }

            game_->window().setView(view);

            drawEntityAnimations("tile");
            drawEntityAnimations("dec");
            drawEntityAnimations("bullet");
            drawEntityAnimations("enemy");
            drawEntityAnimations("vehicle");
            drawEntityAnimations("player");
        }

        if (drawCollisions_)
        {
            for (auto& entity : entityManager_.getEntities())
            {
                if (entity.hasComponent<CBoundingBox>())
                {
                    auto& box       = entity.getComponent<CBoundingBox>();
                    auto& transform = entity.getComponent<CTransform>();

                    sf::RectangleShape rect;
                    rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                    rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                    rect.setPosition(transform.pos.x, transform.pos.y);
                    rect.setFillColor(sf::Color(0, 0, 0, 0));
                    rect.setOutlineColor(sf::Color(255, 255, 255, 255));
                    rect.setOutlineThickness(1);

                    game_->window().draw(rect);
                }
            }
        }

        if (drawGrid_)
        {
            float leftX     = view.getCenter().x - cameraBounds_.leftExtreme;
            float rightX    = leftX + cameraBounds_.rightExtreme + gridSize_;
            float nextGridX = leftX - ((int)leftX % (int)gridSize_);

            int nScreens {3};
            int verticalDistance {static_cast<int>((cameraBounds_.bottomExtreme - cameraBounds_.topExtreme) / height_) + 1};

            float bottomY = nScreens * height_ / verticalDistance;
            float topY    = -height_ / verticalDistance;

            for (float x = nextGridX; x < rightX; x += gridSize_)
            {
                drawLine(x, topY, x, bottomY, sf::Color::White);
            }

            for (float y = topY; y < bottomY; y += gridSize_)
            {
                drawLine(leftX, height_ - y, rightX, height_ - y, sf::Color::White);

                for (float x = nextGridX; x < rightX; x += gridSize_)
                {
                    std::string xCell = std::to_string((int)x / (int)gridSize_);
                    std::string yCell = std::to_string((int)(y - topY) / (int)gridSize_);

                    gridText_.setString("(" + xCell + "," + yCell + ")");
                    gridText_.setPosition(x + 3, height_ - y - gridSize_ + 2);

                    game_->window().draw(gridText_);
                }
            }
        }

        // draw the mouse cursor
        auto world_mPos = windowToWorld(mPos_);

        mouseShape_.setFillColor(sf::Color::Red);
        mouseShape_.setRadius(4);
        mouseShape_.setOrigin(2, 2);
        mouseShape_.setPosition(world_mPos.x, world_mPos.y);

        game_->window().draw(mouseShape_);

        if (drawCamera_)
        {
            drawLine(cameraBounds_.leftExtreme,
                cameraBounds_.topExtreme,
                cameraBounds_.leftExtreme,
                cameraBounds_.bottomExtreme,
                sf::Color::Red);
            drawLine(cameraBounds_.rightExtreme,
                cameraBounds_.topExtreme,
                cameraBounds_.rightExtreme,
                cameraBounds_.bottomExtreme,
                sf::Color::Red);

            drawLine(cameraBounds_.leftExtreme,
                cameraBounds_.topExtreme,
                cameraBounds_.rightExtreme,
                cameraBounds_.topExtreme,
                sf::Color::Red);
            drawLine(cameraBounds_.leftExtreme,
                cameraBounds_.bottomExtreme,
                cameraBounds_.rightExtreme,
                cameraBounds_.bottomExtreme,
                sf::Color::Red);

            // Camera center crosshair
            auto viewCenter = view.getCenter();

            drawLine(viewCenter.x, viewCenter.y - 10 / 2, viewCenter.x, viewCenter.y + 10 / 2, sf::Color::Red);
            drawLine(viewCenter.x - 10 / 2, viewCenter.y, viewCenter.x + 10 / 2, viewCenter.y, sf::Color::Red);
        }
    }

    void ScenePlay::onEnd()
    {
        if (music_)
            music_->stop();

        entityManager_.clearEntities();

        game_->changeScene("MENU", std::make_shared<SceneMenu>(game_, levelPath_));
    }

    void ScenePlay::sAI()
    {
        for (auto& entity : entityManager_.getEntities("vehicle"))
        {
            auto& eTrans = entity.getComponent<CTransform>();

            eTrans.vel = DryPhys::Vector3D {-2.0f, 0, 0};
        }
    }

    void ScenePlay::sMovement()
    {
        player_->getComponent<CState>().state->handleInput(player_);

        for (auto& entity : entityManager_.getEntities())
        {
            auto& transform    = entity.getComponent<CTransform>();
            auto& [vx, vy, vz] = transform.vel;
            assert(vz == static_cast<DryPhys::real>(0));

            if (entity.hasComponent<CGravity>())
                vy += entity.getComponent<CGravity>().gravity;

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
        for (auto& entity : entityManager_.getEntities())
        {
            if (entity.hasComponent<CLifespan>())
            {
                auto& lifespan  = entity.getComponent<CLifespan>();
                int framesAlive = static_cast<int>(currentFrame_) - lifespan.frameCreated;

                if (framesAlive >= lifespan.lifespan)
                {
                    entity.destroy();
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

        // Nothing should have set the z-component of these "2d"-vectors
        assert(pz == static_cast<DryPhys::real>(0) && prev_pz == static_cast<DryPhys::real>(0));

        if (py > height_ * 2)
            spawnPlayer();

        if (px < pBoundingBox.halfSize.x)
            px = pBoundingBox.halfSize.x;

        bool playerCollision {};

        for (auto tile : entityManager_.getEntities("tile"))
        {
            auto& tileTransform   = tile.getComponent<CTransform>();
            auto& tileBoundingBox = tile.getComponent<CBoundingBox>();

            DryPhys::Vector3D overlap = Engine2D::getAABBOverlap(
                tileTransform.pos, tileBoundingBox.halfSize, pTransform.pos, pBoundingBox.halfSize);

            if (overlap.x > 0 && overlap.y > 0)
            {
                // Overlap Detected...
                DryPhys::Vector3D prevOverlap = Engine2D::getAABBOverlap(
                    tileTransform.prevPos, tileBoundingBox.halfSize, pTransform.prevPos, pBoundingBox.halfSize);

                if (prevOverlap.x > 0)
                {
                    if (py > prev_py)
                    {
                        // Collision from above
                        py -= overlap.y;
                        pTransform.vel.y = 0.0f;

                        // Floor collisions determine whether or not we can jump
                        pInput.canJump  = true;
                        playerCollision = true;
                    }
                    else if (py < prev_py)
                    {
                        // Collision from below
                        py += overlap.y;
                    }
                }

                if (prevOverlap.y > 0)
                {
                    if (px > prev_px)
                    {
                        // Collision from left
                        px -= overlap.x;
                    }
                    else if (px < prev_px)
                    {
                        // Collision from right
                        px += overlap.x;
                    }
                }
            }

            for (auto& enemy : entityManager_.getEntities("enemy"))
            {
                auto& eTransform   = enemy.getComponent<CTransform>();
                auto& eBoundingBox = enemy.getComponent<CBoundingBox>();

                auto& [ex, ey, ez]                = eTransform.pos;
                auto& [prev_ex, prev_ey, prev_ez] = eTransform.prevPos;

                if (ex < eBoundingBox.halfSize.x)
                    ex = eBoundingBox.halfSize.x;

                // Nothing should have set the z component of these "2d"-vectors
                assert(ez == static_cast<DryPhys::real>(0) && prev_ez == static_cast<DryPhys::real>(0));

                DryPhys::Vector3D overlap = Engine2D::getAABBOverlap(
                    tileTransform.pos, tileBoundingBox.halfSize, eTransform.pos, eBoundingBox.halfSize);

                if (overlap.x > 0 && overlap.y > 0)
                {
                    // Overlap Detected...
                    DryPhys::Vector3D prevOverlap = Engine2D::getAABBOverlap(
                        tileTransform.prevPos, tileBoundingBox.halfSize, eTransform.prevPos, eBoundingBox.halfSize);

                    if (prevOverlap.x > 0)
                    {
                        if (ey > prev_ey)
                        {
                            // Collision from above
                            ey -= overlap.y;
                            eTransform.vel.y = 0.0f;
                        }
                        else if (py < prev_py)
                        {
                            // Collision from below
                            ey += overlap.y;
                        }
                    }

                    if (prevOverlap.y > 0)
                    {
                        if (ex > prev_ex)
                        {
                            // Collision from left
                            ex -= overlap.x;
                        }
                        else if (px < prev_px)
                        {
                            // Collision from right
                            ex += overlap.x;
                        }
                    }
                }
            }

            for (auto& bullet : entityManager_.getEntities("bullet"))
            {
                auto& bTransform   = bullet.getComponent<CTransform>();
                auto& bBoundingBox = bullet.getComponent<CBoundingBox>();

                DryPhys::Vector3D overlap = Engine2D::getAABBOverlap(
                    tileTransform.pos, tileBoundingBox.halfSize, bTransform.pos, bBoundingBox.halfSize);

                if (overlap.x > 0 && overlap.y > 0)
                {
                    spawnExplosion(tile);
                    bullet.destroy();
                }
            }
        }

        pInput.inputs[CInput::FALL] = !playerCollision;

        for (auto& enemy : entityManager_.getEntities("enemy"))
        {
            auto& eTransform   = enemy.getComponent<CTransform>();
            auto& eBoundingBox = enemy.getComponent<CBoundingBox>();

            for (auto bullet : entityManager_.getEntities("bullet"))
            {
                auto& bTransform   = bullet.getComponent<CTransform>();
                auto& bBoundingBox = bullet.getComponent<CBoundingBox>();

                DryPhys::Vector3D overlap
                    = Engine2D::getAABBOverlap(eTransform.pos, eBoundingBox.halfSize, bTransform.pos, bBoundingBox.halfSize);

                if (overlap.x > 0 && overlap.y > 0)
                {
                    spawnExplosion(enemy);
                    bullet.destroy();
                    enemy.destroy();
                }
            }
        }
    }

    void ScenePlay::sAnimation()
    {
        auto pState = player_->getComponent<CState>().state;
        pState->handleAnimations(player_, game_->assets(), playerConfig_.AVATAR);

        for (auto& entity : entityManager_.getEntities())
        {
            if (!entity.hasComponent<CAnimation>())
                continue;

            auto& anim = entity.getComponent<CAnimation>();

            if (anim.animation.hasEnded() && !anim.repeat)
                entity.destroy();
            else if (entity.tag() != "env")
                anim.animation.update();
        }
    }

    void ScenePlay::sDragAndDrop()
    {
        for (auto& entity : entityManager_.getEntities())
        {
            if (entity.hasComponent<CDraggable>() && entity.getComponent<CDraggable>().dragging)
            {
                entity.getComponent<CTransform>().pos = windowToWorld(mPos_);
            }
        }
    }

    void ScenePlay::sGui()
    {
#ifdef USE_IMGUI
        // ImGui::ShowDemoWindow();

        ImGui::Begin("Scene Properties");

        if (ImGui::BeginTabBar(""))
        {
            if (ImGui::BeginTabItem("Debug"))
            {
                ImGui::Checkbox("Draw Grid", &drawGrid_);
                ImGui::Checkbox("Draw Textures", &drawTextures_);
                ImGui::Checkbox("Draw Collisions", &drawCollisions_);
                ImGui::Checkbox("Draw Camera", &drawCamera_);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Player Selection"))
            {
                sf::Vector2f size {64.0f, 64.0f};

                if (ImGui::ImageButton(game_->assets().getAnimation("Player1Profile").getSprite(), size))
                {
                    playerConfig_.AVATAR = "Player1";
                    spawnPlayer();
                }

                if (ImGui::ImageButton(game_->assets().getAnimation("Player2Profile").getSprite(), size))
                {
                    playerConfig_.AVATAR = "Player2";
                    spawnPlayer();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Music"))
            {
                static int songChoice {};
                std::vector<std::string> songs {};

                int count {};

                for (const auto& [key, song] : game_->assets().getMusicMap())
                {
                    songs.push_back(key);

                    if (count++ % 5)
                        ImGui::SameLine();

                    ImGui::PushID(count);
                    if (ImGui::RadioButton(key.c_str(), &songChoice, count))
                    {
                        if (music_)
                            music_->stop();

                        music_ = game_->assets().getMusic(songs[songChoice - 1]);
                        music_->play();
                    }
                    ImGui::PopID();
                }

                static int volume {20};
                ImGui::NewLine();
                ImGui::SliderInt("Volume", &volume, 0, 100);

                if (music_)
                    music_->setVolume(volume);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Animations"))
            {
                sf::Vector2f size {32.0f, 32.0f};

                int count {};

                ImGui::Text("Click on an animation to add it to the game world.");

                for (const auto& [key, anim] : game_->assets().getAnimations())
                {
                    if (DryChem::foundSubstr("Player", key))
                        continue;

                    if (count++ % 10)
                        ImGui::SameLine();

                    ImGui::PushID(count);

                    if (ImGui::ImageButton(anim.getSprite(), size))
                    {
                        spawnDecorations(key);
                    }

                    ImGui::PopID();

                    if (ImGui::BeginItemTooltip())
                    {
                        ImGui::Text("%s", key.c_str());
                        ImGui::EndTooltip();
                    }
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Entity Manager"))
            {
                ImGui::Text("Click on an entity to delete it.");

                if (ImGui::CollapsingHeader("Entitites by Tag"))
                {
                    if (ImGui::TreeNode("Bullets"))
                    {
                        for (auto entity : entityManager_.getEntities("bullet"))
                            generateGuiInformation(entity);

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Decorations"))
                    {
                        for (auto entity : entityManager_.getEntities("dec"))
                            generateGuiInformation(entity);

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Enemies"))
                    {
                        for (auto entity : entityManager_.getEntities("enemy"))
                            generateGuiInformation(entity);

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Environment"))
                    {
                        for (auto entity : entityManager_.getEntities("env"))
                            generateGuiInformation(entity);
                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Players"))
                    {
                        for (auto entity : entityManager_.getEntities("player"))
                            generateGuiInformation(entity);

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Tiles"))
                    {
                        for (auto entity : entityManager_.getEntities("tile"))
                            generateGuiInformation(entity);

                        ImGui::TreePop();
                    }
                }

                if (ImGui::CollapsingHeader("All Entities"))
                {
                    for (auto entity : entityManager_.getEntities())
                        generateGuiInformation(entity);
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
#endif
    }

    void ScenePlay::sCamera()
    {
        sf::View view   = game_->window().getView();
        auto viewCenter = view.getCenter();

        auto& pTrans = player_->getComponent<CTransform>();

        float windowCenterX = std::clamp(pTrans.pos.x, cameraBounds_.leftExtreme, cameraBounds_.rightExtreme);
        float windowCenterY = std::clamp(pTrans.pos.y, cameraBounds_.topExtreme, cameraBounds_.bottomExtreme);

        view.setCenter(windowCenterX, windowCenterY);
        game_->window().setView(view);

        for (auto& entity : entityManager_.getEntities("env"))
        {
            auto& eCam = entity.getComponent<CCamera>();
            eCam.view.setCenter(viewCenter.x + windowCenterX / eCam.scrollingSpeed, viewCenter.y);
        }
    }

    void ScenePlay::spawnPlayer()
    {
        if (!player_)
            player_ = std::make_shared<ConcreteEntity>(entityManager_.addEntity("player"));

        auto& pAnim  = player_->addComponent<CAnimation>(game_->assets().getAnimation(playerConfig_.AVATAR + "Idle"));
        auto& pTrans = player_->addComponent<CTransform>(gridToMidPixel(playerConfig_.X, playerConfig_.Y, *player_));

        DryPhys::Vector3D animSize = pAnim.animation.getSize();

        float scale;

        if (animSize.x < gridSize_ && animSize.y < gridSize_)
        {
            scale        = gridSize_ / std::min(animSize.x, animSize.y);
            pTrans.scale = DryPhys::Vector3D {scale, scale, 0};
        }
        else
        {
            scale        = gridSize_ * 1.5f / std::min(animSize.x, animSize.y);
            pTrans.scale = DryPhys::Vector3D {scale, scale, 0};
        }

        player_->addComponent<CBoundingBox>(DryPhys::Vector3D {25, 50, 0} * scale);   // Needs to shift down
        player_->addComponent<CInput>();
        player_->addComponent<CState>();
        player_->addComponent<CGravity>(playerConfig_.GRAVITY);
    }

    void ScenePlay::spawnEnemy(const ConfigData& enemyConfig)
    {
        AIFactory factory {"Patrol"};

        auto enemyEntity = entityManager_.addEntity("enemy");

        enemyEntity.addComponent<CAnimation>(game_->assets().getAnimation(enemyConfig.AVATAR + "Idle"));
        auto& enemyTrans = enemyEntity.addComponent<CTransform>(gridToMidPixel(enemyConfig.X, enemyConfig.Y, enemyEntity));

        float scale      = gridSize_ / 50.0f;
        enemyTrans.scale = DryPhys::Vector3D {scale, scale, 0};

        enemyEntity.addComponent<CBoundingBox>(DryPhys::Vector3D {25, 50, 0} * scale);   // Needs to shift down
        enemyEntity.addComponent<CGravity>(enemyConfig.GRAVITY);
        enemyEntity.addComponent<CAI>(factory.create());
    }

    void ScenePlay::spawnDecorations(const std::string& name)
    {
        auto tileEntity = entityManager_.addEntity("dec");

        Engine2D::Animation anim   = game_->assets().getAnimation(name);
        DryPhys::Vector3D animSize = anim.getSize();

        float scale = gridSize_ / std::min(animSize.x, animSize.y);

        tileEntity.addComponent<CAnimation>(game_->assets().getAnimation(name));
        tileEntity.addComponent<CTransform>(
            gridToMidPixel(mPos_.x, mPos_.y, tileEntity), DryPhys::Vector3D {}, DryPhys::Vector3D {scale, scale, 0}, 0.0f);
        tileEntity.addComponent<CDraggable>(true);
    }

    void ScenePlay::spawnBullet(ConcreteEntity entity)
    {
        auto& eTrans = entity.getComponent<CTransform>();

        auto bulletEntity = entityManager_.addEntity("bullet");

        float scale = std::copysign(1.0f, eTrans.scale.x);

        auto& anim = bulletEntity.addComponent<CAnimation>(game_->assets().getAnimation(playerConfig_.WEAPON));

        bulletEntity.addComponent<CTransform>(DryPhys::Vector3D {eTrans.pos.x + (scale * 20), eTrans.pos.y - 7, 0},
            DryPhys::Vector3D {scale * 10, 0, 0},
            DryPhys::Vector3D {scale, 1, 0},
            0);
        bulletEntity.addComponent<CBoundingBox>(anim.animation.getSize());
        bulletEntity.addComponent<CLifespan>(100, currentFrame_);
    }

    void ScenePlay::spawnExplosion(ConcreteEntity entity)
    {
        auto& eTrans = entity.getComponent<CTransform>();
        auto& eAnim  = entity.getComponent<CAnimation>().animation;

        auto explosionEntity = entityManager_.addEntity("dec");

        std::string explosion {(entity.tag() == "enemy") ? "EnemyExplosion" : "ShotHit"};
        auto& anim = explosionEntity.addComponent<CAnimation>(game_->assets().getAnimation(explosion), false);

        auto& animSize  = anim.animation.getSize();
        auto& eAnimSize = eAnim.getSize();

        float maxSize = std::min(eAnimSize.x, eAnimSize.y);

        float scaleX = (maxSize / animSize.x);
        float scaleY = maxSize / animSize.y;

        explosionEntity.addComponent<CTransform>(DryPhys::Vector3D {eTrans.pos.x, eTrans.pos.y, 0},
            DryPhys::Vector3D {},
            DryPhys::Vector3D {scaleX, scaleY, 0},
            0);
    }

    DryPhys::Vector3D ScenePlay::gridToMidPixel(float gridX, float gridY, ConcreteEntity entity) const
    {
        int nScreens {3};

        DryPhys::Vector3D result {gridX * gridSize_,
            nScreens * height_ / ((cameraBounds_.bottomExtreme - cameraBounds_.topExtreme) / height_ + 1)
                - (gridY * gridSize_),
            0};
        DryPhys::Vector3D size {entity.getComponent<CAnimation>().animation.getSize()};

        float scale = gridSize_ / std::min(size.x, size.y);

        result.x += size.x * scale / 2.0f;
        result.y -= size.y * scale / 2.0f;

        return result;
    }

    DryPhys::Vector3D ScenePlay::windowToWorld(const DryPhys::Vector3D& window) const
    {
        auto view = game_->window().getView();

        float worldX = view.getCenter().x - width_ / 2.0f;
        float worldY = view.getCenter().y - height_ / 2.0f;

        return DryPhys::Vector3D {window.x + worldX, window.y + worldY, 0};
    }

    bool ScenePlay::isInside(const DryPhys::Vector3D& pos, ConcreteEntity entity) const
    {
        auto ePos = entity.getComponent<CTransform>().pos;
        auto size = entity.getComponent<CAnimation>().animation.getSize();

        float dx = std::fabs(pos.x - ePos.x);
        float dy = std::fabs(pos.y - ePos.y);

        return (dx <= size.x / 2.0f) && (dy <= size.y / 2.0f);
    }

    void ScenePlay::drawEntityAnimations(const std::string& tag)
    {
        for (auto entity : entityManager_.getEntities(tag))
        {
            auto& transform = entity.getComponent<CTransform>();

            if (entity.hasComponent<CAnimation>())
            {
                auto& animation = entity.getComponent<CAnimation>().animation;

                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);

                game_->window().draw(animation.getSprite());
            }
        }
    }

    void ScenePlay::generateGuiInformation(ConcreteEntity& entity)
    {
        sf::Vector2f size {24.0f, 24.0f};

        auto ePos  = entity.getComponent<CTransform>().pos;
        auto eAnim = entity.getComponent<CAnimation>().animation;

        std::stringstream ss;

        std::string str {
            " (" + std::to_string(static_cast<int>(ePos.x)) + ", " + std::to_string(static_cast<int>(ePos.y)) + ")"};

        ss << std::setw(6) << entity.id() << std::setw(10) << entity.tag() << std::setw(20) << eAnim.getName()
           << std::setw(16) << str;

#ifdef USE_IMGUI
        ImGui::PushID(entity.id());
        if (ImGui::ImageButton(eAnim.getSprite(), size))
        {
            if (entity.tag() == "player")
                spawnPlayer();
            else
                entity.destroy();
        }
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::Text("%s", ss.str().c_str());
#endif
    }
}   // namespace CyberCity
