// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: game.hpp
// Author: crdrisko
// Date: 06/19/2024-05:30:00
// Description:

#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <random>
#include <string>

#include <SFML/Graphics.hpp>
#include <dryphys/vector3d.hpp>

#include "entity.hpp"
#include "entityManager.hpp"

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

class Game
{
private:
    sf::RenderWindow window_;

    EntityManager entities_;

    sf::Font font_;
    sf::Text text_;

    PlayerConfig playerConfig_;
    EnemyConfig enemyConfig_;
    BulletConfig bulletConfig_;

    int score_ {};
    int currentFrame_ {};
    int lastEnemySpawnTime_ {};

    bool paused_ {false};
    bool running_ {true};

    std::shared_ptr<Entity> player_;

    std::mt19937 gen_ {};

    void init(const std::string& config);
    void setPaused(bool paused);

    // Systems
    void sMovement();
    void sUserInput();
    void sLifespan();
    void sRender();
    void sEnemySpawner();
    void sCollision();

    void spawnPlayer();

    void spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> entity);
    void spawnBullet(std::shared_ptr<Entity> entity, const DryPhys::Vector3D& mousePos);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:
    explicit Game(const std::string& config);

    void run();
};

#endif
