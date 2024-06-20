// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: engine.hpp
// Author: crdrisko
// Date: 06/18/2024-05:22:08
// Description:

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <map>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include "engine2d/assets.hpp"
#include "engine2d/scene.hpp"

namespace Engine2D
{
    using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

    class Engine
    {
    protected:
        sf::RenderWindow window_;
        Assets assets_;
        std::string currentScene_;
        SceneMap sceneMap_;
        std::size_t simulationSpeed_ = 1;
        bool running_ {true};

        void init(const std::string& assets);
        void update();
        void sUserInput();

        std::shared_ptr<Scene> currentScene() { return sceneMap_[currentScene_]; }

    public:
        explicit Engine(const std::string& assets);

        template<typename T>
        void setDefaultScene(const std::string& sceneName)
        {
            changeScene(sceneName, std::make_shared<T>(this));
        }

        void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = true);

        void run();
        void quit();

        sf::RenderWindow& window() { return window_; }
        Assets& assets() { return assets_; }
        bool isRunning() { return running_ && window_.isOpen(); }
    };
}   // namespace Engine2D

#endif
