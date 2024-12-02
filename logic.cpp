#include "logic.h"
#include "ui.h"
#include "graphics.h"

#include <stdio.h>



/* Player */

FEHImage *Player::texture;

FEHImage *Player::flipTexture;

Vector Player::position { 50, 50 };

Vector Player::size { 16, 11 };

Vector Player::v { -2, 0 };

int Player::jumpCounter = 0;

void Player::render(Vector screenPosition)
{
    // Just draw a blue rectangle
    if (Player::v.x <= 0)
    {
        Player::texture->Draw(screenPosition.x, screenPosition.y);
    } else
    {
        Player::flipTexture->Draw(screenPosition.x, screenPosition.y);
    }
	
}



/* Collectible */

Collectible::Collectible(Vector position, Vector size, FEHImage *texture, char type): position(position), size(size), texture(texture), type(type), collected(false) { }

void Collectible::render(Vector screenPosition) const
{
    this->texture->Draw(screenPosition.x, screenPosition.y);
}



/* Tile */

Tile::Tile(Vector position, Vector size, FEHImage *texture): position(position), size(size), texture(texture) { }

// TEMPORARY GRAPHICS CODE
void Tile::render(Vector screenPosition) const
{

    // if (this->texture == NULL) {
    //     printf("This is null\n");
    // }
    // Draw a white rectangle
    // printf("DRAWING tile at (%f, %f)\n", screenPosition.x, screenPosition.y);
	this->texture->Draw(screenPosition.x, screenPosition.y);
    // printf("Finished drawing tile\n");
}



/* Level */

std::unordered_map<const char*, FEHImage*> Level::fileTextureMap;

std::unordered_map<char, const char*> Level::tileFileMap;

Level::Level() { }

Level::Level(const std::string &fileName)
{
    // Used to show waiting screen.
    float startTime = TimeNow();

    // Open the current level's file.
    std::ifstream fileStream;
    fileStream.open(fileName);

    // Count the number of dollars in the current level.
    this->dollarsLeft = 0;

    // Check if the file opened successfully.
    if (!fileStream.is_open())
    {
        throw 404;
    }
    else
    {
        // Start the current row and column at zero.
        int row = 0, col = 0;

        // Get level name from text file.
        std::string levelName;
        std::getline(fileStream, levelName);        

        // Write level name to screen
        LCD.Clear();
        LCD.SetFontColor(WHITE);
        LCD.WriteAt(levelName, 0, PROTEUS_HEIGHT / 2);
        LCD.Update();

        // Set level background.
        std::string levelBackground;
        std::getline(fileStream, levelBackground);
        Graphics::background = new FEHImage(levelBackground.c_str());

        // Read every character in the file.
        char objectChar = fileStream.get();

        // Used to set the play area.
        int maxX = 0;
        while(!fileStream.eof())
        {
            while (objectChar != '\n')
            {
                // A space means we render nothing in this tile.
                if (objectChar == ' ')
                {
                    objectChar = fileStream.get();
                    col++;
                    continue;
                }

                // Create a vector that represents the position
                // of the newly created object.
                Vector gridPosition;

                gridPosition.x = col * GRID_CELL_WIDTH;
                gridPosition.y = row * GRID_CELL_HEIGHT;

                printf("LOADING %c to (%f, %f)\n", objectChar, gridPosition.x, gridPosition.y);

                // Get the object type and texture path from
                // the char -> filePath HashMap.
                const char *fileName = Level::tileFileMap.at(objectChar);

                // Extract the object type from the fileName string literal.
                char type = fileName[0];
                fileName++;
                
                // Check if the texture is not already loaded into memory.
                if (Level::fileTextureMap.find(fileName) == Level::fileTextureMap.end())
                {
                    // printf("ADDING TEXTURE TO FILE TEXTURE MAP\n");
                    // Lead the texture into memory,
                    FEHImage *newTexture = new FEHImage();
                    newTexture->Open(fileName);
                    // and insert it in the fileName -> texture HashMap.
                    fileTextureMap.insert({fileName, newTexture});
                }

                // Initialize a pointer to the FEHImage in the
                // pair with the file name as a key.
                FEHImage *texture = Level::fileTextureMap.find(fileName)->second;

                // Initialize object depending on object type.
                if (type == 'p')
                {
                    // Initialize the player.
                    Player::position.x = gridPosition.x;
                    Player::position.y = gridPosition.y;
                    this->startingPosition = gridPosition;
                    //Player::texture = texture;
                }
                else if (type == 't')
                {
                    // Create a new tile.
                    Vector size;
                    size.x = GRID_CELL_WIDTH;
                    size.y = GRID_CELL_HEIGHT;
                    Tile *newTile = new Tile(gridPosition, size, texture);
                    newTile->deadly = false;
                    this->tiles.push_back(newTile);
                }
                else if (type == 'w')
                {
                    // Create a new water tile.
                    Vector size;
                    size.x = GRID_CELL_WIDTH;
                    size.y = GRID_CELL_HEIGHT;
                    Tile *newTile = new Tile(gridPosition, size, texture);
                    newTile->deadly = true;
                    this->tiles.push_back(newTile);
                }
                else if (type == 'c')
                {
                    // Create a new dollar.
                    Vector size;
                    size.x = GRID_CELL_WIDTH;
                    size.y = GRID_CELL_HEIGHT;
                    Collectible *newCollectible = new Collectible(gridPosition, size, texture, 'd');
                    this->collectibles.push_back(newCollectible);
                    // Increment the number of dollars in the current level.
                    this->dollarsLeft++;
                }
                else if (type == 'T')
                {
                    // Create a transparent tile.
                    Vector size;
                    size.x = GRID_CELL_WIDTH;
                    size.y = GRID_CELL_HEIGHT;
                    Collectible *newCollectible = new Collectible(gridPosition, size, texture, 't');
                    this->collectibles.push_back(newCollectible);
                }
                else if (type == 'P')
                {
                    // Create a prop tile.
                    Vector size;
                    size.x = GRID_CELL_WIDTH;
                    size.y = GRID_CELL_HEIGHT - 5;
                    gridPosition.y += 5;
                    Collectible *newCollectible = new Collectible(gridPosition, size, texture, 't');
                    this->collectibles.push_back(newCollectible);
                }
                else if (type == 'n')
                {
                    // Create a new scooter.
                    Vector size;
                    size.x = GRID_CELL_WIDTH;
                    size.y = GRID_CELL_HEIGHT - 5;
                    gridPosition.y += 5;
                    Collectible *newCollectible = new Collectible(gridPosition, size, texture, 's');
                    this->collectibles.push_back(newCollectible);
                }

                maxX = std::fmax(maxX, gridPosition.x);
                // Get the next character.
                objectChar = fileStream.get();
                // Increment column.
                col++;
            }
            
            col = 0;
            // Increment row.
            row++;
            objectChar = fileStream.get();
        }

        // Set the current level's bottom-right corner.
        this->playLimit = {(float)(maxX) + GRID_CELL_WIDTH - 2, (float)(row) * GRID_CELL_HEIGHT - 1};

        // Close the file.
        fileStream.close();
    }

    // Wait until at least three seconds have passed
    // for the player to read the loading screen.
    // Pause the timer while this is happening.
    Game::gameTimer.Pause();
    while (TimeNow() - 3 < startTime);
    Game::gameTimer.Play();
}

