#include "logic.h"
#include "ui.h"

#include "FEHLCD.h"
#include "FEHImages.h"

/**
 * Runs when the game opens.
 * Handles navigation between menues
 * and starts up the game.
 */
int main()
{
    while (true)
    {
        // Reset the main menu flag.
        Game::mainMenu = false;

        // Reload the player's scores from the data file.
        Game::loadScores();

        // Start with the main menu.
        int option = menu();

        // Exit condition:
        // The player chooses to play the game.
        while (option != 0)
        {
            switch (option)
            {
                case 1: // Display main menu
                    option = menu();
                    break;
                case 2: // Display statistics
                    option = stats();
                    break;
                case 3: // Display tutorial
                    option = tutorial();
                    break;
                case 4: // Display credits
                    option = credits();
                    break;
            }
        }

        // Start up the game.
        Game::initialize();
        Game::running = true;
        while(Game::running)
        {
            Game::update();
        } 

        Game::cleanup();

        // Don't quit game if the player wants to go back to the menu.
        if (!Game::mainMenu) break;
    }
}

/* Menu functions */

/**
 * Display the main menu and gets user input.
 */
int menu()
{
    // Display code
    LCD.Clear();
    LCD.WriteAt("Food Robot Simulator", 40, 30);
    FEHImage robot("textures/big_food_robot.png");
    robot.Draw(144, 72);
    LCD.WriteAt("Play", 134, 120);
    LCD.WriteAt("Stats", 130, 145);
    LCD.WriteAt("Tutorial", 112, 170);
    LCD.WriteAt("Credits", 118, 195);

    // User input
    bool touched;
    float x;
    float y;

    // Loops until the player selects an option
    while (true)
    {
        x = 0;
        y = 0;
        touched = LCD.Touch(&x, &y);

        // Play button
        if (x > 134 && x < 185 &&
            y > 120 && y < 140 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 0;
        }

        // Stats button
        if (x > 130 && x < 189 &&
            y > 145 && y < 165 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 2;
        }

        // Tutorial button
        if (x > 112 && x < 271 &&
            y > 170 && y < 190 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 3;
        }

        // Credits button
        if (x > 118 && x < 202 &&
            y > 195 && y < 215 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 4;
        }
    }
}

/**
 * Opens the stats menu.
 */
int stats()
{
    // Display statistics to the screen
    LCD.Clear();
    LCD.WriteLine("< Back to Menu");
    LCD.WriteLine(" "); // Blank line
    LCD.WriteLine("Money Collected: $" + std::to_string(Game::money));
    if (Game::bestSeconds < 10)
        LCD.WriteLine("Best Time: " + std::to_string(Game::bestMinutes) + ":0" + std::to_string(Game::bestSeconds));
    else
        LCD.WriteLine("Best Time: " + std::to_string(Game::bestMinutes) + ":" + std::to_string(Game::bestSeconds));
    LCD.WriteLine("Total Score: " + std::to_string(Game::totalScore));

    // User input
    bool touched;
    float x;
    float y;

    // Wait until back button is pressed.
    while (true)
    {
        x = 0;
        y = 0;
        touched = LCD.Touch(&x, &y);

        if (x > 0 && x < 168 &&
            y > 0 && y < 20 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 1;
        }
    }
}

/**
 * Opens the tutorial menu.
 */
int tutorial()
{
    // Tutorial text.
    // NOTE: there may be extra spaces or places where a space was removed.
    // This is because Proteus doesn't create new lines from whitespace resulting in broken words.
    LCD.Clear();
    LCD.WriteLine("< Back to Menu");
    LCD.WriteLine(" ");
    LCD.WriteLine("You are an Ohio State foodrobot.");
    LCD.WriteLine(" ");
    LCD.WriteLine("Deliver the food to the   student's dorm before the time runs out.");
    LCD.WriteLine(" ");
    LCD.WriteLine("Collect money to use the  green scooters to move to the next level.");

    // User input
    bool touched;
    float x;
    float y;

    // Wait until back button is pressed.
    while (true)
    {
        x = 0;
        y = 0;
        touched = LCD.Touch(&x, &y);

        if (x > 0 && x < 168 &&
            y > 0 && y < 20 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 1;
        }
    }
}

/**
 * Opens the credits menu.
 */
int credits()
{
    // Credits display
    LCD.Clear();
    LCD.WriteLine("< Back to Menu");
    LCD.WriteAt("Credits", 118, 45);
    LCD.WriteAt("Game Logic:", 94, 80);
    LCD.WriteAt("Nathan Ramsey", 82, 100);
    LCD.WriteAt("Graphics Programming:", 34, 125);
    LCD.WriteAt("Andrew Loznianu", 70, 145);
    LCD.WriteAt("Proteus Simulator:", 52, 170);
    LCD.WriteAt("Ohio State FEH Department", 10, 190);

    // User input
    bool touched;
    float x;
    float y;

    // Wait until user decides to go back to menu.
    while (true)
    {
        x = 0;
        y = 0;
        touched = LCD.Touch(&x, &y);

        if (x > 0 && x < 168 &&
            y > 0 && y < 20 &&
            touched)
        {
            while (LCD.Touch(&x, &y));
            return 1;
        }
    }
}