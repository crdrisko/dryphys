// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: components.hpp
// Author: crdrisko
// Date: 06/19/2024-05:35:13
// Description:

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include <dryphys/vector3d.hpp>

class CTransform
{
public:
    DryPhys::Vector3D position {};
    DryPhys::Vector3D velocity {};
    float angle {};

    CTransform(const DryPhys::Vector3D& pos, const DryPhys::Vector3D& vel, float ang)
        : position {pos}, velocity {vel}, angle {ang}
    {
    }
};

class CShape
{
public:
    sf::CircleShape circle;

    CShape(float radius, std::size_t points, const sf::Color& fill, const sf::Color& outline, float thickness)
        : circle {radius, points}
    {
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius);
    }
};

class CCollision
{
public:
    float radius {};

    explicit CCollision(float rad) : radius {rad} {}
};

class CInput
{
public:
    bool up {};
    bool left {};
    bool right {};
    bool down {};
    //bool shoot {};
    //bool special {};

    CInput() = default;
};

class CScore
{
public:
    int score {};

    explicit CScore(int s) : score {s} {}
};

class CLifespan
{
public:
    int remaining {};
    int total {};

    explicit CLifespan(int tot) : remaining {tot}, total {tot} {}
};

#endif
