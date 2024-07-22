// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: playerStates.cpp
// Author: crdrisko
// Date: 06/20/2024-06:25:34
// Description:

#include "cybercity/states/playerStates.hpp"

#include <iostream>
#include <memory>

#include <common-utils/strings.hpp>
#include <dryphys/utilities.hpp>
#include <engine2d/action.hpp>

#include "cybercity/components.hpp"
#include "cybercity/forwardDeclare.hpp"

using namespace DryPhys::Literals;

namespace CyberCity
{
    void PlayerState::changeState(ConcreteEntityPtr player, PlayerState* state)
    {
        if (player->hasComponent<CState>())
            player->getComponent<CState>().state = state;
    }

    void PlayerState::handleActions(ConcreteEntityPtr player, const Engine2D::Action& action)
    {
        auto& pInput = player->getComponent<CInput>();

        if (auto atype = action.type(); atype == Engine2D::Action::START)
        {
            switch (action.sid())
            {
            case "UP"_sID:
                pInput.inputs.set(CInput::UP, true);
                break;
            case "DOWN"_sID:
                pInput.inputs.set(CInput::DOWN, true);
                break;
            case "LEFT"_sID:
                pInput.inputs.set(CInput::LEFT, true);
                break;
            case "RIGHT"_sID:
                pInput.inputs.set(CInput::RIGHT, true);
                break;
            case "SPRINT"_sID:
                pInput.inputs.set(CInput::SPRINT, true);
                break;
            case "JUMP"_sID:
                pInput.inputs.set(CInput::JUMP, true);
                pInput.canJump = false;
                break;
            case "EVADE"_sID:
                pInput.inputs.set(CInput::EVADE, true);
                break;
            case "ATTACK"_sID:
                pInput.inputs.set(CInput::ATTACK, true);
                break;
            case "DEFEND"_sID:
                pInput.inputs.set(CInput::DEFEND, true);
                pInput.canMove = false;
                break;
            default:
                break;
            }
        }
        else if (atype == Engine2D::Action::END)
        {
            switch (action.sid())
            {
            case "UP"_sID:
                pInput.inputs.set(CInput::UP, false);
                break;
            case "DOWN"_sID:
                pInput.inputs.set(CInput::DOWN, false);
                break;
            case "LEFT"_sID:
                pInput.inputs.set(CInput::LEFT, false);
                break;
            case "RIGHT"_sID:
                pInput.inputs.set(CInput::RIGHT, false);
                break;
            case "SPRINT"_sID:
                pInput.inputs.set(CInput::SPRINT, false);
                break;
            case "JUMP"_sID:
                pInput.inputs.set(CInput::JUMP, false);
                break;
            case "EVADE"_sID:
                pInput.inputs.set(CInput::EVADE, false);
                break;
            case "ATTACK"_sID:
                pInput.inputs.set(CInput::ATTACK, false);
                pInput.canMove = true;
                break;
            case "DEFEND"_sID:
                pInput.inputs.set(CInput::DEFEND, false);
                pInput.canMove = true;
                break;
            default:
                break;
            }
        }
    }

    void PlayerState::handleInput(ConcreteEntityPtr player)
    {
        auto& pInput     = player->getComponent<CInput>();
        auto& pTransform = player->getComponent<CTransform>();

        // Maintain y velocity from previous frame so gravity acts as an acceleration
        pTransform.vel.x = 0.0f;

        float horizontalMoveSpeed {5};

        if (pInput.inputs[CInput::SPRINT])
            horizontalMoveSpeed *= 1.5;

        if (pInput.inputs[CInput::LEFT])
        {
            if (pInput.facingRight)
            {
                pTransform.scale.x *= -1.0f;
                pInput.facingRight = false;
            }

            if (pInput.canMove)
                pTransform.vel.x = -horizontalMoveSpeed;
        }
        else if (pInput.inputs[CInput::RIGHT])
        {
            if (!pInput.facingRight)
            {
                pTransform.scale.x *= -1.0f;
                pInput.facingRight = true;
            }

            if (pInput.canMove)
                pTransform.vel.x = horizontalMoveSpeed;
        }

        if (pInput.canJump && pInput.inputs[CInput::JUMP])
        {
            pTransform.vel.y += -5;

            jump(player);
            return;
        }

        if (pInput.inputs[CInput::FALL])
        {
            fall(player);
            return;
        }

        if (pInput.inputs[CInput::ATTACK])
        {
            attack(player);
            return;
        }

        if (pInput.inputs[CInput::EVADE])
        {
            evade(player);
            return;
        }

        if (pInput.inputs[CInput::DEFEND])
        {
            defend(player);
            return;
        }

        if (pInput.canMove && (pInput.inputs[CInput::RIGHT] || pInput.inputs[CInput::LEFT]))
        {
            if (pInput.inputs[CInput::SPRINT])
                run(player);
            else
                walk(player);
        }
        else
        {
            changeState(player, IdlingState::getInstance());
        }
    }

    void PlayerState::handleAnimations(ConcreteEntityPtr player, Engine2D::Assets& assets, const std::string& prefix)
    {
        auto& pAnim  = player->getComponent<CAnimation>().animation;
        auto& pInput = player->getComponent<CInput>();

        std::string name {pAnim.getName()};

        if (name == prefix + "Evade" || name == prefix + "Jump")
        {
            if (!pAnim.hasEnded())
                return;
        }

        switch (queryState())
        {
        case PlayerState::Idling:
            if (name != prefix + "Idle")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Idle"));
            break;
        case PlayerState::Walking:
            if (name != prefix + "Walk")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Walk"));
            break;
        case PlayerState::Running:
            if (name != prefix + "Run")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Run"));
            break;
        case PlayerState::Climbing:
            if (name != prefix + "Climb")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Climb"));
            break;
        case PlayerState::Evading:
            if (name != prefix + "Evade")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Evade"));
            break;
        case PlayerState::Jumping:
            if (pInput.canJump && (name != prefix + "Jump"))
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Jump"));
            break;
        case PlayerState::Falling:
            if ((name != prefix + "Fall"))
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Fall"));
            break;
        case PlayerState::Attacking:
            if (name == prefix + "Run")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "RunShoot"));
            else if (name != prefix + "Shoot" && name != prefix + "RunShoot" && name != prefix + "Crouch")
            {
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Shoot"));
                pInput.canMove = false;
            }
            break;
        case PlayerState::Defending:
            if (name != prefix + "Crouch")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Crouch"));
            pInput.canMove = false;
            break;
        case PlayerState::Hurting:
            if (name != prefix + "Hurt")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Hurt"));
            break;
        default:
            break;
        }
    }
}   // namespace CyberCity
