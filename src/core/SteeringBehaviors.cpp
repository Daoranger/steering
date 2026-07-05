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

    // pursuer and evader headings give us the directions the two are facing
    // dot product tell us how align
    // 1.0: facing exactly same direction (theta = 0 degree)
    // 0.0: perpendicular direction (theta = 90 degree)
    //-1.0: facing directly opposite directions (theta = 180 degree)
    // > 0: mostly same direction
    // < 0: most opposite direction
    double relativeHeading = vehicle_.heading().dot(evader.heading());

    // first check: checks whether evader is in-front of the pursuer
    // positive -> evader is ahead, negative -> evade is behind

    // second check: checks whether the two ships are facing almost opposite direction

    // both toEvader and vehicle_.heading are both evectors from pursuer.
    // so if they point same direction, > 0, evader is in-front
    // but if they point opposite direction, < 0, evader is behind
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

// we always assume the vehicle is at 0,0 facing to the right
sf::Vector2f SteeringBehaviors::wander(float dt)
{
    // adding small random displacement to the target
    wanderTarget += sf::Vector2f(
        randomClamped() * wanderJitter * dt,
        randomClamped() * wanderJitter * dt
        );

    if (wanderTarget.lengthSquared() > 1e-6f)
        wanderTarget = wanderTarget.normalized() * wanderRadius;

    printf("wanderTarget: %.2f, %.2f\n", wanderTarget.x, wanderTarget.y);

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
