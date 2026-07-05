//
// Created by hoang on 5/8/2026.
//

#include "Vehicle.h"

#include "SFML/Graphics/CircleShape.hpp"

Vehicle::Vehicle(sf::Vector2f startPos)
    : mass(1.0f)
    , position(startPos)
    , velocity(sf::Vector2f(0, 0))
    , maxSpeed(150.0f)
    , maxForce(200.0f)
    , texture("assets/f22.png")
    , sprite(texture)
    , steeringBehaviors(*this)
{

    sf::Vector2u size = texture.getSize();
    sprite.setOrigin(sf::Vector2f(size.x / 2.f, size.y / 2.f));
    sprite.setPosition(position);
}

void Vehicle::update(float dt, sf::Vector2f steeringForce, sf::Vector2u windowSize)
{

    // clamp max force
    if (steeringForce.lengthSquared() > maxForce * maxForce)
        steeringForce = steeringForce.normalized() * maxForce;

    sf::Vector2f acceleration = steeringForce / mass;
    velocity += acceleration * dt;

    if (speed() > maxSpeed)
        velocity = velocity.normalized() * maxSpeed;

    position += velocity * dt;

    if (position.x > windowSize.x) position.x = 0;
    if (position.y > windowSize.y) position.y = 0;
    if (position.x < 0) position.x = windowSize.x;
    if (position.y < 0) position.y = windowSize.y;
}

void Vehicle::render(sf::RenderWindow& window)
{
    sprite.setPosition(position);
    float angle = std::atan2(heading().y, heading().x) * 180.f / 3.14159f;
    sprite.setRotation(sf::degrees(angle + 90.0f));
    window.draw(sprite);

    // debug: draw wander circle and target
    // sf::CircleShape circle(steeringBehaviors.wanderRadius);
    // sf::Vector2f circleCenter = position + heading() * steeringBehaviors.wanderDistance;
    // circle.setOrigin({steeringBehaviors.wanderRadius, steeringBehaviors.wanderRadius});
    // circle.setPosition(circleCenter);
    // circle.setFillColor(sf::Color::Transparent);
    // circle.setOutlineColor(sf::Color::Yellow);
    // circle.setOutlineThickness(1.f);
    // window.draw(circle);

}
