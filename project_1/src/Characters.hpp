#pragma once
#include <SFML/Graphics.hpp>

class Character
{
public:
    virtual ~Character() = default;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow &window) const = 0;
};