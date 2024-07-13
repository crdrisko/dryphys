// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: ai.hpp
// Author: crdrisko
// Date: 07/09/2024-14:00:56
// Description:

#ifndef AI_HPP
#define AI_HPP

#include <memory>
#include <string>

namespace CyberCity
{
    class ScenePlay;
    class Entity;

    class AI
    {
    protected:
        std::string name_ = "Default";

    public:
        AI()          = default;
        virtual ~AI() = default;

        const std::string& name() const { return name_; }

        virtual std::size_t getAction(const ScenePlay& battle, std::shared_ptr<Entity> entity) = 0;
    };

    class AI_Patrol : public AI
    {
    public:
        AI_Patrol() { name_ = "Patrol"; }

        std::size_t getAction(const ScenePlay& battle, std::shared_ptr<Entity> entity) override;
    };

    class AI_Follow : public AI
    {
    public:
        AI_Follow() { name_ = "Follow"; }

        std::size_t getAction(const ScenePlay& battle, std::shared_ptr<Entity> entity) override;
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
}   // namespace CyberCity

#endif
