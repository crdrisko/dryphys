// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: states.cpp
// Author: crdrisko
// Date: 06/20/2024-06:24:50
// Description:

#include "cybercity/states/states.hpp"

#include "cybercity/forwardDeclare.hpp"

using namespace CyberCity;

#define CREATE_STATE_ACTIONS(STATE_NAME)                                                                                    \
    void STATE_NAME::walk(ConcreteEntityPtr player) { changeState(player, WalkingState::getInstance()); }                   \
    void STATE_NAME::run(ConcreteEntityPtr player) { changeState(player, RunningState::getInstance()); }                    \
    void STATE_NAME::climb(ConcreteEntityPtr player) { changeState(player, ClimbingState::getInstance()); }                 \
    void STATE_NAME::evade(ConcreteEntityPtr player) { changeState(player, EvadingState::getInstance()); }                  \
    void STATE_NAME::jump(ConcreteEntityPtr player) { changeState(player, JumpingState::getInstance()); }                   \
    void STATE_NAME::fall(ConcreteEntityPtr player) { changeState(player, FallingState::getInstance()); }                   \
    void STATE_NAME::attack(ConcreteEntityPtr player) { changeState(player, AttackingState::getInstance()); }               \
    void STATE_NAME::defend(ConcreteEntityPtr player) { changeState(player, DefendingState::getInstance()); }               \
    void STATE_NAME::hurt(ConcreteEntityPtr player) { changeState(player, HurtingState::getInstance()); }

CREATE_STATE_ACTIONS(IdlingState)
CREATE_STATE_ACTIONS(WalkingState)
CREATE_STATE_ACTIONS(RunningState)
CREATE_STATE_ACTIONS(ClimbingState)
CREATE_STATE_ACTIONS(EvadingState)
CREATE_STATE_ACTIONS(JumpingState)
CREATE_STATE_ACTIONS(FallingState)
CREATE_STATE_ACTIONS(AttackingState)
CREATE_STATE_ACTIONS(DefendingState)
CREATE_STATE_ACTIONS(HurtingState)

#undef CREATE_STATE_ACTIONS
