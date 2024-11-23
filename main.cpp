#include "graphics.h"
#include "logic.h"
#include "ui.h"

#include "FEHLCD.h"
#include "FEHImages.h"

int main()
{
    // Start with the main menu
    int option = menu();

    // Exit condition:
    // The player chooses to play the game.
    while (option != 0)
    {
        switch (option)
        {
            case 1: // Display main menu
                option = menu();
                break;
            case 2: // Display statistics
                option = stats();
                break;
            case 3: // Display tutorial
                option = tutorial();
                break;
            case 4: // Display credits
                option = credits();
                break;
        }
    }

    return 0;
}

/* Menu functions */

int menu()
{
    // Display code
    LCD.Clear();
    LCD.WriteAt("Food Robot Simulator", 40, 30);
    LCD.WriteAt("Play", 134, 120);
    LCD.WriteAt("Stats", 130, 145);
    LCD.WriteAt("Tutorial", 112, 170);
    LCD.WriteAt("Credits", 118, 195);

    // User input
    bool touched;
    float x;
    float y;

    // Loops until the player selects an option
    while (true)
    {
        x = 0;
        y = 0;
        touched = LCD.Touch(&x, &y);

        // Play button
        if (x > 134 && x < 185 &&
            y > 120 && y < 140 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 1; // Change this to 0 when functionality for the play button is added
        }

        // Stats button
        if (x > 130 && x < 189 &&
            y > 145 && y < 165 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 2;
        }

        // Tutorial button
        if (x > 112 && x < 271 &&
            y > 170 && y < 190 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 3;
        }

        // Credits button
        if (x > 118 && x < 202 &&
            y > 195 && y < 215 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 4;
        }
    }
}

int stats()
{
    // Display statistics to the screen
    LCD.Clear();
    LCD.WriteLine("< Back to Menu");
    LCD.WriteLine(" "); // Blank line
    LCD.WriteLine("Score: 0");
    LCD.WriteLine("Money Collected: $0");
    LCD.WriteLine("Best Time: 0:00");

    // User input
    bool touched;
    float x;
    float y;

    // Wait until back button is pressed
    while (true)
    {
        x = 0;
        y = 0;
        touched = LCD.Touch(&x, &y);

        if (x > 0 && x < 168 &&
            y > 0 && y < 20 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 1;
        }
    }
}

int tutorial()
{
    // Tutorial text.
    // NOTE: there may be extra spaces or places where a space was removed.
    // This is because Proteus doesn't create new lines from whitespace resulting in broken words.
    LCD.Clear();
    LCD.WriteLine("< Back to Menu");
    LCD.WriteLine(" ");
    LCD.WriteLine("You are an Ohio State foodrobot.");
    LCD.WriteLine(" ");
    LCD.WriteLine("Deliver the food to the   student's dorm before the time runs out.");
    LCD.WriteLine(" ");
    LCD.WriteLine("Collect money to use the  green scooters to move to the next level.");

    // User input
    bool touched;
    float x;
    float y;
    while (true)
    {
        x = 0;
        y = 0;
        touched = LCD.Touch(&x, &y);

        if (x > 0 && x < 168 &&
            y > 0 && y < 20 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 1;
        }
    }
}

