// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particleSystem.hpp
// Author: crdrisko
// Date: 07/01/2024-07:43:48
// Description:

#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include <imgui.h>

#include <vector>

#include <SFML/Graphics.hpp>

class ParticleSystem
{
private:
    struct Particle
    {
        sf::Vector2f velocity;
        float lifetime = 0;
    };

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    sf::Vector2u m_windowSize;

    int m_size  = 8;
    int m_count = 1024;
    ImVec4 m_color {0.0f, 0.5f, 0.65f, 0.78f};

    void resetParticles();
    void resetParticle(std::size_t index, bool first = false);

public:
    explicit ParticleSystem(std::size_t max_particles);

    void init(sf::Vector2u windowSize);
    void update();
    void draw(sf::RenderWindow& window) const;

    int* size() { return &m_size; }
    int* count() { return &m_count; }
    float* color() { return (float*)&m_color; }
};

#endif
