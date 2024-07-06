// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: ai.hpp
// Author: crdrisko
// Date: 07/01/2024-14:16:22
// Description:

#ifndef DRYPHYS_SRC_DEMOS_LIGHTCYCLE_AI_HPP
#define DRYPHYS_SRC_DEMOS_LIGHTCYCLE_AI_HPP

#include <memory>
#include <string>

#include <engine2d/entity.hpp>

#include "lightCycle/forwardDeclare.hpp"

namespace LightCycle
{
    class AI
    {
    protected:
        std::string name_ = "Default";

    public:
        AI()          = default;
        virtual ~AI() = default;

        const std::string& name() const { return name_; }

        virtual std::size_t getAction(const ScenePlay& battle, ConcreteEntityPtr entity) = 0;
    };

    class AI_Bumpers : public AI
    {
    public:
        AI_Bumpers() { name_ = "Bumpers"; }

        std::size_t getAction(const ScenePlay& battle, ConcreteEntityPtr entity) override;
    };

    class AI_Random : public AI
    {
    public:
        AI_Random() { name_ = "Random"; }

        std::size_t getAction(const ScenePlay& battle, ConcreteEntityPtr entity) override;
    };

    class AI_RandomSafe : public AI
    {
    public:
        AI_RandomSafe() { name_ = "RandomSafe"; }

        std::size_t getAction(const ScenePlay& battle, ConcreteEntityPtr entity) override;
    };

    class AI_GoStraight : public AI
    {
    public:
        AI_GoStraight() { name_ = "GoStraight"; }

        std::size_t getAction(const ScenePlay& battle, ConcreteEntityPtr entity) override;
    };

    // Create the AI's using the factory design pattern
    class AIFactory
    {
    private:
        const std::string name_;

    public:
        explicit AIFactory(const std::string& name) : name_ {name} {}

        std::shared_ptr<AI> create() const;
    };
}   // namespace LightCycle

#endif
