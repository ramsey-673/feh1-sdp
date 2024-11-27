#include "graphics.h"
#include "logic.h"
#include "ui.h"


#define PROTEUS_WIDTH 319
#define PROTEUS_HEIGHT 239
#define DEFAULT_SPRITE_SIZE 16


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

FEHImage *Graphics::background;

void Graphics::render()
{
    // Ensure the camera stays centered on the player
    // during this rendering cycle.
    // printf("MOVING TO PLAYER\n");
    Camera::follow(Player::position);

    Graphics::background->Draw(0, 0);


    // printf("RENDERING TILES\n");
    // Iterate through every tile in the level.
    for (const Tile *tile : Game::currentLevel->tiles)
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
    for (const Collectible *collectible : Game::currentLevel->collectibles)
    {
        // Don't render a collectible that has already been picked up.
        if (collectible->collected) continue;
        
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