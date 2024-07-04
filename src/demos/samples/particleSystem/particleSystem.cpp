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

ParticleSystem::ParticleSystem(std::size_t max_particles) { m_particles.reserve(max_particles); }

void ParticleSystem::init(sf::Vector2u windowSize)
{
    m_windowSize = windowSize;
    resetParticles();
}

void ParticleSystem::update()
{
    if (m_count != static_cast<int>(m_particles.size()))
        resetParticles();

    for (std::size_t i {}; i < m_particles.size(); ++i)
    {
        if (m_particles[i].lifetime-- == 0)
            resetParticle(i);

        m_vertices[4 * i + 0].position += m_particles[i].velocity;
        m_vertices[4 * i + 1].position += m_particles[i].velocity;
        m_vertices[4 * i + 2].position += m_particles[i].velocity;
        m_vertices[4 * i + 3].position += m_particles[i].velocity;
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) const { window.draw(m_vertices); }

void ParticleSystem::resetParticles()
{
    m_particles.resize(m_count);

    m_vertices = sf::VertexArray(sf::Quads, m_count * 4);

    for (std::size_t i {}; i < m_particles.size(); ++i)
        resetParticle(i, true);
}

void ParticleSystem::resetParticle(std::size_t index, bool first)
{
    // Give the particle an initial position
    float mx {m_windowSize.x / 2.0f};
    float my {m_windowSize.y / 2.0f};

    m_vertices[4 * index + 0].position = sf::Vector2f(mx, my);
    m_vertices[4 * index + 1].position = sf::Vector2f(mx + m_size, my);
    m_vertices[4 * index + 2].position = sf::Vector2f(mx + m_size, my + m_size);
    m_vertices[4 * index + 3].position = sf::Vector2f(mx, my + m_size);

    // Give the particle a color
    sf::Uint8 r = static_cast<sf::Uint8>(m_color.x * 255);
    sf::Uint8 g = static_cast<sf::Uint8>(m_color.y * 255);
    sf::Uint8 b = static_cast<sf::Uint8>(m_color.z * 255);
    sf::Uint8 a = static_cast<sf::Uint8>(m_color.w * 255);

    sf::Color color = sf::Color(r, g, b, a + rand() % a);

    // Avoid initial "burst" of particles
    if (first)
        color.a = 0;

    m_vertices[4 * index + 0].color = color;
    m_vertices[4 * index + 1].color = color;
    m_vertices[4 * index + 2].color = color;
    m_vertices[4 * index + 3].color = color;

    // Give the particle a velocity
    float rx = ((float)rand() / RAND_MAX) * 10 - 5;
    float ry = ((float)rand() / RAND_MAX) * 10 - 5;

    m_particles[index].velocity = sf::Vector2f(rx, ry);

    // Give the particles a lifespan
    m_particles[index].lifetime = 30 + rand() % 60;
}
