#pragma once

#define SCORE_X 0
#define SCORE_Y 0
#define TIMER_X 0
#define TIMER_Y 50

#define OUTER_CIRCLE_RADIUS 50
#define INNER_CIRCLE_RADIUS 10

#define OUTER_CIRCLE_COLOR WHITE
#define INNER_CIRCLE_COLOR WHITE

#include <string>


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
     * Draws the game's gameplay UI to the screen.
     */
    static void renderUI();
};

void UIManager::renderUI()
{
    UIManager::renderTimer(Game::gameTimer.getTimeLeft());
}

void UIManager::renderTimer(double timeLeft)
{
    std::string timerMessage = "Time left: " + std::to_string(timeLeft); 
    LCD.WriteAt(timerMessage, TIMER_X, TIMER_Y);
}

void UIManager::renderScore(int score)
{
    std::string scoreMessage = "Score: " + std::to_string(score);
    LCD.WriteAt(scoreMessage, SCORE_X, SCORE_Y);
}

// Handes user input
class InputHandler {
private:
    /**
     * True if the player touched the screen on the last frame.
     * False if the player did not touch the screen on the last frame.
     */
    static bool previousState;
    /**
     * The screen position where the player initially put their finger down.
     */
    static Position touchOrigin;
public:
    /**
     * Updates the game's state based on user's input on the current frame.
     */
    static void processInput();
};

bool InputHandler::previousState = false;
Position InputHandler::touchOrigin = {-1, -1};

void InputHandler::processInput()
{
    // The finger's position on this frame.
    // Undefined values are -1.
    int touchX = -1, touchY = -1;

    // Check if the player is touching the screen on this frame.
    // If they are, update the values inside x and y.
    bool currentState = LCD.Touch(&touchX, &touchY);

    if (currentState)
    {
        if (InputHandler::previousState)
        {
            // The user is holding theif finger down
            // on this frame.
        }
        else
        {
            // The user put their finger down
            // on this frame.

            // Update the origin of the outer circle.
            InputHandler::touchOrigin = {touchX, touchY};
        }

        // Draw the outer circle.
        LCD.SetFontColor(OUTER_CIRCLE_COLOR);
        LCD.DrawCircle(InputHandler::touchOrigin.x, InputHandler::touchOrigin.y, OUTER_CIRCLE_RADIUS);

        // Draw the inner circle.
        LCD.SetFontColor(INNER_CIRCLE_COLOR);
        LCD.DrawCircle(touchX, touchY, INNER_CIRCLE_RADIUS);

        int playerMovementX = std::min(touchX - InputHandler::touchOrigin.x, OUTER_CIRCLE_RADIUS);

        // TODO: Update player's position based on input.
    }
    else
    {
        if (InputHandler::previousState)
        {
            // The user released their finger
            // on this frame.

            // TODO: Make the player jump.
        }
        else
        {
            // The user is not touching the screen
            // on this frame.
        }
    }

    // Update the previous state to the current state.
    InputHandler::previousState = currentState;
}