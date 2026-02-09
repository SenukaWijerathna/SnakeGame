/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : Pixel.cpp
* Description : - This provides the definitions for the methods in Pixel class, objects of which represents a single pixel in the game grid.
*			    - Used in the grid of the main snake gameplay.
*/

#include "Pixel.h"

/**** CONSTRUCTOR ****/

Pixel::Pixel()
{
	nature = "blank";
}


/**** GETTERS ****/

std::string Pixel::getNature()
{
	return nature;
}


/**** SETTERS ****/

void Pixel::setNature(std::string n)
{
	if (n == "blank" || n == "snakeHead" || n == "snakeBody" || n == "fruit") nature = n;
}


/**** OTHER METHODS ****/

/*
* pixelColor() : - Returns the color of the pixel based on its nature.
*/
int Pixel::pixelColor()
{
	if (nature == "blank") return 14;          //Blank        : Light Yellow
	else if (nature == "snakeHead") return 9;  //Snake's Head : Light Blue
	else if (nature == "snakeBody") return 3;  //Snake's Body : Aqua
	else return 4;                             //Fruit        : Red
}