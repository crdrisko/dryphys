// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: window.hpp
// Author: crdrisko
// Date: 01/07/2025-06:19:52
// Description: A wrapper around a GLFWwindow object for easy window handling

#ifndef DRYPHYS_INCLUDE_PARTITIONENGINE_WINDOW_WINDOW_HPP
#define DRYPHYS_INCLUDE_PARTITIONENGINE_WINDOW_WINDOW_HPP

#include <queue>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "partitionEngine/graphics/color.hpp"
#include "partitionEngine/window/event.hpp"

namespace PartitionEngine
{
    class Window
    {
    private:
        int width_, height_;
        GLFWwindow* window_;

        std::queue<Event> events_;

        void init();
        void resize(int width, int height);
        void spawnEvent(const Event& event) { events_.push(event); }

    public:
        //! Constructors
        Window();
        Window(int width, int height, const std::string& title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);

        //! Destructor
        ~Window();

        /*!
         * Creates a GLFWwindow and initializes ImGui. 
         *
         * @param width 
         * @param height 
         * @param title 
         * @param monitor 
         * @param share 
         */
        void create(
            int width, int height, const std::string& title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
        bool isOpen();
        void pollEvents();
        void clear(const ColorRGBA& clearColor = Colors::Black);
        void display();

        GLFWwindow* window() { return window_; }

        const std::queue<Event>& getEvents() const { return events_; }
        std::queue<Event>& getEvents() { return events_; }
    };
}   // namespace PartitionEngine

#endif
