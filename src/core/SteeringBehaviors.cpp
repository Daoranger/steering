#include "SteeringBehaviors.h"
#include "Vehicle.h"
#include "Utils.h"

SteeringBehaviors::SteeringBehaviors(const Vehicle& vehicle)
    : vehicle_(vehicle)
    , wanderTarget(wanderRadius, 0)
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

sf::Vector2f SteeringBehaviors::pursuit(const Vehicle& evader) const
{
    // vector from pursuer to evader
    sf::Vector2f toEvader = evader.position - vehicle_.position;

    double relativeHeading = vehicle_.heading().dot(evader.heading());
    // comment out for now cuz we always them to pursue
    // if ((toEvader.dot(vehicle_.heading()) > 0) && (relativeHeading < -0.95))
    // {
    //     return seek(evader.position);
    // }

    float lookAheadTime = toEvader.length() / (vehicle_.maxSpeed + evader.speed());

    // the math below say where will evader be after lookAheadTime (secs)
    return seek(evader.position + evader.velocity * lookAheadTime);
}

sf::Vector2f SteeringBehaviors::evade(const Vehicle& pursuer) const
{
    sf::Vector2f toPursuer = pursuer.position - vehicle_.position;

    float lookAheadTime = toPursuer.length() / (vehicle_.maxSpeed + pursuer.speed());

    return flee(pursuer.position + pursuer.velocity * lookAheadTime);
}

sf::Vector2f SteeringBehaviors::wander(float dt)
{
    // adding small random displacement to the target
    wanderTarget += sf::Vector2f(randomClamped() * wanderJitter * dt, randomClamped() * wanderJitter * dt);

    if (wanderTarget.lengthSquared() > 1e-6f)
        wanderTarget = wanderTarget.normalized() * wanderRadius;

    sf::Vector2f targetLocal = wanderTarget + sf::Vector2f(wanderDistance, 0);
    sf::Vector2f targetWorld = pointToWorldSpace(targetLocal);
    return seek(targetWorld);
}

sf::Vector2f SteeringBehaviors::pointToWorldSpace(sf::Vector2f targetLocal)
{
    sf::Vector2f heading = vehicle_.heading();
    sf::Vector2f side = vehicle_.side();
    sf::Vector2f position = vehicle_.position;

    return sf::Vector2f(
        position.x + heading.x * targetLocal.x + side.x * targetLocal.y,
        position.y + heading.y * targetLocal.x + side.y * targetLocal.y
    );
}
