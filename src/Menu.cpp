/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : Menu.cpp
* Description : - This provides the definitions for the main menu, which contains the code for all the game screens except the gameplay screen.
* 			    - Contains logic for starting a new game, saving the necessary games loading and deleting a saved game,viewing high scores, and quitting the game.
*/

#include "Menu.h"


using namespace std;


/**** FUNCTIONS THAT CONTAINS MAIN MENU LOGIC ****/

/*
* application() : - This is the function that contains the game loop.
*/
void application()
{
	//Uses the current time as a seed for the random number generator.
	srand(unsigned(time(NULL)));
	std::deque<GameRecord> games;         //Deque that contains all the saved games. Game records are loaded to this deque at the start of the program, and this enables saving and modification of game records.
	
	//Declaration of the required variables.
	bool appStatus = true;                                                                                        //Running status of the game.
	deque<Button> mainMenu = { Button("New Game"),Button("Load Game"),Button("High Scores"),Button("Quit") };     //Deque that contains the main menu buttons.
	int choice = 0;                                                                                               //Currently selected button.
	bool blinkToggle = false;                                                                                     //Used to enable the 'blinking' effect of the selected button.

	//Calculation of the gaps (Left and right gap of the buttons, gap above the button etc.)
	pair<int, int> buttonLeftRightGap = paddingCalculation(((GRID_WIDTH * 2)), mainMenu[0].getWidth());
	int buttonGap = 1;
	int contentHeight = (mainMenu[0].getHeight() + buttonGap) * mainMenu.size();
	pair<int, int> topBottomGap = paddingCalculation(GRID_HEIGHT, contentHeight);

	//Load the games from the TXT file that contains the saved games.
	retrieveGames(retrieveFileRecords("savedGames.txt"), games);

	//Clears the console window before starting the game.
	system("cls");

	//Game loop. Runs until the player confirms quitting the game.
	while (appStatus)
	{
		//Refresh and display the title card
		refreshScreen();
		displayBanner("SNAKE GAME", VISUAL_WIDTH, 1, BANNER_COLOR, TEXT_COLOR);

		//Checks for keystrokes and directs to another screen if needed.
		if (_kbhit())
		{
			bool visitedSub = changeSelection(appStatus, choice, (int)mainMenu.size(), games);

			//Refreshes the screen if the player didn't enter into another screen.
			if (visitedSub)
			{
				refreshScreen();
				displayBanner("SNAKE GAME", VISUAL_WIDTH, 1, BANNER_COLOR, TEXT_COLOR);
			}
		}

		//Changes the blinking status, enabling the 'blinking' effect.
		blinkToggle = !blinkToggle;

		for (int i = 0; i < topBottomGap.first; i++) displayLine(BORDER_COLOR);

		//Display the main menu buttons.
		for (int i = 0; i < mainMenu.size(); i++)
		{
			//Checks if the current button is selected and changes the color if needed.
			if (i == choice)
			{
				mainMenu[i].setSelectionStatus(true);
				mainMenu[i].setBlinking(blinkToggle);
			}
			else
			{
				mainMenu[i].setSelectionStatus(false);
			}

			//Displays the button.
			for (int line = 0; line < mainMenu[0].getHeight(); line++)
			{
				setPixelColor(BORDER_COLOR, 0);
				cout << " ";
				mainMenu[i].printButton(line, buttonLeftRightGap.first, buttonLeftRightGap.second, BORDER_COLOR);
				setPixelColor(BORDER_COLOR, 0);
				cout << " " << endl;
				setPixelColor(0, 7);
			}
			for (int i = 0; i < buttonGap; i++) displayLine(BORDER_COLOR);
		}
		for (int i = 0; i < topBottomGap.second; i++) displayLine(BORDER_COLOR);

		//Delay between frames.
		this_thread::sleep_for(chrono::milliseconds(SCREEN_REFRESH_RATE));
	}

	//Clears the console after quitting the game.
	system("cls");
}


/*
* changeSelection(bool& status, int& c, int menuSize, std::deque<GameRecord>& g) : - This is the function that enables interaction with the game's menus according to the player's keystrokes.
*/
bool changeSelection(bool& status, int& c, int menuSize, std::deque<GameRecord>& g)
{
	//retrieve the player keystroke
	int key = _getch();

	//'Enter' button
	if (key == 13)
	{
		//Directs the player to another game screen.
		directToActivity(status, c, g);
		return true;
	}

	//Arrow keys
	else if (key == 0 || key == 224) {
		//Changes the button selected.
		int arrow = _getch();
		changeChoice(arrow, c, menuSize);
	}
	return false;
}


