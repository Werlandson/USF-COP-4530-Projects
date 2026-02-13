#pragma once
#include "CircleList.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

CircleList<sf::Vector2f> createCircularPath(float cx, float cy, float radius, int numPoints);
