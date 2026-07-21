//
// Created by hoang on 7/14/2026.
//

#ifndef STEERINGBEHAVIORS_WALL_H
#define STEERINGBEHAVIORS_WALL_H
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"


class Wall
{
public:
    Wall(sf::Vector2f start, sf::Vector2f end);
    void render(sf::RenderWindow& window);
    sf::Vector2f recalculateNormal();
    sf::Vector2f startPoint;
    sf::Vector2f endPoint;
    sf::Vector2f normal;
};



#endif //STEERINGBEHAVIORS_WALL_H
