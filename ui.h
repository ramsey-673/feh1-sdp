#pragma once

#include "FEHLCD.h"
#include "utils.h"

#include <cmath>
#include <string>

// Positions for the score and timer
#define SCORE_X 5
#define SCORE_Y 25
#define TIMER_X 5
#define TIMER_Y 5

// Dimensions for the score/timer box
#define BOX_X 0
#define BOX_Y 0
#define BOX_W 60
#define BOX_H 45

// Dimensions for the quit button
#define QUIT_X 300
#define QUIT_Y 0
#define QUIT_W 20
#define QUIT_H 20
#define QUIT_PADDING 3

#define OUTER_CIRCLE_RADIUS 35
#define INNER_CIRCLE_RADIUS 10

#define OUTER_CIRCLE_COLOR WHITE
#define INNER_CIRCLE_COLOR WHITE

#define SCORE_COLOR WHITE
#define TIMER_COLOR WHITE
#define HUD_COLOR BLACK
#define HUD_BORDER WHITE



// Menu functions
int menu();
int stats();
int tutorial();
int credits();

/**
 * Handles the in-game UI.
 */
class UIManager {
private:
    /**
     * Draws the game timer to the screen.
     * 
     * @param timeLeft
     *      the current value of the timer
     * 
     * @author Andrew Loznianu
     */
    static void renderTimer();
    /**
     * Draws the game score to the screen.
     * 
     * @param score
     *      the current value of score
     * 
     * @author Andrew Loznianu
     */
    static void renderScore(int score);
public:
    /**
     * Draws the game's gameplay UI to the screen
     * excluding player input UI.
     * 
     * @author Andrew Loznianu
     */
    static void renderUI();
};

/**
 * Handles in-game user input.
 */
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
     * 
     * @author Andrew Loznianu
     */
    static void processInput();
    /**
     * The screen position where the player initially put their finger down.
     * Undefined position is (-1, -1).
     */
    static Vector touchOrigin;
    static Vector smallCircle;

    /**
     * Clears input and the previous state.
     * 
     * @author Nathan Ramsey
     */
    static void ClearInput();
};