/*
* directToActivity(bool& status, int c) : - This is the function that directs the player to a different screen of choice.
*/
void directToActivity(bool& status, int c, std::deque<GameRecord>& g)
{
	switch (c)
	{
	case 0:
		newGame(g);              //Directs to 'New Game'.
		break;
	case 1:
		loadGame(g);             //Directs to 'Load Game'.
		break;
	case 2:
		displayHighScores();     //Directs to 'High Scores'.
		break;
	case 3:
		quitScreen(status, g);   //Directs to quit screen.
		break;
	default:
		break;
	}
}


/*
* retrieveGames(deque<string> rawRecords) : - This is the function that parses the string records retrieved from the TXT file into understandable GameRecord objects.
*/
void retrieveGames(deque<string> rawRecords, deque<GameRecord>& g)
{
	//Goes through each record retrieved from the TXT file.
	for (int i = 0; i < rawRecords.size(); i++)
	{
		//Declaration of string variables to separate the data items in a record.
		string name;
		string difficultyStr;
		string scoreStr;
		string directionStr;
		string snakeStr;
		string fruitStr;


		//Declaration of variables to store the data items after parsing before appending to the games Deque.
		GameRecord::Difficulty difficulty;
		int score;
		GameRecord::Direction direction;
		deque<pair<int, int>> convertedSnake;
		pair<int, int> convertedFruit;

		//Converts the raw string into a string stream so the data items can be separated by the commas.
		stringstream record(rawRecords[i]);

		getline(record, name, ',');						//Parse name

		getline(record, difficultyStr, ',');			//Parse difficulty
		difficulty = stringToDifficulty(difficultyStr);

		getline(record, scoreStr, ',');					//Parse score
		score = stoi(scoreStr);

		getline(record, directionStr, ',');				//Parse direction
		direction = stringToDirection(directionStr);

		getline(record, snakeStr, ',');					//Parse snake's coordinates
		convertedSnake = stringToSnake(snakeStr);

		getline(record, fruitStr);						//Parse fruit's coordinates
		convertedFruit = stringToFruit(fruitStr);

		//Append to the games deque.
		g.push_back(GameRecord(name, difficulty, score, direction, convertedSnake, convertedFruit));
	}
}


/*
* saveGame() : - This is the function that saves the game records into the TXT file.
*/
void saveGame(deque<GameRecord>& g)
{
	deque<string> saveGames;
	for (int r = 0; r < g.size(); r++)
	{
		saveGames.push_back(g[r].gameDetails());
	}

	saveFileRecords("savedGames.txt", saveGames);
}


