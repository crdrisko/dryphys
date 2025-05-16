// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: window.cpp
// Author: crdrisko
// Date: 01/07/2025-06:21:41
// Description: A wrapper around a GLFWwindow object for easy window handling

#include "partitionEngine/window/window.hpp"

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <drychem/drychem.hpp>
#include <glad/glad.h>

#ifdef USE_IMGUI
    #include <imgui.h>
    #include <imgui_impl_glfw.h>
    #include <imgui_impl_opengl3.h>
#endif

#include "partitionEngine/graphics/color.hpp"
#include "partitionEngine/window/event.hpp"

namespace PartitionEngine
{
    Window::Window()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }

    Window::Window(int width, int height, const std::string& title, GLFWmonitor* monitor, GLFWwindow* share) : Window {}
    {
        create(width, height, title, monitor, share);
    }

    Window::~Window()
    {
#ifdef USE_IMGUI
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
#endif

        if (window_)
            glfwDestroyWindow(window_);

        glfwTerminate();
    }

    void Window::init()
    {
        if (!window_)
        {
            throw DryChem::FatalException {
                DryChem::ErrorMessage {"glfw", "Failed to create GLFW window", __FILE__, __LINE__}};
        }

        glfwMakeContextCurrent(window_);
        glfwSetWindowUserPointer(window_, this);

        auto window_close_callback = [](GLFWwindow* window)
        { static_cast<Window*>(glfwGetWindowUserPointer(window))->spawnEvent(Event {Event::Closed {}}); };

        auto framebuffer_size_callback = [](GLFWwindow* window, int width, int height)
        { static_cast<Window*>(glfwGetWindowUserPointer(window))->spawnEvent(Event {Event::Resize {width, height}}); };

        auto scroll_callback = [](GLFWwindow* window, double x, double y)
        { static_cast<Window*>(glfwGetWindowUserPointer(window))->spawnEvent(Event {Event::Scroll {x, y}}); };

        auto cursor_pos_callback = [](GLFWwindow* window, double xpos, double ypos)
        { static_cast<Window*>(glfwGetWindowUserPointer(window))->spawnEvent(Event {Event::MouseMoved {xpos, ypos}}); };

        // Register our callbacks
        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
        glfwSetWindowCloseCallback(window_, window_close_callback);
        glfwSetScrollCallback(window_, scroll_callback);
        glfwSetCursorPosCallback(window_, cursor_pos_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw DryChem::FatalException {DryChem::ErrorMessage {"glad", "Failed to initialize GLAD.", __FILE__, __LINE__}};
        }

        glEnable(GL_DEPTH_TEST);
    }

    void Window::create(int width, int height, const std::string& title, GLFWmonitor* monitor, GLFWwindow* share)
    {
        width_  = width;
        height_ = height;

        window_ = glfwCreateWindow(width, height, title.c_str(), monitor, share);

        try
        {
            init();
        }
        catch (const DryChem::FatalException& except)
        {
            except.handleErrorWithMessage();
        }

#ifdef USE_IMGUI
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
        // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // IF using Docking Branch

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window_, true);
        ImGui_ImplOpenGL3_Init();
#endif
    }

    void Window::resize(int width, int height)
    {
        // Avoid divide by zero
        if (height <= 0)
            height = 1;

        width_  = width;
        height_ = height;
        glViewport(0, 0, width, height);
    }

    bool Window::isOpen() { return !glfwWindowShouldClose(window_); }

    void Window::pollEvents()
    {
        glfwPollEvents();

#ifdef USE_IMGUI
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
#endif
    }

    void Window::clear(const ColorRGBA& clearColor)
    {
        const ColorGL colorGL = clearColor.convertToColorGL();

        glClearColor(colorGL.r, colorGL.g, colorGL.b, colorGL.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::display()
    {
#ifdef USE_IMGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

        glfwSwapBuffers(window_);
    }
}   // namespace PartitionEngine
