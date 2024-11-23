#pragma once

#define PROTEUS_WIDTH 319
#define PROTEUS_HEIGHT 239
#define DEFAULT_SPRITE_SIZE 16

#include "logic.h"
#include <unordered_map>

#include "FEHImages.h"


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
    static Position origin;
public:

    /**
     * Returns the screen position of parameter &spritePosition
     * based on the camera.
     * 
     * @param &gamePosition
     *      game position to convert to screen position
     * @returns screen position of parameter based on camera
     */
    static Position getScreenPosition(const Position &gamePosition);

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
    static bool isInFrame(const Position &screenPosition, const int spriteWidth, const int spriteHeight);
    static bool isInFrame(const Position &screenPosition);

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
    static void follow(const Position &targetPosition, const int spriteWidth, const int spriteHeight);
    static void follow(const Position &targetPosition);
    
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
    /**
     * A hashmap that maps texture filenames
     * to the texture's memory location - an FEHImage.
     */
    static std::unordered_map<std::string, FEHImage> fileTextureMap;

};

/* Implementations */

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