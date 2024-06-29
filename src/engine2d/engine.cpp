// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: engine.cpp
// Author: crdrisko
// Date: 06/18/2024-05:33:31
// Description:

#include "engine2d/engine.hpp"

#include <iostream>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include "engine2d/action.hpp"
#include "engine2d/assets.hpp"
#include "engine2d/scene.hpp"

namespace Engine2D
{
    Engine::Engine(const std::string& assets) { init(assets); }

    void Engine::init(const std::string& assets)
    {
        assets_.loadFromFile(assets);

        auto& [name, width, height, fps, fullscreen] = assets_.getWindowConfig();

        window_.create(sf::VideoMode(width, height), name, (fullscreen + sf::Style::Default));
        window_.setFramerateLimit(fps);
    }

    void Engine::update()
    {
        if (sceneMap_.empty())
        {
            std::cerr << "No scene set - try calling setDefaultScene() before run().\n";
            quit();
            return;
        }

        sUserInput();
        currentScene()->simulate(simulationSpeed_);
        currentScene()->sRender();

        window_.display();
    }

    void Engine::sUserInput()
    {
        sf::Event event;
        while (window_.pollEvent(event))
        {
#ifdef USE_IMGUI
            ImGui::SFML::ProcessEvent(window_, event);
#endif
            if (event.type == sf::Event::Closed)
            {
                quit();
            }

            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
            {
                // if the current scene does not have an action associated with this key, skip the event
                if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
                {
                    continue;
                }

                // determine start or end action by whether it was a key press or release
                const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

                // look up the action and send the action to the scene
                currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
            }

            auto mousePos = sf::Mouse::getPosition(window_);
            DryPhys::Vector3D mpos {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y), 0};

            if (event.type == sf::Event::MouseButtonPressed)
            {
                switch (event.mouseButton.button)
                {
                case sf::Mouse::Left:
                {
                    currentScene()->doAction(Action {"LEFT_CLICK", "START", mpos});
                    break;
                }
                case sf::Mouse::Middle:
                {
                    currentScene()->doAction(Action {"MIDDLE_CLICK", "START", mpos});
                    break;
                }
                case sf::Mouse::Right:
                {
                    currentScene()->doAction(Action {"RIGHT_CLICK", "START", mpos});
                    break;
                }
                default:
                    break;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                switch (event.mouseButton.button)
                {
                case sf::Mouse::Left:
                {
                    currentScene()->doAction(Action {"LEFT_CLICK", "END", mpos});
                    break;
                }
                case sf::Mouse::Middle:
                {
                    currentScene()->doAction(Action {"MIDDLE_CLICK", "END", mpos});
                    break;
                }
                case sf::Mouse::Right:
                {
                    currentScene()->doAction(Action {"RIGHT_CLICK", "END", mpos});
                    break;
                }
                default:
                    break;
                }
            }

            if (event.type == sf::Event::MouseMoved)
            {
                DryPhys::Vector3D mouseMoved {
                    static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y), 0};

                currentScene()->doAction(Action {"MOUSE_MOVE", "START", mouseMoved});
            }
        }
    }

    void Engine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
    {
        if (endCurrentScene)
        {
            if (auto it = sceneMap_.find(currentScene_); it != sceneMap_.end())
                sceneMap_.erase(it);
        }

        if (sceneMap_.find(sceneName) == sceneMap_.end())
        {
            sceneMap_[sceneName] = scene;
        }

        currentScene_ = sceneName;
    }

    void Engine::run()
    {
        while (isRunning())
        {
            update();
        }
    }

    void Engine::quit() { running_ = false; }
}   // namespace Engine2D
