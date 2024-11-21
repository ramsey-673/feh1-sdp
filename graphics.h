#pragma once

#include "logic.h"

#include "FEHImages.h"



class Camera {
private:
    static Position corner1;
    static Position corner2;
public:
    Camera(Position corner1, Position corner2);
    ~Camera();
    // Returns whether or not the camera can see the sprite.
    static bool isInFrame(Sprite sprite);
    // Returns the position of the sprite
    // relative to the position of the camera.
    static Position getRelativePosition(Sprite sprite);
    static void moveTo(Position targetPosition);
    static void zoom(float scale);

    
};

// Handles the graphics.
class Graphics {
private:
    // Maps a filename to a texture object.
public:
    // Iterate through every object in Level and render it to the screen.
    static void render();
    static std::unordered_map<std::string, FEHImage> fileTextureMap;

};