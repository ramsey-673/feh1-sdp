#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <vector>

#include "FEHImages.h"
#include "FEHUtility.h"

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

/**
 * A timer that begins running when instantiated.
 * Used to keep track of game's timer.
 */
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
     * Checke if the timer is ongoing.
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

class Game
{
public:
    static bool running;
    static void initialize();
    static void update();
    static void cleanup();

    static Level currentLevel;

    static int score;
    static Timer gameTimer;
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
