#pragma once

#include "FEHLCD.h"
#include "FEHImages.h"
#include "FEHUtility.h"

#define PROTEUS_WIDTH 319
#define PROTEUS_HEIGHT 239
#define DEFAULT_SPRITE_SIZE 16

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <fstream>

#define GRID_CELL_WIDTH 16
#define GRID_CELL_HEIGHT 16

#define GRAVITY_X 0
#define GRAVITY_Y 1

#define PLAYER_SPEED 7
#define JUMP_STRENGTH 10
#define NUMBER_JUMPS 2



// "Buffer" used to prevent overlapping collisions.
// This should always be GREATER than the player's maximum horizontal velocity.
// Otherwise, the player may bypass it and phase through a wall.
#define CBUFFER 3

class Timer;

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

    static int jumpCounter;

    // Texture associated with the player
    static FEHImage *texture;
    static FEHImage *flipTexture;

    /*
     * TEMPORARY GRAPHICS CODE
     */
	static void render(Vector screenPosition);
};

// An item that the player can pick up.
class Collectible
{
private:
    FEHImage *texture;
public:
    // Collectible's absolute position in the level
	Vector position;
    // Size of collectible's hitbox
	Vector size;

    // Type of the collectible.
    // 'd' represents a dollar,
    // and 's' represents a scooter.
    char type;

	Collectible(Vector position, Vector size, FEHImage *texture, char type);

    void render(Vector screenPosition) const;


};

// Building blocks of the level.
class Tile
{
private:
    FEHImage *texture;
public:
    // Absolute position in the level
	Vector position;
    // Hitbox size
	Vector size;
	
	Tile(Vector position, Vector size, FEHImage *texture);

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
	std::vector<Tile*> tiles;
    // Contains every collectible in the current level.
	std::vector<Collectible*> collectibles;

    // Maps characters from input files to the corresponding game object to create.
	static std::unordered_map<char, const char*> tileFileMap;

    /**
     * A hashmap that maps texture filenames
     * to the texture's memory location - an FEHImage.
     */
    static std::unordered_map<const char*, FEHImage*> fileTextureMap;

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

