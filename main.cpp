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