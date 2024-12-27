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

class Firework : public DryPhys::Particle
{
public:
    DryPhys::real lifespan;

    bool update(DryPhys::real duration)
    {
        DryPhys::Vector3D constantAcceleration = acceleration_;

        moveA(duration);
        addForce(constantAcceleration * getMass());
        moveB(duration);

        acceleration_ = constantAcceleration;

        // We work backwards from our age to zero.
        lifespan -= duration;
        return (lifespan < 0) || (position_.y < 0);
    }
};

class ParticleSystem
{
private:
    DryPhys::ParticleWorld world;
    std::vector<std::pair<DryPhys::Particle, DryPhys::real>> particles_;
    DryPhys::ParticleGravity gravity;

    sf::VertexArray vertices_;
    sf::Vector2u windowSize_;

    int size_  = 8;
    int count_ = 1024;
    ImVec4 color_ {0.0f, 0.5f, 0.65f, 0.78f};

    void resetParticles();
    void resetParticle(unsigned index, bool first = false);

public:
    explicit ParticleSystem(unsigned max_particles);

    void init(sf::Vector2u windowSize);
    void update();
    void draw(sf::RenderWindow& window) const;

    int* size() { return &size_; }
    int* count() { return &count_; }
    float* color() { return (float*)&color_; }
};

#endif
