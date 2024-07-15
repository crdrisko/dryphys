// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: engine.hpp
// Author: crdrisko
// Date: 06/18/2024-05:22:08
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE2D_ENGINE_HPP
#define DRYPHYS_INCLUDE_ENGINE2D_ENGINE_HPP

#include <map>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include "engine2d/assets.hpp"
#include "engine2d/scene.hpp"

namespace Engine2D
{
    class Engine
    {
        using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

    private:
        sf::RenderWindow window_;
        sf::Clock deltaClock_;
        Assets assets_;
        std::string currentScene_;
        SceneMap sceneMap_;
        std::size_t simulationSpeed_ {1};

        bool running_ {true};
        bool createWindow_ {true};

        void init(const std::string& assets);
        void update();
        void processInput();

        std::shared_ptr<Scene> currentScene() { return sceneMap_[currentScene_]; }

    public:
        explicit Engine(const std::string& assets, bool createWindow = true);

        template<typename T, typename... TArgs>
        void setDefaultScene(const std::string& sceneName, TArgs... args)
        {
            changeScene(sceneName, std::make_shared<T>(this, args...));
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
