//
// Created by hoang on 5/8/2026.
//

#ifndef STEERINGBEHAVIORS_GAME_H
#define STEERINGBEHAVIORS_GAME_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "Vehicle.h"


class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();

private:
    sf::RenderWindow window_;
    sf::Clock clock_;
    Vehicle vehicle_;



};



#endif //STEERINGBEHAVIORS_GAME_H
