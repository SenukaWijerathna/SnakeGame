/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : GameRecord.cpp
* Description : - This provides the definitions for the methods in GameRecord class, which is used to store a single record of a gameplay.
*			    - Used in the Snake Game's save system.
*/

#include "GameRecord.h"

/**** CONSTRUCTOR ****/

GameRecord::GameRecord(std::string n, Difficulty dif, int s, Direction dir, std::deque<std::pair<int, int>> snake, std::pair<int, int> fruit)
{
    name = n;
    difficulty = dif;
    score = s;
    direction = dir;
    snakeCoordinates = snake;
    fruitLastLocation = fruit;

}


/**** GETTERS ****/
std::string GameRecord::getName()
{
    return name;
}

GameRecord::Difficulty GameRecord::getDifficulty()
{
    return difficulty;
}

int GameRecord::getScore()
{
    return score;
}

GameRecord::Direction GameRecord::getDirection()
{
    return direction;
}

std::deque<std::pair<int, int>> GameRecord::getSnakeCoordinates()
{
    return snakeCoordinates;
}

std::pair<int, int> GameRecord::getFruitLastLocation()
{
    return fruitLastLocation;
}


/**** SETTERS ****/

void GameRecord::setname(std::string n)
{
    name = n;
}

void GameRecord::setDifficulty(Difficulty dif)
{
    difficulty = dif;
}

void GameRecord::setScore(int s)
{
    score = s;
}

void GameRecord::setDirection(Direction dir)
{
    direction = dir;
}

void GameRecord::setSnakeCoordinates(std::deque<std::pair<int, int>> snake)
{
    snakeCoordinates = snake;
}

void GameRecord::setFruitLastLocation(std::pair<int, int> fruit)
{
    fruitLastLocation = fruit;
}


/**** OTHER METHODS ****/

/*
* growSnake(std::pair<int, int>) : - This method is used to grow the snake.
*                                  - Used when the snake eats a fruit.
*/
void GameRecord::growSnake(std::pair<int, int> newBodyPart)
{
    snakeCoordinates.push_back(newBodyPart);
}

/*
* gameDetails() : - This method is used to convert the game record to a string, which can be written to a file.
*                 - This returns a string that contains all the details of the game stored.
*/
std::string GameRecord::gameDetails()
{
    std::string record;             //The string that will contain all the information after the conversion.
    std::string difficultyString;
    std::string directionString;
    std::string snakeString;

    switch (difficulty) {
    case EASY:
        difficultyString = "easy";
        break;
    case MEDIUM:
        difficultyString = "medium";
        break;
    case HARD:
        difficultyString = "hard";
        break;
    }

    switch (direction) {
    case UP:
        directionString = "up";
        break;
    case DOWN:
        directionString = "down";
        break;
    case LEFT:
        directionString = "left";
        break;
    case RIGHT:
        directionString = "right";
        break;
    }

    for (int i = 0; i < snakeCoordinates.size(); i++)
    {
        //combines the row and column with a '-' symbol and appends it to the snakeString
        snakeString += std::to_string(snakeCoordinates[i].first) + "-" + std::to_string(snakeCoordinates[i].second);

        //Adds a ':' symbol to separate the middle coordinates
        if (!(snakeCoordinates[i].first == snakeCoordinates.back().first && snakeCoordinates[i].second == snakeCoordinates.back().second))
        {
            snakeString += ":";
        }
    }

    //Appends all the details of the game into the record string.
    record = name + "," + difficultyString + "," + std::to_string(score) + "," + directionString + "," + snakeString + "," + std::to_string(fruitLastLocation.first) + "-" + std::to_string(fruitLastLocation.second);

    return record;
}

/*
* getDifficultyStr() : - This method is used to convert the difficulty to a string.
*                      - Used in 'LOAD GAME' and 'DELETE GAME' screens to display the records.
*/
std::string GameRecord::getDifficultyStr()
{
    switch (difficulty)
    {
    case GameRecord::EASY:
        return "Easy";
    case GameRecord::MEDIUM:
        return "Medium";
    case GameRecord::HARD:
        return "Hard";
    default:
        return "Easy"; //Default return statement to satisfy the compiler's checking process. This statement is never accessed under normal conditions.
    }
}


/*
* moveSnake() : - This method is used to change the coordinates of the snake.
*               - Results in the movement of the snake along the grid.
*/
void GameRecord::moveSnake()
{
    std::pair<int,int> prevCycleHead = snakeCoordinates.front();
    if (direction == UP)
    {
        snakeCoordinates.front().first = snakeCoordinates.front().first - 1;
    }
    else if (direction == DOWN)
    {
        snakeCoordinates.front().first = snakeCoordinates.front().first + 1;
    }
    else if (direction == LEFT)
    {
        snakeCoordinates.front().second = snakeCoordinates.front().second - 1;
    }
    else if (direction == RIGHT)
    {
        snakeCoordinates.front().second = snakeCoordinates.front().second + 1;
    }

    for (int s = snakeCoordinates.size() - 1; s > 0; s--)
    {
        snakeCoordinates[s] = snakeCoordinates[s - 1];
    }
    snakeCoordinates[1] = prevCycleHead;
}


/*
* placeFruit() : - This method is used to change the location of the snake.
*                - Used in the gameplay to place the fruit when snake eats it.
*/
void GameRecord::placeFruit()
{
    //This variable is used to ensure the current location of the fruit is valied (i.e. it is within the grid).
    bool placeValidity = false;

    //This loop ensures that the fruit's location is changed randomly until it it valid.
    do
    {
        //Place the fruit randomly within the grid.
        fruitLastLocation = { (rand() % GRID_HEIGHT) ,(rand() % GRID_WIDTH) };

        //Ensures that the coordinates of the fruit is not as same as the coordinates of snake's body parts.
        if (!checkMatch(fruitLastLocation.first, fruitLastLocation.second, snakeCoordinates).first) placeValidity = true;

    } while (!placeValidity);
}