/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : MainGame.cpp
* Description : - This provides the definitions for the main gameplay, which contains the code for all the game logic.
*			    - Contains logic for playing the game, pausing the game, and other gameplay-related functions
*/

#include "MainGame.h"

using namespace std;

/*
* gamePlay(GameRecord& game) : - This function is the main gameplay.
*                              - Returns the game over status and the latest version of the grid to display the game over screen if needed.
*/
pair<bool, deque<deque<Pixel>>> gamePlay(GameRecord& game)
{
	bool gameRunning = true;       //Used for the game loop.
	bool windowSizeError = false;  //Used to retrieve the window status (If the window width height requirements are met).
	bool gamePause = false;        //Signals the program if the user chooses to pause the game.
	bool gameOverStatus = false;   //Signals the program if the currently running game is over or not.

	//Initializes the game grid.
	deque<deque<Pixel>> grid;

	//Initializes each row and appedns to the grid.
	for (int r = 0; r < GRID_HEIGHT; r++)
	{
		deque <Pixel> pixelRow;
		for (int c = 0; c < GRID_WIDTH; c++)
		{
			pixelRow.push_back(Pixel());
		}
		grid.push_back(pixelRow);
	}

	//Main game loop.
	while (gameRunning)
	{
		//Stores the coordinates of the snake's head in the previous frame.
		pair<int, int> prevCycleHead = game.getSnakeCoordinates().front();

		//Checks if the terminal window width and height requirements have been met.
		windowSizeError = refreshScreen();

		//Signals the program to pause the game if the width-height requirements have not been met. This prevents the game resume right after resizing the window, so the player has the time to get back to the game.
		if (windowSizeError == true)
		{
			gamePause = true;
			windowSizeError = false;
			continue;
		}

		//Directs the player to the pause screen if needed. Stops the game loop if the player chooses to stop the game.
		if (gamePause)
		{
			gameRunning = pause(game.getScore());
			gamePause = false;
			continue;
		}

		//Checks for player's keystrokes.
		if (_kbhit())
		{
			int key = _getch();

			//Checks for special keys (Arrow keys in this instance).
			if (key == 0 || key == 224)
			{
				key = _getch();
			}

			//Spacebar or the Escape key.
			else if (key == ' ' || key == 27)
			{
				gamePause = true;
				continue;
			}

			//Temporarily stores the direction.
			GameRecord::Direction newDir = game.getDirection();
			changeDirection(key, newDir);

			//Checks if the new direction is not the opposite of the current direction to prevent accidental collisions.
			bool directionUpLogic = game.getDirection() == GameRecord::UP && newDir != GameRecord::DOWN;       //Keypress is up but the current direction is not down.
			bool directionDownLogic = game.getDirection() == GameRecord::DOWN && newDir != GameRecord::UP;	   //Keypress is down but the current direction is not up.
			bool directionLeftLogic = game.getDirection() == GameRecord::LEFT && newDir != GameRecord::RIGHT;  //Keypress is left but the current direction is not right.
			bool directionRightLogic = game.getDirection() == GameRecord::RIGHT && newDir != GameRecord::LEFT; //Keypress is right but the current direction is not left.

			//Changes the snake's direction if all the conditions are met.
			if (directionUpLogic || directionDownLogic || directionLeftLogic || directionRightLogic) game.setDirection(newDir);
		}

		//Moves the snake (Changes the coordinates of the snake's parts according to the current direction).
		game.moveSnake();

		//Checks if the snake has either hit a boundary or bit itself.
		if (!boundaryCheck(game.getSnakeCoordinates()))
		{
			gameOverStatus = true;
			gameRunning = false;
			continue;
		}

		//Checks if the snake has eaten a fruit.
		if (game.getFruitLastLocation().first == game.getSnakeCoordinates().front().first && game.getFruitLastLocation().second == game.getSnakeCoordinates().front().second)
		{
			game.growSnake({ game.getSnakeCoordinates().back().first,game.getSnakeCoordinates().back().second });  //Grow the snake.
			game.setScore(game.getScore() + 10);                                                                   //Increase the score.
			game.placeFruit();																					   //Changes the location of the fruit.
		}

		//Changes the nature of the pixel (what each pixel currently displays) according to the coordinates of the fruit and the snake's body parts.
		setGrid(grid, game.getSnakeCoordinates(), game.getFruitLastLocation());

		//Prints the grid with the score banner.
		printGrid(grid, game.getScore());

		//Delays the screen refresh according to the difficulty level.
		this_thread::sleep_for(chrono::milliseconds(game.getDifficulty()));
	}

	//If the game is not finished (snake has neither hit a boundary nor bit itself), asks the player if they wish to save the game.
	if (gameOverStatus == false) gameOverStatus = !(yesNoScreen("Do you want to save this game?"));

	//Returns the status of the game (true if the game is over, otherwise false) and the latest version of the game grid (To be used in the game over screen if needed).
	return { gameOverStatus,grid };
}


