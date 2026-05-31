#include "../core/game.h"

#include <iostream>
#include <chrono>

#include "imgui.h"
#include "imgui-SFML.h"

Game::Game()
    : window_(sf::VideoMode( { 1200, 700 } ), "A* Pathfinding")
    , view_(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(window_.getPosition().x, window_.getPosition().y)))
    , grid_(50, 50, 50)
{
    window_.setView(view_);
    ImGui::SFML::Init(window_);
}

void Game::run()
{
    while (window_.isOpen() )
    {
        processEvents();
        update();
        draw();
    }
}

void Game::processEvents()
{
    while ( const std::optional event = window_.pollEvent() )
    {
        ImGui::SFML::ProcessEvent(window_, *event);

        if (event->is<sf::Event::Closed>())
            window_.close();

        // on resized event
        if (const auto* resizedEvent = event->getIf<sf::Event::Resized>())
        {
            // created a view from fixed visible area using FloatRect
            sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(resizedEvent->size.x, resizedEvent->size.y));
            view_ = sf::View(visibleArea);
            window_.setView(view_);
        }

        // Mouse Button Pressed Event
        if (const auto* mouseButtonPressedEvent = event->getIf<sf::Event::MouseButtonPressed>())
        {
            is_dragging_ = true;

            sf::Vector2i pixelPos = mouseButtonPressedEvent->position;
            sf::Vector2f worldPos = window_.mapPixelToCoords(pixelPos, view_);

            sf::Vector2f gridOffset = getGridOffset();
            sf::Vector2f localPos = worldPos - gridOffset;

            onMouseClick(*mouseButtonPressedEvent, localPos);
        }

        // Keyboard Keys Pressed Events
        if (const auto* keyPressedEvent = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num1)
            {
                input_mode_ = InputMode::kSelecting;
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num2)
            {
                input_mode_ = InputMode::kDeselecting;
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num3)
            {
                if (app_state_ == AppState::kIdle && placement_state_ == PlacementState::kPlacingObstacles)
                {
                    runAStar();
                    app_state_ = AppState::kAnimating;
                }
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num4)
            {
                reset();
            }
        }

        // Mouse Button Released Event
        if (const auto* mouseButtonReleasedEvent = event->getIf<sf::Event::MouseButtonReleased>())
        {
            is_dragging_ = false;
        }

        // Mouse Drag Event
        if (const auto* mouseMovedEvent = event->getIf<sf::Event::MouseMoved>())
        {
            sf::Vector2i pixelPos = mouseMovedEvent->position;
            sf::Vector2f worldPos = window_.mapPixelToCoords(pixelPos, view_);

            sf::Vector2f gridOffset = getGridOffset();
            sf::Vector2f localPos = worldPos - gridOffset;

            if (is_dragging_)
            {
                onDrag(*mouseMovedEvent, localPos);
            }
        }

        // Mouse Wheel Scrolled Event
        if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            if (mouseWheelScrolled->delta > 0)
            {
                view_.zoom(0.9f);
            }
            else if (mouseWheelScrolled->delta < 0)
            {
                view_.zoom(1.1f);
            }

            window_.setView(view_);
        }
    }
}

void Game::update()
{
    if (app_state_ == AppState::kAnimating && agent_)
    {
        if (snapshot_clock_.getElapsedTime().asSeconds() > delay_)
        {
            if (agent_->snapshot_index_ < agent_->snapshots_.size() - 1)
            {
                agent_->snapshot_index_++;
                agent_->snapshots_[agent_->snapshot_index_].prepareSnapshot();
            }
            else
            {
                app_state_ = AppState::kDone;
                if (!agent_->path_.empty())
                {
                    for (int i = 1; i < agent_->path_.size() - 1; ++i)
                    {
                    agent_->path_[i]->setType(CellType::path);
                    }
                }
            }
            snapshot_clock_.restart();
        }
    }

    ImGui::SFML::Update(window_, imgui_clock_.restart());
    ImGui::SetNextWindowSize(ImVec2(330, 170)); // change ImGUI size
    ImGui::Begin("Metrics");                    // start ImGUI window, with title "Metrics"
    ImGui::SetWindowFontScale(2.0f);            // scale up font of ImGUI by 2
    if (agent_)
    {
        ImGui::Text("Path Found: %s", agent_->metrics_.path_found ? "Yes" : "No");
        ImGui::Text("Path Length: %zu",agent_->metrics_.path_size);
        ImGui::Text("Nodes Expanded: %zu", agent_->metrics_.nodes_expanded);
        ImGui::Text("Search Time: %.2f ms", agent_->metrics_.search_time);
    }
    ImGui::End();
}

