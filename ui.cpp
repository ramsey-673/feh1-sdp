#include "ui.h"
#include "logic.h"
#include "Utils.h"

bool InputHandler::previousState = false;
Vector InputHandler::touchOrigin = {-1, -1};
Vector InputHandler::smallCircle = {-1, -1};

void InputHandler::processInput()
{
    // The finger's position on this frame.
    // Undefined position is (-1, -1).
    Vector touch = {-1, -1};

    // Check if the player is touching the screen on this frame.
    // If they are, update the values inside x and y.
    // Casts float input into integer position.
    bool currentState = LCD.Touch(&touch.x, &touch.y);

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
            InputHandler::touchOrigin = {touch.x, touch.y};
        }

        /* Calculate input offset from touch origin. */

        Vector touchOriginOffset = {touch.x - InputHandler::touchOrigin.x,
            touch.y - InputHandler::touchOrigin.y};

        // Check if the player's x input exceeds the outer circle.
        if (std::fabs(touchOriginOffset.x) > OUTER_CIRCLE_RADIUS)
        {
            // Cap offsetX to the outer circle radius.
            // OffsetX will never be zero if it reached here.
            if (touchOriginOffset.x > 0) {
                // Positive offset
                touchOriginOffset.x = OUTER_CIRCLE_RADIUS;
            }
            else
            {
                // Negative offset
                touchOriginOffset.x = -OUTER_CIRCLE_RADIUS;
            }
        }

        // Check if the player's y input exceeds the outer circle.
        if (std::fabs(touchOriginOffset.y) > OUTER_CIRCLE_RADIUS)
        {
            // Cap offsetX to the outer circle radius.
            // OffsetX will never be zero if it reached here.
            if (touchOriginOffset.y > 0) {
                // Positive offset
                touchOriginOffset.y = OUTER_CIRCLE_RADIUS;
            }
            else
            {
                // Negative offset
                touchOriginOffset.y = -OUTER_CIRCLE_RADIUS;
            }
        }

        /* Draw input UI */

        

        // Calculate the position of the inner circle,
        // which is at the player's touch location if its within the outer circle
        // or at the circumference of the outer circle if the player's touch exceeds it.
        Vector innerCircleOrigin = {InputHandler::touchOrigin.x + touchOriginOffset.x,
            InputHandler::touchOrigin.y + touchOriginOffset.y};

        // Allow Graphics::render() to render the circles.
        InputHandler::smallCircle.x = innerCircleOrigin.x;
        InputHandler::smallCircle.y = innerCircleOrigin.y;

        // Calculate player's x movement as a real scalar from [-1, 1].
        float playerMovementX = (float)touchOriginOffset.x / OUTER_CIRCLE_RADIUS;

        // Update player's horizontal velocity based on input.
        Player::v.x = (playerMovementX * PLAYER_SPEED);
    }
    else
    {
        if (InputHandler::previousState)
        {
            // The user released their finger
            // on this frame.

            // Update touchOrigin to undefined values.
            InputHandler::touchOrigin = {-1, -1};
            InputHandler::smallCircle = {-1, -1};

            // Make the player jump.
            if (Player::jumpCounter > 0) {
                Player::v.y = -JUMP_STRENGTH;
                Player::jumpCounter--;
            }
            
        }
        else
        {
            // The user is not touching the screen
            // on this frame.
        }
    }

    // Update the previous touch state to the current touch state.
    InputHandler::previousState = currentState;
}

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