/*
* newGame(deque<GameRecord>& g) : - This function enables players to start a new game
*/
void newGame(deque<GameRecord>& g)
{
	bool gameOver = false;								   //Signals the program if the new game is over after returning from the gameplay.
	bool gameRunning = true;							   //Used for the while loop that display the screen that allows players to enter their name and the difficulty. 
	string name;										   //Used to store the name while player is typing it.
	GameRecord::Difficulty difficulty = GameRecord::EASY;  //Initializes the difficulty.
	int score = 0;										   //Initializes the score.
	GameRecord::Direction dir = GameRecord::RIGHT;		   //Initializes the snake's direction.
	deque<pair<int, int>> snake;						   //Initializes the snake.
	pair<int, int> fruit;								   //Initializes the fruit.
	
	

	int difficultyButtonWidth = 22;
	pair<int, int> leftRightPadding = paddingCalculation(VISUAL_WIDTH, difficultyButtonWidth);
	bool nameIsChosen = false;							   //Signals the program if the name is chosen.
	int choice = 0;										   //Signals which data player is currently entering (Name/Difficulty).
	const int totalButtons = 4;
	bool blinkToggle = false;

	//Array that contains the buttons in the new game screen.
	Button newGameMenuButtons[totalButtons] = { Button("Name:",7,3),Button("Easy",difficultyButtonWidth,3) ,Button("Medium",difficultyButtonWidth,3) ,Button("Hard",difficultyButtonWidth,3) };
	
	//Calculations for the gaps after name and after the difficulty selection buttons.
	int widthAfterNameBtn = (GRID_WIDTH * 2) - newGameMenuButtons[0].getWidth() - 1;
	int bottomPad = VISUAL_HEIGHT - 3 - 1 - newGameMenuButtons[0].getHeight() - 2 - ((newGameMenuButtons[1].getHeight() + 1) * 3);
	int maxNameLength = widthAfterNameBtn - 2;
	if (bottomPad < 0) bottomPad = 0;

	//New game loop.
	while (gameRunning)
	{
		refreshScreen();

		//Checks for keystrokes.
		if (_kbhit())
		{
			int key = _getch();
			//Escape key.
			if (key == 27)
			{
				return;
			}

			//Enter key.
			else if (key == 13)
			{
				//Confirms the name and allows players to select the difficulty.
				if (nameIsChosen == false && name.length() > 0)
				{
					nameIsChosen = true;
					choice += 1;
				}

				//Selection of difficulty level.
				else if (choice == 1)
				{
					difficulty = GameRecord::EASY;
					gameRunning = false;
				}
				else if (choice == 2)
				{
					difficulty = GameRecord::MEDIUM;
					gameRunning = false;
				}
				else if (choice == 3)
				{
					difficulty = GameRecord::HARD;
					gameRunning = false;
				}
			}

			//Checks for special keys (i.e. arrow keys).
			else if (key == 0 || key == 224)
			{
				int arrow = _getch();

				//Up/Left
				if (arrow == 72 || arrow == 75)
				{
					if (nameIsChosen && (choice - 1) == 0)
					{
						nameIsChosen = false;
						choice = 0;
						continue;
					}
					else if (nameIsChosen && choice > 1)
					{
						choice -= 1;
					}
				}

				//Down/Right
				else if (arrow == 80 || arrow == 77)
				{
					if (nameIsChosen && choice < totalButtons - 1)
					{
						choice += 1;
					}
					else if (nameIsChosen && choice == totalButtons - 1)
					{
						choice = 1;
					}

				}
			}
			else if (key == ','); //IMPORTANT: Prevents players from entring commas in the name. This is very important as the game's save system differentiates data in records by commas.

			//These two else-if statements contains the 'choice==0' check to ensure that the player can't modify the name while going through the difficulty options.
			//Makes sure that the name does not exceed the screen width.
			else if (isprint(key) && name.length() < maxNameLength && choice == 0)
			{
				name += string(1, key);
			}

			//Backspace
			else if (key == 8 && choice == 0)
			{
				if (!name.empty())
				{
					name.pop_back();
				}
			}
		}

		//Enables the blinking effect.
		blinkToggle = !blinkToggle;
		for (int b = 0; b < totalButtons; b++)
		{
			newGameMenuButtons[b].setSelectionStatus(false);
		}
		newGameMenuButtons[choice].setSelectionStatus(true);
		newGameMenuButtons[choice].setBlinking(blinkToggle);

		//Used to display the area that shows the name.
		string nameDisplay[3];
		nameDisplay[0] = string(widthAfterNameBtn, ' ');
		nameDisplay[1] = " " + name + string(widthAfterNameBtn - 1 - name.length(), ' ');
		nameDisplay[2] = string(widthAfterNameBtn, ' ');

		//Display starts from here.
		refreshScreen();
		displayBanner("NEW GAME", VISUAL_WIDTH, 1, BANNER_COLOR, TEXT_COLOR);
		displayLine(BORDER_COLOR);

		//Displays the area that shows the player name
		for (int line = 0; line < 3; line++)
		{
			newGameMenuButtons[0].printButton(line, 1, 1, BORDER_COLOR);
			setPixelColor(1, 15);
			cout << nameDisplay[line];
			setPixelColor(BORDER_COLOR, 0);
			cout << " " << endl;
			setPixelColor(0, 7);
		}
		displayLine(BORDER_COLOR);

		//Displays the difficulty selection buttons.
		for (int b = 1; b < totalButtons; b++)
		{
			for (int line = 0; line < newGameMenuButtons[1].getHeight(); line++)
			{
				newGameMenuButtons[b].printButton(line, leftRightPadding.first, leftRightPadding.second, BORDER_COLOR);
				cout << endl;
			}
			displayLine(BORDER_COLOR);
		}
		for (int i = 0; i < bottomPad; i++) displayLine(BORDER_COLOR);
		this_thread::sleep_for(chrono::milliseconds(SCREEN_REFRESH_RATE));
	}

	//Creates a new Game Record with the details entered by the player.
	GameRecord newGame(name, difficulty, score, dir, snake, fruit);

	//Creates the snake's body and assigns a location to the fruit.
	for (int s = 10; s > 7; s--) newGame.growSnake({ 10,s });
	newGame.placeFruit();
	
	//Passes the new game record to the main gameplay function. Returns the game over status.
	pair<bool, deque<deque<Pixel>>> game = gamePlay(newGame);
	gameOver = game.first;
	deque<deque<Pixel>> gameGrid = game.second;


	//Saves the game if it is not finished.
	if (gameOver == false)
	{
		//Used in the save message that shows to the player.
		string saveMessage;

		//Checks if there are enough slots to save the current game. Otherwise informs the user.
		if (g.size() < NUMBER_OF_SLOTS)
		{
			g.push_back(newGame);
			saveGame(g);
			saveMessage = "Game Saved Successfully!";
		}
		else saveMessage = "No available space!";

		//Displays the message that says either the game is saved successfully or not.
		refreshScreen();

		//Two rows were manually changed between the top and bottom paddings to center the save message properly, while using the common padding calculation function.
		pair <int, int> saveScreenTopBottom = paddingCalculation(GRID_HEIGHT, (3+2));
		for (int i = 0; i < saveScreenTopBottom.second+2; i++) displayLine(BORDER_COLOR);
		displayBanner(saveMessage, VISUAL_WIDTH, 1, BANNER_COLOR, 15);
		for (int i = 0; i < saveScreenTopBottom.first; i++) displayLine(BORDER_COLOR);
		this_thread::sleep_for(chrono::milliseconds(3000));
	}

	//Displays the 
	else
	{
		gameOverScreen(newGame, gameGrid);
	}
}


