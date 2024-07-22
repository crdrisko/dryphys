// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: components.hpp
// Author: crdrisko
// Date: 06/19/2024-05:35:13
// Description:

#ifndef DRYPHYS_SRC_DEMOS_GEOMETRYWAVES_COMPONENTS_HPP
#define DRYPHYS_SRC_DEMOS_GEOMETRYWAVES_COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include <dryphys/math.hpp>
#include <engine2d/components.hpp>

namespace GeometryWaves
{
    class CTransform : public Engine2D::Component
    {
    public:
        DryPhys::Vector3D position {};
        DryPhys::Vector3D velocity {};
        float angle {};

        CTransform() = default;
        CTransform(const DryPhys::Vector3D& pos, const DryPhys::Vector3D& vel, float ang)
            : position {pos}, velocity {vel}, angle {ang}
        {
        }
    };

    class CShape : public Engine2D::Component
    {
    public:
        sf::CircleShape circle;

        CShape() = default;
        CShape(float radius, std::size_t points, const sf::Color& fill, const sf::Color& outline, float thickness)
            : circle {radius, points}
        {
            circle.setFillColor(fill);
            circle.setOutlineColor(outline);
            circle.setOutlineThickness(thickness);
            circle.setOrigin(radius, radius);
        }
    };

    class CCollision : public Engine2D::Component
    {
    public:
        float radius {};

        CCollision() = default;
        explicit CCollision(float rad) : radius {rad} {}
    };

    class CInput : public Engine2D::Component
    {
    public:
        bool up {};
        bool left {};
        bool right {};
        bool down {};

        CInput() = default;
    };

    class CScore : public Engine2D::Component
    {
    public:
        int score {};

        CScore() = default;
        explicit CScore(int s) : score {s} {}
    };

    class CLifespan : public Engine2D::Component
    {
    public:
        int remaining {};
        int total {};

        CLifespan() = default;
        explicit CLifespan(int tot) : remaining {tot}, total {tot} {}
    };
}   // namespace GeometryWaves

#endif
