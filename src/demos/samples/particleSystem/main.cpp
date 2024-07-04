// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: main.cpp
// Author: crdrisko
// Date: 07/01/2024-07:43:16
// Description:

#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>

#include <SFML/Graphics.hpp>

#include "particleSystem/particleSystem.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1760, 800), "Particle System Demo");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window))
        return -1;

    static const std::size_t MAX_PARTICLES {16'384};

    ParticleSystem particles {MAX_PARTICLES};
    particles.init(window.getSize());

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        // ImGui::ShowDemoWindow();

        ImGui::SliderInt("Size of Particles", particles.size(), 0, 12);
        ImGui::SliderInt("Number of Particles", particles.count(), 0, MAX_PARTICLES);
        ImGui::ColorEdit3("Particle Color", particles.color());

        particles.update();

        window.clear();
        particles.draw(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
