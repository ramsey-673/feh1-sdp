#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "FEHImages.h"

class Game {
private:
    static int score;
public:
    static bool running;
    static void initialize();
    static void update();
    static void cleanup();

    static Level currentLevel;
    static Player mainCharacter;
    static Timer gameTimer;

    static int getScore();
};
class Position {
public:
    int x;
    int y;
    Position(int x, int y);
    Position();
};
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
// Represents a renderable object.
class Sprite {
private:
    static int idCounter;
public:
    
    Sprite();
    ~Sprite();
    virtual void render(Position screenPosition) const;

    int getId();
    
};


// Represents a player.
class Player: public Sprite {
};

// Representes an item collectible by the player
class Collectible: public Sprite {
private:
    int id;
public:
    Position position;
    Collectible(Position position);
    bool operator==(const Collectible& other) const;
    int id() const;
};

// Representes a platform tile
class Tile: public Sprite {
private:
    int id;
public:
    Position position;
    Tile(Position position);
    bool operator==(const Collectible& other) const;
    int id() const;
};

// Stores all the data 
class Level {
public:
    std::unordered_set<Collectible> collectibles;
    std::unordered_set<Tile> tiles;

    Level(const std::string& fileName);
    ~Level();

    // Maps a char on the string vector to a texture.
    std::unordered_map<char, std::string> tileFileMap;
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

class UIManager {
private:
    static void renderTimer(int timeLeft);
    static void renderScore(int score);
public:
    static void renderUI();
};

// Handles game logic.
class Logic {
public:
    
    static void updateLogic();
};

// Timer object
class Timer {
private:
    int startTime;
public:
    Timer(int duration);
    void update();  // Decrement the timer
    bool IsTimeUp() const; // Returns true if the time is up
    int getTimeLeft() const; // Get remaining time
};

// Handes user input
class InputHandler {
public:
    static void processInput();
};

class Physics {
public:
    static void applyGravity();
    static bool checkCollision(const Tile& tile);
    static bool checkCollision(const Collectible& collectible);
};