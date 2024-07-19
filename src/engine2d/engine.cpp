// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: engine.cpp
// Author: crdrisko
// Date: 06/18/2024-05:33:31
// Description:

#include "engine2d/engine.hpp"

#ifdef USE_IMGUI
    #include <imgui-SFML.h>
    #include <imgui.h>
#endif

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include "engine2d/action.hpp"
#include "engine2d/assets.hpp"
#include "engine2d/scene.hpp"

namespace Engine2D
{
    Engine::Engine(const std::string& assets, bool createWindow) : createWindow_ {createWindow} { init(assets); }

    void Engine::init(const std::string& assets)
    {
        assets_.loadFromFile(assets);

        if (createWindow_)
        {
            auto& [name, width, height, fps, fullscreen] = assets_.getWindowConfig();

            window_.create(sf::VideoMode(width, height), name, (fullscreen + sf::Style::Default));
            window_.setFramerateLimit(fps);

#ifdef USE_IMGUI
            if (!ImGui::SFML::Init(window_))
                std::exit(EXIT_FAILURE);
#endif
        }
    }

    void Engine::update()
    {
        if (sceneMap_.empty())
        {
            std::cerr << "No scene set - try calling setDefaultScene() before run().\n";
            quit();
            return;
        }

        processInput();
        currentScene()->simulate(simulationSpeed_);
        currentScene()->sRender();

#ifdef USE_IMGUI
        ImGui::SFML::Render(window_);
#endif
        window_.display();
    }

    void Engine::processInput()
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
                // If the current scene does not have an action associated with this key, skip the event
                if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
                {
                    continue;
                }

                const Action::Types actionType = (event.type == sf::Event::KeyPressed) ? Action::START : Action::END;

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
                    currentScene()->doAction(Action {"LEFT_CLICK", Action::START, mpos});
                    break;
                }
                case sf::Mouse::Middle:
                {
                    currentScene()->doAction(Action {"MIDDLE_CLICK", Action::START, mpos});
                    break;
                }
                case sf::Mouse::Right:
                {
                    currentScene()->doAction(Action {"RIGHT_CLICK", Action::START, mpos});
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
                    currentScene()->doAction(Action {"LEFT_CLICK", Action::END, mpos});
                    break;
                }
                case sf::Mouse::Middle:
                {
                    currentScene()->doAction(Action {"MIDDLE_CLICK", Action::END, mpos});
                    break;
                }
                case sf::Mouse::Right:
                {
                    currentScene()->doAction(Action {"RIGHT_CLICK", Action::END, mpos});
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

                currentScene()->doAction(Action {"MOUSE_MOVE", Action::START, mouseMoved});
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
        // auto previous = std::chrono::high_resolution_clock::now();
        // long long lag {};

        while (isRunning())
        {
#ifdef USE_IMGUI
            ImGui::SFML::Update(window_, deltaClock_.restart());
#endif
            // auto current = std::chrono::high_resolution_clock::now();
            // auto elapsed = current - previous;
            // previous     = current;
            // lag += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

            update();
        }

#ifdef USE_IMGUI
        ImGui::SFML::Shutdown();
#endif
    }

    void Engine::quit() { running_ = false; }
}   // namespace Engine2D