/*
* changeDirection(int key, GameRecord::Direction& dir) : - This function changes the direction of the snake according to the player's keystrokes.
*														 - Both arrow keys and 'WASD' keys can be used to control the snake.
*/
void changeDirection(int key, GameRecord::Direction& dir)
{
	if (key == 'w' || key == 'W' || key == 72) dir = GameRecord::UP;
	else if (key == 's' || key == 'S' || key == 80) dir = GameRecord::DOWN;
	else if (key == 'a' || key == 'A' || key == 75) dir = GameRecord::LEFT;
	else if (key == 'd' || key == 'D' || key == 77) dir = GameRecord::RIGHT;
}


/*
* displayScore(int score) : - This function displays the score banner.
*                           - Used in the main game loop, pause menu and the game over screen.
*/
void displayScore(int score)
{
	string scoreLabel = "Score: " + to_string(score);
	displayBanner(scoreLabel, (GRID_WIDTH * 2) + 2, 1, BORDER_COLOR, 15);
}


/*
* boundaryCheck(std::deque<std::pair<int, int>> snakeCoords) : - This function checks if the snake has either hit a boundary or has b
*                          
*/
bool boundaryCheck(std::deque<std::pair<int, int>> snakeCoords)
{
	//Snake's head.
	pair<int, int> head = snakeCoords.front();

	//Rest of the body parts.
	deque<pair<int, int>> tail;
	for (int b = 1; b < snakeCoords.size(); b++) tail.push_back(snakeCoords[b]);

	//Checks if the coordiantes of the snake's head are out of the grid's coordinates or matches with the coordinates of any of the snake's body part.
	if (head.first == -1 || head.first == GRID_HEIGHT || head.second == -1 || head.second == GRID_WIDTH || (checkMatch(head.first, head.second, tail).first)) return false;
	
	//Else, returns true.
	return true;
}


/*
* setGrid(deque<deque<Pixel>>& gameGrid, deque<pair<int, int>> snakeCoords, pair<int, int> fruitCoords) : - This function sets the nature of the pixels (if a pixel represents a part of the snake, the fruit of a blank space of the grid) in the grid.
*                                                                                                         - Used before printing the grid.
*/
void setGrid(deque<deque<Pixel>>& gameGrid, deque<pair<int, int>> snakeCoords, pair<int, int> fruitCoords)
{
	//Goes through each pixel (each element) of the grid and sets their nature based on the coordinates of the snake and the fruit.
	for (int r = 0; r < gameGrid.size(); r++)
	{
		for (int c = 0; c < gameGrid[r].size(); c++)
		{
			if (r == snakeCoords[0].first && c == snakeCoords[0].second)
			{
				gameGrid[r][c].setNature("snakeHead");
				continue;
			}
			if (checkMatch(r, c, snakeCoords).first)
			{
				gameGrid[r][c].setNature("snakeBody");
			}
			else if (r == fruitCoords.first && c == fruitCoords.second)
			{
				gameGrid[r][c].setNature("fruit");
			}
			else
			{
				gameGrid[r][c].setNature("blank");
			}
		}
	}
}


