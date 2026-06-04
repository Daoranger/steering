//
// Created by hoang on 5/8/2026.
//

#include "Game.h"

Game::Game()
    : window(sf::VideoMode(sf::Vector2u(1000, 1000)), "Steering Behaviors")
    , vehicle(sf::Vector2f(500, 500))
{
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
    sf::Vector2f steeringForce = vehicle.steeringBehaviors.arrive(target);
    vehicle.update(dt, steeringForce, window.getSize());
}

void Game::render()
{
    window.clear(sf::Color::Black);
    vehicle.render(window);
    window.display();
}