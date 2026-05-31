//
// Created by hoang on 5/8/2026.
//

#include "Vehicle.h"

#include <iostream>

#include "SFML/Graphics/CircleShape.hpp"

Vehicle::Vehicle(sf::Vector2f startPos)
    : position(startPos)
    , velocity(sf::Vector2f(10, 0))
    , heading(0.f, -1.f)
    , side(1.f, 0.f)
    , texture_("assets/f22.png")
    , sprite_(texture_)
{

    sf::Vector2u size = texture_.getSize();
    sprite_.setOrigin(sf::Vector2f(size.x / 2.f, size.y / 2.f));
    sprite_.setPosition(position);
}

void Vehicle::update(float dt, sf::Vector2u windowSize)
{
    sf::Vector2f steeringForce(50.f, 20.f);
    sf::Vector2f acceleration = steeringForce / 1.0f;
    velocity += acceleration * dt;

    float speed = velocity.length();

    if (speed > maxSpeed)
    {
        velocity = (velocity / speed) * maxSpeed;
    }

    position += velocity * dt;

    if (velocity.length() > 0.00000001f)
    {
        heading = velocity.normalized();
        side = velocity.perpendicular();
    }

    std::cout << "Velocity: " << velocity.length() << "\n";

    // wrap around
    if (position.x > windowSize.x) position.x = 0;
    if (position.x < 0)             position.x = (float)windowSize.x;
    if (position.y > windowSize.y) position.y = 0;
    if (position.y < 0)             position.y = (float)windowSize.y;
}

void Vehicle::render(sf::RenderWindow& window)
{
    float angleRadian = std::atan2(heading.y, heading.x);
    float angleDegree = angleRadian * 180.f / 3.14159f;
    sprite_.setRotation(sf::degrees(angleDegree + 90.f)); // + 90 because the sprite image is originally point upward
    sprite_.setPosition(position);
    window.draw(sprite_);
}
