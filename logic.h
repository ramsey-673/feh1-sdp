#pragma once

#include "FEHLCD.h"
#include "FEHImages.h"
#include "FEHUtility.h"
#include "utils.h"

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <fstream>

#define PROTEUS_WIDTH 319
#define PROTEUS_HEIGHT 239
#define DEFAULT_SPRITE_SIZE 16

#define GRID_CELL_WIDTH 16
#define GRID_CELL_HEIGHT 16

#define GRAVITY_X 0
#define GRAVITY_Y .8

#define PLAYER_SPEED 4
#define JUMP_STRENGTH 8
#define NUMBER_JUMPS 2

#define SECOND_VALUE 100
#define DOLLAR_VALUE 10

/**
 * Represents the player.
 */
class Player
{
public:
    /**
     * The player's in-game position.
     */
	static Vector position;
    /**
     * The size of the player's hitbox.
     */
	static Vector size;
    /**
     * The player's current velocity, change in position per frame.
     */
	static Vector v;

    /**
     * Used to keep track of how many jumps the player has left
     * before they need to touch the ground.
     */
    static int jumpCounter;

    /**
     * The player's non-inverted texture.
     */
    static FEHImage *texture;
    /**
     * The player's inverted texture.
     */
    static FEHImage *flipTexture;

    /**
     * Renders the player.
     * 
     * @param screenPosition
     *      the screen position of this
     * 
     * @author Andrew Loznianu
     */
	static void render(Vector screenPosition);
};

/**
 * Represents a collectible,
 * which is a tile that the player can pass though
 * and can typically interact with.
 */
class Collectible
{
private:
    FEHImage *texture;
public:
    /**
     * The collectible's in-game position.
     */
	Vector position;
    /**
     * The size of the collectible's hitbox.
     */
	Vector size;
    /**
     * Indicates whether or not the collectible has been picked up.
     * If true, the collectible is no longer rendered,
     * and the player can no longer interact with it.
     */
    bool collected;

    /**
     * Indicates the functionality of the collectible.
     * A value of 'd' represents a dollar,
     * a value of 's' indicates the collectible
     * will take the player to the next level
     * if all dollars are collected,
     * and a value of 't' or any other undefined value
     * indicates no special functionality.
     */
    char type;

    /**
     * Constructor for a collectible object.
     * 
     * @param position
     *      the in-game position of this
     * @param size
     *      the size of this in pixels
     * @param texture
     *      the FEHImage used to render this
     * @param type
     *      used to determine functionality of collectible by game logic methods
     * 
     * @author Andrew Loznianu
     */
	Collectible(Vector position, Vector size, FEHImage *texture, char type);

    /**
     * Renders this.
     * 
     * @param screenPosition
     *      the screen position of this
     * 
     * @author Andrew Loznianu
     */
    void render(Vector screenPosition) const;
};

/**
 * Represents a game object
 * that the player cannot pass though
 * or interact with.
 */
class Tile
{
private:
    FEHImage *texture;
public:
    /**
     * The tile's in-game position.
     */
	Vector position;
    /**
     * The size of the tile's hitbox.
     */
	Vector size;
    /**
     * If set to true, the player dies on contact.
     */
    bool deadly;
	
    /**
     * Constructor for a tile.
     * 
     * @param position
     *      the in-game position of this
     * @param size
     *      the size of this in pixels
     * @param texture
     *      the FEHImage used to render this
     * 
     * @author Andrew Loznianu
     */
	Tile(Vector position, Vector size, FEHImage *texture);

    /**
     * Renders this.
     * 
     * @param screenPosition
     *      the screen position of this
     * 
     * @author Andrew Loznianu
     */
	void render(Vector screenPosition) const;
};

/**
 * Represents the current level.
 */
class Level
{
public:
    /**
     * The number of dollars to be collected
     * to proceed to the next level.
     * Passage to the next level is allowed
     * if the value is zero.
     */
    int dollarsLeft;
    /**
     * Contains every tile in the current level.
     */
	std::vector<Tile*> tiles;
    /**
     * Contains every collectible in the current level.
     */
	std::vector<Collectible*> collectibles;
    /**
     * The player's starting position for the current level.
     */
    Vector startingPosition;

    /**
     * Stores the bottom-right corner of the play area.
     * The upper-left corner is always (0, 0)
     */
    Vector playLimit;


    /**
     * Maps characters from input files to the corresponding game object to create.
     * Game object types are serialized as a char representing its functionality
     * followed by the relative path to the game object's texture.
     */
	static std::unordered_map<char, const char*> tileFileMap;

    /**
     * A hashmap that maps texture filenames
     * to the texture's memory location, an FEHImage.
     */
    static std::unordered_map<const char*, FEHImage*> fileTextureMap;

    /**
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
    /**
     * Simulate gravity on the player by changing its velocity.
     * 
     * @author Nathan Ramsey
     */
	static void applyGravity();
    /**
     * Check collision between the player and a tile.
     * Calculates how the player is hitting the tile and moves the player accordingly.
     * Returns true if any part of the player's hitbox overlaps with the tile's hitbox.
     * 
     * @author Nathan Ramsey
     */
	static bool checkCollision(Tile &tile);
    /**
     * Check collision between the player and any collectible item.
     * 
     * @author Nathan Ramsey
     */
	static bool checkCollision(Collectible &collectible);
};

/**
 * Handles the game logic.
 */
class Logic
{
public:
    /**
     * Handle all non-physics game logic.
     * 
     * @author Nathan Ramsey
     */
	static void updateLogic();
};



/**
 * Represents the game's overall state.
 */
class Game
{
public:
    /**
     * Number of dollars the player has picked up.
     * Not to be confused with the player's overall score.
     */
	static int score;

    /**
     * True if the game is updating.
     * Is set to false if the game is done running.
     */
	static bool running;
    /**
     * True if the player opts to go back to the main menu.
     */
    static bool mainMenu;
    /**
     * Initializes the game.
     * 
     * @author Nathan Ramsey
     */
	static void initialize();
    /**
     * Runs every frame and handles all aspects of the game.
     * 
     * @author Nathan Ramsey
     */
	static void update();
    /**
     * Called when game is over
     * 
     * @author Nathan Ramsey
     */
	static void cleanup();

    /**
     * The level currently loaded into memory.
     */
	static Level *currentLevel;
    /**
     * Timer that indicates how much time the player has left in the game.
     */
	static Timer gameTimer;

    /**
     * Value of gravity in the level.
     */
	static Vector gravity;

    /**
     * The player's statistics, which will be saved in persistent memory
     * and will be displayed to the player in the Stats menu.
     */
    static int bestMinutes;
    static int bestSeconds;
    static int money;
    static int totalScore;

    /**
     * Proceeds to the next level.
     * 
     * @author Andrew Loznianu
     */
    static void nextLevel();
    /**
     * Displays an end screen if the player fails to complete the game on time.
     * 
     * @author Nathan Ramsey
     */
    static void gameOver();
    /*
     * Reads/writes the player's scores.
     */
    static void loadScores();
    static void writeScores(bool finished);

    /**
     * Keeps track of the current level number.
     * Used to determine the next level.
     */
    static int level;
    /**
     * Keeps track of the game's levels in order.
     * Each level is represented as a path to a text file,
     * which the Level class constructor will translate.
     */
    static std::vector<std::string> levels;
};