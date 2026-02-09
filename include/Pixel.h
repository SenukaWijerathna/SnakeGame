/*
* Author : Senuka Wijerathna (GitHub: SenukaWijerathna)
* File : Pixel.h
* Description : - This is the header file for the Pixel class, objects of which represents a single pixel in the game grid.
*  			    - Used in the grid of the main snake gameplay.
*/


#pragma once

#include <string>

class Pixel
{
private:
	std::string nature;            //What the pixel represents: the fruit, the snake's head, a part of snake's body, or the background.

public:
	/**** CONSTRUCTOR ****/
	Pixel();

	/**** GETTERS ****/
	std::string getNature();

	/**** SETTERS ****/
	void setNature(std::string n);

	/**** OTHER METHODS ****/
	int pixelColor();              //Used to get the color of the pixel based on its nature.
};