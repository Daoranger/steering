//
// Created by hoang on 5/8/2026.
//

#include "Game.h"

#include "SFML/Graphics/CircleShape.hpp"

Game::Game()
    : window(sf::VideoMode(sf::Vector2u(1920, 1080)), "Steering Behaviors")
    , vehicle1(sf::Vector2f(300, 540))
    , vehicle2(sf::Vector2f(1800, 100))
{
    vehicle1.maxSpeed = 300;
    vehicle2.maxSpeed = 200;
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    while ( const std::optional event = window.pollEvent() )
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* resizedEvent = event->getIf<sf::Event::Resized>())
        {
            sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(resizedEvent->size.x, resizedEvent->size.y));
            window.setView(sf::View(visibleArea));
        }
    }
}

void Game::update()
{
    float dt = clock.restart().asSeconds();
    sf::Vector2f target = sf::Vector2f(sf::Mouse::getPosition(window));

    //sf::Vector2f steeringForceV1 = vehicle1.steeringBehaviors.wander(dt);
    sf::Vector2f steeringForceV1 = sf::Vector2f(100,0);

    sf::Vector2f steeringForceV2 = vehicle2.steeringBehaviors.pursuit(vehicle1);
    //sf::Vector2f steeringForceV2 = vehicle2.steeringBehaviors.seek(vehicle1.position);

    vehicle1.update(dt, steeringForceV1, window.getSize());
    vehicle2.update(dt, steeringForceV2, window.getSize());
}

void Game::render()
{
    window.clear(sf::Color::Black);
    vehicle1.render(window);
    vehicle2.render(window);


    // line from pursuer to predicted point
    sf::Vertex line[] = {
        sf::Vertex{vehicle2.position, sf::Color::Green},
        sf::Vertex{vehicle2.position + sf::Vector2f(vehicle2.heading().x * 1000, vehicle2.heading().y * 1000), sf::Color::Green}
    };
    window.draw(line, 2, sf::PrimitiveType::Lines);

    window.display();
}