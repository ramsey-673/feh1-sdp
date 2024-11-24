#pragma once

#include "FEHLCD.h"
#include "FEHImages.h"
#include "FEHUtility.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>

#define OUTER_CIRCLE_RADIUS 50
#define INNER_CIRCLE_RADIUS 10

#define OUTER_CIRCLE_COLOR WHITE
#define INNER_CIRCLE_COLOR WHITE



// "Buffer" used to prevent overlapping collisions.
// This should always be GREATER than the player's maximum horizontal velocity.
// Otherwise, the player may bypass it and phase through a wall.
#define CBUFFER 3



// Simple (x, y) pair with overloaded math operators
class Vector
{
public:
    // Pair of coordinates or measurements
    float x;
    float y;

    /*
     * Operators for mathematically manipulating vectors.
     */
    Vector operator+(const Vector& a);
    Vector operator-(const Vector& a);
    Vector operator*(const int a);
    Vector operator/(const int a);

    /*
     * Assignment operators for vectors.
     */
    void operator+=(const Vector& a);
    void operator-=(const Vector& a);
    void operator*=(const int a);
    void operator/=(const int a);
};

// Represents a renderable object.
class Sprite
{
private:
    // Gives every sprite a unique ID.
	static int idCounter;

public:
	Sprite();
	~Sprite();

    /*
     * Draw the sprite on the screen - each derived class is different
     */
	virtual void render(Vector screenPosition) const;
};

// The player
class Player
{
public:
    // Player's absolute position
	static Vector position;
    // Size of player's hitbox
	static Vector size;
    // The player's current velocity; change in position per frame
	static Vector v;

    // Texture associated with the player
    static FEHImage *texture;

    /*
     * TEMPORARY GRAPHICS CODE
     */
	static void render(Vector screenPosition);
};

// An item that the player can pick up.
class Collectible: public Sprite
{
public:
    // Collectible's absolute position in the level
	Vector position;
    // Size of collectible's hitbox
	Vector size;

	Collectible(Vector position, Vector size);
};

// Building blocks of the level.
class Tile: public Sprite
{
public:
    // Absolute position in the level
	Vector position;
    // Hitbox size
	Vector size;
	
	Tile(Vector position, Vector size);

    /*
     * TEMPORARY GRAPHICS CODE
     */
	void render(Vector screenPosition) const;
};

// Contains all the data for each level.
class Level
{
public:
    // Contains every tile in the current level.
	std::vector<Tile> tiles;
    // Contains every collectible in the current level.
	std::vector<Collectible> collectibles;

    // Maps characters from input files to the corresponding game object to create.
	std::unordered_map<char, std::string> tileFileMap;

    /*
     * Loads a level from a text file.
     * The default constructor creates a completely blank level.
     * If a fileName is provided, a level will be loaded from the file.
     * Each character and position in the file is mapped to a specific object to add to the level.
     */
    Level();
	Level(const std::string &fileName);
	~Level();
};

// Functions for calculating gravity and collisions.
class Physics
{
public:
    /*
     * Simulate gravity on the player by changing its velocity.
     */
	static void applyGravity();
    /*
     * Check collision between the player and a tile.
     * Calculates how the player is hitting the tile and moves the player accordingly.
     * Returns true if any part of the player's hitbox overlaps with the tile's hitbox.
     */
	static bool checkCollision(Tile &tile);
    /*
     * Check collision between the player and any collectible item.
     */
	static bool checkCollision(Collectible &collectible);
};

// Manipulates game logic
class Logic
{
public:
    /*
     * Handle all non-physics game logic.
     */
	static void updateLogic();
};

// Timer that the player has to beat
class Timer
{
private:
    /**
     * The relative stop time of the timer.
     */
    double stopTime;
public:
    Timer(double duration);
    /**
     * Check if the timer is ongoing.
     * 
     * @returns true if the timer is ongoing
     * and false if it expired.
     */
    bool isTimeUp() const;
    /**
     * Calculates the time in seconds until the timer expires.
     * 
     * @returns time in seconds until timer expires
     */
    double getTimeLeft() const; // Get remaining time
};

// Contains the game's overall state
class Game
{
public:
    // Number of collectibles the player has picked up.
	static int score;

    // True if the game is updating.
	static bool running;
    /*
     * Initializes the game.
     */
	static void initialize();
    /*
     * Runs every frame and handles all aspects of the game.
     */
	static void update();
    /*
     * Called when game is over
     */
	static void cleanup();

    // The level currently loaded + displayed to the player
	static Level currentLevel;
    // Timer that will end the game if it runs out
	static Timer gameTimer;