int credits()
{
    // Credits display
    LCD.Clear();
    LCD.WriteLine("< Back to Menu");
    LCD.WriteAt("Credits", 118, 45);
    LCD.WriteAt("Game Logic:", 94, 80);
    LCD.WriteAt("Nathan Ramsey", 82, 100);
    LCD.WriteAt("Graphics Programming:", 34, 125);
    LCD.WriteAt("Andrew Loznianu", 70, 145);
    LCD.WriteAt("Proteus Simulator:", 52, 170);
    LCD.WriteAt("Ohio State FEH Department", 10, 190);

    // User input
    bool touched;
    float x;
    float y;

    // Wait until user decides to go back to menu
    while (true)
    {
        x = 0;
        y = 0;
        touched = LCD.Touch(&x, &y);

        if (x > 0 && x < 168 &&
            y > 0 && y < 20 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 1;
        }
    }
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



/*
 * logic.h implementations
 */

Level Game::currentLevel;
Position Player::position;

void Player::render(struct Position screenPosition) {}
void Tile::render(struct Position screenPosition) const {}
void Collectible::render(struct Position screenPosition) const {}




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

void Physics::applyGravity() {}

/* Level Methods */
Level::Level(const std::string& fileName)
{
   
}

Level::Level() {}

Level::~Level()
{
    free(&this->tiles);
    free(&this->collectibles);
}

/*
 * render.h implementations
 */

Position Camera::origin = {0, 0};

Position Camera::getScreenPosition(const Position &gamePosition)
{
    // Initialize return struct on the stack.
    Position screenPosition;

    // Offset the game position by the camera's origin.
    screenPosition.x = gamePosition.x - Camera::origin.x;
    screenPosition.y = gamePosition.y - Camera::origin.y;

    // Return a copy of the resulting screen position.
    return screenPosition;
}

bool Camera::isInFrame(const Position &screenPosition, const int spriteWidth, const int spriteHeight)
{
    // Check if the game object is within the camera's bounds.
    if (screenPosition.x + spriteWidth < 0 ||  // Is the sprite to the left of the camera?
        screenPosition.x > PROTEUS_WIDTH ||    // Is the sprite to the right of the camera?
        screenPosition.y + spriteHeight < 0 || // Is the sprite above the camera?
        screenPosition.y > PROTEUS_HEIGHT)     // Is the sprite below the camera?
    {
        // The game object is outside one of the camera's bounds.
        return false;
    }
    else
    {
        // The game object is within all of the camera's bounds.
        return true;
    }
}
bool Camera::isInFrame(const Position &screenPosition)
{
    // Return result of overloaded method with default values.
    return Camera::isInFrame(screenPosition, DEFAULT_SPRITE_SIZE, DEFAULT_SPRITE_SIZE);
}

void Camera::follow(const Position &targetPosition, const int spriteWidth, const int spriteHeight)
{
    // Set the camera's origin one half-screen above and to the left of the sprite's origin.
    Camera::origin.x = targetPosition.x - PROTEUS_WIDTH / 2;
    Camera::origin.y = targetPosition.y - PROTEUS_HEIGHT / 2;

    // Offset the camera's position by the sprite's size,
    // so the center of the camera is at the center of the sprite.
    Camera::origin.x += spriteWidth / 2;
    Camera::origin.y += spriteHeight / 2;
}
void Camera::follow(const Position &targetPosition)
{
    // Return result of overloaded method with default values.
    Camera::follow(targetPosition, DEFAULT_SPRITE_SIZE, DEFAULT_SPRITE_SIZE);
}

void Graphics::render()
{
    // Ensure the camera stays centered on the player
    // during this rendering cycle.
    Camera::follow(Player::position);

    // Iterate through every tile in the level.
    for (const Tile tile : Game::currentLevel.tiles)
    {
        // Find the screen position of the current tile.
        Position screenPosition = Camera::getScreenPosition(tile.position);

        // Render the tile if the camera can see it.
        if (Camera::isInFrame(screenPosition)) {
            tile.render(screenPosition);
        }
    }

    // Iterate through every collectible in the level.
    for (const Collectible collectible : Game::currentLevel.collectibles)
    {
        // Find the screen position of the current collectible.
        Position screenPosition = Camera::getScreenPosition(collectible.position);

        // Render the collectible if the camera can see it.
        if (Camera::isInFrame(screenPosition)) {
            collectible.render(screenPosition);
        }
    }

    // Find the screen position of the player.
    Position screenPosition = Camera::getScreenPosition(Player::position);

    // Render the player to the screen.
    // No need to check if the player is in frame
    // because they are always in frame.
    Player::render(screenPosition);
}

/*
 * ui.h implementations
 */

void UIManager::renderUI() {}