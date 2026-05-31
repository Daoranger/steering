// Copyright 2026, Hoang Nguyen
//
// game.h
//
// Defines the Game class, which manages the main application loop,
// handles user input, and orchestrates the grid, pathfinding, and visualization.

#ifndef PATHFINDING_GAME_H
#define PATHFINDING_GAME_H

#include <optional>

#include "SFML/Graphics/RenderWindow.hpp"

#include "../grid/grid.h"
#include "../pathfinding/agent.h"

enum class InputMode
{
    kSelecting,
    kDeselecting
};

enum class PlacementState
{
    kNeedsStart,
    kNeedsGoal,
    kPlacingObstacles
};

enum class AppState
{
    kIdle,
    kAnimating,
    kDone
};

class Game
{

public:

    Game();
    void run();
    void processEvents();
    void update();
    void draw();
    void onDrag(const sf::Event::MouseMoved& mouseEvent, const sf::Vector2f& worldPos);
    void onMouseClick(const sf::Event::MouseButtonPressed& mouseEvent, const sf::Vector2f& worldPos);
    void runAStar();

private:

    sf::Vector2f getGridOffset() const;
    void selectCell(int row, int col);
    void deselectCell(int row, int col);
    void reset();

    Cell* start_cell_ = nullptr;
    Cell* goal_cell_ = nullptr;

    std::optional<Agent> agent_;

    sf::RenderWindow window_;
    sf::View view_;
    Grid grid_;
    sf::Clock snapshot_clock_;
    float delay_ = 0.0001;
    sf::Clock imgui_clock_;

    PlacementState placement_state_ = PlacementState::kNeedsStart;
    InputMode input_mode_ = InputMode::kSelecting;
    AppState app_state_ = AppState::kIdle;
    bool is_dragging_ = false;

};


#endif  // PATHFINDING_GAME_H