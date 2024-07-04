// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: main.cpp
// Author: crdrisko
// Date: 07/01/2024-13:48:46
// Description:

#include <imgui-SFML.h>
#include <imgui.h>

#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "lightingSystem/intersection.hpp"

template<typename... TArgs>
sf::ConvexShape createPolygon(TArgs... args)
{
    const std::size_t n = sizeof...(TArgs);
    sf::ConvexShape convex {n};
    int count {};

    (convex.setPoint(count++, args), ...);

    convex.setFillColor(sf::Color::Black);
    convex.setOutlineColor(sf::Color {128, 128, 128});
    convex.setOutlineThickness(1);

    return convex;
}

std::vector<sf::ConvexShape> createPolygons()
{
    // Point2D a {100.f, 150.f}, b {120.f, 50.f}, c {200.f, 80.f}, d {140.f, 210.f};

    sf::ConvexShape convex1 = createPolygon(
        sf::Vector2f(100.f, 150.f), sf::Vector2f(120.f, 50.f), sf::Vector2f(200.f, 80.f), sf::Vector2f(140.f, 210.f));

    sf::ConvexShape convex2
        = createPolygon(sf::Vector2f(100.f, 200.f), sf::Vector2f(120.f, 250.f), sf::Vector2f(60.f, 300.f));

    sf::ConvexShape convex3 = createPolygon(
        sf::Vector2f(200.f, 260.f), sf::Vector2f(220.f, 150.f), sf::Vector2f(300.f, 200.f), sf::Vector2f(350.f, 320.f));

    sf::ConvexShape convex4 = createPolygon(sf::Vector2f(540.f, 60.f), sf::Vector2f(560.f, 40.f), sf::Vector2f(570.f, 70.f));

    sf::ConvexShape convex5 = createPolygon(
        sf::Vector2f(650.f, 190.f), sf::Vector2f(760.f, 170.f), sf::Vector2f(740.f, 270.f), sf::Vector2f(630.f, 290.f));

    sf::ConvexShape convex6
        = createPolygon(sf::Vector2f(600.f, 95.f), sf::Vector2f(780.f, 50.f), sf::Vector2f(680.f, 150.f));

    std::vector<sf::ConvexShape> shapes {convex1, convex2, convex3, convex4, convex5, convex6};

    return shapes;
}

sf::VertexArray getRay(const Point2D& a, const Point2D& b, sf::Color color)
{
    sf::VertexArray lines(sf::LinesStrip, 2);

    lines[0].position.x = a.x;
    lines[0].position.y = a.y;
    lines[0].color      = color;

    lines[1].position.x = b.x;
    lines[1].position.y = b.y;
    lines[1].color      = color;

    return lines;
}

Point2D projectOutX(const Point2D& a, const Point2D& b, const Point2D& c)
{
    Point2D result {c.x, c.y};

    DryPhys::real slope = (a.y - b.y) / (a.x - b.x);

    result.y = slope * (result.x - b.x) + b.y;

    return result;
}

