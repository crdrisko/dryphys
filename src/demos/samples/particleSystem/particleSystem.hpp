// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: particleSystem.hpp
// Author: crdrisko
// Date: 07/01/2024-07:43:48
// Description:

#ifndef DRYPHYS_SRC_DEMOS_SAMPLES_PARTICLESYSTEM_PARTICLESYSTEM_HPP
#define DRYPHYS_SRC_DEMOS_SAMPLES_PARTICLESYSTEM_PARTICLESYSTEM_HPP

#include <imgui.h>

#include <vector>

#include <SFML/Graphics.hpp>
#include <dryphys/dryphys.hpp>

class ParticleSystem
{
private:
    std::vector<std::pair<DryPhys::Particle, float>> particles_;
    sf::VertexArray vertices_;
    sf::Vector2u windowSize_;

    int size_  = 8;
    int count_ = 1024;
    ImVec4 color_ {0.0f, 0.5f, 0.65f, 0.78f};

    void resetParticles();
    void resetParticle(std::size_t index, bool first = false);

public:
    explicit ParticleSystem(std::size_t max_particles);

    void init(sf::Vector2u windowSize);
    void update();
    void draw(sf::RenderWindow& window) const;

    int* size() { return &size_; }
    int* count() { return &count_; }
    float* color() { return (float*)&color_; }
};

#endif
