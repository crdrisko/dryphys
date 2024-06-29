// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: components.hpp
// Author: crdrisko
// Date: 06/19/2024-10:56:29
// Description:

#ifndef DRYPHYS_SRC_DEMOS_CYBERCITY_COMPONENTS_HPP
#define DRYPHYS_SRC_DEMOS_CYBERCITY_COMPONENTS_HPP

#include <bitset>

#include <dryphys/vector3d.hpp>
#include <engine2d/animation.hpp>
#include <engine2d/components.hpp>

#include "cybercity/states/playerStates.hpp"

namespace CyberCity
{
    class CAnimation : public Engine2D::Component
    {
    public:
        Engine2D::Animation animation;
        bool repeat = false;

        CAnimation() = default;
        CAnimation(const Engine2D::Animation& animation, bool r = true) : animation {animation}, repeat {r} {}
    };

    class CTransform : public Engine2D::Component
    {
    public:
        DryPhys::Vector3D pos {};
        DryPhys::Vector3D vel {};
        DryPhys::Vector3D scale {1.0f, 1.0f, 0.0f};
        DryPhys::Vector3D prevPos {};

        float angle {};

        CTransform() = default;
        explicit CTransform(const DryPhys::Vector3D& p) : pos {p} {}
        CTransform(const DryPhys::Vector3D& p, const DryPhys::Vector3D& v, const DryPhys::Vector3D& sc, float a)
            : pos {p}, vel {v}, scale {sc}, angle {a}
        {
        }
    };

    class CBoundingBox : public Engine2D::Component
    {
    public:
        DryPhys::Vector3D size {};
        DryPhys::Vector3D halfSize {};

        CBoundingBox() = default;
        explicit CBoundingBox(const DryPhys::Vector3D& s) : size {s}, halfSize {s / 2.0f} {}
    };

    class CInput : public Engine2D::Component
    {
    public:
        enum Inputs
        {
            BEGININDEX = 0,
            UP         = BEGININDEX,
            DOWN,
            LEFT,
            RIGHT,
            SPRINT,
            JUMP,
            EVADE,
            ATTACK,
            DEFEND,
            ENDINDEX
        };

        using InputBitSet = std::bitset<ENDINDEX - BEGININDEX>;

        InputBitSet inputs {0b000000000};

        bool canJump     = true;
        bool canMove     = true;
        bool facingRight = true;

        CInput() = default;
    };

    class CDraggable : public Engine2D::Component
    {
    public:
        bool dragging = false;

        CDraggable() = default;
    };

    class CGravity : public Engine2D::Component
    {
    public:
        float gravity {};

        CGravity() = default;
        explicit CGravity(float g) : gravity {g} {}
    };

    class CState : public Engine2D::Component
    {
    public:
        PlayerState* state;

        CState() : state {IdlingState::getInstance()} {}
    };

    class CLifespan : public Engine2D::Component
    {
    public:
        int lifespan {};
        int frameCreated {};

        CLifespan() = default;
        CLifespan(int duration, int frame) : lifespan {duration}, frameCreated {frame} {}
    };
}   // namespace CyberCity

#endif
