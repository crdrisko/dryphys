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

#include "dryphys/vector3d.hpp"

namespace Engine2D
{
    using ActionMap = std::map<int, std::string>;

    class Action;
    class Engine;

    class Scene
    {
    protected:
        Engine* game_ {nullptr};
        ActionMap actionMap_;

        bool paused_   = false;
        bool hasEnded_ = false;

        std::size_t currentFrame_ = 0;

        virtual void onEnd() = 0;

        void setPaused(bool paused) { paused_ = paused; }

        virtual void update()                        = 0;
        virtual void sDoAction(const Action& action) = 0;

    public:
        Scene();
        explicit Scene(Engine* engine);
        virtual ~Scene() = default;

        virtual void sRender() = 0;

        void doAction(const Action& action);
        void simulate(const std::size_t frames);
        void registerAction(int inputKeyCode, const std::string& actionName);
        void registerMouseAction(int inputMouseCode, const std::string& actionName);
        void drawLine(const DryPhys::Vector3D& p1, const DryPhys::Vector3D& p2);

        std::size_t width() const;
        std::size_t height() const;

        std::size_t currentFrame() const { return currentFrame_; }
        bool hasEnded() const { return hasEnded_; }
        const ActionMap& getActionMap() const { return actionMap_; }
    };
}   // namespace Engine2D

#endif
