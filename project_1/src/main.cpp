#include <SFML/Graphics.hpp>
#include "Window.hpp"
#include "Characters.hpp"
#include "CircleList.hpp"
#include "Paths.hpp"
#include <memory>

int main()
{
    Window window(800, 600, "Patrick Star Circular Path", 60);

    auto path = createCircularPath(400.f, 300.f, 200.f, 500);

    auto patrick = std::make_shared<PatrickStar>(path.getCursor());
    window.addObject(patrick);

    window.run();
    return 0;
}