void Game::draw()
{
    window_.clear(sf::Color::White);
    grid_.draw(window_);
    ImGui::SFML::Render(window_);
    window_.display();
}

void Game::onDrag(const sf::Event::MouseMoved &mouseEvent, const sf::Vector2f &worldPos)
{
    if (app_state_ != AppState::kIdle) return;

    const int row = worldPos.x / grid_.getCellSize();
    const int col = worldPos.y / grid_.getCellSize();

    if (row >= grid_.getRows() || row < 0 || col >= grid_.getCols() || col < 0)
    {
        return;
    }

    switch (input_mode_)
    {
        case InputMode::kSelecting:
        {
            selectCell(row, col);
            break;
        }
        case InputMode::kDeselecting:
        {
            deselectCell(row, col);
            break;
        }
    }
}

void Game::onMouseClick(const sf::Event::MouseButtonPressed &mouseEvent, const sf::Vector2f &worldPos)
{
    if (app_state_ != AppState::kIdle) return;

    const int row = worldPos.x / grid_.getCellSize();
    const int col = worldPos.y / grid_.getCellSize();

    if (row >= grid_.getRows() || row < 0 || col >= grid_.getCols() || col < 0)
    {
        return;
    }

    switch (input_mode_)
    {
        case InputMode::kSelecting:
        {
            selectCell(row, col);
            break;
        }
        case InputMode::kDeselecting:
        {
            deselectCell(row, col);
            break;
        }
    }
}

void Game::runAStar()
{
    if (placement_state_ != PlacementState::kPlacingObstacles) return;

    auto start = std::chrono::high_resolution_clock::now();
    agent_.emplace(start_cell_, goal_cell_, grid_, sf::Color::Blue);
    agent_->runAStar();
    auto end = std::chrono::high_resolution_clock::now();

    // Save agent's metrics to display
    agent_->metrics_.search_time = std::chrono::duration<double, std::milli>(end - start).count();
    agent_->metrics_.path_found = !agent_->path_.empty();
    agent_->metrics_.path_size = agent_->path_.size();
}

sf::Vector2f Game::getGridOffset() const
{
    float x = window_.getSize().x / 2.f - grid_.getCellSize() * grid_.getRows() / 2.f;
    float y = window_.getSize().y / 2.f - grid_.getCellSize() * grid_.getCols() / 2.f;
    return sf::Vector2f(x, y);
}

void Game::selectCell(int row, int col)
{
    switch (placement_state_)
    {
        case PlacementState::kNeedsStart:
        {
            if (grid_.cells_[row][col].getType() == CellType::goal)
            {
                break;
            }

            if (start_cell_)
                start_cell_->setType(CellType::open);

            grid_.cells_[row][col].setType(CellType::start);
            start_cell_ = &grid_.cells_[row][col];
            placement_state_ = PlacementState::kNeedsGoal;

            break;
        }
        case PlacementState::kNeedsGoal:
        {
            if (grid_.cells_[row][col].getType() == CellType::start)
            {
                break;
            }

            if (goal_cell_)
                goal_cell_->setType(CellType::open);

            grid_.cells_[row][col].setType(CellType::goal);
            goal_cell_ = &grid_.cells_[row][col];
            placement_state_ = PlacementState::kPlacingObstacles;
            break;
        }
        case PlacementState::kPlacingObstacles:
        {
            if (grid_.cells_[row][col].getType() != CellType::start &&
                    grid_.cells_[row][col].getType() != CellType::goal)
            {
                grid_.cells_[row][col].setType(CellType::obstacle);
            }
            break;
        }
    }
}

void Game::deselectCell(int row, int col)
{
    switch (grid_.cells_[row][col].getType())
    {
        case CellType::start:
        {
            grid_.cells_[row][col].setType(CellType::open);
            start_cell_ = nullptr;
            placement_state_ = PlacementState::kNeedsStart;
            break;
        }
        case CellType::goal:
        {
            grid_.cells_[row][col].setType(CellType::open);
            goal_cell_ = nullptr;
            placement_state_ = start_cell_ ? PlacementState::kNeedsGoal : PlacementState::kNeedsStart;
            break;
        }
        case CellType::obstacle:
        {
            grid_.cells_[row][col].setType(CellType::open);
            break;
        }
        default:
        {
            break;
        }
    }
}

void Game::reset()
{
    for (int i = 0; i < grid_.getRows(); ++i)
    {
        for (int j = 0; j < grid_.getCols(); ++j)
        {
            grid_.cells_[i][j].reset();
        }
    }

    agent_.reset();

    start_cell_ = nullptr;
    goal_cell_ = nullptr;

    placement_state_ = PlacementState::kNeedsStart;
    app_state_ = AppState::kIdle;
}
