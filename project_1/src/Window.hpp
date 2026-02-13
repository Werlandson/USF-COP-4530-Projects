#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Characters.hpp"

class Window
{
private:
    sf : RenderWindow window;
    std::vector<std::shared_ptr<Character>> characters;
    float frameRate;

public:
    Window(unsigned width, unsigned height, const std::string &title, float frameRate);
    void addObject(const std::shared_ptr<Character> &object);
    void run();
};