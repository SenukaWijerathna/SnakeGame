/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : GameRecord.h
* Description : - This is the header file for the GameRecord class, which is used to store a single record of a gameplay.
*			    - Used in the Snake Game's save system.
*/

#pragma once

#include <deque>
#include <string>
#include <utility>

#include "SharedFunctionsAndVariables.h"


class GameRecord
{
public:
	//Difficulty enum is used to control the speed of the snake in the game.
	enum Difficulty
	{
		EASY = 200,
		MEDIUM = 150,
		HARD = 100
	};

	//Direction enum is used to control the direction that the snake moves to.
	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

private:
	std::string name;									//Player name
	Difficulty difficulty;								//Difficulty
	int score;											//Score
	Direction direction;								//Direction of the snake
	std::deque<std::pair<int, int>> snakeCoordinates;	//Coordinates of the position of the snake on the grid
	std::pair<int, int> fruitLastLocation;				//Coordinates of the last position of the fruit on the grid

public:
	/**** CONSTRUCTOR ****/
	GameRecord(std::string n, Difficulty dif, int s, Direction dir, std::deque<std::pair<int, int>> snake, std::pair<int, int> fruit);

	/**** GETTERS ****/
	std::string getName();
	Difficulty getDifficulty();
	int getScore();
	Direction getDirection();
	std::deque<std::pair<int, int>> getSnakeCoordinates();
	std::pair<int, int> getFruitLastLocation();

	/**** SETTERS ****/
	void setname(std::string n);
	void setDifficulty(Difficulty dif);
	void setScore(int s);
	void setDirection(Direction dir);
	void setSnakeCoordinates(std::deque<std::pair<int, int>> snake);
	void setFruitLastLocation(std::pair<int, int> fruit);

	/**** OTHER METHODS ****/
	void growSnake(std::pair<int, int> newBodyPart); //Grows the snake after eating a fruit.
	std::string gameDetails();						 //Returns the stored record as a string which can be written to a file.
	std::string getDifficultyStr();					 //Used to get the difficulty attribute as a string to display in the menus.
	void moveSnake();                                //Changes the coordinates of the snake.
	void placeFruit();								 //Changes the location of the fruit.
};