/*
* printGrid(deque<deque<Pixel>> grid, int score) : - This function prints the grid according to the nature of the pixel (snake's head or body, fruit or a blank space).
*
*/
void printGrid(deque<deque<Pixel>> grid, int score)
{
	//Displays the score banner.
	displayScore(score);

	//Goes through each element and sets the color and displays it according to the nature of the pixel.
	for (int r = 0; r < grid.size(); r++)
	{
		setPixelColor(5, 5);
		cout << " ";
		for (int c = 0; c < grid[0].size(); c++)
		{
			//Changes the color of each pixel according to their nature.
			int pixelColor = grid[r][c].pixelColor();
			setPixelColor(pixelColor, pixelColor);
			cout << "  ";
		}
		setPixelColor(5, 5);
		cout << " " << endl;

	}
	for (int b = 0; b < (grid[0].size() + 1) * 2; b++) cout << " ";

	//Resets the color back to the terminal's original colors.
	setPixelColor(0, 7);

}


/*
* pause(int score) : - This function handles the logic for the pause menu.
*/
bool pause(int score)
{
	Button pauseButtons[2] = { Button("Continue"),Button("Exit") };  //Initializes the pause menu buttons.
	bool pauseMenuRunning = true;									 //Used for the pause menu loop.
	int choice = 0;													 //Currently selected button.
	bool blinkToggle = false;									     //Used for the blinking effect of the buttons.

	//Pause menu loop.
	while (pauseMenuRunning)
	{
		refreshScreen();
		if (_kbhit())
		{
			int key = _getch();

			//Escape key.
			if (key == 27)
			{
				pauseMenuRunning = false;
				return true;
			}

			//Enter key.
			else if (key == 13)
			{
				if (choice == 0) return true;
				else
				{
					bool confirmChoice = yesNoScreen("Are you sure you want to exit this game?");
					if (confirmChoice) return false;
				}
			}

			//Arrow keys.
			else if (key == 0 || key == 224) {
				int arrow = _getch();
				changeChoice(arrow, choice, 2);
			}
		}

		//Blinking effect.
		blinkToggle = !blinkToggle;

		for (int b = 0; b < 2; b++)
		{
			if (b == choice)
			{
				pauseButtons[b].setSelectionStatus(true);
				pauseButtons[b].setBlinking(blinkToggle);
			}
			else
			{
				pauseButtons[b].setSelectionStatus(false);
			}
		}

		//Displays the pause menu.
		displayPauseMenu(pauseButtons, score);
		this_thread::sleep_for(chrono::milliseconds(SCREEN_REFRESH_RATE));
	}

	//Default return statement to satisfy the compiler's checking process. This statement is never accessed under normal conditions.
	return true;
}


/*
* displayPauseMenu(Button pauseButtons[2], int score) : - This function displays the pause menu.
*/
void displayPauseMenu(Button pauseButtons[2], int score)
{
	//Used for the padding calculations.
	int realGridWidth = GRID_WIDTH * 2;

	string pauseMessage[5] = { "                   " ,
							   "                   " ,
							   "    Game Paused    " ,
							   "                   ",
							   "                   " };

	//Gap calculations.
	int labelButtonGap = 1;
	int contentHeight = size(pauseMessage) + pauseButtons[0].getHeight() + labelButtonGap;
	pair<int, int> topBottomGap = paddingCalculation(GRID_HEIGHT, contentHeight);
	int buttonGap = 2;
	int totalButtonWidth = (pauseButtons[0].getWidth() * 2) + buttonGap;
	pair<int, int> leftRightPadding = paddingCalculation(realGridWidth, totalButtonWidth);
	pair<int, int> msgLeftRight = paddingCalculation(realGridWidth, pauseMessage[0].length());


	//Display starts from here.
	displayScore(score);

	setPixelColor(0, 7);
	//print the top gap
	for (int h = 0; h < topBottomGap.first; h++) displayLine(14);

	//print the content
	for (int g = 0; g < 5; g++)
	{

		setPixelColor(BORDER_COLOR, 0);
		cout << " ";
		setPixelColor(14, 15);
		for (int s = 0; s < msgLeftRight.first; s++) cout << " ";
		setPixelColor(9, 15);
		cout << pauseMessage[g];
		setPixelColor(14, 15);
		for (int s = 0; s < msgLeftRight.second; s++) cout << " ";
		setPixelColor(BORDER_COLOR, 0);
		cout << " " << endl;
	}

	displayLine(14);

	//Pause buttons.
	for (int line = 0; line < 3; line++)
	{
		setPixelColor(BORDER_COLOR, 0);
		cout << " ";
		pauseButtons[0].printButton(line, leftRightPadding.first, 0, 14);
		pauseButtons[1].printButton(line, buttonGap, leftRightPadding.second, 14);
		setPixelColor(BORDER_COLOR, 0);
		cout << " " << endl;
	}
	for (int h = 0; h < topBottomGap.second; h++) displayLine(14);
	displayLine(BORDER_COLOR);
	setPixelColor(0, 7);
}


