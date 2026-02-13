#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Test");

    while (window.isOpen())
    {
        while (auto eventOpt = window.pollEvent())
        {
            sf::Event event = *eventOpt;

            if (event.is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(sf::Color::White);
        window.display();
    }
}