/*
* loadGame(deque<GameRecord>& g) : - This function enables players to either continue a game or directs them to the delete screen if needed.
*/
void loadGame(deque<GameRecord>& g)
{
	//Checks if there are any saved games. Otherwise a message will be shown to the player saying there are no saved games.
	if (!g.empty())
	{
		int gameOver = false;                      //Signals the program if the game player continued (if they did) is over or not. 
		bool menuStatus = true;                    //Used for the screen menu.
		int recordsPerFrame = 3;                   //Maximum number of records shown in the display at a single frame.
		int recordHeight = 3;                      //Height of each record.
		int returnBtnIndex = NUMBER_OF_SLOTS;      //Index of the 'Return to Menu' button.
		int deleteBtnIndex = returnBtnIndex + 1;   //Index of the button that directs player to the delete game screen.
		int totalChoices = NUMBER_OF_SLOTS + 2;    //10 slots + Return button + Delete button
		int recordColor = 1;					   //Color used to display the saved games.
		int emptySlotColor = 9;                    //Color used to display an occupied slot

		//Initialization of the buttons.
		deque<Button> selectButtons;
		for (int r = 0; r < NUMBER_OF_SLOTS; r++)
		{
			selectButtons.push_back(Button("", 2, recordHeight));
		}
		selectButtons.push_back(Button("Return to Menu"));
		selectButtons.push_back(Button("Delete a Game"));

		int choice = 0;
		int topRecord = 0;
		int bottomRecord = topRecord + recordsPerFrame;
		bool blinkToggle = false;
		int selectionStatus = true;

		//Calculation of the gaps.
		int topGap = 1;
		int gapAfterRecords = 2;
		int bottomGap = GRID_HEIGHT - topGap - ((recordHeight + 1) * recordsPerFrame) - gapAfterRecords - selectButtons[returnBtnIndex].getHeight();
		if (bottomGap < 0) bottomGap = 0;

		int gapBetweenButtons = 2;
		int totalBtnWidth = selectButtons[returnBtnIndex].getWidth() + gapBetweenButtons + selectButtons[deleteBtnIndex].getWidth();
		int btnLeftPadding = (VISUAL_WIDTH - totalBtnWidth) / 2;
		int btnRightPadding = VISUAL_WIDTH - btnLeftPadding - totalBtnWidth;

		//Load screen display loop.
		while (menuStatus)
		{
			refreshScreen();

			//Checks for keystrokes.
			if (_kbhit())
			{
				int key = _getch();

				//Escape key.
				if (key == 27)
				{
					menuStatus = false;
					continue;
				}

				//Enter key
				if (key == 13)
				{
					//If the player selects a saved game, continue the game.
					if (choice < g.size())
					{
						refreshScreen();
						pair<bool, deque<deque<Pixel>>> game = gamePlay(g[choice]);
						gameOver = game.first;
						deque<deque<Pixel>> gameGrid = game.second;

						//If the game is over, remove it from the records.
						if (gameOver)
						{
							gameOverScreen(g[choice], gameGrid);
							deque<GameRecord> resetRecords;
							for (int r = 0; r < g.size(); r++)
							{
								if (r != choice) resetRecords.push_back(g[r]);
							}
							g = resetRecords;
						}

						//If the selected game is not over, updates the record and saves it to the file to preserve it.
						else
						{
							saveGame(g);
							refreshScreen();
							//two rows were manually changed between the top and bottom paddings to center the save message.
							pair <int, int> saveScreenTopBottom = paddingCalculation(GRID_HEIGHT, (3 + 2));
							for (int i = 0; i < saveScreenTopBottom.second + 2; i++) displayLine(BORDER_COLOR);
							displayBanner("Game Saved Successfully!", VISUAL_WIDTH, 1, BANNER_COLOR, 15);
							for (int i = 0; i < saveScreenTopBottom.first; i++) displayLine(BORDER_COLOR);
							this_thread::sleep_for(chrono::milliseconds(3000));
						}
						menuStatus = false;
						continue;
					}

					//Returns to the menu if the player selects the return button.
					else if (choice == returnBtnIndex)
					{
						menuStatus = false;
					}

					//Directs the player to the delete game screen.
					else if (choice == deleteBtnIndex)
					{
						choice = 0;
						deleteGame(g);
					}
					continue;
				}

				//Changes the selected button according to arrow keys.
				else if (key == 0 || key == 224) {
					int arrow = _getch();
					changeChoice(arrow, choice, totalChoices);
				}
			}

			//Enables the scrolling logic to go through all the records, even the empty slots.
			if (choice < NUMBER_OF_SLOTS)
			{
				if (choice >= topRecord + recordsPerFrame) topRecord = choice - (recordsPerFrame - 1);
				if (choice < topRecord) topRecord = choice;
			}

			//Blinking effect of the currently selected button.
			blinkToggle = !blinkToggle;
			for (int b = 0; b < selectButtons.size(); b++) selectButtons[b].setSelectionStatus(false);
			selectButtons[choice].setSelectionStatus(true);
			selectButtons[choice].setBlinking(blinkToggle);

			//Display starts from here.
			displayBanner("LOAD GAME", VISUAL_WIDTH, 1, 9, 15);
			for (int g = 0; g < topGap; g++) displayLine(BORDER_COLOR);

			//Prints three records at a time.
			for (int i = 0; i < recordsPerFrame; i++)
			{
				int r = topRecord + i; //Slot index

				if (r >= NUMBER_OF_SLOTS) break;

				string recordStr[3];
				//Prepares to display a saved game record.
				if (r < (int)g.size())
				{
					recordStr[0] = "Name: " + g[r].getName();
					recordStr[1] = "Difficulty: " + g[r].getDifficultyStr();
					recordStr[2] = "Score: " + to_string(g[r].getScore());
				}

				//Prepares to display an empty game slot.
				else
				{
					string emptyMsg = "(EMPTY SLOT)";
					int emptyMsgLeft = max(0, (int)(((GRID_WIDTH * 2) - (selectButtons[0].getWidth() + 1 + emptyMsg.length())) / 2));
					string padStr(emptyMsgLeft, ' ');

					recordStr[0] = "";
					recordStr[1] = padStr + emptyMsg;
					recordStr[2] = "";
				}

				//Adds spaces to properly display the records.
				for (int k = 0; k < 3; k++)
				{
					int recordRightPadding = (GRID_WIDTH * 2) - (selectButtons[r].getWidth() + 1 + (int)recordStr[k].length());
					if (recordRightPadding > 0) recordStr[k] += string(recordRightPadding, ' ');
				}

				//Display the record or the empty slot.
				for (int line = 0; line < recordHeight; line++)
				{
					setPixelColor(BORDER_COLOR, 0);
					cout << " ";
					selectButtons[r].printButton(line, 0, 0, 14);
					setPixelColor(BORDER_COLOR, 0);
					cout << " ";

					setPixelColor((r < g.size() ? recordColor : emptySlotColor), 15);
					cout << recordStr[line];

					setPixelColor(BORDER_COLOR, 0);
					cout << " " << endl;
					setPixelColor(0, 7);
				}
				displayLine(BORDER_COLOR);
			}

			//Displays the remaining gap and the return and delete buttons.
			for (int g = 0; g < gapAfterRecords; g++) displayLine(BORDER_COLOR);
			for (int line = 0; line < selectButtons[returnBtnIndex].getHeight(); line++)
			{
				selectButtons[returnBtnIndex].printButton(line, btnLeftPadding, 0, BORDER_COLOR);
				selectButtons[deleteBtnIndex].printButton(line, gapBetweenButtons, btnRightPadding, BORDER_COLOR);
				cout << endl;
			}
			for (int g = 0; g < bottomGap; g++) displayLine(BORDER_COLOR);
			this_thread::sleep_for(chrono::milliseconds(SCREEN_REFRESH_RATE));
		}
	}
	//Prints a no records message if there aren't any saved games.
	else
	{
		refreshScreen();
		displayBanner("LOAD GAME", VISUAL_WIDTH, 1, BANNER_COLOR, 15);
		pair<int, int> topBottomPad = paddingCalculation(GRID_HEIGHT, 3);
		for (int i = 0; i < topBottomPad.first; i++) displayLine(BORDER_COLOR);
		displayBanner("There Are No Saved Games. Keep Playing!", VISUAL_WIDTH, 1, BANNER_COLOR, 15);
		for (int i = 0; i < topBottomPad.second; i++) displayLine(BORDER_COLOR);
		this_thread::sleep_for(chrono::milliseconds(3000));
	}
}


