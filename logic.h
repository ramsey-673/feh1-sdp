#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <vector>

#include "FEHImages.h"

struct Position
{
    int x;
    int y;
};




// Represents a player.
class Player
{
    static FEHImage *texture;

public:
    static Position position;
    static void render(struct Position screenPosition);
};

// Representes an item collectible by the player
class Collectible
{
private:
    static int quantity;
    int id;
    FEHImage *texture;

public:
    struct Position position;
    Collectible(struct Position position, std::string texture);
    int getId() const;
    void render(struct Position screenPosition) const;
};



// Representes a platform tile
class Tile
{
private:
    static int quantity;
    int id;
    FEHImage *texture;

public:
    struct Position position;
    Tile(struct Position position, std::string texture);
    int getId() const;
    void render(struct Position screenPosition) const;
};

// Stores all the data 
class Level
{
public:
    std::vector<Collectible> collectibles;
    std::vector<Tile> tiles;

    Level(const std::string &fileName);
    Level();
    ~Level();

    // Maps a char on the string vector to a texture.
    std::unordered_map<char, std::string> tileFileMap;
};

class Game
{
public:
    static bool running;
    static void initialize();
    static void update();
    static void cleanup();

    static Level currentLevel;
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