Level::~Level()
{
    // Free all tiles from memory.
    for (Tile *tile : this->tiles)
    {
        delete tile;
    }
    // Free all collectibles from memory.
    for (Collectible *collectible : this->collectibles)
    {
        delete collectible;
    }
}

void Level::restart()
{
    // Resets the player's starting position.
    Player::position = this->startingPosition;
    // Resets the player's velocity.
    Player::v.x = 0;
    Player::v.y = 0;

}



/* Physics */

void Physics::applyGravity()
{
    // Change the player's velocity to simulate gravity
	Player::v += Game::gravity;
}

bool Physics::checkCollision(Tile &tile)
{
    // Check if the player will hit the tile on the next frame.
    if (Player::position.x + Player::size.x + Player::v.x > tile.position.x &&
        Player::position.x + Player::v.x < tile.position.x + tile.size.x &&
        Player::position.y + Player::size.y + Player::v.y > tile.position.y &&
        Player::position.y + Player::v.y < tile.position.y + tile.size.y)
    {
        // Check if the tile is deadly.
        if (tile.deadly)
        {
            Game::currentLevel->restart();
            return true;
        }

        // Vertical collisions
        if (Player::position.x + Player::size.x > tile.position.x &&
            Player::position.x < tile.position.x + tile.size.x)
        {
            // Bottom of the player hits the top of the tile
            if (Player::position.y < tile.position.y &&
                Player::position.y + Player::size.y + std::ceil(Player::v.y) > tile.position.y)
            {
                // Only apply friction if the player isn't trying to move.
                if (InputHandler::touchOrigin.x == -1 && InputHandler::touchOrigin.y == -1)
                {
                    if (std::fabs(Player::v.x) > 0.05)
                        Player::v.x /= 1.5;
                    else
                        Player::v.x = 0;
                }
                Player::v.y = 0;
                Player::position.y = tile.position.y - Player::size.y;

                Player::jumpCounter = NUMBER_JUMPS;
            }
            // Top of the player hits the bottom of the tile
            else if (Player::position.y + Player::size.y > tile.position.y + tile.size.y &&
                     Player::position.y + std::ceil(Player::v.y) < tile.position.y + tile.size.y)
            {
                Player::v.y = 0;
                Player::position.y = tile.position.y + tile.size.y;
            }
        }

        // Horizontal collisions
        if (Player::position.y + Player::size.y > tile.position.y &&
            Player::position.y < tile.position.y + tile.size.y)
        {
            // Right side of the player hits the left side of the tile
            if (Player::position.x < tile.position.x &&
                Player::position.x + Player::size.x + std::ceil(Player::v.x) > tile.position.x)
            {
                Player::v.x = 0;
                Player::position.x = tile.position.x - Player::size.x;
            }
            else if (Player::position.x + Player::size.x > tile.position.x + tile.size.x &&
                     Player::position.x + std::ceil(Player::v.x) < tile.position.x + tile.size.x)
            {
                Player::v.x = 0;
                Player::position.x = tile.position.x + tile.size.x;
            }
        }

        return true;
    }

    return false;
}

