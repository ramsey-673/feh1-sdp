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


#define OUTER_CIRCLE_RADIUS 35
#define INNER_CIRCLE_RADIUS 10

#define OUTER_CIRCLE_COLOR WHITE
#define INNER_CIRCLE_COLOR WHITE

#define GRID_CELL_WIDTH 16
#define GRID_CELL_HEIGHT 16

#define GRAVITY_X 0
#define GRAVITY_Y 0.5

#define JUMP_STRENGTH 7
#define NUMBER_JUMPS 2



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
class Collectible: public Sprite
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
class Tile: public Sprite
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

FEHImage *Player::texture;
FEHImage *Player::flipTexture;

Vector Player::position { 50, 50 };

Vector Player::size { 16, 11 };

Vector Player::v { -2, 0 };

int Player::jumpCounter = 0;

// TEMPORARY GRAPHICS CODE
void Player::render(Vector screenPosition)
{
    // Just draw a blue rectangle
    if (Player::v.x <= 0)
    {
        Player::texture->Draw(screenPosition.x, screenPosition.y);
    } else
    {
        Player::flipTexture->Draw(screenPosition.x, screenPosition.y);
    }
	
}



/* Collectible */

Collectible::Collectible(Vector position, Vector size, FEHImage *texture, char type): position(position), size(size), texture(texture), type(type) { }

void Collectible::render(Vector screenPosition) const
{
    this->texture->Draw(screenPosition.x, screenPosition.y);
}

/* Tile */

Tile::Tile(Vector position, Vector size, FEHImage *texture): position(position), size(size), texture(texture) { }

// TEMPORARY GRAPHICS CODE
void Tile::render(Vector screenPosition) const
{

    // if (this->texture == NULL) {
    //     printf("This is null\n");
    // }
    // Draw a white rectangle
    // printf("DRAWING tile at (%f, %f)\n", screenPosition.x, screenPosition.y);
	this->texture->Draw(screenPosition.x, screenPosition.y);
    // printf("Finished drawing tile\n");
}



/* Level */

std::unordered_map<const char*, FEHImage*> Level::fileTextureMap;
std::unordered_map<char, const char*> Level::tileFileMap;

Level::Level() {

}

Level::Level(const std::string &fileName) {
    // Open the current level's file.
    std::ifstream fileStream;
    fileStream.open(fileName);

    // Check if the file opened successfully.
    if (!fileStream.is_open()) {
        throw 404;
    }
    else
    {
        // Start the current row and column at zero.
        int row = 0, col = 0;


        char objectChar = fileStream.get();
        while(!fileStream.eof())
        {
            
            while (objectChar != '\n')
            {
                // A space means we render nothing in this tile.
                if (objectChar == ' ')
                {
                    objectChar = fileStream.get();
                    col++;
                    continue;
                }

                // Create a vector that represents the position
                // of the newly created object.
                Vector gridPosition;

                gridPosition.x = col * GRID_CELL_WIDTH;
                gridPosition.y = row * GRID_CELL_HEIGHT;

                printf("LOADING %c to (%f, %f)\n", objectChar, gridPosition.x, gridPosition.y);

                // Get the object type and texture path from
                // the char -> filePath HashMap.
                const char *fileName = Level::tileFileMap.at(objectChar);

                // Extract the object type from the fileName string literal.
                char type = fileName[0];
                fileName++;

                
                // printf("CHECKING IF MAP CONTAINS TEXTURE, %s\n", fileName);

                
                // Check if the texture is not already loaded into memory.
                if (Level::fileTextureMap.find(fileName) == Level::fileTextureMap.end())
                {
                    // printf("ADDING TEXTURE TO FILE TEXTURE MAP\n");
                    // Lead the texture into memory,
                    FEHImage *newTexture = new FEHImage();
                    newTexture->Open(fileName);
                    // and insert it in the fileName -> texture HashMap.
                    fileTextureMap.insert({fileName, newTexture});
                }

                // Initialize a pointer to the FEHImage in the
                // pair with the file name as a key.
                FEHImage *texture = Level::fileTextureMap.find(fileName)->second;

                // Initialize object depending on object type.
                if (type == 'p')
                {
                    // Initialize the player.
                    Player::position.x = gridPosition.x;
                    Player::position.y = gridPosition.y;
                    //Player::texture = texture;
                }
                else if (type == 't')
                {
                    // Create a new tile.
                    Vector size;
                    size.x = GRID_CELL_WIDTH;
                    size.y = GRID_CELL_HEIGHT;
                    Tile *newTile = new Tile(gridPosition, size, texture);
                    this->tiles.push_back(newTile);
                }
                else if (type == 'c')
                {
                    // Create a new dollar.
                    Vector size;
                    size.x = GRID_CELL_WIDTH;
                    size.y = GRID_CELL_HEIGHT;
                    Collectible *newCollectible = new Collectible(gridPosition, size, texture, 'd');
                    this->collectibles.push_back(newCollectible);
                }
                else if (type == 'n')
                {
                    // Create a new scooter.
                    Vector size;
                    size.x = GRID_CELL_WIDTH;
                    size.y = GRID_CELL_HEIGHT;
                    Collectible *newCollectible = new Collectible(gridPosition, size, texture, 's');
                    this->collectibles.push_back(newCollectible);
                }

                // Get the next character.
                objectChar = fileStream.get();
                // Increment column.
                col++;
            }
            // Increment row.
            col = 0;
            row++;
            objectChar = fileStream.get();
        }

        // Close the file.
        fileStream.close();
    }
}