    // Value of gravity in the level
	static Vector gravity;
};



/**************************************************************************************************************/



/* Vector */

Vector Vector::operator+(const Vector& a)
{
    return { x + a.x, y + a.y };
}

Vector Vector::operator-(const Vector& a)
{
    return { x - a.x, y - a.y };
}

Vector Vector::operator*(const int a)
{
    return { x*a, y*a };
}

Vector Vector::operator/(const int a)
{
    return { x/a, y/a };
}

void Vector::operator+=(const Vector& a)
{
    x += a.x;
    y += a.y;
}

void Vector::operator-=(const Vector& a)
{
    x -= a.x;
    y -= a.y;
}

void Vector::operator*=(const int a)
{
    x *= a;
    y *= a;
}

void Vector::operator/=(const int a)
{
    x /= a;
    y /= a;
}



/* Sprite */

Sprite::Sprite() { }

Sprite::~Sprite() { }

void Sprite::render(Vector screenPosition) const { }



/* Player */

Vector Player::position { 50, 50 };

Vector Player::size { 25, 25 };

Vector Player::v { 0, 0 };

// TEMPORARY GRAPHICS CODE
void Player::render(Vector screenPosition)
{
    // Just draw a blue rectangle
	LCD.SetFontColor(BLUE);
	LCD.FillRectangle(position.x, position.y, size.x, size.y);
}



/* Collectible */

Collectible::Collectible(Vector position, Vector size): position(position), size(size) { }



/* Tile */

Tile::Tile(Vector position, Vector size): position(position), size(size) { }

// TEMPORARY GRAPHICS CODE
void Tile::render(Vector screenPosition) const
{
    // Draw a white rectangle
	LCD.SetFontColor(WHITE);
	LCD.FillRectangle(position.x, position.y, size.x, size.y);
}



/* Level */

Level::Level() { }

Level::Level(const std::string &fileName) { }

Level::~Level() { }



/* Physics */

void Physics::applyGravity()
{
    // Change the player's velocity to simulate gravity
	Player::v += Game::gravity;
}

bool Physics::checkCollision(Tile &tile)
{
    // Do the two hitboxes overlap?
	if (Player::position.x + Player::size.x > tile.position.x &&
        Player::position.x < tile.position.x + tile.size.x &&
        Player::position.y + Player::size.y > tile.position.y &&
        Player::position.y < tile.position.y + tile.size.y)
    {
        // For horizontal collisions, the tile has a "buffer" to prevent it from overlapping
        // with the vertical collisions. If the edge of the player is inside the buffer, a horizontal
        // collision is detected. The function then returns to stop a vertical collision from being
        // erroneously detected as well.

        // Player hits the left side of the tile
        if (Player::position.x + Player::size.x > tile.position.x &&
            Player::position.x + Player::size.x < tile.position.x + CBUFFER)
        {
            Player::v.x = 0;
            Player::position.x = tile.position.x - Player::size.x;
            return true;
        }
        // Player hits the right side of the tile
        if (Player::position.x < tile.position.x + tile.size.x &&
            Player::position.x > tile.position.x + tile.size.x - CBUFFER)
        {
            Player::v.x = 0;
            Player::position.x = tile.position.x + tile.size.x;
            return true;
        }

        // Player hits the top of the tile
        if (Player::position.y < tile.position.y)
        {
            Player::v.y = 0;
            Player::position.y = tile.position.y - Player::size.y;
        }
        // Player hits the bottom of the tile
        if (Player::position.y > tile.position.y)
        {
            Player::v.y = 0;
            Player::position.y = tile.position.y + tile.size.y;
        }

        return true;
    }

    return false;
}

bool Physics::checkCollision(Collectible &collectible) { }


// Handes user input
class InputHandler {
private:
    /**
     * True if the player touched the screen on the last frame.
     * False if the player did not touch the screen on the last frame.
     */
    static bool previousState;
    /**
     * The screen position where the player initially put their finger down.
     * Undefined position is (-1, -1).
     */
    static Vector touchOrigin;
public:
    /**
     * Updates the game's state based on user's input on the current frame.
     */
    static void processInput();
};

bool InputHandler::previousState = false;
Vector InputHandler::touchOrigin = {-1, -1};

