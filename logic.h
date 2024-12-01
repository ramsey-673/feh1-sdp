#pragma once

#include "FEHLCD.h"
#include "FEHImages.h"
#include "FEHUtility.h"
#include "utils.h"

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
#define GRAVITY_Y .8

#define PLAYER_SPEED 4
#define JUMP_STRENGTH 8
#define NUMBER_JUMPS 2

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
    // Whether or not the collectible has been picked up.
    bool collected;

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
    // If true, player dies on contact
    bool deadly;
	
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
    // Dollars required to proceed to the next level.
    int dollarsLeft;
    // Contains every tile in the current level.
	std::vector<Tile*> tiles;
    // Contains every collectible in the current level.
	std::vector<Collectible*> collectibles;
    // The player's starting position for the current level.
    Vector startingPosition;

    // Stores the bottom-right corner of the play area.
    // The upper-left corner is always (0, 0)
    Vector playLimit;


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
     * 
     * @author Andrew Loznianu
     */
	Level(const std::string &fileName);
    Level();
    /**
     * Destructor for level objects.
     * Free tiles and collectibles from memory.
     * Loaded textures stay in memory.
     * 
     * @author Andrew Loznianu
     */
	~Level();

    /**
     * Restarts the state of the level.
     * Typically called whenever the player dies.
     * 
     * @author Andrew Loznianu
     */
    void restart();
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
    // True if the player opts to go back to the main menu.
    static bool mainMenu;
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
	static Level *currentLevel;
    // Timer that will end the game if it runs out
	static Timer gameTimer;

    // Value of gravity in the level
	static Vector gravity;

    /**
     * Proceeds to the next level.
     * 
     * @author Andrew Loznianu
     */
    static void nextLevel();
    /*
     * Displays an end screen if the player fails to complete the game on time.
     */
    static void gameOver();

    // Used to keep track of current level.
    static int level;
    // Keeps track of levels in order.
    static std::vector<std::string> levels;
};