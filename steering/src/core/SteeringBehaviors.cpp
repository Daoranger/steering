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

sf::Vector2f SteeringBehaviors::flee(sf::Vector2f target) const
{
    sf::Vector2f desired_velocity = (vehicle_.position - target).normalized() * vehicle_.maxSpeed;
    return desired_velocity - vehicle_.velocity;
}

sf::Vector2f SteeringBehaviors::arrive(sf::Vector2f target) const
{
    sf::Vector2f toTarget = target - vehicle_.position;
    float dist = toTarget.length();

    if (dist > 0)
    {
        const float DECELERATION_TWEAKER = 0.3; // because deceleration is integer, need this value to tweak deceleration
        const int deceleration = 1;             // slow = 3, normal = 2, fast = 1

        // this is one of many way to get the arrive speed
        float speed = dist / deceleration * DECELERATION_TWEAKER;
        
        speed = std::min(speed, vehicle_.maxSpeed);

        sf::Vector2f desired_velocity = toTarget * speed / dist;

        return desired_velocity - vehicle_.velocity;
    }

    // stop when reached target
    return sf::Vector2f(0,0);
}