/*
* gameOver(GameRecord finishedGame, deque<deque<Pixel>> grid) : - This function displays the game over screen and updates the highscore records if necessary.
*/
void gameOverScreen(GameRecord finishedGame, deque<deque<Pixel>> grid)
{
	//Signals the program if the currently finished game has a new high score.
	bool newHighScore = resetHighScores(finishedGame);

	int topGap = 7;
	int gameOverMsgHeight = 3;
	int bottomGap = topGap + gameOverMsgHeight;
	if (newHighScore)
	{
		bottomGap += 3;
	}
	int messageWidth = (GRID_WIDTH * 2) + 2;

	//Display starts from here.
	refreshScreen();
	displayScore(finishedGame.getScore());
	for (int r = 0; r < topGap; r++)
	{
		setPixelColor(BORDER_COLOR, 0);
		cout << " ";
		for (int c = 0; c < GRID_WIDTH; c++)
		{
			setPixelColor(grid[r][c].pixelColor(), 0);
			cout << "  ";
		}
		setPixelColor(BORDER_COLOR, 0);
		cout << " ";
		setPixelColor(0, 7);
		cout << endl;
	}

	displayBanner("GAME OVER", messageWidth, 1, BORDER_COLOR, 15);

	//This part displays only if there is a new high score.
	if (newHighScore) displayBanner("NEW HIGHSCORE!", messageWidth, 1, BORDER_COLOR, 15);

	for (int r = bottomGap; r < GRID_HEIGHT; r++)
	{
		setPixelColor(BORDER_COLOR, 0);
		cout << " ";
		for (int c = 0; c < GRID_WIDTH; c++)
		{
			setPixelColor(grid[r][c].pixelColor(), 0);
			cout << "  ";
		}
		setPixelColor(BORDER_COLOR, 0);
		cout << " ";
		setPixelColor(0, 7);
		cout << endl;
	}
	displayLine(BORDER_COLOR);
	this_thread::sleep_for(chrono::milliseconds(3000));
}


/*
* yesNoScreen(string question) : - This function displays screens with Yes/No questions.
*/
bool yesNoScreen(string question)
{
	bool running = true;
	int answerChoice = 0;
	bool blinkToggle = false;
	const int answerButtons = 2;
	Button answerOptions[answerButtons] = { Button("Yes"), Button("No") };
	pair<int, int> buttonLeftRightPadding = paddingCalculation(GRID_WIDTH * 2, answerOptions[0].getWidth());

	int buttonGap = 2;
	int bannerHeight = 3;
	int contentHeight = (answerOptions[0].getHeight() + buttonGap) * answerButtons;
	pair<int, int> topBottomGap = paddingCalculation(GRID_HEIGHT, contentHeight);

	while (running)
	{
		refreshScreen();

		if (_kbhit())
		{
			int key = _getch();
			if (key == 13)
			{
				if (answerChoice == 0)
				{
					running = false;
					return true;
				}
				else
				{
					running = false;
					return false;
				}
			}
			else if (key == 0 || key == 224) {
				int arrow = _getch();
				changeChoice(arrow, answerChoice, answerButtons);
			}
		}


		blinkToggle = !blinkToggle;
		for (int i = 0; i < answerButtons; i++)
		{
			if (i == answerChoice)
			{
				answerOptions[i].setSelectionStatus(true);
				answerOptions[i].setBlinking(blinkToggle);
			}
			else
			{
				answerOptions[i].setSelectionStatus(false);
			}
		}

		//Display starts from here.
		displayBanner(question, VISUAL_WIDTH, 1, BANNER_COLOR, TEXT_COLOR);
		for (int g = 0; g < topBottomGap.first; g++) displayLine(BORDER_COLOR);

		for (int b = 0; b < answerButtons; b++)
		{
			for (int line = 0; line < answerOptions[0].getHeight(); line++)
			{
				setPixelColor(BORDER_COLOR, 0);
				cout << " ";
				answerOptions[b].printButton(line, buttonLeftRightPadding.first, buttonLeftRightPadding.second, BORDER_COLOR);
				setPixelColor(BORDER_COLOR, 0);
				cout << " " << endl;
			}
			for (int p = 0; p < buttonGap; p++) displayLine(BORDER_COLOR);

		}
		for (int g = 0; g < topBottomGap.second; g++) displayLine(BORDER_COLOR);
		setPixelColor(0, 7);
		this_thread::sleep_for(chrono::milliseconds(SCREEN_REFRESH_RATE));
	}
	return false;
}




