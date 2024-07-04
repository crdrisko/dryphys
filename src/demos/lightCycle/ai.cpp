// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: ai.cpp
// Author: crdrisko
// Date: 07/01/2024-14:16:24
// Description:

#include "lightCycle/ai.hpp"

#include <iostream>
#include <memory>
#include <queue>
#include <vector>

#include "lightCycle/components.hpp"
#include "lightCycle/scenes/scenePlay.hpp"

namespace LightCycle
{
    std::size_t AI_Student::getAction(const ScenePlay&, ConcreteEntityPtr)
    {
        std::size_t actionToTake {};

        return actionToTake;
    }

    std::size_t AI_Random::getAction(const ScenePlay& battle, ConcreteEntityPtr entity)
    {
        // get all the legal actions for this cycle
        std::vector<std::size_t> legalActions = battle.getLegalActions(entity);

        // choose a random action index to perform
        std::size_t randomAction = rand() % legalActions.size();

        return legalActions[randomAction];
    }

    std::size_t AI_RandomSafe::getAction(const ScenePlay& battle, ConcreteEntityPtr entity)
    {
        // figure out which actions are 'safe' (won't immediately die)
        std::vector<std::size_t> safeActions;

        auto& eTransform = entity->getComponent<CTransform>();

        // look through each legal action
        for (auto action : battle.getLegalActions(entity))
        {
            // calculate the next position of the cycle if it took that action
            Direction dir = battle.getDirection(action);

            int newX = eTransform.pos[0] + dir.x;
            int newY = eTransform.pos[1] + dir.y;

            if (battle.isSafe(newX, newY))
                safeActions.push_back(action);
        }

        // if we couldn't find any safe actions, don't turn
        if (safeActions.size() == 0)
            return eTransform.dir;

        // otherwise, choose a random safe action index to perform
        std::size_t randomAction = rand() % safeActions.size();

        return safeActions[randomAction];
    }

    std::size_t AI_GoStraight::getAction(const ScenePlay& battle, ConcreteEntityPtr entity)
    {
        auto& eTransform = entity->getComponent<CTransform>();

        // compute where the cycle will be if it goes straight
        Direction dir = battle.getDirection(eTransform.dir);

        int newX = eTransform.pos[0] + dir.x;
        int newY = eTransform.pos[1] + dir.y;

        // if that is a safe location, keep going straight
        if (battle.isSafe(newX, newY))
            return eTransform.dir;

        // otherwise choose a random safe action index to perform using the existing AI
        AI_RandomSafe randomSafe;
        return randomSafe.getAction(battle, entity);
    }

    std::shared_ptr<AI> AIFactory::create() const
    {
        if (name_ == "Student")
            return std::make_shared<AI_Student>();

        else if (name_ == "Random")
            return std::make_shared<AI_Random>();

        else if (name_ == "RandomSafe")
            return std::make_shared<AI_RandomSafe>();

        else if (name_ == "GoStraight")
            return std::make_shared<AI_GoStraight>();

        else
            return std::make_shared<AI_Random>();
    }
}   // namespace LightCycle