/*
* deleteGame(deque<GameRecord>& g) : - This function enables players to delete records.
*/
void deleteGame(deque<GameRecord>& g)
{
	int deleteColor = 4;   //Color used for the screen's banner and the currently selected record.
	int recordColor = 1;   //Color used for the rest of the records.

	//Checks if there are no saved games.
	if (!g.empty())
	{
		deque<GameRecord> newRecords;                   //Used to temporarily store all the saved games except the deleted game.
		int deleteIndex;                                //Index of the delete button.
		bool deleteMenuStatus = true;                   //Used to run the delete menu loop
		int recordsPerFrame = (g.size()<3)? g.size():3; //Maximum number of games displayed at a time (3 if there are 3 or more records. Else, it is the number of records if there are less than 3 records.
		int recordHeight = 3;                           //Height of each record.
		int cancelBtnIndex = g.size();                  //Index of the button that redirects user to load game screen.
		int totalChoices = g.size() + 1;                //Total number of buttons in the delete menu.

		//Initialization of the buttons.
		deque<Button> selectButtons;
		for (int r = 0; r < g.size(); r++)
		{
			selectButtons.push_back(Button("", 2, recordHeight));
		}
		selectButtons.push_back(Button("Cancel"));

		int choice = 0;                                  //Currently selected button.
		int topRecord = 0;                               //First of the three records currently shown.
		int bottomRecord = topRecord + recordsPerFrame;  //Last of the three records currently shown.

		bool blinkToggle = false;
		int selectionStatus = true;

		//Calculation of gaps.
		int topGap = 1;
		int bottomGap = 2;
		int gapAfterRecords = GRID_HEIGHT - topGap - ((recordHeight + 1) * recordsPerFrame) - selectButtons[cancelBtnIndex].getHeight() - bottomGap;
		int btnLeftPadding = (VISUAL_WIDTH - selectButtons[cancelBtnIndex].getWidth()) / 2;
		int btnRightPadding = VISUAL_WIDTH - btnLeftPadding - selectButtons[cancelBtnIndex].getWidth();

		//Delete menu loop
		while (deleteMenuStatus)
		{
			refreshScreen();

			if (_kbhit())
			{
				int key = _getch();

				//Escape key.
				if (key == 27)
				{
					deleteMenuStatus = false;
					continue;
				}

				//Enter key
				if (key == 13)
				{
					//If player selects a game to delete.
					if (choice < g.size())
					{
						//Re-confirmation from the player.
						bool deleteConfirmation = yesNoScreen("Are you sure you want to delete this game?");
						if (deleteConfirmation)
						{
							//Game deletion and re-save process.
							deleteMenuStatus = false;
							for (int r = 0; r < g.size(); r++)
							{
								if (r != choice)
								{
									newRecords.push_back(g[r]);
								}
							}
							g = newRecords;
							saveGame(g);

							//Message that says the game has been deleted successfully.
							refreshScreen();
							displayBanner("DELETE GAME", VISUAL_WIDTH, 1, deleteColor, 15);
							int topPad = (GRID_HEIGHT - 3) / 2;
							int bottomPad = GRID_HEIGHT - topPad - 3;
							for (int i = 0; i < topPad; i++) displayLine(BORDER_COLOR);
							displayBanner("Game Deleted Successfully", VISUAL_WIDTH, 1, 12, 15);
							for (int i = 0; i < bottomPad; i++) displayLine(BORDER_COLOR);
							this_thread::sleep_for(chrono::milliseconds(3000));
						}
					}

					//Cancel button
					else if (choice == cancelBtnIndex)
					{
						deleteMenuStatus = false;
					}
					continue;
				}

				//Arrow keys
				else if (key == 0 || key == 224) {
					int arrow = _getch();
					changeChoice(arrow, choice, totalChoices);
				}
			}

			//Enables scrolling through records.
			if (choice < g.size())
			{
				if (choice >= topRecord + recordsPerFrame)
				{
					topRecord = choice - (recordsPerFrame - 1);
				}
				if (choice < topRecord)
				{
					topRecord = choice;
				}
			}

			//Blinking effect.
			blinkToggle = !blinkToggle;
			for (int b = 0; b < selectButtons.size(); b++) selectButtons[b].setSelectionStatus(false);
			selectButtons[choice].setSelectionStatus(true);
			selectButtons[choice].setBlinking(blinkToggle);

			//Display starts from here.
			displayBanner("DELETE GAME", VISUAL_WIDTH, 1, deleteColor, 15);
			for (int g = 0; g < topGap; g++) displayLine(BORDER_COLOR);

			//Prepares each record and displays them.
			for (int i = 0; i < recordsPerFrame; i++)
			{
				int r = topRecord + i; //slot index

				if (r >= g.size()) break;

				string recordStr[3];
				recordStr[0] = "Name: " + g[r].getName();
				recordStr[1] = "Difficulty: " + g[r].getDifficultyStr();
				recordStr[2] = "Score: " + to_string(g[r].getScore());

				for (int k = 0; k < 3; k++)
				{
					int recordRightPadding = (GRID_WIDTH * 2) - (selectButtons[r].getWidth() + 1 + (int)recordStr[k].length());
					if (recordRightPadding > 0) recordStr[k] += string(recordRightPadding, ' ');
				}

				for (int line = 0; line < recordHeight; line++)
				{
					setPixelColor(BORDER_COLOR, 0);
					cout << " ";
					selectButtons[r].printButton(line, 0, 0, 14);
					setPixelColor(BORDER_COLOR, 0);
					cout << " ";

					setPixelColor(((choice == r) ? deleteColor : recordColor), 15);
					cout << recordStr[line];

					setPixelColor(BORDER_COLOR, 0);
					cout << " " << endl;
					setPixelColor(0, 7);
				}
				displayLine(BORDER_COLOR);
			}

			//Displays the cancel button, delete button and the rest of the gaps.
			for (int g = 0; g < gapAfterRecords; g++) displayLine(BORDER_COLOR);
			for (int line = 0; line < selectButtons[cancelBtnIndex].getHeight(); line++)
			{
				selectButtons[cancelBtnIndex].printButton(line, btnLeftPadding, btnRightPadding, BORDER_COLOR);
				cout << endl;
			}
			for (int g = 0; g < bottomGap; g++) displayLine(BORDER_COLOR);
			this_thread::sleep_for(chrono::milliseconds(SCREEN_REFRESH_RATE));
		}
	}

	//Displays a no records message if there are no saved games.
	else
	{
		refreshScreen();
		displayBanner("DELETE GAME", VISUAL_WIDTH, 1, deleteColor, 15);
		pair<int, int> topBottomPad = paddingCalculation(GRID_HEIGHT, 3);
		for (int i = 0; i < topBottomPad.first; i++) displayLine(BORDER_COLOR);
		displayBanner("There Are No Saved Games. Keep Playing!", VISUAL_WIDTH, 1, deleteColor, 15);
		for (int i = 0; i < topBottomPad.second; i++) displayLine(BORDER_COLOR);
		this_thread::sleep_for(chrono::milliseconds(3000));
	}
}


