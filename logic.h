#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>



class Game
{
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



struct Position
{
    int x;
    int y;
};



// Represents a renderable object.
class Sprite
{
private:
    static int idCounter;

public:
    Sprite();
    ~Sprite();
    virtual void render(struct Position screenPosition) const;

    int getId();
};



// Represents a player.
class Player: public Sprite
{
public:
    static Position position;
};



// Representes an item collectible by the player
class Collectible: public Sprite
{
private:
    int id;

public:
    struct Position position;
    Collectible(struct Position position);
    bool operator==(const Collectible& other) const;
    int id() const;
};



// Representes a platform tile
class Tile: public Sprite
{
private:
    int id;

public:
    struct Position position;
    Tile(struct Position position);
    bool operator==(const Collectible& other) const;
    int id() const;
};



class Physics
{
public:
    static void applyGravity();
    static bool checkCollision(const Tile& tile);
    static bool checkCollision(const Collectible& collectible);
};



// Handles game logic.
class Logic
{
public:
    static void updateLogic();
};



// Stores all the data 
class Level
{
public:
    std::unordered_set<Collectible> collectibles;
    std::unordered_set<Tile> tiles;

    Level(const std::string& fileName);
    ~Level();

    // Maps a char on the string vector to a texture.
    std::unordered_map<char, std::string> tileFileMap;
};