/*
* resetHighScores(GameRecord finishedGame) : - Reads the high scores file and changes if the finished game has a new highscore.
*/
bool resetHighScores(GameRecord finishedGame)
{
	bool newHighScore = false;  //Signals the program if the score of the finished game is a new highscore.
	bool recordExists = false;  //Signals the program if a high score record for the game's difficulty is already there.

	//If the score is 0, it is not counted as a high score and disregards the rest of the code.
	if (finishedGame.getScore() == 0) return newHighScore;
	
	//Reads the TXT file that contains the high score records.
	deque<string> highScores = retrieveFileRecords("highScores.txt");

	//Goes through each of the saved records and checks if a record of similar difficulty level is found.
	for (int h = 0; h < highScores.size(); h++)
	{
		string difficulty;
		string name;
		string scoreStr;
		stringstream scoreStream(highScores[h]);
		getline(scoreStream, difficulty, ',');
		difficulty[0] = toupper(difficulty[0]);  //Capitalizes the first letter of the difficulty string to compare with the game's difficulty.
		getline(scoreStream, name, ',');
		getline(scoreStream, scoreStr);

		//Checks if the difficulty level matches the game's difficulty.
		if (difficulty == finishedGame.getDifficultyStr())
		{
			recordExists = true;
			int score = stoi(scoreStr);
			if (finishedGame.getScore() > score)
			{
				newHighScore = true;
				difficulty[0] = tolower(difficulty[0]);
				highScores[h] = difficulty + "," + finishedGame.getName() + "," + to_string(finishedGame.getScore());
			}
			break;
		}
	}

	//Only runs if a saved record of similar difficulty level is not there.
	if (!recordExists)
	{
		string difficulty = finishedGame.getDifficultyStr();
		difficulty[0] = tolower(difficulty[0]); //convert the first letter of the difficulty to lowercase to write into the file.
		newHighScore = true;
		deque<string> newHighScores; //Used to temporarily store the sorted high score records.

		//This variable decides the position of the game's record in the high score records.
		int difficultyIndex;
		if (finishedGame.getDifficultyStr() == "Easy") difficultyIndex = 0;
		else if (finishedGame.getDifficultyStr() == "Medium") difficultyIndex = 1;
		else difficultyIndex = 2;

		string newScoreStr = difficulty + "," + finishedGame.getName() + "," + to_string(finishedGame.getScore());

		//Runs the loop three times (for the three difficulties).
		for (int r = 0; r < 3; r++)
		{
			if (r == difficultyIndex)
			{
				newHighScores.push_back(newScoreStr);
			}
			else if (!highScores.empty())
			{
				newHighScores.push_back(highScores.front());
				highScores.pop_front();
			}
		}
		highScores = newHighScores;
	}

	//Saves the sorted new records into the TXT file.
	saveFileRecords("highScores.txt", highScores);

	//Returns if the game's score is a high score or not (to the game over screen to decide whether to display the 'NEW HIGHSCORE' part.
	return newHighScore;
}