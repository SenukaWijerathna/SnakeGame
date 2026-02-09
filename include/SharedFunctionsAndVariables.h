/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : SharedFunctionsAndVariables.h
* Description : - This is the header file for the shared functions, which contains the code for functions that is used across the game.
*			    - Contains the screen refreshing logic, logic for changing the color, and the logic for checking the window size.
*/

#pragma once

#include <chrono>
#include <conio.h>
#include <ctime>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <Windows.h>


/**** VARIABLES USED THROUGHOUT THE ENTIRE GAME ****/
const int NUMBER_OF_SLOTS = 10;              //Maximum Number of games that can be saved at a time.
const int GRID_WIDTH = 26;                   //Width of the main gameplay grid.
const int GRID_HEIGHT = 20;                  //Height of the main gameplay grid.
const int VISUAL_WIDTH = GRID_WIDTH * 2 + 2; //Minimum required width of the console window.
const int VISUAL_HEIGHT = GRID_HEIGHT + 4;   //Minimum required height of the console window.
const int BORDER_COLOR = 5;                  //Base background color used throughout the game.
const int TEXT_COLOR = 15;                   //Base text color used throughout the game.
const int BANNER_COLOR = 9;                  //Base background color used in the banners (screen titles) in the game.
const int GAMEPLAY_BACKGROUND_COLOR = 14;    //Background color used in the background of the main gameplay.
const int SCREEN_REFRESH_RATE = 100;		 //Refresh rate of every menu.


/**** FUNCTIONS USED THROUGHOUT THE ENTIRE GAME ****/
void hideTextCursor();																							 //Make the text cursor invisible.
COORD getConsoleSize();																							 //Retrieves the width and height of the console window.
bool windowSizeCheck(int width, int height);																	 //Displays a warning message if the minimum console window width and height requirements are not met.
bool refreshScreen();																							 //Resets the text cursor position and refreshes the screen
void setPixelColor(int backgroundColor, int textColor);															 //Adjusts the colors in the console window.
void displayBanner(std::string text, int bannsoleerWidth, int topBottomGap, int backgroundColor, int textColor); //Prints banners (screen titles)
void displayLine(int backgroundColor);																			 //Prints a single line in the interface.
std::pair<int, int> paddingCalculation(int availableLength, int contentLength);									 //Used for the gap calculations.
void changeChoice(int key, int& choice, int noOfChoices);														 //Enables players to go through each button in the game.
std::pair<bool, int> checkMatch(int row, int col, std::deque<std::pair<int, int>> snakeCoords);                  //Checks if any of the coordinates given matches to the coordinates of a part of snake.
std::deque<std::string> retrieveFileRecords(std::string fileName);	                                             //Reads files (created for TXT files).
void saveFileRecords(std::string fileName, std::deque<std::string> records);                                     //Saves data to files (created for TXT files).