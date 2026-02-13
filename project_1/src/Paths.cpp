#include "Paths.hpp"

CircleList<sf::Vector2f> createCircularPath(float cx, float cy, float radius, int numPoints)
{
    CircleList<sf::Vector2f> path = CircleList<sf::Vector2f>();

    for (int i = 0; i < numPoints; i++)
    {
        float angle = 2.f * 3.1415926f * i / numPoints;
        float x = cx + radius * std::cos(angle);
        float y = cy + radius * std::sin(angle);
        path.add({x, y});
    }

    return path;
}