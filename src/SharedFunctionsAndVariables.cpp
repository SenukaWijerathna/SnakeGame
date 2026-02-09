/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : SharedFunctionsAndVariables.cpp
* Description : - This provides the definitions for the shared functions, which contains the code for functions that is used across the game.
*			    - Contains the screen refreshing logic, logic for changing the color, and the logic for checking the window size.
*/

#include "SharedFunctionsAndVariables.h"

using namespace std;

/**** FUNCTIONS USED THROUGHOUT THE ENTIRE GAME ****/

/*
* hideTextCursor() : - This function is used to hide the text cursor in the terminal window.
*                    - Used to prevent it from appearing when printing the game screen.
*/
void hideTextCursor()
{
	//Enables modification to the text cursor.
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;

	//This makes the cursor invisible.
	info.dwSize = 100;
	info.bVisible = false;

	SetConsoleCursorInfo(out, &info);
}


/*
* getConsoleSize() : - This function returns the current width and height of the console window.
*/
COORD getConsoleSize()
{
	//Enables modification to the text cursor.
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		//calculate the visible window width(right-left+1)
		int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

		//calculate the visible window height(bottom-top+1)
		int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		return { (short)width, (short)height };

	}

	return { 5,5 };
}


/*
* windowSizeCheck(int width, int height) : - This function temporarily pauses the gameplay if the minimum 'width' and 'height' requirements of the console window are not met.
*                                          - Used throughout the entire gameplay to prevent glitches in the interface.
*/
bool windowSizeCheck(int width, int height)
{
	//Retrieves the size of the console window.
	COORD currentSize = getConsoleSize();

	//Condition that checks if the width and height requirements have been met.
	if (currentSize.X < width || currentSize.Y < height)
	{
		system("cls");
		setPixelColor(0, 4);     //Changes the color of the warning message to red.

		//This loop continues until the player resizes the window to meet the width and height requirements.
		while (currentSize.X < width || currentSize.Y < height)
		{
			//Prints a warning message and blocks the game.
			hideTextCursor();
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
			cout << "    WARNING! WINDOW IS TOO SMALL!     " << endl;
			cout << "Resize the window to a width of " << width << " units and a height of " << height << " units." << endl;

			//delays the refreshing speed between the frames.
			this_thread::sleep_for(chrono::milliseconds(100));
			currentSize = getConsoleSize();
		}
		setPixelColor(0, 7);
		system("cls");
		return true;
	}
	return false;
}


/*
* refreshConsole() : - This resets the text cursor position and checks the window size.
*                    - Used at the beginning of every screen.
*/
bool refreshScreen()
{
	hideTextCursor();
	bool windowError = windowSizeCheck(VISUAL_WIDTH, VISUAL_HEIGHT);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
	return windowError;
}


/*
* setPixelColor(int backgroundColor, int textColor) : - This function changes the color of the console.
*                                                     - 'backgroundColor' decides the background color of the character, while 'textColor' changes the color of the character itself.
*/
void setPixelColor(int backgroundColor, int textColor)
{
	//Enables modification to the text cursor and terminal background.
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//color of the console window is decided based on two factors: background and the text color, using a single hexadecimal number, which is calculated as shown below.
	int colorAttribute = (backgroundColor * 16) + textColor;

	//Sets the color.
	SetConsoleTextAttribute(hConsole, colorAttribute);
}


/*
* displayBanner(string text, int bannerWidth, int topBottomGap, int backgroundColor, int textColor) : - This function displays banners in the game.
*                                                                                                     - Used to display title cards in different screens.
*/
void displayBanner(string text, int bannerWidth, int topBottomGap, int backgroundColor, int textColor)
{
	//Calculation of the padding to the left and right of the text.
	pair<int, int> gapBeforeAfterText = paddingCalculation(bannerWidth, text.length());

	//Top padding.
	setPixelColor(backgroundColor, textColor);
	for (int p = 0; p < topBottomGap; p++)
	{
		for (int t = 0; t < bannerWidth; t++) cout << " ";
		cout << endl;
	}

	//Line that contains the text
	for (int g = 0; g < gapBeforeAfterText.first; g++) cout << " ";
	cout << text;
	for (int g = 0; g < gapBeforeAfterText.second; g++) cout << " ";
	cout << endl;

	//Bottom padding.
	for (int p = 0; p < topBottomGap; p++)
	{
		for (int t = 0; t < bannerWidth; t++) cout << " ";
		cout << endl;
	}
	setPixelColor(0, 7);
}


