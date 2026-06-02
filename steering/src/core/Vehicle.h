//
// Created by hoang on 5/8/2026.
//

#ifndef STEERINGBEHAVIORS_VEHICLE_H
#define STEERINGBEHAVIORS_VEHICLE_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"

class Vehicle
{
public:
    float mass;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float maxSpeed;
    float maxForce;

    sf::Vector2f heading() const { return velocity.normalized(); }
    sf::Vector2f side() const { return velocity.perpendicular().normalized(); }
    float speed() const { return velocity.length(); }


    Vehicle(sf::Vector2f startPos);
    void update(float dt, sf::Vector2f steeringForce, sf::Vector2u windowSize);
    void render(sf::RenderWindow& window);



private:
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif //STEERINGBEHAVIORS_VEHICLE_H
