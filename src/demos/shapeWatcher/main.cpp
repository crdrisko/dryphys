// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: main.cpp
// Author: crdrisko
// Date: 06/19/2024-05:28:03
// Description:

#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <common-utils/files.hpp>
#include <common-utils/strings.hpp>
#include <dryphys/vector3d.hpp>

struct TextFormatting
{
    sf::Font font;
    int size;
    int r, g, b;
};

struct SFMLShapeWrapper
{
    std::string name {};
    TextFormatting format;
    DryPhys::Vector3D pos {};
    DryPhys::Vector3D speed {};
    std::shared_ptr<sf::Shape> shape {nullptr};
};

std::vector<SFMLShapeWrapper> parseInputFile(const std::string& fileName, int& width, int& height);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <config_file>\n";
        return EXIT_FAILURE;
    }

    int width {800}, height {600};
    std::vector<SFMLShapeWrapper> shapes = parseInputFile(argv[1], width, height);

    sf::RenderWindow window(sf::VideoMode(width, height), "Shape Watcher");
    // window.setFramerateLimit(60);

    sf::Text text;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (auto& shape : shapes)
        {
            window.draw(*shape.shape);

            text.setString(shape.name);

            text.setFont(shape.format.font);
            text.setCharacterSize(shape.format.size);
            text.setFillColor(sf::Color(shape.format.r, shape.format.g, shape.format.b));

            sf::FloatRect shapeBoundingBox = shape.shape->getGlobalBounds();

            if (shapeBoundingBox.getPosition().x < 0.0F
                || shapeBoundingBox.getPosition().x + shapeBoundingBox.width > static_cast<float>(width))
            {
                shape.speed[0] *= -1.0F;
            }

            if (shapeBoundingBox.getPosition().y < 0.0F
                || shapeBoundingBox.getPosition().y + shapeBoundingBox.height > static_cast<float>(height))
            {
                shape.speed[1] *= -1.0F;
            }

            DryPhys::Vector3D pos {shape.shape->getPosition().x, shape.shape->getPosition().y, 0};
            pos += shape.speed;

            shape.shape->setPosition(pos[0], pos[1]);

            sf::FloatRect textBoundingBox = text.getGlobalBounds();

            text.setPosition(shape.shape->getPosition().x + 0.5f * (shapeBoundingBox.width - textBoundingBox.width),
                shape.shape->getPosition().y + 0.5f * (shapeBoundingBox.height - textBoundingBox.height));

            window.draw(text);
        }

        window.display();
    }
}

std::vector<SFMLShapeWrapper> parseInputFile(const std::string& fileName, int& width, int& height)
{
    TextFormatting format {};
    std::vector<SFMLShapeWrapper> shapes;

    DryChem::FileParser parser {fileName};
    auto rows = parser.parseDataFile(DryChem::AsRows());

    for (const auto& row : rows)
    {
        DryChem::Tokenizer tok {row};
        std::vector<std::string> splitRow = tok.split();

        if (splitRow[0] == "Window")
        {
            /*
             * Window W H(first line)
             *  Width: W - int
             *  Height : H - int
             */
            width  = std::stoi(splitRow[1]);
            height = std::stoi(splitRow[2]);
        }
        else if (splitRow[0] == "Font")
        {
            /*
             * Font F S R G B
             *	File: F - std::string
             *  Size : S - int
             *  Color : (R, G, B) - int, int, int
             */
            if (!format.font.loadFromFile(splitRow[1]))
            {
                std::cerr << "Could not find font: " << splitRow[1] << '\n';
                std::exit(EXIT_FAILURE);
            }

            format.size = std::stoi(splitRow[2]);

            format.r = std::stoi(splitRow[3]);
            format.g = std::stoi(splitRow[4]);
            format.b = std::stoi(splitRow[5]);
        }
        else if (splitRow[0] == "Rectangle")
        {
            /*
             * Rectangle N X Y SX SY R G B W H
             *  Name : N - std::string
             *  Position : (X, Y) - float, float
             *  Speed : (SX, SY) - float, float
             *  Color : (R, G, B) - int, int, int
             *  Size : (W, H) - float, float
             */
            SFMLShapeWrapper rectShape {};

            rectShape.format = format;

            rectShape.name = splitRow[1];

            float x = std::stof(splitRow[2]);
            float y = std::stof(splitRow[3]);

            rectShape.pos = DryPhys::Vector3D {x, y, 0};

            float sx = std::stof(splitRow[4]);
            float sy = std::stof(splitRow[5]);

            rectShape.speed = DryPhys::Vector3D {sx, sy, 0};

            int r = std::stoi(splitRow[6]);
            int g = std::stoi(splitRow[7]);
            int b = std::stoi(splitRow[8]);

            float rectWidth  = std::stof(splitRow[9]);
            float rectHeight = std::stof(splitRow[10]);

            rectShape.shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(rectWidth, rectHeight));
            rectShape.shape->setFillColor(sf::Color(r, g, b));
            rectShape.shape->setPosition(x, y);

            shapes.push_back((rectShape));
        }
        else if (splitRow[0] == "Circle")
        {
            /*
             * Circle N X Y SX SY R G B R
             *  Name : N - std::string
             *  Position : (X, Y) - float, float
             *  Speed : (SX, SY) - float, float
             *  Color : (R, G, B) - int, int, int
             *  Radius : R - float
            */
            SFMLShapeWrapper circleShape {};

            circleShape.format = format;

            circleShape.name = splitRow[1];

            float x = std::stof(splitRow[2]);
            float y = std::stof(splitRow[3]);

            circleShape.pos = DryPhys::Vector3D {x, y, 0};

            float sx = std::stof(splitRow[4]);
            float sy = std::stof(splitRow[5]);

            circleShape.speed = DryPhys::Vector3D {sx, sy, 0};

            int r = std::stoi(splitRow[6]);
            int g = std::stoi(splitRow[7]);
            int b = std::stoi(splitRow[8]);

            float radius = std::stof(splitRow[9]);

            circleShape.shape = std::make_shared<sf::CircleShape>(radius);
            circleShape.shape->setFillColor(sf::Color(r, g, b));
            circleShape.shape->setPosition(x, y);

            shapes.push_back((circleShape));
        }
    }

    return shapes;
}
