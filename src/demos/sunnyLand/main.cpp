#include <SFML/Graphics.hpp>
#include <engine2d/animation.hpp>
#include <engine2d/assets.hpp>

using namespace Engine2D;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <config_file>\n";
        return EXIT_FAILURE;
    }

    sf::RenderWindow window(sf::VideoMode(1200, 1200), "Asset Tests");
    window.setFramerateLimit(60);

    Assets assets {};
    assets.loadFromFile(argv[1]);

    Animation animation1 = assets.getAnimation("EnemyDeath");
    Animation animation2 = assets.getAnimation("ItemFeedback");
    Animation animation3 = assets.getAnimation("Cherry");
    Animation animation4 = assets.getAnimation("Gems");

    Animation back   = assets.getAnimation("Back");
    Animation middle = assets.getAnimation("Middle");

    sf::Vector2u windowSize = window.getSize();
    const sf::IntRect sizes {0, 0, static_cast<int>(windowSize.x) + 200, static_cast<int>(windowSize.y) + 200};

    // Set the background
    auto backgroundSize = back.getSize();

    float backgroundScaleX = static_cast<float>(windowSize.x) / backgroundSize[0];
    float backgroundScaleY = static_cast<float>(windowSize.y) / backgroundSize[1];

    back.getSprite().setScale(backgroundScaleX, backgroundScaleY);

    // Set the middleground
    auto middlegroundSize = middle.getSize();

    float middlegroundScaleX = static_cast<float>(windowSize.x) / middlegroundSize[0] / 2.0f;
    float middlegroundScaleY = static_cast<float>(windowSize.y) / middlegroundSize[1];

    middle.getSprite().setTextureRect(sizes);
    middle.getSprite().setScale(middlegroundScaleX, middlegroundScaleY);
    middle.getSprite().move(0.0f, windowSize.y / 2.0f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.draw(back.getSprite());
        window.draw(middle.getSprite());

        // Animation 1
        animation1.getSprite().setPosition(400, 400);
        animation1.getSprite().setScale(2, 2);
        animation1.update();
        animation1.hasEnded();

        window.draw(animation1.getSprite());

        // Animation 2
        animation2.getSprite().setPosition(800, 400);
        animation2.getSprite().setScale(2, 2);
        animation2.update();
        animation2.hasEnded();

        window.draw(animation2.getSprite());

        // Animation 3
        animation3.getSprite().setPosition(400, 800);
        animation3.getSprite().setScale(4, 4);
        animation3.update();
        animation3.hasEnded();

        window.draw(animation3.getSprite());

        // Animation 4
        animation4.getSprite().setPosition(800, 800);
        animation4.getSprite().setScale(4, 4);
        animation4.update();
        animation4.hasEnded();

        window.draw(animation4.getSprite());

        window.display();
    }
}
