// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: scene.hpp
// Author: crdrisko
// Date: 06/18/2024-05:23:45
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE2D_SCENE_HPP
#define DRYPHYS_INCLUDE_ENGINE2D_SCENE_HPP

#include <cstddef>
#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "dryphys/vector3d.hpp"
#include "engine2d/forwardDeclare.hpp"

namespace Engine2D
{
    class Scene
    {
        using ActionMap = std::map<int, std::string>;

    protected:
        Engine* game_ {nullptr};
        std::size_t currentFrame_ {};
        ActionMap actionMap_;

        bool paused_ {};

        void setPaused(bool paused) { paused_ = paused; }

        virtual void update()                        = 0;
        virtual void sDoAction(const Action& action) = 0;
        virtual void onEnd()                         = 0;

    public:
        Scene() = default;
        explicit Scene(Engine* engine) : game_ {engine} {}
        virtual ~Scene() = default;

        virtual void sRender() = 0;

        void doAction(const Action& action);
        void simulate(const std::size_t frames);
        void registerAction(int inputKeyCode, const std::string& actionName);
        void drawLine(const DryPhys::Vector3D& p1, const DryPhys::Vector3D& p2);
        void drawLine(float x1, float y1, float x2, float y2, sf::Color color);

        std::size_t currentFrame() const { return currentFrame_; }
        const ActionMap& getActionMap() const { return actionMap_; }
    };
}   // namespace Engine2D

#endif
