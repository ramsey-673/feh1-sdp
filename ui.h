#pragma once

#include "logic.h"

#include "FEHLCD.h"

#include <cmath>
#include <string>

#define SCORE_X 0
#define SCORE_Y 0
#define TIMER_X 0
#define TIMER_Y 50

#define SCORE_COLOR WHITE
#define TIMER_COLOR WHITE




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
    static void renderTimer(double timeLeft);
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

void UIManager::renderUI()
{
    UIManager::renderTimer(Game::gameTimer.getTimeLeft());
    UIManager::renderScore(Game::score);
}

void UIManager::renderTimer(double timeLeft)
{
    //Generate output message.
    std::string timerMessage = "Time left: " + std::to_string(timeLeft); 

    // Draw timer to screen.
    LCD.SetFontColor(TIMER_COLOR);
    LCD.WriteAt(timerMessage, TIMER_X, TIMER_Y);
}

void UIManager::renderScore(int score)
{
    // Generate output message.
    std::string scoreMessage = "Score: " + std::to_string(score);

    // Draw score to screen.
    LCD.SetFontColor(SCORE_COLOR);
    LCD.WriteAt(scoreMessage, SCORE_X, SCORE_Y);
}

