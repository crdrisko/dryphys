// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: components.hpp
// Author: crdrisko
// Date: 07/04/2024-07:10:51
// Description:

#ifndef DRYPHYS_SRC_DEMOS_LIGHTCYCLE_COMPONENTS_HPP
#define DRYPHYS_SRC_DEMOS_LIGHTCYCLE_COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include <dryphys/math.hpp>
#include <engine2d/components.hpp>

#include "lightCycle/forwardDeclare.hpp"

namespace LightCycle
{
    class CTransform : public Engine2D::Component
    {
    public:
        DryPhys::Vector3D pos {};
        std::size_t dir {};

        CTransform() = default;
        explicit CTransform(const DryPhys::Vector3D& p, std::size_t d = 0) : pos {p}, dir {d} {}
    };

    class CAI : public Engine2D::Component
    {
    public:
        std::shared_ptr<AI> ai {};

        CAI() = default;
        explicit CAI(std::shared_ptr<AI> ai) : ai {ai} {}
    };

    class CCycle : public Engine2D::Component
    {
    private:
        bool alive_ = true;

    public:
        int score {};
        sf::Color color;

        CCycle() = default;
        explicit CCycle(const sf::Color& fill) : color {fill} {}

        bool isAlive() const { return alive_; }
        void kill() { alive_ = false; }
    };
}   // namespace LightCycle

#endif