void InputHandler::processInput()
{
    // The finger's position on this frame.
    // Undefined position is (-1, -1).
    Vector touch = {-1, -1};

    // Check if the player is touching the screen on this frame.
    // If they are, update the values inside x and y.
    // Casts float input into integer position.
    bool currentState = LCD.Touch(&touch.x, &touch.y);

    if (currentState)
    {
        if (InputHandler::previousState)
        {
            // The user is holding theif finger down
            // on this frame.
        }
        else
        {
            // The user put their finger down
            // on this frame.

            // Update the origin of the outer circle.
            InputHandler::touchOrigin = {touch.x, touch.y};
        }

        /* Calculate input offset from touch origin. */

        Vector touchOriginOffset = {touch.x - InputHandler::touchOrigin.x,
            touch.y - InputHandler::touchOrigin.y};

        // Check if the player's x input exceeds the outer circle.
        if (std::fabs(touchOriginOffset.x) > OUTER_CIRCLE_RADIUS)
        {
            // Cap offsetX to the outer circle radius.
            // OffsetX will never be zero if it reached here.
            if (touchOriginOffset.x > 0) {
                // Positive offset
                touchOriginOffset.x = OUTER_CIRCLE_RADIUS;
            }
            else
            {
                // Negative offset
                touchOriginOffset.x = -OUTER_CIRCLE_RADIUS;
            }
        }

        // Check if the player's y input exceeds the outer circle.
        if (std::fabs(touchOriginOffset.y) > OUTER_CIRCLE_RADIUS)
        {
            // Cap offsetX to the outer circle radius.
            // OffsetX will never be zero if it reached here.
            if (touchOriginOffset.y > 0) {
                // Positive offset
                touchOriginOffset.y = OUTER_CIRCLE_RADIUS;
            }
            else
            {
                // Negative offset
                touchOriginOffset.y = -OUTER_CIRCLE_RADIUS;
            }
        }

        /* Draw input UI */

        // Draw the outer circle.
        LCD.SetFontColor(OUTER_CIRCLE_COLOR);
        LCD.DrawCircle(InputHandler::touchOrigin.x, InputHandler::touchOrigin.y, OUTER_CIRCLE_RADIUS);

        // Calculate the position of the inner circle,
        // which is at the player's touch location if its within the outer circle
        // or at the circumference of the outer circle if the player's touch exceeds it.
        Vector innerCircleOrigin = {InputHandler::touchOrigin.x + touchOriginOffset.x,
            InputHandler::touchOrigin.y + touchOriginOffset.y};

        // Draw the inner circle.
        LCD.SetFontColor(INNER_CIRCLE_COLOR);
        LCD.DrawCircle(innerCircleOrigin.x, innerCircleOrigin.y, INNER_CIRCLE_RADIUS);

        // Calculate player's x movement as a real scalar from [-1, 1].
        float playerMovementX = (float)touchOriginOffset.x / OUTER_CIRCLE_RADIUS;

        // Update player's horizontal velocity based on input.
        Player::v.x = (playerMovementX * 5);
    }
    else
    {
        if (InputHandler::previousState)
        {
            // The user released their finger
            // on this frame.

            // Update touchOrigin to undefined values.
            InputHandler::touchOrigin = {-1, -1};

            // Make the player jump.
            Player::v.y = -3;
        }
        else
        {
            // The user is not touching the screen
            // on this frame.
        }
    }

    // Update the previous touch state to the current touch state.
    InputHandler::previousState = currentState;
}

/* Logic */

void Logic::updateLogic()
{
    Physics::applyGravity();
    InputHandler::processInput();
	Player::position += Player::v;
}



/* Timer */

Timer::Timer(double duration)
{
    this->stopTime = TimeNow() + duration;
}

bool Timer::isTimeUp() const
{
    return TimeNow() > this->stopTime;
}

double Timer::getTimeLeft() const
{
    return this->stopTime - TimeNow();
}



/* Game */

int Game::score { 0 };

bool Game::running { false };

Level Game::currentLevel { };

Timer Game::gameTimer(5);

Vector Game::gravity { 0, 0.2 };

void Game::initialize() { }

void Game::update()
{
    // TEMPORARY GRAPHICS/PHYSICS CODE

	
    LCD.Clear();
    
    Logic::updateLogic();

    // Testing to see how much the player can hang over the edge before it falls.
    Tile test1({ 72, 200 }, { 100, 25 });
    Tile test2({ 172, 175 }, { 25, 25 });
    currentLevel.tiles.push_back(test1);
    currentLevel.tiles.push_back(test2);

    for (auto i = 0; i < currentLevel.tiles.size(); i++)
	{
        Physics::checkCollision(currentLevel.tiles[i]);
		currentLevel.tiles[i].render({ 0, 0 });
	}

    Player::render({ 0, 0 });

    
    LCD.Update();
}

void Game::cleanup() { }