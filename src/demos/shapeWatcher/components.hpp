// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: components.hpp
// Author: crdrisko
// Date: 06/30/2024-06:49:19
// Description:

#ifndef DRYPHYS_SRC_DEMOS_SHAPEWATCHER_COMPONENTS_HPP
#define DRYPHYS_SRC_DEMOS_SHAPEWATCHER_COMPONENTS_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include <dryphys/dryphys.hpp>
#include <engine2d/components.hpp>

namespace ShapeWatcher
{
    class CTransform : public Engine2D::Component
    {
    public:
        DryPhys::Particle particle {};

        CTransform() = default;
        CTransform(const DryPhys::Vector3D& pos, const DryPhys::Vector3D& vel)
        {
            particle.setPosition(pos);
            particle.setVelocity(vel);
        }
    };

    class CShape : public Engine2D::Component
    {
    public:
        sf::Text text {};
        std::shared_ptr<sf::Shape> shape {nullptr};

        CShape() = default;
        CShape(const sf::Text& t, std::shared_ptr<sf::Shape> s, const sf::Color& c) : text {t}, shape {s}
        {
            shape->setFillColor(c);
        }
    };
}   // namespace ShapeWatcher

#endif
