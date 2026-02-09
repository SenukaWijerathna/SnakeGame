/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : MainGame.h
* Description : - This is the header file for the main gameplay, which contains the code for all the game logic.
*			    - Contains logic for playing the game, pausing the game, and other gameplay-related functions
*/

#pragma once

#include <chrono>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <cwchar>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <Windows.h>

#include "Button.h"
#include "GameRecord.h"
#include "Pixel.h"
#include "SharedFunctionsAndVariables.h"


std::pair<bool, std::deque<std::deque<Pixel>>> gamePlay(GameRecord& game);																//Main game loop.
void changeDirection(int key, GameRecord::Direction& dir);																				//Changes the direction of the snake.
void displayScore(int score);																											//Displays the score banner in the main game loop, pause screen and the game over screen.
bool boundaryCheck(std::deque<std::pair<int, int>> snakeCoords);																		//Checks if the snake has hit the boundary or bit itself.
void setGrid(std::deque<std::deque<Pixel>>& gameGrid, std::deque<std::pair<int, int>> snakeCoords, std::pair<int, int> fruitCoords);	//Changes the nature of the pixels in the grid.
void printGrid(std::deque<std::deque<Pixel>> grid, int score);																			//Displays the grid.
bool pause(int score);																													//Pause screen.
void displayPauseMenu(Button pauseButtons[2], int score);																				//Displays the pause screen.
void gameOverScreen(GameRecord finishedGame, std::deque<std::deque<Pixel>> grid);														//Displays the game over screen and updates records if needed.
bool yesNoScreen(std::string question);																									//Used to display menus with yes no questions.
bool resetHighScores(GameRecord finishedGame);																							//Updates the high score records if necessary.
