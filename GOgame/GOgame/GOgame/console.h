/*
	A module for writing in the console. The only interactions with the console
	are happening here.
*/
#include "conio2.h"

#define ConsoleMarginX 2
#define ConsoleMarginY 2


// initializes the consoles
void consoleInit();

// returns to normal console
void consoleEnd();

// prints a given string at XY with given or default background color and text color
void printAtXY(const char* text, int x, int y, int backgroundColor, int textColor);

// prints a given string at XY with given or default background color and text color
void printCharAtXY(const char character, int x, int y, int backgroundColor, int textColor);

// prints centered originalString depending on starting position and target width
void printCenteredString(const char* originalString, int x, int y, int targetWidth, int backgroundColor, int textColor);

// clears from console from XY to X+clearWidthY
void clearAtXY(int x, int y, int clearWidth, int backgroundColor, int textColor);