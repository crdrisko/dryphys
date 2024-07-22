// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scenePlay.hpp
// Author: crdrisko
// Date: 06/19/2024-05:30:00
// Description:

#ifndef DRYPHYS_SRC_DEMOS_GEOMETRYWAVES_SCENES_SCENEPLAY_HPP
#define DRYPHYS_SRC_DEMOS_GEOMETRYWAVES_SCENES_SCENEPLAY_HPP

#include <memory>
#include <random>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <dryphys/math.hpp>
#include <engine2d/action.hpp>
#include <engine2d/engine.hpp>

#include "geometryWaves/forwardDeclare.hpp"

namespace GeometryWaves
{
    class ScenePlay : public Engine2D::Scene
    {
        struct PlayerConfig
        {
            int SR, CR, V;
            sf::Uint8 FR, FG, FB, OR, OG, OB, OT;
            float S;
        };

        struct EnemyConfig
        {
            int SR, CR, VMIN, VMAX, L, SI;
            sf::Uint8 OR, OG, OB, OT;
            float SMIN, SMAX;
        };

        struct BulletConfig
        {
            int SR, CR, V, L;
            sf::Uint8 FR, FG, FB, OR, OG, OB, OT;
            float S;
        };

    private:
        ConcreteEntityPtr player_;
        ConcreteEntityManager entityManager_;
        PlayerConfig playerConfig_;
        EnemyConfig enemyConfig_;
        BulletConfig bulletConfig_;

        sf::Text text_;
        std::unique_ptr<sf::Music> music_;

        int score_ {};
        int currentFrame_ {};
        int lastEnemySpawnTime_ {};

        std::mt19937 gen_ {};

        void init(const std::string& config);

        void update() override;
        void doActionImpl(const Engine2D::Action& action) override;
        void render() override;
        void onEnd() override;

        // Systems
        void sMovement();
        void sLifespan();
        void sEnemySpawner();
        void sCollision();

        // Spawners
        void spawnPlayer();
        void spawnEnemy();
        void spawnSmallEnemies(ConcreteEntityPtr entity);
        void spawnBullet(ConcreteEntityPtr entity, const DryPhys::Vector3D& mousePos);
        void spawnSpecialWeapon(ConcreteEntityPtr entity, const DryPhys::Vector3D& mousePos);

    public:
        ScenePlay(Engine2D::Engine* gameEngine, const std::string& config);
    };
}   // namespace GeometryWaves

#endif
