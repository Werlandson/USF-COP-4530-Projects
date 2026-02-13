#include "Characters.hpp"

PatrickStar::PatrickStar(CNode<sf::Vector2f> *position) : position(position)
{
    star.setPointCount(5);
    star.setPoint(0, sf::Vector2f(0.f, -50.f));
    star.setPoint(1, sf::Vector2f(47.f, 15.f));
    star.setPoint(2, sf::Vector2f(29.f, 50.f));
    star.setPoint(3, sf::Vector2f(-29.f, 50.f));
    star.setPoint(4, sf::Vector2f(-47.f, 15.f));
    star.setFillColor(sf::Color(255, 192, 203));
    star.setPosition(position->elem);

    pants.setSize(sf::Vector2f(60.f, 25.f));
    pants.setFillColor(sf::Color(0, 128, 0));
    pants.setOrigin({30.f, 12.5f});

    leftEye.setRadius(7.f);
    rightEye.setRadius(7.f);
    leftEye.setFillColor(sf::Color::White);
    rightEye.setFillColor(sf::Color::White);
    leftEye.setOrigin({7.f, 7.f});
    rightEye.setOrigin({7.f, 7.f});
}

void PatrickStar::update(float deltaTime)
{
    position = position->next;
    sf::Vector2f center = position->elem;
    star.setPosition(center);

    pants.setPosition({center.x, center.y + 25.f});
    leftEye.setPosition({center.x - 15.f, center.y - 20.f});
    rightEye.setPosition({center.x + 15.f, center.y - 20.f});
}

void PatrickStar::draw(sf::RenderWindow &window) const
{
    window.draw(star);
    window.draw(pants);
    window.draw(leftEye);
    window.draw(rightEye);
}