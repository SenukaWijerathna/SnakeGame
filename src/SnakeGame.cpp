/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : SnakeGame.cpp
* Description : - This file contains the main function that executes the 'SNAKE GAME'.
*			    - 'SNAKE GAME' is a simple game that runs in Windows Terminal, whcih features a Text-based User Interface (TUI).
*               - Instead of a basic snake game, this games includes a Pause/Play feature and a TXT-file based Save System, where players can pause and save their game if they wish to continue later.
*               - These saved games are stored in "savedGames.txt" file, while the high scores are saved in "highScores.txt" file.
*               - Colors in the game were made possible by the default Windows Terminal colors.
* 
* Important note : This application only runs on Windows operating systems, as the keystrokes are retrieved based on 'Windows.h' library. 
* 
* WARNING: At a minimum, the width shown in VISUAL_WIDTH variable (Default: 54 characters) and the height shown in VISUAL_HEIGHT variable (Default: 24 characters) MUST be maintained throughout the entire gameplay. 
*          If the console window is less in either the minimum width or height, the gameplay will be temporarily stopped and a warning message will appear in order to prevent glitches from occuring in the interface.
           Gameplay will continue once the width and height requirements are met.

           Most of the variables (used for functions that calculates gap between contents, content width height etc.) were calculated based on GRID_WIDTH and GRID_HEIGHT variables in SharedFunctions
*/

#include "Menu.h"

//Main function
int main()
{
    application();
}