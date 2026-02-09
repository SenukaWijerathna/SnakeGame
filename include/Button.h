/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : Button.h
* Description : - This is the header file for the Button class, which is used create interactive buttons.
*			    - Used throughout the entire 'SNAKE GAME'.
*/

#pragma once


#include <iostream>
#include <string>
#include <utility>

//Used to change the colors when displaying the button.
#include "SharedFunctionsAndVariables.h"

class Button
{
private:
	static std::pair<int, int> NORMAL_BUTTON_COLOR;   //Color of a button in its normal state.
	static std::pair<int, int> BLINKING_BUTTON_COLOR; //Color of a button when it is blinking.

	int width = 22;                  //Width of a button. A default width of 22 characters is used.
	int height = 3;				     //Height of a button. A default height of 3 characters is used.
	std::string label;				 //Text shown in the button.
	bool selectionStatus;            //Changes if the button is selected.
	bool blinking;                   //Enables the 'blinking' effect of the button.

public:
	/**** CONSTRUCTOR ****/
	Button(std::string l);                                      //Only accepts the text shown in the button
	Button(std::string l, int buttonWidth, int buttonHeight);   //Accepts the text as well as the width and height of the button. Used in custom-sized buttons.

	/**** GETTERS ****/
	int getWidth();
	int getHeight();
	std::string getLabel();
	bool getSelectionStatus();
	bool getBlinking();

	/**** SETTERS ****/
	void setWidth(int buttonWidth);
	void setHeight(int buttonHeight);
	void setLabel(std::string l);
	void setSelectionStatus(bool status);
	void setBlinking(bool blink);

	/**** OTHER METHODS ****/
	void printButton(int line, int leftPadding, int rightPadding, int paddingColor); //Used to print the button line by line.
};