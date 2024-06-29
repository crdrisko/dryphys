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

#include "cybercity/entity.hpp"
#include "cybercity/states/playerStates.hpp"

namespace CyberCity
{
    class WalkingState : public PlayerState
    {
    private:
        void walk(std::shared_ptr<Entity> player) override;
        void run(std::shared_ptr<Entity> player) override;
        void climb(std::shared_ptr<Entity> player) override;
        void evade(std::shared_ptr<Entity> player) override;
        void jump(std::shared_ptr<Entity> player) override;
        void attack(std::shared_ptr<Entity> player) override;
        void defend(std::shared_ptr<Entity> player) override;
        void hurt(std::shared_ptr<Entity> player) override;

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
        void walk(std::shared_ptr<Entity> player) override;
        void run(std::shared_ptr<Entity> player) override;
        void climb(std::shared_ptr<Entity> player) override;
        void evade(std::shared_ptr<Entity> player) override;
        void jump(std::shared_ptr<Entity> player) override;
        void attack(std::shared_ptr<Entity> player) override;
        void defend(std::shared_ptr<Entity> player) override;
        void hurt(std::shared_ptr<Entity> player) override;

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
        void walk(std::shared_ptr<Entity> player) override;
        void run(std::shared_ptr<Entity> player) override;
        void climb(std::shared_ptr<Entity> player) override;
        void evade(std::shared_ptr<Entity> player) override;
        void jump(std::shared_ptr<Entity> player) override;
        void attack(std::shared_ptr<Entity> player) override;
        void defend(std::shared_ptr<Entity> player) override;
        void hurt(std::shared_ptr<Entity> player) override;

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
        void walk(std::shared_ptr<Entity> player) override;
        void run(std::shared_ptr<Entity> player) override;
        void climb(std::shared_ptr<Entity> player) override;
        void evade(std::shared_ptr<Entity> player) override;
        void jump(std::shared_ptr<Entity> player) override;
        void attack(std::shared_ptr<Entity> player) override;
        void defend(std::shared_ptr<Entity> player) override;
        void hurt(std::shared_ptr<Entity> player) override;

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
        void walk(std::shared_ptr<Entity> player) override;
        void run(std::shared_ptr<Entity> player) override;
        void climb(std::shared_ptr<Entity> player) override;
        void evade(std::shared_ptr<Entity> player) override;
        void jump(std::shared_ptr<Entity> player) override;
        void attack(std::shared_ptr<Entity> player) override;
        void defend(std::shared_ptr<Entity> player) override;
        void hurt(std::shared_ptr<Entity> player) override;

    public:
        static PlayerState* getInstance()
        {
            static JumpingState instance {};

            return &instance;
        }

        States queryState() const override { return Jumping; };
    };

    class AttackingState : public PlayerState
    {
    private:
        void walk(std::shared_ptr<Entity> player) override;
        void run(std::shared_ptr<Entity> player) override;
        void climb(std::shared_ptr<Entity> player) override;
        void evade(std::shared_ptr<Entity> player) override;
        void jump(std::shared_ptr<Entity> player) override;
        void attack(std::shared_ptr<Entity> player) override;
        void defend(std::shared_ptr<Entity> player) override;
        void hurt(std::shared_ptr<Entity> player) override;

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
        void walk(std::shared_ptr<Entity> player) override;
        void run(std::shared_ptr<Entity> player) override;
        void climb(std::shared_ptr<Entity> player) override;
        void evade(std::shared_ptr<Entity> player) override;
        void jump(std::shared_ptr<Entity> player) override;
        void attack(std::shared_ptr<Entity> player) override;
        void defend(std::shared_ptr<Entity> player) override;
        void hurt(std::shared_ptr<Entity> player) override;

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
        void walk(std::shared_ptr<Entity> player) override;
        void run(std::shared_ptr<Entity> player) override;
        void climb(std::shared_ptr<Entity> player) override;
        void evade(std::shared_ptr<Entity> player) override;
        void jump(std::shared_ptr<Entity> player) override;
        void attack(std::shared_ptr<Entity> player) override;
        void defend(std::shared_ptr<Entity> player) override;
        void hurt(std::shared_ptr<Entity> player) override;

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
