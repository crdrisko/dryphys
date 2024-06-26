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

namespace CyberCity
{
    class Entity;

    class PlayerState
    {
    protected:
        void changeState(std::shared_ptr<Entity> player, PlayerState* state);

        // Actions
        virtual void walk(std::shared_ptr<Entity>) {}
        virtual void run(std::shared_ptr<Entity>) {}
        virtual void climb(std::shared_ptr<Entity>) {}
        virtual void evade(std::shared_ptr<Entity>) {}
        virtual void jump(std::shared_ptr<Entity>) {}
        virtual void attack(std::shared_ptr<Entity>) {}
        virtual void defend(std::shared_ptr<Entity>) {}
        virtual void hurt(std::shared_ptr<Entity>) {}

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

        void handleActions(std::shared_ptr<Entity> player, const Engine2D::Action& action);
        void handleInput(std::shared_ptr<Entity> player);
        void handleAnimations(std::shared_ptr<Entity> player, Engine2D::Assets& assets, const std::string& prefix);

        virtual States queryState() const = 0;
    };

    class IdlingState : public PlayerState
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
            static IdlingState instance {};

            return &instance;
        }

        States queryState() const override { return Idling; };
    };
}   // namespace CyberCity

#endif