bool Physics::checkCollision(Collectible &collectible)
{ 
    // Determine if the player's hitbox overlaps with that of the collectible.
    if (Player::position.x + Player::size.x > collectible.position.x &&
        Player::position.x < collectible.position.x + collectible.size.x &&
        Player::position.y + Player::size.y > collectible.position.y &&
        Player::position.y < collectible.position.y + collectible.size.y)
    {
        if (collectible.type == 'd' && !collectible.collected)
        {
            Game::score++;
            Game::currentLevel->dollarsLeft--;
            collectible.collected = true;
        }
        else if (collectible.type == 's' && Game::currentLevel->dollarsLeft <= 0)
        {
            Game::nextLevel();
        }
    }
}

void Logic::updateLogic()
{
    Physics::applyGravity();
    InputHandler::processInput();
    for (auto i = 0; i < Game::currentLevel->tiles.size(); i++)
        Physics::checkCollision(*Game::currentLevel->tiles[i]);
    for (auto i = 0; i < Game::currentLevel->collectibles.size(); i++)
        Physics::checkCollision(*Game::currentLevel->collectibles[i]);
	Player::position += Player::v;

    if (Player::position.y > 2000)
    {
        printf("ERROR: Player is out of bounds!");
        Game::running = false;
    }
}



/* Game */

int Game::score { 0 };

bool Game::running { false };

bool Game::mainMenu { false };

Level *Game::currentLevel { };

Timer Game::gameTimer(5*60);

Vector Game::gravity { GRAVITY_X, GRAVITY_Y };

int Game::level = 0;

std::vector<std::string> Game::levels = { "levels/union.txt" };

int Game::bestMinutes { 5 };

int Game::bestSeconds { 0 };

int Game::money { 0 };

int Game::totalScore { 0 };

void Game::nextLevel()
{
    // The game is over:
    if (Game::level >= Game::levels.size() - 1)
    {
        // Write end screen.
        LCD.Clear();
        LCD.SetFontColor(WHITE);
        LCD.WriteLine("You win!");
        LCD.WriteLine("Time Left: " + Game::gameTimer.Display());
        LCD.Update();

        // Write the player's scores.
        writeScores(true);

        // Wait so player can read it.
        Sleep(3.0);

        // Close game instance.
        running = false;
        mainMenu = true;
    }
    else
    {
        Level *oldLevel = Game::currentLevel;

        Game::level++;
        Game::currentLevel = new Level(Game::levels[level]);

        // Reset input and player velocity.
        Player::v = { 0, 0 };
        InputHandler::ClearInput();

        delete oldLevel;
    }
}

void Game::gameOver()
{
    // Display game over screen
    LCD.Clear();
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Game Over.");
    LCD.WriteLine("Money Collected: $" + std::to_string(score));
    LCD.Update();
    // Stop so the player can read.
    Sleep(3.0);
    // Stop the game and return to the main menu.
    running = false;
    mainMenu = true;
    // Write scores.
    writeScores(false);
}