/*
* displayHighScores(): - This function displays the high scores screen.
*/
void displayHighScores()
{
	//Return Button initialization and padding (left/right) calculation.
	Button returnMenu = Button("Return to Menu");
	returnMenu.setSelectionStatus(true);
	bool blinkToggle = true;
	bool highScoreMenuRunning = true;
	pair<int, int> leftRightPadding = paddingCalculation(VISUAL_WIDTH, returnMenu.getWidth());

	deque<string> highScores = retrieveFileRecords("highScores.txt"); //Reads the file that contains the high scores.
	string difficultyLabel[3] = { "Easy","Medium","Hard" };           //Array that contains the three difficulty levels.
	bool recordExists[3] = { false,false,false };                     //Array that stores the record existence of the three difficulty levels.
	string nameLabel[3];                                              //Stores the player names.
	string scoreLabel[3];                                             //Stores the respective high scores.
	deque<string> names,scores;                                       //Used to temporarily store the names and scores to properly assign into the previous arrays.

	string noRecordMsg = "No High Score Yet. Keep Playing!";                                             //Message that displays if there is no record for a difficulty level.
	for (int p = 0; p < ((GRID_WIDTH * 2) - noRecordMsg.length()); p++) noRecordMsg = " " + noRecordMsg; //Used to center the no record message.

	//Calculation of the gaps.
	int headerHeight = 3;
	int difficultyHeight = 0;
	int contentGap = 1;
	int linesPerRecord = (difficultyHeight * 2 + 1) + 2 + contentGap;
	int contentHeight = linesPerRecord * 3 + 4;
	if (highScores.size() == 0) contentHeight += 1;
	int topGap = 1;
	int bottomGap = GRID_HEIGHT - (topGap + contentHeight);
	if (bottomGap < 0) bottomGap = 0;

	//Appends the names and scores into their respective deques.
	for (int r = 0; r < highScores.size(); r++)
	{
		string difficultyStr, nameStr, scoreStr;
		stringstream details(highScores[r]);
		getline(details, difficultyStr, ',');
		difficultyStr[0] = toupper(difficultyStr[0]);

		if (difficultyStr == "Easy") recordExists[0] = true;
		else if (difficultyStr == "Medium") recordExists[1] = true;
		else if (difficultyStr == "Hard") recordExists[2] = true;

		getline(details, nameStr, ',');
		names.push_back("Name: " + nameStr);
		
		getline(details, scoreStr);
		scores.push_back("Score: " + scoreStr);
	}
	
	//Creates formatted record for each difficulty to display.
	for (int d = 0; d < 3; d++)
	{
		//If the current difficulty has a record, creates a formatted record with name and the score
		if (recordExists[d])
		{
			nameLabel[d] = names.front();
			names.pop_front();
			scoreLabel[d] = scores.front();
			scores.pop_front();
		}

		//Else, creates a record that informs user that no record exists for a difficulty.
		else
		{
			nameLabel[d] = noRecordMsg;
			scoreLabel[d] = "";
		}
	}

	while (highScoreMenuRunning)
	{
		refreshScreen();
		if (_kbhit())
		{
			int key = _getch();
			//Enter key or the Escape key.
			if (key == 13 || key == 27)
			{
				highScoreMenuRunning = false;
				continue;
			}
		}

		//Display starts from here.
		displayBanner("HIGH SCORES", VISUAL_WIDTH, (headerHeight - 1) / 2, 9, TEXT_COLOR);
		for (int g = 0; g < topGap; g++) displayLine(BORDER_COLOR);

		//Displays records for each difficulty.
		for (int i = 0; i < 3; i++)
		{
			int namePadding = (GRID_WIDTH * 2) - nameLabel[i].length();
			int scorePadding = (GRID_WIDTH * 2) - scoreLabel[i].length();

			displayBanner(difficultyLabel[i], VISUAL_WIDTH, difficultyHeight, 9, TEXT_COLOR);
			setPixelColor(1, TEXT_COLOR);
			cout << " " << nameLabel[i];
			for (int p = 0; p < namePadding; p++) cout << " ";
			cout << " \n ";
			cout << scoreLabel[i];
			for (int p = 0; p < scorePadding; p++) cout << " ";
			cout << " " << endl;
			setPixelColor(0, 7);

			for (int i = 0; i < contentGap; i++) displayLine(BORDER_COLOR);
		}

		//Displays the return to menu button and the rest of the gaps.
		blinkToggle = !blinkToggle;
		returnMenu.setBlinking(blinkToggle);
		displayLine(BORDER_COLOR);

		for (int b = 0; b < returnMenu.getHeight(); b++)
		{
			returnMenu.printButton(b, leftRightPadding.first, leftRightPadding.second, BORDER_COLOR);
			cout << endl;
			setPixelColor(0, 7);
		}

		for (int i = 0; i < bottomGap; i++) displayLine(BORDER_COLOR);
		this_thread::sleep_for(chrono::milliseconds(SCREEN_REFRESH_RATE));

	}

}


