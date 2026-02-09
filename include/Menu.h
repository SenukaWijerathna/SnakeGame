/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : Menu.h
* Description : - This is the header file for the main menu, which contains the code for all the game screens except the gameplay screen.
*			    - Contains logic for starting a new game, saving the necessary games loading and deleting a saved game,viewing high scores, and quitting the game.
*/

#pragma once

#include <algorithm>
#include <cctype>
#include <chrono>
#include <conio.h>
#include <ctime>
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
#include "MainGame.h"
#include "SharedFunctionsAndVariables.h"


/**** FUNCTIONS THAT CONTAINS MAIN MENU LOGIC ****/
void application();																		//Main gameplay loop contains here.
bool changeSelection(bool& status, int& c, int menuSize, std::deque<GameRecord>& g);	//Handles player's keystrokes in the menu.
void directToActivity(bool& status, int c, std::deque<GameRecord>& g);					//Directs the player to different screens based on their choice.
void retrieveGames(std::deque<std::string> rawRecords, std::deque<GameRecord>& g);      //Parses the string records retrieved from the TXT files.
void saveGame(std::deque<GameRecord>& g);                                               //Saves the game.


/**** FUNCTIONS FOR EACH SCREEN ****/
void newGame(std::deque<GameRecord>& g);                   //Code for the 'New Game' Screen.
void loadGame(std::deque<GameRecord>& g);				   //Code for the 'Load Game' Screen.
void deleteGame(std::deque<GameRecord>& g);				   //Code for the 'Delete Games' Screen.
void displayHighScores();								   //Code for the 'High Scores' Screen.
void quitScreen(bool& status, std::deque<GameRecord>& g);  //Code for the quit Screen.


/**** FUNCTIONS USED IN retrieveGames() FUNCTION TO PARSE DIFFERENT DATA ****/
GameRecord::Difficulty stringToDifficulty(std::string difStr);        //Parses the difficulty of the game.
GameRecord::Direction stringToDirection(std::string dirStr);		  //Parses the last direction of the snake.
std::deque<std::pair<int, int>> stringToSnake(std::string snkStr);	  //Parses the coordinates of the snake.
std::pair<int, int> stringToFruit(std::string fStr);				  //Parses the coordinates of the fruit.