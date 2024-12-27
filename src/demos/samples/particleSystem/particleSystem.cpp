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

ParticleSystem::ParticleSystem(unsigned max_particles) : world {max_particles}, gravity {DryPhys::Vector3D {0, 5, 0}}
{
    particles_.reserve(max_particles);
}

void ParticleSystem::init(sf::Vector2u windowSize)
{
    windowSize_ = windowSize;
    resetParticles();
}

void ParticleSystem::update()
{
    if (count_ != static_cast<int>(particles_.size()))
        resetParticles();

    // Clear accumulators
    world.startFrame();

    // Run the simulation
    world.runPhysics(0.1);

    for (std::size_t i {}; i < particles_.size(); ++i)
    {
        if (particles_[i].second-- == 0)
            resetParticle(i);
        // else if (particles_[i].second < 45)
        // {
        //     for (std::size_t j {}; j < 4; ++j)
        //     {
        //         auto color = vertices_[4 * i + j].color;

        //         float gray                 = (color.r + color.g + color.b) / 3.0f;
        //         vertices_[4 * i + j].color = sf::Color {
        //             static_cast<sf::Uint8>(gray), static_cast<sf::Uint8>(gray), static_cast<sf::Uint8>(gray), color.a};
        //     }
        // }

        sf::Vector2f vel {static_cast<float>(particles_[i].first.getVelocity().x),
            static_cast<float>(particles_[i].first.getVelocity().y)};

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
    world.getParticles().clear();
    world.getParticleForceRegistry().clear();

    for (int i = 0; i < count_; i++)
    {
        world.getParticles().push_back(&particles_[i].first);
        world.getParticleForceRegistry().add(&particles_[i].first, &gravity);
    }

    vertices_ = sf::VertexArray(sf::Quads, count_ * 4);

    for (std::size_t i {}; i < particles_.size(); ++i)
        resetParticle(i, true);
}

void ParticleSystem::resetParticle(unsigned index, bool first)
{
    // Give the particle an initial position
    float mx {};
    float my {10.0f};

    if (index <= particles_.size() / 3)
        mx = 1.0f * windowSize_.x / 4.0f;
    else if (index <= 2 * particles_.size() / 3)
        mx = windowSize_.x / 2.0f;
    else
        mx = 3.0f * windowSize_.x / 4.0f;

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
    particles_[index].first.setMass(size_ * 2);

    // Give the particles a lifespan
    particles_[index].second = 30 + rand() % 60;
}
