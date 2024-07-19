// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particleSystem.cpp
// Author: crdrisko
// Date: 07/01/2024-07:43:51
// Description:

#include "particleSystem/particleSystem.hpp"

#include <imgui.h>

#include <vector>

#include <SFML/Graphics.hpp>

ParticleSystem::ParticleSystem(std::size_t max_particles) { particles_.reserve(max_particles); }

void ParticleSystem::init(sf::Vector2u windowSize)
{
    windowSize_ = windowSize;
    resetParticles();
}

void ParticleSystem::update()
{
    if (count_ != static_cast<int>(particles_.size()))
        resetParticles();

    for (std::size_t i {}; i < particles_.size(); ++i)
    {
        if (particles_[i].second-- == 0)
            resetParticle(i);

        auto [vx, vy, vz] = particles_[i].first.getVelocity();
        assert(vz == static_cast<DryPhys::real>(0));
        sf::Vector2f vel {vx, vy};

        vertices_[4 * i + 0].position += vel;
        vertices_[4 * i + 1].position += vel;
        vertices_[4 * i + 2].position += vel;
        vertices_[4 * i + 3].position += vel;
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) const { window.draw(vertices_); }

void ParticleSystem::resetParticles()
{
    particles_.resize(count_);

    vertices_ = sf::VertexArray(sf::Quads, count_ * 4);

    for (std::size_t i {}; i < particles_.size(); ++i)
        resetParticle(i, true);
}

void ParticleSystem::resetParticle(std::size_t index, bool first)
{
    // Give the particle an initial position
    float mx {windowSize_.x / 2.0f};
    float my {windowSize_.y / 2.0f};

    vertices_[4 * index + 0].position = sf::Vector2f(mx, my);
    vertices_[4 * index + 1].position = sf::Vector2f(mx + size_, my);
    vertices_[4 * index + 2].position = sf::Vector2f(mx + size_, my + size_);
    vertices_[4 * index + 3].position = sf::Vector2f(mx, my + size_);

    // Give the particle a color
    sf::Uint8 r = static_cast<sf::Uint8>(color_.x * 255);
    sf::Uint8 g = static_cast<sf::Uint8>(color_.y * 255);
    sf::Uint8 b = static_cast<sf::Uint8>(color_.z * 255);
    sf::Uint8 a = static_cast<sf::Uint8>(color_.w * 255);

    sf::Color color = sf::Color(r, g, b, a + rand() % a);

    // Avoid initial "burst" of particles
    if (first)
        color.a = 0;

    vertices_[4 * index + 0].color = color;
    vertices_[4 * index + 1].color = color;
    vertices_[4 * index + 2].color = color;
    vertices_[4 * index + 3].color = color;

    // Give the particle a velocity
    float rx = ((float)rand() / RAND_MAX) * 10 - 5;
    float ry = ((float)rand() / RAND_MAX) * 10 - 5;

    particles_[index].first.setVelocity(rx, ry, 0);

    // Give the particles a lifespan
    particles_[index].second = 30 + rand() % 60;
}
