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
#include <engine2d/action.hpp>

#include "cybercity/entity.hpp"

namespace CyberCity
{
    void PlayerState::changeState(std::shared_ptr<Entity> player, PlayerState* state)
    {
        if (player->hasComponent<CState>())
            player->getComponent<CState>().state = state;
    }

    void PlayerState::handleActions(std::shared_ptr<Entity> player, const Engine2D::Action& action)
    {
        auto& pInput = player->getComponent<CInput>();

        if (action.type() == "START")
        {
            if (action.name() == "UP")
            {
                pInput.inputs.set(CInput::UP, true);
            }
            else if (action.name() == "DOWN")
            {
                pInput.inputs.set(CInput::DOWN, true);
            }
            else if (action.name() == "LEFT")
            {
                pInput.inputs.set(CInput::LEFT, true);
            }
            else if (action.name() == "RIGHT")
            {
                pInput.inputs.set(CInput::RIGHT, true);
            }
            else if (action.name() == "SPRINT")
            {
                pInput.inputs.set(CInput::SPRINT, true);
            }
            else if (action.name() == "JUMP")
            {
                pInput.inputs.set(CInput::JUMP, true);
                pInput.canJump = false;
            }
            else if (action.name() == "EVADE")
            {
                pInput.inputs.set(CInput::EVADE, true);
                // pInput.canMove = false;
            }
            else if (action.name() == "ATTACK")
            {
                pInput.inputs.set(CInput::ATTACK, true);
            }
            else if (action.name() == "DEFEND")
            {
                pInput.inputs.set(CInput::DEFEND, true);
                pInput.canMove = false;
            }
        }
        else if (action.type() == "END")
        {
            if (action.name() == "UP")
            {
                pInput.inputs.set(CInput::UP, false);
            }
            else if (action.name() == "DOWN")
            {
                pInput.inputs.set(CInput::DOWN, false);
            }
            else if (action.name() == "LEFT")
            {
                pInput.inputs.set(CInput::LEFT, false);
            }
            else if (action.name() == "RIGHT")
            {
                pInput.inputs.set(CInput::RIGHT, false);
            }
            else if (action.name() == "SPRINT")
            {
                pInput.inputs.set(CInput::SPRINT, false);
            }
            else if (action.name() == "JUMP")
            {
                pInput.inputs.set(CInput::JUMP, false);
            }
            else if (action.name() == "EVADE")
            {
                pInput.inputs.set(CInput::EVADE, false);
            }
            else if (action.name() == "ATTACK")
            {
                pInput.inputs.set(CInput::ATTACK, false);
            }
            else if (action.name() == "DEFEND")
            {
                pInput.inputs.set(CInput::DEFEND, false);
                pInput.canMove = true;
            }
        }
    }

