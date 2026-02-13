#pragma once
#include <SFML/Graphics.hpp>
#include "CircleList.hpp"
#include <memory>

class Character
{
public:
    virtual ~Character() = default;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow &window) const = 0;
};

class PatrickStar : public Character
{
public:
    PatrickStar(CNode<sf::Vector2f> *position);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow &window) const override;

private:
    sf::ConvexShape star;
    sf::RectangleShape pants;
    sf::CircleShape leftEye, rightEye;
    CNode<sf::Vector2f> *position;
};