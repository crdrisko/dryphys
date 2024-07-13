// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: ai.cpp
// Author: crdrisko
// Date: 07/09/2024-14:00:59
// Description:

#include "cybercity/ai.hpp"

#include <bitset>
#include <iostream>
#include <memory>
#include <vector>

#include "cybercity/scenes/scenePlay.hpp"

namespace CyberCity
{
    std::size_t AI_Patrol::getAction(const ScenePlay&, std::shared_ptr<Entity>) { return 0; }

    std::size_t AI_Follow::getAction(const ScenePlay&, std::shared_ptr<Entity>) { return 0; }

    std::shared_ptr<AI> AIFactory::create() const
    {
        if (name_ == "Patrol")
            return std::make_shared<AI_Patrol>();

        else if (name_ == "Follow")
            return std::make_shared<AI_Follow>();

        else
            return std::make_shared<AI_Patrol>();
    }
}   // namespace CyberCity