Point2D projectOutY(const Point2D& a, const Point2D& b, const Point2D& c)
{
    Point2D result {c.x, c.y};

    DryPhys::real slope = (a.y - b.y) / (a.x - b.x);

    result.x = (result.y - b.y) / slope + b.x;

    return result;
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(840, 360), "Lighting System Demo", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window))
        return -1;

    std::vector<sf::ConvexShape> shapes = createPolygons();
    std::vector<sf::VertexArray> rays;

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

        window.clear(sf::Color::Black);
        rays.clear();

        auto mPos = sf::Mouse::getPosition(window);
        Point2D mPoint {static_cast<float>(mPos.x), static_cast<float>(mPos.y)};

        auto windowSize = window.getSize();
        Point2D topLeft {0.0f, 0.0f};
        Point2D topRight {static_cast<float>(windowSize.x), 0.0f};
        Point2D bottomLeft {0.0f, static_cast<float>(windowSize.y)};
        Point2D bottomRight {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)};

        for (const auto& shape : shapes)
        {
            window.draw(shape);

            for (std::size_t i {}; i < shape.getPointCount(); ++i)
            {
                auto vertex = shape.getPoint(i);
                Point2D vertexPoint {vertex.x, vertex.y};

                Point2D projected {};

                if (vertexPoint.x > mPos.x)
                {   // Right
                    if (vertexPoint.y > mPos.y)
                    {   // Bottom
                        projected = projectOutX(mPoint, vertexPoint, bottomRight);
                    }
                    else if (vertexPoint.y < mPos.y)
                    {   // Top
                        projected = projectOutY(mPoint, vertexPoint, topRight);
                    }
                    else
                    {   // Equal
                        projected = Point2D {static_cast<float>(windowSize.x), vertex.y};
                    }
                }
                else if (vertexPoint.x < mPos.x)
                {   // Left
                    if (vertexPoint.y > mPos.y)
                    {   // Bottom
                        projected = projectOutX(mPoint, vertexPoint, bottomLeft);
                    }
                    else if (vertexPoint.y < mPos.y)
                    {   // Top
                        projected = projectOutY(mPoint, vertexPoint, topLeft);
                    }
                    else
                    {   // Equal
                        projected = Point2D {0.0f, vertex.y};
                    }
                }
                else
                {   // Equal
                    if (vertexPoint.y > mPos.y)
                    {   // Bottom
                        projected = Point2D {vertex.x, static_cast<float>(windowSize.y)};
                    }
                    else if (vertexPoint.y < mPos.y)
                    {   // Top
                        projected = Point2D {vertex.x, 0.0f};
                    }
                    else
                    {   // Equal
                        continue;
                    }
                }

                float angle = std::atan2(projected.y, projected.x);

                float dx1 = std::cos(angle - 0.001f);
                float dy1 = std::sin(angle - 0.001f);

                float dx2 = std::cos(angle + 0.001f);
                float dy2 = std::sin(angle + 0.001f);

                float length = std::sqrt(projected.x * projected.x + projected.y * projected.y);

                rays.push_back(getRay(mPoint, Point2D {length * dx1, length * dy1}, sf::Color::Red));
                rays.push_back(getRay(mPoint, projected, sf::Color::Red));
                rays.push_back(getRay(mPoint, Point2D {length * dx2, length * dy2}, sf::Color::Red));
            }
        }

        rays.push_back(getRay(mPoint, topLeft, sf::Color::Red));
        rays.push_back(getRay(mPoint, topRight, sf::Color::Red));
        rays.push_back(getRay(mPoint, bottomLeft, sf::Color::Red));
        rays.push_back(getRay(mPoint, bottomRight, sf::Color::Red));

        for (auto& ray : rays)
        {
            for (const auto& shape : shapes)
            {
                std::size_t verticies = shape.getPointCount();

                for (std::size_t i {}; i < verticies; ++i)
                {
                    Point2D a {ray[0].position.x, ray[0].position.y};
                    Point2D b {ray[1].position.x, ray[1].position.y};

                    auto vertex1 = shape.getPoint(i);
                    Point2D c {vertex1.x, vertex1.y};

                    auto vertex2 = shape.getPoint((i + 1) % verticies);
                    Point2D d {vertex2.x, vertex2.y};

                    auto [doesIntesect, intersection] = lineIntersection(a, b, c, d);

                    if (doesIntesect)
                        ray = getRay(mPoint, intersection, sf::Color::Red);
                }
            }
        }

        std::sort(rays.begin(),
            rays.end(),
            [](auto a, auto b)
            {
                DryPhys::Vector3D a_res
                    = Point2D {a[1].position.x, a[1].position.y} - Point2D {a[0].position.x, a[0].position.y};
                DryPhys::Vector3D b_res
                    = Point2D {b[1].position.x, b[1].position.y} - Point2D {b[0].position.x, b[0].position.y};

                return std::atan2(a_res[1], a_res[0]) < std::atan2(b_res[1], b_res[0]);
            });

        sf::VertexArray va {sf::TriangleStrip, 4 * rays.size()};

        for (std::size_t i {}; i < rays.size(); ++i)
        {
            va[4 * i + 0].position.x = rays[i][0].position.x;
            va[4 * i + 0].position.y = rays[i][0].position.y;
            va[4 * i + 0].color      = sf::Color(255, 255, 255, 255);

            va[4 * i + 1].position.x = rays[i][1].position.x;
            va[4 * i + 1].position.y = rays[i][1].position.y;
            va[4 * i + 1].color      = sf::Color(255, 255, 255, 255);

            std::size_t next {(i + 1) % rays.size()};
            va[4 * i + 2].position.x = rays[next][1].position.x;
            va[4 * i + 2].position.y = rays[next][1].position.y;
            va[4 * i + 2].color      = sf::Color(255, 255, 255, 255);

            va[4 * i + 3].position.x = rays[i][0].position.x;
            va[4 * i + 3].position.y = rays[i][0].position.y;
            va[4 * i + 3].color      = sf::Color(255, 255, 255, 255);
        }

        window.draw(va);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
