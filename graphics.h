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