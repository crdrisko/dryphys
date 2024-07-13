// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: playerStates.hpp
// Author: crdrisko
// Date: 06/20/2024-06:25:40
// Description:

#ifndef DRYPHYS_SRC_DEMOS_CYBERCITY_STATES_PLAYERSTATES_HPP
#define DRYPHYS_SRC_DEMOS_CYBERCITY_STATES_PLAYERSTATES_HPP

#include <memory>
#include <string>

#include <engine2d/action.hpp>
#include <engine2d/assets.hpp>

#include "cybercity/forwardDeclare.hpp"

namespace CyberCity
{
    class PlayerState
    {
    protected:
        void changeState(ConcreteEntityPtr player, PlayerState* state);

        // Actions
        virtual void walk(ConcreteEntityPtr) {}
        virtual void run(ConcreteEntityPtr) {}
        virtual void climb(ConcreteEntityPtr) {}
        virtual void evade(ConcreteEntityPtr) {}
        virtual void jump(ConcreteEntityPtr) {}
        virtual void fall(ConcreteEntityPtr) {}
        virtual void attack(ConcreteEntityPtr) {}
        virtual void defend(ConcreteEntityPtr) {}
        virtual void hurt(ConcreteEntityPtr) {}

    public:
        enum States
        {
            Idling,
            Walking,
            Running,
            Climbing,
            Evading,
            Jumping,
            Falling,
            Attacking,
            Defending,
            Hurting
        };

        virtual ~PlayerState() = default;

        void handleActions(ConcreteEntityPtr player, const Engine2D::Action& action);
        void handleInput(ConcreteEntityPtr player);
        void handleAnimations(ConcreteEntityPtr player, Engine2D::Assets& assets, const std::string& prefix);

        virtual States queryState() const = 0;
    };

    class IdlingState : public PlayerState
    {
    private:
        void walk(ConcreteEntityPtr player) override;
        void run(ConcreteEntityPtr player) override;
        void climb(ConcreteEntityPtr player) override;
        void evade(ConcreteEntityPtr player) override;
        void jump(ConcreteEntityPtr player) override;
        void fall(ConcreteEntityPtr player) override;
        void attack(ConcreteEntityPtr player) override;
        void defend(ConcreteEntityPtr player) override;
        void hurt(ConcreteEntityPtr player) override;

    public:
        static PlayerState* getInstance()
        {
            static IdlingState instance {};

            return &instance;
        }

        States queryState() const override { return Idling; };
    };
}   // namespace CyberCity

#endif
