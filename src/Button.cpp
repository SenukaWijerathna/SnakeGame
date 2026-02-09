/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : Button.cpp
* Description : - This provides the definitions for the methods in Button class, which is used create interactive buttons.
*			    - Used throughout the entire 'SNAKE GAME'.
*/

#include "Button.h"

//sets the values for the constants
std::pair<int, int> Button::NORMAL_BUTTON_COLOR = { 10,15 };
std::pair<int, int> Button::BLINKING_BUTTON_COLOR = { 9,0 };


/**** CONSTRUCTORS ****/
Button::Button(std::string l)
{
    label = l;
    selectionStatus = false;
    blinking = false;
}

Button::Button(std::string l, int buttonWidth, int buttonHeight)
{
    label = l;
    width = buttonWidth;
    height = (buttonHeight % 2 == 0) ? buttonHeight + 1 : buttonHeight;
    selectionStatus = false;
    blinking = false;
}


/**** GETTERS ****/

int Button::getWidth()
{
    return width;
}

int Button::getHeight()
{
    return height;
}

std::string Button::getLabel()
{
    return label;
}

bool Button::getSelectionStatus()
{
    return selectionStatus;
}

bool Button::getBlinking()
{
    return blinking;
}


/**** SETTERS ****/

void Button::setWidth(int buttonWidth)
{
    width = buttonWidth;
}

void Button::setHeight(int buttonHeight)
{
    height = buttonHeight;
}

void Button::setLabel(std::string l)
{
    label = l;
}

void Button::setSelectionStatus(bool status)
{
    selectionStatus = status;
}

void Button::setBlinking(bool blink)
{
    blinking = blink;
}

/**** OTHER METHODS ****/

/*
* printButton() : - Used to display the button.
*                 - Prints out the button line by line according to given constraints and colors (For-loops are used to go through each line).
*/
void Button::printButton(int line, int leftPadding, int rightPadding, int paddingColor)
{
    int borderColor;
    int textColor;

    //if the button is selected, colors will change according to the current state of 'blinking'.
    if (selectionStatus)
    {
        if (blinking)
        {
            borderColor = BLINKING_BUTTON_COLOR.first;
            textColor = BLINKING_BUTTON_COLOR.second;
        }
        else
        {
            borderColor = NORMAL_BUTTON_COLOR.first;
            textColor = NORMAL_BUTTON_COLOR.second;
        }
    }
    else
    {
        borderColor = NORMAL_BUTTON_COLOR.first;
        textColor = NORMAL_BUTTON_COLOR.second;
    }

    //calculation of the padding around the text within the button
    int topBottomPadding = (height - 1) / 2;                        //padding above and below the text
    int widthWithoutBorders = width - 2;
    int labelLength = label.length();                               //length of the text
    int leftSpace = (widthWithoutBorders - labelLength) / 2;        //padding to the left of the text
    int rightSpace = widthWithoutBorders - leftSpace - labelLength; //padding to the right of the text


    //display the gap before the button in the screen
    setPixelColor(paddingColor, 0);
    for (int i = 0; i < leftPadding; i++) std::cout << " ";

    //display the button according to a for-loop (A for-loop is implemented whereever this method is used)
    switch (line)
    {
    //padding above (0) and below (2) the text in the button
    case 0:
    case 2:
        for (int gap = 0; gap < topBottomPadding; gap++)
        {

            setPixelColor(borderColor, 0);
            for (int i = 0; i < width; i++) std::cout << " ";
        }
        break;

    //The line of the button that contains the text of the button
    case 1:
        setPixelColor(borderColor, textColor);
        std::cout << " ";
        for (int i = 0; i < leftSpace; i++) std::cout << " ";
        std::cout << label;
        for (int i = 0; i < rightSpace; i++) std::cout << " ";
        std::cout << " ";
        break;

    default:
        break;
    }

    //display the gap after the button in the screen
    setPixelColor(paddingColor, 0);
    for (int i = 0; i < rightPadding; i++) std::cout << " ";
    setPixelColor(0, 7);
}