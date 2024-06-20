// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: playerStates.hpp
// Author: crdrisko
// Date: 06/20/2024-06:25:40
// Description:

#ifndef PLAYERSTATES_HPP
#define PLAYERSTATES_HPP

#include <memory>
#include <string>

#include <engine2d/action.hpp>

#include "scenes/scenePlay.hpp"

class PlayerState
{
protected:
    void changeState(EntityPtr player, PlayerState* state);

    // Actions
    virtual void walk(EntityPtr player) {}
    virtual void run(EntityPtr player) {}
    virtual void climb(EntityPtr player) {}
    virtual void evade(EntityPtr player) {}
    virtual void jump(EntityPtr player) {}
    virtual void attack(EntityPtr player) {}
    virtual void defend(EntityPtr player) {}
    virtual void hurt(EntityPtr player) {}

public:
    enum States
    {
        Idling,
        Walking,
        Running,
        Climbing,
        Evading,
        Jumping,
        Attacking,
        Defending,
        Hurting
    };

    virtual ~PlayerState() = default;

    void handleActions(EntityPtr player, const Engine2D::Action& action);
    void handleInput(EntityPtr player);

    virtual States queryState() const = 0;
};

class IdlingState : public PlayerState
{
private:
    void walk(EntityPtr player) override;
    void run(EntityPtr player) override;
    void climb(EntityPtr player) override;
    void evade(EntityPtr player) override;
    void jump(EntityPtr player) override;
    void attack(EntityPtr player) override;
    void defend(EntityPtr player) override;
    void hurt(EntityPtr player) override;

public:
    static PlayerState* getInstance()
    {
        static IdlingState instance {};

        return &instance;
    }

    States queryState() const override { return Idling; };
};

#endif
