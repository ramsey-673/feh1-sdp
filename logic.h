#pragma once

#include "FEHLCD.h"
#include "FEHImages.h"
#include "FEHUtility.h"

#include <string>
#include <unordered_map>
#include <vector>



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



/* Logic */

void Logic::updateLogic()
{
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

    Physics::applyGravity();
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