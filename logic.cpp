#include "logic.h"
#include "ui.h"
#include "graphics.h"





FEHImage *Player::texture;
FEHImage *Player::flipTexture;

Vector Player::position { 50, 50 };

Vector Player::size { 16, 11 };

Vector Player::v { -2, 0 };

int Player::jumpCounter = 0;

// TEMPORARY GRAPHICS CODE
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

std::unordered_map<const char*, FEHImage*> Level::fileTextureMap;
std::unordered_map<char, const char*> Level::tileFileMap;

Level::Level() {

}

Level::Level(const std::string &fileName) {
    // Open the current level's file.
    std::ifstream fileStream;
    fileStream.open(fileName);

    // Check if the file opened successfully.
    if (!fileStream.is_open()) {
        throw 404;
    }
    else
    {
        // Start the current row and column at zero.
        int row = 0, col = 0;


        char objectChar = fileStream.get();
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

                
                // printf("CHECKING IF MAP CONTAINS TEXTURE, %s\n", fileName);

                
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
                    //Player::texture = texture;
                }
                else if (type == 't')
                {
                    // Create a new tile.
                    Vector size;
                    size.x = GRID_CELL_WIDTH;
                    size.y = GRID_CELL_HEIGHT;
                    Tile *newTile = new Tile(gridPosition, size, texture);
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

                // Get the next character.
                objectChar = fileStream.get();
                // Increment column.
                col++;
            }
            // Increment row.
            col = 0;
            row++;
            objectChar = fileStream.get();
        }

        // Close the file.
        fileStream.close();
    }
}

Level::~Level() { }

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
        // Vertical collisions
        if (Player::position.x + Player::size.x > tile.position.x &&
            Player::position.x < tile.position.x + tile.size.x)
        {
            // Bottom of the player hits the top of the tile
            if (Player::position.y < tile.position.y &&
                Player::position.y + Player::size.y + std::ceil(Player::v.y) > tile.position.y)
            {
                if (std::fabs(Player::v.x) > 0.05)
                    Player::v.x /= 1.5;
                else
                    Player::v.x = 0;
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
        Player::position.y < collectible.position.y + collectible.size.y &&
        !collectible.collected && collectible.type != 's')
    {
        Game::score++;
        collectible.collected = true;
    }
}

void Logic::updateLogic()
{
    Physics::applyGravity();
    InputHandler::processInput();
    for (auto i = 0; i < Game::currentLevel.tiles.size(); i++)
        Physics::checkCollision(*Game::currentLevel.tiles[i]);
    for (auto i = 0; i < Game::currentLevel.collectibles.size(); i++)
        Physics::checkCollision(*Game::currentLevel.collectibles[i]);
	Player::position += Player::v;

    if (Player::position.y > 2000) {
        printf("ERROR: Player is out of bounds!");
        
        Game::running = false;
    }
}

int Game::score { 0 };

bool Game::running { false };

Level Game::currentLevel { };

Timer Game::gameTimer(5);

Vector Game::gravity { GRAVITY_X, GRAVITY_Y };

void Game::initialize() {
    FEHImage *playerNormal = new FEHImage("textures/food_robot.png");
    FEHImage *playerFlipped = new FEHImage("textures/food_robot_right.png");

    Player::texture = playerNormal;
    Player::flipTexture = playerFlipped;

    FEHImage *background = new FEHImage("textures/ohio_union_background.png");
    Graphics::background = background;

    printf("INITIALIZING GAME\n");
    Level::tileFileMap.insert({'p', "ptextures/food_robot.png"});
    Level::tileFileMap.insert({'t', "ttextures/dirt.png"});
    Level::tileFileMap.insert({'c', "ctextures/dollar.png"});
    Level::tileFileMap.insert({'n', "ntextures/scooter.png"});
    printf("LOADED TILEFILEMAP\n");

    printf("LOADING LEVEL\n");
    Level newLevel("levels/testLevel.txt");
    printf("LOADED LEVEL\n");
    Game::currentLevel = newLevel;

    LCD.SetBackgroundColor(0x89e0f0);
}

void Game::update()
{
    // TEMPORARY GRAPHICS/PHYSICS CODE
    //printf("LOADING FRAME\n");

    

    //printf("COMPUTING LOGIC\n");
    Logic::updateLogic();
    //printf("COMPUTED LOGIC\n");
    
    LCD.Clear();

    //printf("RENDERING GRAPHICS\n");
    Graphics::render();


    LCD.Update();
}

void Game::cleanup() { }