void Game::initialize()
{
    
    gameTimer.SetTimer(5*60);
    LCD.SetBackgroundColor(BLACK);

    FEHImage *playerNormal = new FEHImage("textures/food_robot.png");
    FEHImage *playerFlipped = new FEHImage("textures/food_robot_right.png");

    Player::texture = playerNormal;
    Player::flipTexture = playerFlipped;

    printf("INITIALIZING GAME\n");
    Level::tileFileMap.insert({'p', "ptextures/food_robot.png"});
    Level::tileFileMap.insert({'d', "ttextures/dirt.png"});
    Level::tileFileMap.insert({'g', "ttextures/grass.png"});
    Level::tileFileMap.insert({'s', "ttextures/stone.png"});
    Level::tileFileMap.insert({'S', "ttextures/stone_top.png"});
    Level::tileFileMap.insert({'b', "ttextures/stone_bricks.png"});
    Level::tileFileMap.insert({'r', "ttextures/red_bricks.png"});
    Level::tileFileMap.insert({'t', "ttextures/union_floor.png"});
    Level::tileFileMap.insert({'B', "ttextures/border.png"});
    Level::tileFileMap.insert({'D', "wtextures/border.png"});
    Level::tileFileMap.insert({'k', "Ptextures/desk.png"});
    Level::tileFileMap.insert({'h', "Ptextures/chair.png"});
    Level::tileFileMap.insert({'c', "ctextures/dollar.png"});
    Level::tileFileMap.insert({'n', "ntextures/scooter.png"});
    Level::tileFileMap.insert({'w', "wtextures/water.png"});
    Level::tileFileMap.insert({'T', "Ptextures/treadmill.png"});
    Level::tileFileMap.insert({'R', "Ptextures/squat_rack.png"});
    Level::tileFileMap.insert({'P', "Ptextures/bench_press.png"});
    Level::tileFileMap.insert({'i', "ttextures/pillar.png"});
    Level::tileFileMap.insert({'I', "Ttextures/pillar_background.png"});
    Level::tileFileMap.insert({'.', "wtextures/spikes.png"});
    Level::tileFileMap.insert({',', "wtextures/acid.png"});
    Level::tileFileMap.insert({'l', "ttextures/tan-brick.png"});
    Level::tileFileMap.insert({'C', "ntextures/customer.png"});
    printf("LOADED TILEFILEMAP\n");

    printf("LOADING LEVEL\n");
    Game::level = 0;
    Level *newLevel = new Level("levels/union.txt");
    printf("LOADED LEVEL\n");
    Game::currentLevel = newLevel;
}

void Game::update()
{
    // Quit the game if the timer runs out.
    if (gameTimer.Remaining() < 0)
    {
        gameOver();
        return;
    }

    Logic::updateLogic();

    // Go back to the main menu if the player hits the X button.
    if (InputHandler::touchOrigin.x > QUIT_X && InputHandler::touchOrigin.x < QUIT_X + QUIT_X &&
        InputHandler::touchOrigin.y > QUIT_Y && InputHandler::touchOrigin.y < QUIT_Y + QUIT_H)
    {
        running = false;
        mainMenu = true;
        // Write the player's scores to the file.
        writeScores(false);
        // Reset score, input, and player velocity.
        score = 0;
        Player::v = { 0, 0 };
        InputHandler::ClearInput();
        return;
    }
    
    LCD.Clear();
    Graphics::render();
    UIManager::renderUI();

    LCD.Update();
}

void Game::cleanup() { }

void Game::loadScores()
{
    FILE* playerData;
    playerData = fopen("player_data.txt", "r");
    if (playerData == NULL) return;
    fscanf(playerData, "%d", &money);
    fscanf(playerData, "%d:%d", &bestMinutes, &bestSeconds);
    fscanf(playerData, "%d", &totalScore);
    fclose(playerData);
}

void Game::writeScores(bool finished)
{
    // Open output file
    FILE* playerData;
    playerData = fopen("player_data.txt", "w");

    // Total number of dollars player has collected
    fprintf(playerData, "%d\n", money + score);

    // If the player finished the game
    if (finished)
    {
        // Player beat their best time: write the new high score
        if (gameTimer.Minutes() < bestMinutes ||
            gameTimer.Minutes() == bestMinutes && gameTimer.Seconds() < bestSeconds)
        {
            fprintf(playerData, "%d:%d\n", gameTimer.Minutes(), gameTimer.Seconds());
        }
        // Didn't beat their best time, write the old record instead
        else
        {
            fprintf(playerData, "%d:%d\n", bestMinutes, bestSeconds);
        }
        // Update the score, giving points for seconds left when finishing the game
        fprintf(playerData, "%d\n", totalScore + gameTimer.Remaining() * SECOND_VALUE + score * DOLLAR_VALUE);
    }
    // Player didn't finish the game
    else
    {
        // The player can't have beaten their time if they didn't finish
        fprintf(playerData, "%d:%d\n", bestMinutes, bestSeconds);
        // Update score, but only count the money the player collected
        fprintf(playerData, "%d\n", totalScore + score * DOLLAR_VALUE);
    }

    fclose(playerData);
}