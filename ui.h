#pragma once

#include "FEHLCD.h"
#include "utils.h"

#include <cmath>
#include <string>

#define SCORE_X 5
#define SCORE_Y 25
#define TIMER_X 5
#define TIMER_Y 5

#define BOX_X 0
#define BOX_Y 0
#define BOX_W 60
#define BOX_H 45

#define OUTER_CIRCLE_RADIUS 35
#define INNER_CIRCLE_RADIUS 10

#define OUTER_CIRCLE_COLOR WHITE
#define INNER_CIRCLE_COLOR WHITE

#define SCORE_COLOR WHITE
#define TIMER_COLOR WHITE
#define BOX_COLOR BLACK
#define BOX_BORDER WHITE



// Menu functions
int menu();
int stats();
int tutorial();
int credits();

class UIManager {
private:
    /**
     * Draws the game timer to the screen.
     * 
     * @param timeLeft
     *      the current value of the timer
     */
    static void renderTimer();
    /**
     * Draws the game score to the screen.
     * 
     * @param score
     *      the current value of score
     */
    static void renderScore(int score);
public:
    /**
     * Draws the game's gameplay UI to the screen
     * excluding player input UI.
     */
    static void renderUI();
};



// Handes user input
class InputHandler {
private:
    /**
     * True if the player touched the screen on the last frame.
     * False if the player did not touch the screen on the last frame.
     */
    static bool previousState;
    
public:
    /**
     * Updates the game's state based on user's input on the current frame.
     */
    static void processInput();
    /**
     * The screen position where the player initially put their finger down.
     * Undefined position is (-1, -1).
     */
    static Vector touchOrigin;
    static Vector smallCircle;

};