/*
* displayLine(int backgroundColor) : - This function displays a single line the the game interface that doesn't belong to any button or a title card.
*                                    - Used throughout the entire game.                                   
*/
void displayLine(int backgroundColor)
{
	//Left border.
	setPixelColor(BORDER_COLOR, 0);
	cout << " ";

	//Middle area.
	setPixelColor(backgroundColor, 15);
	for (int w = 0; w < (GRID_WIDTH * 2); w++) cout << " ";

	//Right border.
	setPixelColor(BORDER_COLOR, 0);
	cout << " " << endl;
	setPixelColor(0, 7);
}


/*
* paddingCalculation(int availableLength, int contentLength) : - This method is used to calculate padding (above and below/left and right) around a button, a group of buttons or other content.
*															   - Used throughout the entire game.
*/
std::pair<int, int> paddingCalculation(int availableLength, int contentLength)
{
	//Get the half of the length(width/height) available after deducting the length of the content. This is the length of the padding before the content.
	int gapBeforeContents = (availableLength - contentLength) / 2;

	//Get the remaining length(width/height) available after deducting the length of the content and the top/left padding. This is the length of the padding after the content.
	int gapAfterContents = availableLength - (gapBeforeContents + contentLength);

	//Makes sure that the padding after content is not a negative number to avoid glitches.
	if (gapAfterContents < 0) gapAfterContents = 0;

	//Return the values.
	return { gapBeforeContents,gapAfterContents };
}


/*
* changeChoice(int key, int& choice, int noOfChoices) : - This is the function that enables moving from one option to another based on player's keystrokes.
*                                                       - Enables circular navigation (jump back to the first choice after the last choice, and vice versa).
*/
void changeChoice(int key, int& choice, int noOfChoices)
{
	//Up or Left key.
	if (key == 72 || key == 75)
	{
		//Moves Up/Left/Previous button if there's button before the current selected button.
		if (choice > 0) choice -= 1;

		//Jumps to the last options
		else choice = noOfChoices - 1;
	}

	//Down or right key.
	else if (key == 80 || key == 77)
	{
		//Moves Down/Right/Next button
		if (choice < noOfChoices - 1) choice += 1;

		//Jumps to the first option
		else choice = 0;
	}
}


/*
* checkMatch(int row, int col, deque<pair<int, int>> snakeCoords) : - This method is used to check if the given row and column numbers matches with the coordinates of any part of the snake.
*																    - Used to check if the fruit has been eaten, or if the snake has hit the boundary.
*/
pair<bool, int> checkMatch(int row, int col, deque<pair<int, int>> snakeCoords)
{
	//Goes through each part of the snake and returns true and the index of the matching part in the deque.
	for (int i = 0; i < snakeCoords.size(); i++) {
		if (snakeCoords[i].first == row && snakeCoords[i].second == col) {
			return { true,i };
		}
	}

	//Else returns false and -1.
	return { false,-1 };
}


/*
* retrieveFileRecords(string fileName) : - Reads a TXT file.
*                                        - Used to read the files that contain the saved games and the high scores.
*/
deque<string> retrieveFileRecords(string fileName)
{
	//The deque that will be returned.
	deque<string> recordStrings;

	//Add the file path.
	fileName = "data/" + fileName;

	//Opens the file to read.
	ifstream loadFile;
	loadFile.open(fileName);

	//Checks if the file is open, readable and if it is not empty.
	if (loadFile.good() && loadFile.is_open() && !(loadFile.peek() == ifstream::traits_type::eof()))
	{
		string fileLine;
		while (getline(loadFile, fileLine))
		{
			if (!fileLine.empty()) recordStrings.push_back(fileLine);
		}
	}
	//Closes the file.
	loadFile.close();

	return recordStrings;
}


/*
* saveFileRecords(std::string fileName, std::deque<std::string> records) : - Writes a deque of strings into a TXT file.
*																		   - Used to write the saved games and the high scores into their respective TXT files.
*/
void saveFileRecords(std::string fileName, std::deque<std::string> records)
{
	//Add the file path.
	fileName = "data/" + fileName;

	//Opens the file to write.
	ofstream saveFile;
	saveFile.open(fileName);

	//Checks if the file is open and not empty.
	if (saveFile.is_open() && !records.empty()) 
	{
		for (int i = 0; i < records.size(); i++)
		{
			//Writes the record that is converted to a string into the TXT file.
			saveFile << records[i] << endl;
		}
	}
	//Close the file.
	saveFile.close();
}