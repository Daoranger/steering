//
// Created by hoang on 6/3/2026.
//

#ifndef STEERINGBEHAVIORS_STEERINGBEHAVIORS_H
#define STEERINGBEHAVIORS_STEERINGBEHAVIORS_H

#include "SFML/System/Vector2.hpp"

class Vehicle;

class SteeringBehaviors
{
public:
    SteeringBehaviors(const Vehicle& vehicle);

    sf::Vector2f seek(sf::Vector2f target) const;

private:
    const Vehicle& vehicle_;
};

#endif //STEERINGBEHAVIORS_STEERINGBEHAVIORS_H
