#include "SteeringBehaviors.h"
#include "Vehicle.h"

SteeringBehaviors::SteeringBehaviors(const Vehicle& vehicle)
    :vehicle_(vehicle)
{
}

sf::Vector2f SteeringBehaviors::seek(sf::Vector2f target) const
{
    sf::Vector2f desired_velocity = (target - vehicle_.position).normalized() * vehicle_.maxSpeed;
    return desired_velocity - vehicle_.velocity;
}
