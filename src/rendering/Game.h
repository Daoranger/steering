//
// Created by hoang on 5/8/2026.
//

#ifndef STEERINGBEHAVIORS_GAME_H
#define STEERINGBEHAVIORS_GAME_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "../core/Vehicle.h"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window;
    sf::Clock clock;
    Vehicle vehicle1;
    Vehicle vehicle2;
};

#endif //STEERINGBEHAVIORS_GAME_H
