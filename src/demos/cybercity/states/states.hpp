// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: states.hpp
// Author: crdrisko
// Date: 06/20/2024-06:24:48
// Description:

#ifndef DRYPHYS_SRC_DEMOS_CYBERCITY_STATES_STATES_HPP
#define DRYPHYS_SRC_DEMOS_CYBERCITY_STATES_STATES_HPP

#include <memory>

#include "cybercity/forwardDeclare.hpp"
#include "cybercity/states/playerStates.hpp"

namespace CyberCity
{
    class WalkingState : public PlayerState
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
            static WalkingState instance {};

            return &instance;
        }

        States queryState() const override { return Walking; };
    };

    class RunningState : public PlayerState
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
            static RunningState instance {};

            return &instance;
        }

        States queryState() const override { return Running; };
    };

    class ClimbingState : public PlayerState
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
            static ClimbingState instance {};

            return &instance;
        }

        States queryState() const override { return Climbing; };
    };

    class EvadingState : public PlayerState
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
            static EvadingState instance {};

            return &instance;
        }

        States queryState() const override { return Evading; };
    };

    class JumpingState : public PlayerState
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
            static JumpingState instance {};

            return &instance;
        }

        States queryState() const override { return Jumping; };
    };

    class FallingState : public PlayerState
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
            static FallingState instance {};

            return &instance;
        }

        States queryState() const override { return Falling; };
    };

    class AttackingState : public PlayerState
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
            static AttackingState instance {};

            return &instance;
        }

        States queryState() const override { return Attacking; };
    };

    class DefendingState : public PlayerState
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
            static DefendingState instance {};

            return &instance;
        }

        States queryState() const override { return Defending; };
    };

    class HurtingState : public PlayerState
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
            static HurtingState instance {};

            return &instance;
        }

        States queryState() const override { return Hurting; };
    };
}   // namespace CyberCity

#endif