Level::~Level() { }



/* Physics */

void Physics::applyGravity()
{
    // Change the player's velocity to simulate gravity
	Player::v += Game::gravity;
}

bool Physics::checkCollision(Tile &tile)
{
    // Check if the player will hit the tile on the next frame.
    if (Player::position.x + Player::size.x + Player::v.x > tile.position.x &&
        Player::position.x + Player::v.x < tile.position.x + tile.size.x &&
        Player::position.y + Player::size.y + Player::v.y > tile.position.y &&
        Player::position.y + Player::v.y < tile.position.y + tile.size.y)
    {
        // Vertical collisions
        if (Player::position.x + Player::size.x > tile.position.x &&
            Player::position.x < tile.position.x + tile.size.x)
        {
            // Bottom of the player hits the top of the tile
            if (Player::position.y < tile.position.y &&
                Player::position.y + Player::size.y + std::ceil(Player::v.y) > tile.position.y)
            {
                if (std::fabs(Player::v.x) > 0.05)
                    Player::v.x /= 1.5;
                else
                    Player::v.x = 0;
                Player::v.y = 0;
                Player::position.y = tile.position.y - Player::size.y;

                Player::jumpCounter = NUMBER_JUMPS;
            }
            // Top of the player hits the bottom of the tile
            else if (Player::position.y + Player::size.y > tile.position.y + tile.size.y &&
                     Player::position.y + std::ceil(Player::v.y) < tile.position.y + tile.size.y)
            {
                Player::v.y = 0;
                Player::position.y = tile.position.y + tile.size.y;
            }
        }

        // Horizontal collisions
        if (Player::position.y + Player::size.y > tile.position.y &&
            Player::position.y < tile.position.y + tile.size.y)
        {
            // Right side of the player hits the left side of the tile
            if (Player::position.x < tile.position.x &&
                Player::position.x + Player::size.x + std::ceil(Player::v.x) > tile.position.x)
            {
                Player::v.x = 0;
                Player::position.x = tile.position.x - Player::size.x;
            }
            else if (Player::position.x + Player::size.x > tile.position.x + tile.size.x &&
                     Player::position.x + std::ceil(Player::v.x) < tile.position.x + tile.size.x)
            {
                Player::v.x = 0;
                Player::position.x = tile.position.x + tile.size.x;
            }
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
    
public:
    /**
     * Updates the game's state based on user's input on the current frame.
     */
    static void processInput();
    /**
     * The screen position where the player initially put their finger down.
     * Undefined position is (-1, -1).
     */
    static Vector touchOrigin;
    static Vector smallCircle;

};

bool InputHandler::previousState = false;
Vector InputHandler::touchOrigin = {-1, -1};
Vector InputHandler::smallCircle = {-1, -1};

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

        

        // Calculate the position of the inner circle,
        // which is at the player's touch location if its within the outer circle
        // or at the circumference of the outer circle if the player's touch exceeds it.
        Vector innerCircleOrigin = {InputHandler::touchOrigin.x + touchOriginOffset.x,
            InputHandler::touchOrigin.y + touchOriginOffset.y};

        // Allow Graphics::render() to render the circles.
        InputHandler::smallCircle.x = innerCircleOrigin.x;
        InputHandler::smallCircle.y = innerCircleOrigin.y;

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
            InputHandler::smallCircle = {-1, -1};

            // Make the player jump.
            if (Player::jumpCounter > 0) {
                Player::v.y = -JUMP_STRENGTH;
                Player::jumpCounter--;
            }
            
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
    for (auto i = 0; i < Game::currentLevel.tiles.size(); i++)
        Physics::checkCollision(*Game::currentLevel.tiles[i]);
	Player::position += Player::v;

    if (Player::position.y > 2000) {
        printf("ERROR: Player is out of bounds!");
        
        Game::running = false;
    }
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

/**
 * Handles the relationship between game position and screen position.
 * Essentially functions as the game's "camera."
 */
class Camera
{
private:
    /**
     * The upper-left corner (origin) of the game's camera.
     */
    static Vector origin;
public:

    /**
     * Returns the screen position of parameter &spritePosition
     * based on the camera.
     * 
     * @param &gamePosition
     *      game position to convert to screen position
     * @returns screen position of parameter based on camera
     */
    static Vector getScreenPosition(const Vector &gamePosition);

    /**
     * Returns true if the sprite is visible from the camera
     * and false if it not.
     * 
     * @param &screenPosition
     *      the screen position to check
     * @param spriteWidth
     *      the width of the game object's sprite
     * @param spriteHeight
     *      the height of the game object's sprite
     * @returns whether the sprite object is on the screen
     */
    static bool isInFrame(const Vector &screenPosition, const int spriteWidth, const int spriteHeight);
    static bool isInFrame(const Vector &screenPosition);

   /**
    * Changes the location of the camera's origin
    * to ensure that the game object is in the center of the camera.
    * 
    * @param &targetPosition
    *       the game position of the game object to follow
    * @param spriteWidth
    *       the width of the game object to follow's sprite
    * @param spriteHeight
    *       the height of the game object to follow's sprite
    */
    static void follow(const Vector &targetPosition, const int spriteWidth, const int spriteHeight);
    static void follow(const Vector &targetPosition);
    
};

/**
 * Responsible for the game's graphics.
 * Does not handle UI graphics.
 */
class Graphics
{
private:
    // Maps a filename to a texture object.
public:
    /**
     * Iterate through every game object and render them to the screen.
     */
    static void render();
};

/* Implementations */

Vector Camera::origin = {0, 0};

Vector Camera::getScreenPosition(const Vector &gamePosition)
{
    // Initialize return struct on the stack.
    Vector screenPosition;

    // Offset the game position by the camera's origin.
    screenPosition.x = gamePosition.x - Camera::origin.x;
    screenPosition.y = gamePosition.y - Camera::origin.y;

    // Return a copy of the resulting screen position.
    return screenPosition;
}

bool Camera::isInFrame(const Vector &screenPosition, const int spriteWidth, const int spriteHeight)
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
bool Camera::isInFrame(const Vector &screenPosition)
{
    // Return result of overloaded method with default values.
    return Camera::isInFrame(screenPosition, DEFAULT_SPRITE_SIZE, DEFAULT_SPRITE_SIZE);
}

void Camera::follow(const Vector &targetPosition, const int spriteWidth, const int spriteHeight)
{
    // Set the camera's origin one half-screen above and to the left of the sprite's origin.
    Camera::origin.x = targetPosition.x - PROTEUS_WIDTH / 2;
    Camera::origin.y = targetPosition.y - PROTEUS_HEIGHT / 2;

    // Offset the camera's position by the sprite's size,
    // so the center of the camera is at the center of the sprite.
    Camera::origin.x += spriteWidth / 2;
    Camera::origin.y += spriteHeight / 2;
}
void Camera::follow(const Vector &targetPosition)
{
    // Return result of overloaded method with default values.
    Camera::follow(targetPosition, DEFAULT_SPRITE_SIZE, DEFAULT_SPRITE_SIZE);
}

void Graphics::render()
{
    // Ensure the camera stays centered on the player
    // during this rendering cycle.
    // printf("MOVING TO PLAYER\n");
    Camera::follow(Player::position);


    // printf("RENDERING TILES\n");
    // Iterate through every tile in the level.
    for (const Tile *tile : Game::currentLevel.tiles)
    {
        // printf("GETTING SCREEN POSITION FOR TILE\n");
        // Find the screen position of the current tile.
        Vector screenPosition = Camera::getScreenPosition(tile->position);

        // printf("CHECKING IF TILE IS IN FRAME\n");
        // Render the tile if the camera can see it.
        if (Camera::isInFrame(screenPosition, tile->size.x, tile->size.y)) {
            // printf("RENDERING TILE\n");
            tile->render(screenPosition);
            // printf("RENDERED TILE\n");
        }
    }

    // printf("RENDERING COLLECTIBLES\n");
    // Iterate through every collectible in the level.
    for (const Collectible *collectible : Game::currentLevel.collectibles)
    {
        // Find the screen position of the current collectible.
        Vector screenPosition = Camera::getScreenPosition(collectible->position);

        // Render the collectible if the camera can see it.
        if (Camera::isInFrame(screenPosition, collectible->size.x, collectible->size.y)) {
            collectible->render(screenPosition);
        }
    }

    // printf("RENDERING PLAYER\n");
    // Find the screen position of the player.
    Vector screenPosition = Camera::getScreenPosition(Player::position);

    // Render the player to the screen.
    // No need to check if the player is in frame
    // because they are always in frame.
    Player::render(screenPosition);

    /* Draw input */

    if (InputHandler::touchOrigin.x != -1)
    {
        // Draw the outer circle.

        LCD.SetFontColor(OUTER_CIRCLE_COLOR);
        LCD.DrawCircle(InputHandler::touchOrigin.x, InputHandler::touchOrigin.y, OUTER_CIRCLE_RADIUS);

        // Draw the inner circle.
        LCD.SetFontColor(INNER_CIRCLE_COLOR);
        LCD.DrawCircle(InputHandler::smallCircle.x, InputHandler::smallCircle.y, INNER_CIRCLE_RADIUS);
    }

    
}

/* Game */

int Game::score { 0 };

bool Game::running { false };

Level Game::currentLevel { };

Timer Game::gameTimer(5);

Vector Game::gravity { GRAVITY_X, GRAVITY_Y };

void Game::initialize() {
    FEHImage *playerNormal = new FEHImage("food_robot.png");
    FEHImage *playerFlipped = new FEHImage("food_robot_right.png");

    Player::texture = playerNormal;
    Player::flipTexture = playerFlipped;

    printf("INITIALIZING GAME\n");
    Level::tileFileMap.insert({'p', "pfood_robot.png"});
    Level::tileFileMap.insert({'t', "tdirt.png"});
    Level::tileFileMap.insert({'c', "cdollar.png"});
    Level::tileFileMap.insert({'n', "nscooter.png"});
    printf("LOADED TILEFILEMAP\n");

    printf("LOADING LEVEL\n");
    Level newLevel("levels/testLevel.txt");
    printf("LOADED LEVEL\n");
    Game::currentLevel = newLevel;

    LCD.SetBackgroundColor(BLUE);
}

void Game::update()
{
    // TEMPORARY GRAPHICS/PHYSICS CODE
    //printf("LOADING FRAME\n");

    

    //printf("COMPUTING LOGIC\n");
    Logic::updateLogic();
    //printf("COMPUTED LOGIC\n");
    
    LCD.Clear();

    //printf("RENDERING GRAPHICS\n");
    Graphics::render();


    LCD.Update();
}

void Game::cleanup() { }