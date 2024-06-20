// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scenePlay.hpp
// Author: crdrisko
// Date: 06/19/2024-08:53:46
// Description:

#ifndef SCENEPLAY_HPP
#define SCENEPLAY_HPP

#include <map>
#include <memory>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <dryphys/vector3d.hpp>
#include <engine2d/action.hpp>
#include <engine2d/engine.hpp>
#include <engine2d/entity.hpp>
#include <engine2d/entityManager.hpp>
#include <engine2d/scene.hpp>

#include "../components.hpp"

using EntityPtr
    = std::shared_ptr<Engine2D::Entity<CAnimation, CTransform, CBoundingBox, CInput, CState, CGravity, CLifespan>>;

using EntityManager = Engine2D::EntityManager<CAnimation, CTransform, CBoundingBox, CInput, CState, CGravity, CLifespan>;

// class PlayerState;

class ScenePlay : public Engine2D::Scene
{
private:
    struct ConfigData
    {
        float X {}, Y {}, CX {}, CY {}, SPEED {}, MAXSPEED {}, JUMP {}, GRAVITY {};
        std::string AVATAR, WEAPON;
    };

private:
    EntityPtr player_;
    EntityManager entityManager_;
    ConfigData playerConfig_;

    std::string levelPath_;

    bool drawTextures_   = true;
    bool drawCollisions_ = false;
    bool drawGrid_       = false;

    const DryPhys::Vector3D gridSize_ {64, 64, 0};
    sf::Text gridText_;
    sf::Text pauseText_;

    std::unique_ptr<sf::Music> music_;

    std::map<std::string, sf::View> envViews;

    void init(const std::string& levelPath);
    void loadLevel(const std::string& filename);
    DryPhys::Vector3D gridToMidPixel(float gridX, float gridY, EntityPtr entity);

    void spawnPlayer();
    void spawnEnemy(const ConfigData& enemyConfig);
    void spawnBullet(EntityPtr spawner);

    void update() override;
    void sDoAction(const Engine2D::Action& action) override;
    void sRender() override;
    void onEnd() override;

    void sMovement();
    void sLifespan();
    void sCollision();
    void sAnimation();

public:
    ScenePlay(Engine2D::Engine* gameEngine, const std::string& levelPath);
};

#endif
