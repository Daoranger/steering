//
// Created by hoang on 5/8/2026.
//

#include "Vehicle.h"

Vehicle::Vehicle(sf::Vector2f startPos)
    : mass(1.0f)
    , position(startPos)
    , velocity(sf::Vector2f(0, 0))
    , maxSpeed(200.0f)
    , maxForce(100.0f)
    , texture("assets/f22.png")
    , sprite(texture)
{

    sf::Vector2u size = texture.getSize();
    sprite.setOrigin(sf::Vector2f(size.x / 2.f, size.y / 2.f));
    sprite.setPosition(position);
}

void Vehicle::update(float dt, sf::Vector2f steeringForce, sf::Vector2u windowSize)
{
    sf::Vector2f acceleration = steeringForce / mass;
    velocity += acceleration * dt;

    if (speed() > maxSpeed)
        velocity = velocity.normalized() * maxSpeed;

    position += velocity * dt;
}

void Vehicle::render(sf::RenderWindow& window)
{
    sprite.setPosition(position);
    float angle = std::atan2(heading().y, heading().x) * 180.f / 3.14159f;
    sprite.setRotation(sf::degrees(angle + 90.0f));
    window.draw(sprite);

}