/*
* quitScreen(bool& status, deque<GameRecord>& g): - This function enables players to either continue a game or directs them to the delete screen if needed.
*/
void quitScreen(bool& status, deque<GameRecord>& g)
{
	status = !(yesNoScreen("Are you sure you want to quit the game ?"));
	if (!status) saveGame(g);
}


/*
* stringToDifficulty(string difStr): - Parses the difficulty from a string record of a game.
*									 - Used in the retrieveGames function.
*/
GameRecord::Difficulty stringToDifficulty(string difStr)
{
	if (difStr == "easy") return GameRecord::EASY;
	if (difStr == "medium") return GameRecord::MEDIUM;
	if (difStr == "hard") return GameRecord::HARD;

	return GameRecord::EASY;

}


/*
* stringToDirection(string dirStr): - Parses the direction from a string record of a game.
*									- Used in the retrieveGames function.
*/
GameRecord::Direction stringToDirection(string dirStr)
{
	if (dirStr == "up") return GameRecord::UP;
	else if (dirStr == "down") return GameRecord::DOWN;
	else if (dirStr == "left") return GameRecord::LEFT;
	else if (dirStr == "right") return GameRecord::RIGHT;
	else return GameRecord::LEFT;
}


/*
* stringToSnake(string snkStr): - Parses the snake's coordinates from a string record of a game.
*							    - Used in the retrieveGames function.
*/
deque<pair<int, int>> stringToSnake(string snkStr)
{
	stringstream snk(snkStr);
	string pairStr;
	deque<string> parsedPairs;
	deque<pair<int, int>> convertedSnake;
	for (int i = 0; i < count(snkStr.begin(), snkStr.end(), ':'); i++)
	{
		getline(snk, pairStr, ':');
		parsedPairs.push_back(pairStr);
	}
	getline(snk, pairStr);
	parsedPairs.push_back(pairStr);
	for (int l = 0; l < parsedPairs.size(); l++)
	{
		stringstream pairStream(parsedPairs[l]);
		string first;
		string second;
		getline(pairStream, first, '-');
		getline(pairStream, second);
		convertedSnake.push_back({ stoi(first), stoi(second) });
	}
	return convertedSnake;
}


/*
* stringToFruit(string fStr): - Parses the coordinates of the fruit from a string record of a game.
*							  - Used in the retrieveGames function.
*/
pair<int, int> stringToFruit(string fStr)
{
	stringstream fruitStream(fStr);
	string first;
	string second;
	getline(fruitStream, first, '-');
	getline(fruitStream, second);
	return { stoi(first),stoi(second) };
}