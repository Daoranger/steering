//
// Created by hoang on 5/8/2026.
//

#include "Vehicle.h"

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

Vehicle::Vehicle(sf::Vector2f startPos)
    : mass(1.0f)
    , position(startPos)
    , velocity(sf::Vector2f(0, 0))
    , maxSpeed(500.0f)
    , maxForce(500.0f)
    , texture_("assets/f22.png")
    , sprite_(texture_)
    , steeringBehaviors(*this)
    , collision_radius_(70.0f)
    , detection_box_length_(280.0f)
    , min_detection_box_length_(280.0f)
{

    sf::Vector2u size = texture_.getSize();
    sprite_.setOrigin(sf::Vector2f(size.x / 2.f, size.y / 2.f));
    sprite_.setPosition(position);
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

    // avoids normalizing a zero vector
    if (velocity.lengthSquared() > 1e-6f)
    {
        heading_ = velocity.normalized();
        side_ = heading_.perpendicular();
    }

    position += velocity * dt;

    if (position.x > windowSize.x) position.x = 0;
    if (position.y > windowSize.y) position.y = 0;
    if (position.x < 0) position.x = windowSize.x;
    if (position.y < 0) position.y = windowSize.y;
}

void Vehicle::render(sf::RenderWindow& window)
{
    sprite_.setPosition(position);
    float angle = std::atan2(heading().y, heading().x) * 180.f / 3.14159f;
    sprite_.setRotation(sf::degrees(angle + 90.0f));
    window.draw(sprite_);

    // DEBUG: collision circle
    sf::CircleShape collision_circle(collision_radius_);
    collision_circle.setOrigin(sf::Vector2f(collision_radius_, collision_radius_));
    collision_circle.setPosition(position);
    collision_circle.setFillColor(sf::Color::Transparent);
    collision_circle.setOutlineColor(sf::Color::White);
    collision_circle.setOutlineThickness(3.0f);
    window.draw(collision_circle);

    // DEBUG: detection box
    sf::RectangleShape detection_box(sf::Vector2f(detection_box_length_, collision_radius_ * 2));
    detection_box.setOrigin(sf::Vector2f(0, collision_radius_));
    detection_box.setPosition(position);
    detection_box.setRotation(sprite_.getRotation() - sf::degrees(90.0f));
    detection_box.setFillColor(sf::Color::Transparent);
    detection_box.setOutlineColor(sf::Color::Green);
    detection_box.setOutlineThickness(3.0f);
    window.draw(detection_box);


    // DEBUG FOR WANDER

    // sf::CircleShape circle(steeringBehaviors.wanderRadius);
    // sf::Vector2f circleCenter = position + heading() * steeringBehaviors.wanderDistance;
    // circle.setOrigin({steeringBehaviors.wanderRadius, steeringBehaviors.wanderRadius});
    // circle.setPosition(circleCenter);
    // circle.setFillColor(sf::Color::Transparent);
    // circle.setOutlineColor(sf::Color::Yellow);
    // circle.setOutlineThickness(1.f);
    // window.draw(circle);
    //
    // // debug: draw small jitter circle, centered at the wander target's world position
    // sf::Vector2f wanderTargetLocal = steeringBehaviors.wanderTarget + sf::Vector2f(steeringBehaviors.wanderDistance, 0);
    // sf::Vector2f wanderTargetWorld = position
    //     + heading() * wanderTargetLocal.x
    //     + side() * wanderTargetLocal.y;
    //
    // sf::CircleShape jitterCircle(steeringBehaviors.wanderJitter);
    // jitterCircle.setOrigin({steeringBehaviors.wanderJitter, steeringBehaviors.wanderJitter});
    // jitterCircle.setPosition(wanderTargetWorld);
    // jitterCircle.setFillColor(sf::Color::Transparent);
    // jitterCircle.setOutlineColor(sf::Color::Cyan);
    // jitterCircle.setOutlineThickness(1.f);
    // window.draw(jitterCircle);

    // DEBUG: feelers
    for (auto& feelerEnd : steeringBehaviors.feelers)
    {
        std::array<sf::Vertex, 2> line =
        {
            sf::Vertex{position, sf::Color::Yellow},
            sf::Vertex{feelerEnd, sf::Color::Yellow}
        };
        window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
    }
}
