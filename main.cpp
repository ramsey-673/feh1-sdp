#include "main.h"

int main()
{
    Game::update();

    return 0;
}

/* Game Methods */

void Game::initialize()
{
    // TODO
}
void Game::update()
{
    Logic::updateLogic();
    Graphics::render();
    UIManager::renderUI();
}
void Game::cleanup()
{
    // TODO
}

/* Graphics Methods */

// Renders graphics to the screen.
// Runs after logic is updated
// every Update cycle.
void Graphics::render()
{
    // Render all the static tiles.
    for (const Tile tile : Game::currentLevel.tiles)
    {
        if (!Camera::isInFrame(tile)) {
            Position screenPosition = Camera::getRelativePosition(tile);
            tile.render(screenPosition);
            free(&screenPosition);
        }
    }

    // Render all the collectibles.
    for (const Collectible collectible : Game::currentLevel.collectibles)
    {
        if (!Camera::isInFrame(collectible)) {
            Position screenPosition = Camera::getRelativePosition(collectible);
            collectible.render(screenPosition);
            free(&screenPosition);
        }
    }

    // Render the main character.
    Position screenPosition = Camera::getRelativePosition(Game::mainCharacter);
    Game::mainCharacter.render(screenPosition);
    free(&screenPosition);
}

/* UIManager Methods */

// Renders the game's UI.
// Runs after graphics rendering
// in the Update cycle.
void UIManager::renderUI()
{
    UIManager::renderTimer(Game::gameTimer.getTimeLeft());
    UIManager::renderScore(Game::getScore());
}
void UIManager::renderTimer(int timeLeft)
{

}
void UIManager::renderScore(int score)
{

}

/* Logic Methods */

// Updates the game's logic.
// Runs first every Update cycle.
void Logic::updateLogic()
{
    InputHandler::processInput();
    Physics::applyGravity();

    // Iterate through every tile and see if they collide with the player.
    for (Tile tile : Game::currentLevel.tiles)
    {
        if (Physics::checkCollision(tile))
        {
            // do stuff idk
        }
    }

    // Iterate through every collectible and see if they collide with the player.
    for (Collectible collectible : Game::currentLevel.collectibles)
    {
        if (Physics::checkCollision(collectible))
        {
            // stuff
        }
    }

    // stuff
}

/* InputHander methods */

// Checks the state of user input
// and changes the game's state appropriately.
void InputHandler::processInput()
{
    // stuff
}

/* Physics Methods */
bool Physics::checkCollision(const Collectible& collectible)
{
    // stuff
}
bool Physics::checkCollision(const Tile& tile)
{
    //stuff
}

/* Position Methods */
Position::Position(int x, int y)
{
    this->x = x;
    this->y = y;
}
Position::Position() : Position(0, 0) {}

/* Camera Methods */
Camera::Camera(Position corner1, Position corner2)
{
    this->corner1 = corner1;
    this->corner2 = corner2;
}
Camera::~Camera()
{
    free(&this->corner1);
    free(&this->corner2);
}
bool Camera::isInFrame(Sprite sprite)
{
    // TODO
}
Position Camera::getRelativePosition(Sprite sprite)
{
    // TODO
}
void Camera::moveTo(Position targetPosition)
{
    // TODO
}
void Camera::zoom(float scale)
{
    // TODO
}

namespace std {
    template <>
    struct hash<Collectible> {
        size_t operator()(const Collectible& c) const {
            return hash<int>()(c.id());
        }
    };

    template <>
    struct hash<Tile> {
        size_t operator()(const Tile& t) const {
            return hash<int>()(t.id());
        }
    };
}

/* Level Methods */
Level::Level(const std::string& fileName)
{
   
}

Level::~Level()
{
    free(&this->tiles);
    free(&this->collectibles);
}