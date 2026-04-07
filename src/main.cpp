#include <SFML/Graphics.hpp>
#include <optional>

#include "game/game.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)),
                            "Fabrizio <3");

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.display();
    }

    return 0;
}