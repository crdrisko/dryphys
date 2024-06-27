// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scenePlay.hpp
// Author: crdrisko
// Date: 06/19/2024-08:53:46
// Description:

#ifndef DRYPHYS_SRC_DEMOS_CYBERCITY_SCENES_SCENEPLAY_HPP
#define DRYPHYS_SRC_DEMOS_CYBERCITY_SCENES_SCENEPLAY_HPP

#include <map>
#include <memory>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <dryphys/vector3d.hpp>
#include <engine2d/action.hpp>
#include <engine2d/engine.hpp>
#include <engine2d/scene.hpp>

#include "cybercity/entityManager.hpp"

namespace CyberCity
{
    class Entity;

    class ScenePlay : public Engine2D::Scene
    {
        struct ConfigData
        {
            float X {}, Y {}, CX {}, CY {}, SPEED {}, MAXSPEED {}, JUMP {}, GRAVITY {};
            std::string AVATAR, WEAPON;
        };

    private:
        std::shared_ptr<Entity> player_;
        EntityManager entityManager_;
        ConfigData playerConfig_;

        std::string levelPath_;

        const DryPhys::Vector3D gridSize_ {64, 64, 0};
        sf::Text gridText_;
        sf::Text pauseText_;

        DryPhys::Vector3D mPos_;
        sf::CircleShape mouseShape_;

        std::unique_ptr<sf::Music> music_;
        std::map<std::string, sf::View> envViews;

        bool drawTextures_   = true;
        bool drawCollisions_ = false;
        bool drawGrid_       = false;

        void init(const std::string& levelPath);
        void loadLevel(const std::string& filename);
        void update() override;
        void sDoAction(const Engine2D::Action& action) override;
        void sRender() override;
        void onEnd() override;

        // Systems
        void sMovement();
        void sLifespan();
        void sCollision();
        void sAnimation();
        void sDragAndDrop();

        // Spawners
        void spawnPlayer();
        void spawnEnemy(const ConfigData& enemyConfig);
        void spawnBullet(std::shared_ptr<Entity> spawner);

        DryPhys::Vector3D gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity) const;
        DryPhys::Vector3D windowToWorld(const DryPhys::Vector3D& window) const;
        bool isInside(const DryPhys::Vector3D& pos, std::shared_ptr<Entity> e) const;

    public:
        ScenePlay(Engine2D::Engine* gameEngine, const std::string& levelPath);
    };
}   // namespace CyberCity

#endif
