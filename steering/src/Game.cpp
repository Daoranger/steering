//
// Created by hoang on 5/8/2026.
//

#include "Game.h"


Game::Game()
    : window_(sf::VideoMode(sf::Vector2u(1000, 1000)), "Steering Behaviors")
    , vehicle_(sf::Vector2f(500, 500))
{
}

void Game::run()
{
    while (window_.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    while ( const std::optional event = window_.pollEvent() )
    {
        if (event->is<sf::Event::Closed>())
            window_.close();

        if (const auto* resizedEvent = event->getIf<sf::Event::Resized>())
        {
            sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(resizedEvent->size.x, resizedEvent->size.y));
            window_.setView(sf::View(visibleArea));
        }
    }
}

void Game::update()
{
    float dt = clock_.restart().asSeconds();
    vehicle_.update(dt, window_.getSize());
}

void Game::render()
{
    window_.clear(sf::Color::Black);
    vehicle_.render(window_);
    window_.display();
}