    void PlayerState::handleInput(std::shared_ptr<Entity> player)
    {
        auto& pInput = player->getComponent<CInput>();

        if (pInput.canJump && pInput.inputs[CInput::JUMP])
        {
            jump(player);
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

    void PlayerState::handleAnimations(std::shared_ptr<Entity> player, Engine2D::Assets& assets, const std::string& prefix)
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
        case PlayerState::Attacking:
            if (name == prefix + "Run")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "RunShoot"));
            else if (name != prefix + "Shoot" && name != prefix + "RunShoot")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Shoot"));
            break;
        case PlayerState::Defending:
            if (name != prefix + "Crouch")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Crouch"));
            break;
        case PlayerState::Hurting:
            if (name != prefix + "Hurt")
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Hurt"));
            break;
        default:
            break;
        }
    }

    /* void PlayerState::handleAnimations(std::shared_ptr<Entity> player, Engine2D::Assets& assets, const std::string& prefix)
{
    auto& pAnim  = player->getComponent<CAnimation>();
    auto& pInput = player->getComponent<CInput>();
    // auto pState  = player->getComponent<CState>().state;

    std::string name {player->getComponent<CAnimation>().animation.getName()};

    if (name == prefix + "Idle")
    {
        switch (queryState())
        {
        case PlayerState::Walking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Walk"));
            break;
        case PlayerState::Running:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Run"));
            break;
        case PlayerState::Climbing:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Climb"));
            break;
        case PlayerState::Evading:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Evade"));
            break;
        case PlayerState::Jumping:
            if (pInput.canJump)
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Jump"));
            break;
        case PlayerState::Attacking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Shoot"));
            break;
        case PlayerState::Defending:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Crouch"));
            break;
        case PlayerState::Hurting:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Hurt"));
            break;
        default:
            break;
        }
    }
    else if (name == prefix + "Walk")
    {
        switch (queryState())
        {
        case PlayerState::Idling:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Idle"));
            break;
        case PlayerState::Running:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Run"));
            break;
        case PlayerState::Climbing:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Climb"));
            break;
        case PlayerState::Evading:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Evade"));
            break;
        case PlayerState::Jumping:
            if (pInput.canJump)
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Jump"));
            break;
        case PlayerState::Attacking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "RunShoot"));
            break;
        case PlayerState::Defending:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Crouch"));
            break;
        case PlayerState::Hurting:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Hurt"));
            break;
        default:
            break;
        }
    }
    else if (name == prefix + "Run")
    {
        switch (queryState())
        {
        case PlayerState::Idling:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Idle"));
            break;
        case PlayerState::Walking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Walk"));
            break;
        case PlayerState::Climbing:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Climb"));
            break;
        case PlayerState::Evading:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Evade"));
            break;
        case PlayerState::Jumping:
            if (pInput.canJump)
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Jump"));
            break;
        case PlayerState::Attacking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "RunShoot"));
            break;
        case PlayerState::Defending:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Crouch"));
            break;
        case PlayerState::Hurting:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Hurt"));
            break;
        default:
            break;
        }
    }
    else if (name == prefix + "Jump")
    {
        if (pAnim.animation.hasEnded())
        {
            switch (queryState())
            {
            case PlayerState::Idling:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Idle"));
                break;
            case PlayerState::Walking:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Walk"));
                break;
            case PlayerState::Running:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Run"));
                break;
            case PlayerState::Climbing:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Climb"));
                break;
            case PlayerState::Evading:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Evade"));
                break;
            case PlayerState::Attacking:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Shoot"));
                break;
            case PlayerState::Defending:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Crouch"));
                break;
            case PlayerState::Hurting:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Hurt"));
                break;
            default:
                break;
            }
        }
    }
    else if (name == prefix + "Evade")
    {
        if (pAnim.animation.hasEnded())
        {
            switch (queryState())
            {
            case PlayerState::Idling:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Idle"));
                break;
            case PlayerState::Walking:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Walk"));
                break;
            case PlayerState::Running:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Run"));
                break;
            case PlayerState::Climbing:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Climb"));
                break;
            case PlayerState::Jumping:
                if (pInput.canJump)
                    player->addComponent<CAnimation>(assets.getAnimation(prefix + "Jump"));
                break;
            case PlayerState::Attacking:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Shoot"));
                break;
            case PlayerState::Defending:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Crouch"));
                break;
            case PlayerState::Hurting:
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Hurt"));
                break;
            default:
                break;
            }

            pInput.canMove = true;
        }
    }
    else if (name == prefix + "Climb")
    {
        switch (queryState())
        {
        case PlayerState::Idling:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Idle"));
            break;
        case PlayerState::Walking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Walk"));
            break;
        case PlayerState::Running:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Run"));
            break;
        case PlayerState::Evading:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Evade"));
            break;
        case PlayerState::Jumping:
            if (pInput.canJump)
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Jump"));
            break;
        case PlayerState::Attacking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Shoot"));
            break;
        case PlayerState::Defending:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Crouch"));
            break;
        case PlayerState::Hurting:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Hurt"));
            break;
        default:
            break;
        }
    }
    else if (name == prefix + "Crouch")
    {
        switch (queryState())
        {
        case PlayerState::Idling:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Idle"));
            break;
        case PlayerState::Walking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Walk"));
            break;
        case PlayerState::Running:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Run"));
            break;
        case PlayerState::Climbing:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Climb"));
            break;
        case PlayerState::Evading:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Evade"));
            break;
        case PlayerState::Jumping:
            if (pInput.canJump)
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Jump"));
            break;
        case PlayerState::Attacking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Shoot"));
            break;
        case PlayerState::Hurting:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Hurt"));
            break;
        default:
            break;
        }
    }
    else if (name == prefix + "Hurt")
    {
        switch (queryState())
        {
        case PlayerState::Idling:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Idle"));
            break;
        case PlayerState::Walking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Walk"));
            break;
        case PlayerState::Running:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Run"));
            break;
        case PlayerState::Climbing:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Climb"));
            break;
        case PlayerState::Evading:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Evade"));
            break;
        case PlayerState::Jumping:
            if (pInput.canJump)
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Jump"));
            break;
        case PlayerState::Attacking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Shoot"));
            break;
        case PlayerState::Defending:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Crouch"));
            break;
        default:
            break;
        }
    }
    else if (name == prefix + "Shoot" || name == prefix + "RunShoot")
    {
        switch (queryState())
        {
        case PlayerState::Idling:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Idle"));
            break;
        case PlayerState::Walking:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Walk"));
            break;
        case PlayerState::Running:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Run"));
            break;
        case PlayerState::Climbing:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Climb"));
            break;
        case PlayerState::Evading:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Evade"));
            break;
        case PlayerState::Jumping:
            if (pInput.canJump)
                player->addComponent<CAnimation>(assets.getAnimation(prefix + "Jump"));
            break;
        case PlayerState::Defending:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Crouch"));
            break;
        case PlayerState::Hurting:
            player->addComponent<CAnimation>(assets.getAnimation(prefix + "Hurt"));
            break;
        default:
            break;
        }
    }
} */
}   // namespace CyberCity
