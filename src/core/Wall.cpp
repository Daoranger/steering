//
// Created by hoang on 7/14/2026.
//

#include "Wall.h"

Wall::Wall(sf::Vector2f start, sf::Vector2f end)
    : startPoint(start)
    , endPoint(end)
{
    normal = recalculateNormal();
}

void Wall::render(sf::RenderWindow& window)
{
    std::array<sf::Vertex, 2> line =
    {
        sf::Vertex{startPoint, sf::Color::White},
        sf::Vertex{endPoint, sf::Color::White}
    };

    window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
}

sf::Vector2f Wall::recalculateNormal()
{
    sf::Vector2f wallDir = endPoint - startPoint;
    sf::Vector2f n(wallDir.y, -wallDir.x);              // n = wallDir rotated 90 degree

    float length = sqrtf(n.x * n.x + n.y * n.y);
    n.x /= length;
    n.y /= length;

    return n;   // unit normal
}
