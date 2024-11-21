#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class Game {
private:
    static int score;
public:
    static bool running;
    static void start();
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
public:
    Position position;
    Sprite(Position position);
    ~Sprite();
    virtual void render(Position screenPosition) const;
    
    
};

// Represents a movine sprite.
class Entity: public Sprite {
};

// Represents a player.
class Player: public Entity {
};

// Represents an enemy.
class Enemy: public Entity {
};

// Representes an item collectible by the player
class Collectible: public Sprite {
};

// Representes a platform tile
class Tile: public Sprite {
};

// Stores all the data 
class Level {
public:
    std::unordered_set<Enemy> enemies;
    std::unordered_set<Collectible> collectibles;
    std::unordered_set<Tile> tiles;

    Level(const std::string& fileName);
    ~Level();

    // Maps a char on the string vector to a texture.
    std::unordered_map<char, std::string> tileFileMap;

    // Generates unordered of objects based on a vector of strings.
    bool LoadFromFile(const std::string& filename);
};

// Represents a texture loaded into memory.
class Texture {
public:
    Texture(const std::string& filename);
    ~Texture();
};

// Handles the graphics.
class Graphics {
private:
    // Maps a filename to a texture object.
    static std::unordered_map<std::string, Texture> fileTextureMap;
    static void flushGraphicsMemory();
public:
    // Iterate through every object in Level and render it to the screen.
    static void render();
    
    static void loadNewLevel();
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
private:
    
    // Update's the player's state based on
    // user input, collisions, and physics.
    static void updatePlayer();
    // Update's all the enemies' states based on collisions and physics.
    static void updateEnemies();
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
    static void applyGravity(Entity& entity);
    static bool checkCollision(const Entity& entity1, const Entity& entity2);
    static bool checkCollision(const Entity& entity, const Tile& tile);
    static bool checkCollision(const Entity& entity, const Collectible& collectible);
};