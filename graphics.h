#pragma once

#include "utils.h"
#include "FEHImages.h"

/**
 * Handles the relationship between game position and screen position.
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
     * 
     * @author Andrew Loznianu
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
     * 
     * @author Andrew Loznianu
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
    * 
    * @author Andrew Loznianu
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
public:
    /**
     * Iterate through every game object and render them to the screen.
     * 
     * @author Andrew Loznianu
     */
    static void render();

    /**
     * Used to render the background image of the current level.
     */
    static FEHImage *background;
};