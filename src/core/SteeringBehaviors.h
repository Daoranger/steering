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
    sf::Vector2f flee(sf::Vector2f target) const;
    sf::Vector2f arrive(sf::Vector2f target) const;
    sf::Vector2f pursuit(const Vehicle& evader) const;
    sf::Vector2f evade(const Vehicle& pursuer) const;
    sf::Vector2f wander(float dt);

    float wanderRadius = 30.0f;
    float wanderDistance = 300.f;
    float wanderJitter = 100.f;
    sf::Vector2f wanderTarget;

private:
    const Vehicle& vehicle_;
    sf::Vector2f pointToWorldSpace(sf::Vector2f targetLocal);
};

#endif //STEERINGBEHAVIORS_STEERINGBEHAVIORS_H
