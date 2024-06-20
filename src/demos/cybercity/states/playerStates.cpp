// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: playerStates.cpp
// Author: crdrisko
// Date: 06/20/2024-06:25:34
// Description:

#include "states/playerStates.hpp"

#include <iostream>
#include <memory>

#include <engine2d/action.hpp>

#include "scenes/scenePlay.hpp"

void PlayerState::changeState(EntityPtr player, PlayerState* state)
{
    if (player->hasComponent<CState>())
        player->getComponent<CState>().state = state;
}

void PlayerState::handleActions(EntityPtr player, const Engine2D::Action& action)
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
            pInput.canMove = false;
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

void PlayerState::handleInput(EntityPtr player)
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
