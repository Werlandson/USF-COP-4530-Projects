#include "Window.hpp"

Window::Window(unsigned width, unsigned height, const std::string &title, float frameRate)
    : window(sf::VideoMode({width, height}), title)
{
    window.setFramerateLimit(frameRate);
}

void Window::addObject(const std::shared_ptr<Character> &character)
{
    characters.push_back(character);
}

void Window::run()
{
    while (window.isOpen())
    {
        // Poll events
        while (auto eventOpt = window.pollEvent())
        {
            sf::Event event = *eventOpt;
            if (event.is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Update objects
        for (auto &chara : characters)
        {
            chara->update(1.f / frameRate); // deltaTime placeholder
        }

        // Draw objects
        window.clear(sf::Color::White);
        for (auto &chara : characters)
        {
            chara->draw(window);
        }
        window.display();
    }
}
