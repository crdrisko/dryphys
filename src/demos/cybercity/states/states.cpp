// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: states.cpp
// Author: crdrisko
// Date: 06/20/2024-06:24:50
// Description:

#include "states/states.hpp"

#include "scenes/scenePlay.hpp"

#define CREATE_STATE_ACTIONS(STATE_NAME)                                                                                    \
    void STATE_NAME::walk(EntityPtr player) { changeState(player, WalkingState::getInstance()); }                           \
    void STATE_NAME::run(EntityPtr player) { changeState(player, RunningState::getInstance()); }                            \
    void STATE_NAME::climb(EntityPtr player) { changeState(player, ClimbingState::getInstance()); }                         \
    void STATE_NAME::evade(EntityPtr player) { changeState(player, EvadingState::getInstance()); }                          \
    void STATE_NAME::jump(EntityPtr player) { changeState(player, JumpingState::getInstance()); }                           \
    void STATE_NAME::attack(EntityPtr player) { changeState(player, AttackingState::getInstance()); }                       \
    void STATE_NAME::defend(EntityPtr player) { changeState(player, DefendingState::getInstance()); }                       \
    void STATE_NAME::hurt(EntityPtr player) { changeState(player, HurtingState::getInstance()); }

CREATE_STATE_ACTIONS(IdlingState)
CREATE_STATE_ACTIONS(WalkingState)
CREATE_STATE_ACTIONS(RunningState)
CREATE_STATE_ACTIONS(ClimbingState)
CREATE_STATE_ACTIONS(EvadingState)
CREATE_STATE_ACTIONS(JumpingState)
CREATE_STATE_ACTIONS(AttackingState)
CREATE_STATE_ACTIONS(DefendingState)
CREATE_STATE_ACTIONS(HurtingState)

#undef CREATE_STATE_ACTIONS
