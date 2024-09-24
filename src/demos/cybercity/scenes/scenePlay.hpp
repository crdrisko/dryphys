// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scenePlay.hpp
// Author: crdrisko
// Date: 06/19/2024-08:53:46
// Description:

#ifndef DRYPHYS_SRC_DEMOS_CYBERCITY_SCENES_SCENEPLAY_HPP
#define DRYPHYS_SRC_DEMOS_CYBERCITY_SCENES_SCENEPLAY_HPP

#include <memory>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <dryphys/math.hpp>
#include <engine2d/action.hpp>
#include <engine2d/engine.hpp>
#include <engine2d/scene.hpp>

#include "cybercity/forwardDeclare.hpp"

namespace CyberCity
{
    class ScenePlay : public Engine2D::Scene
    {
        struct ConfigData
        {
            float X {}, Y {}, CX {}, CY {}, SPEED {}, MAXSPEED {}, JUMP {}, GRAVITY {};
            std::string AVATAR, WEAPON;
        };

        struct CameraConfig
        {
            enum CameraTypes
            {
                X, Y, BOUNDS, SMOOTHING
            };
            std::bitset<4> cameraType {};

            DryPhys::real leftExtreme {}, topExtreme {}, rightExtreme {}, bottomExtreme {};
        };

    private:
        ConcreteEntityPtr player_;
        ConcreteEntityManager entityManager_;
        ConfigData playerConfig_;
        CameraConfig cameraBounds_;

        std::string levelPath_;

        float width_ {}, height_ {};
        const float gridSize_ {64};

        sf::Text gridText_;
        sf::Text pauseText_;

        DryPhys::Vector3D mPos_;
        sf::CircleShape mouseShape_;
        std::unique_ptr<sf::Music> music_;

        bool drawTextures_   = true;
        bool drawCollisions_ = false;
        bool drawGrid_       = false;
        bool drawCamera_     = false;

        void init();
        void loadLevel(const std::string& filename);
        void update() override;
        void doActionImpl(const Engine2D::Action& action) override;
        void render() override;
        void onEnd() override;

        // Systems
        void sAI();
        void sMovement();
        void sLifespan();
        void sCollision();
        void sAnimation();
        void sDragAndDrop();
        void sGui();
        void sCamera();

        // Spawners
        void spawnPlayer();
        void spawnEnemy(const ConfigData& enemyConfig);
        void spawnDecorations(const std::string& name);
        void spawnBullet(ConcreteEntity entity);
        void spawnExplosion(ConcreteEntity entity);

        DryPhys::Vector3D gridToMidPixel(float gridX, float gridY, ConcreteEntity entity) const;
        DryPhys::Vector3D windowToWorld(const DryPhys::Vector3D& window) const;
        bool isInside(const DryPhys::Vector3D& pos, ConcreteEntity entity) const;
        void drawEntityAnimations(const std::string& tag);
        void generateGuiInformation(ConcreteEntity& entity);

    public:
        ScenePlay(Engine2D::Engine* gameEngine, const std::string& levelPath);
    };
}   // namespace CyberCity

#endif