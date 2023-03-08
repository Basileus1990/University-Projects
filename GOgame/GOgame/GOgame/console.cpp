#define _CRT_SECURE_NO_WARNINGS
#include "console.h"
#include <stdio.h>
#include <string.h>
#include "conio2.h"

void consoleInit() {
	settitle("Pawel Bogdanowicz, 193123");
	_setcursortype(_NOCURSOR);
}

void consoleEnd() {
	_setcursortype(_NORMALCURSOR);
	textbackground(BLACK);
	textcolor(WHITE);
}

void printAtXY(const char* text, int x, int y, int backgroundColor, int textColor) {
	textbackground(backgroundColor);
	textcolor(textColor);
	gotoxy(x + ConsoleMarginX, y + ConsoleMarginY);
	cputs(text);
	textbackground(BLACK);
	textcolor(WHITE);
}



void printCenteredString(const char* originalString, int x, int y, int targetWidth, int backgroundColor, int textColor) {
	printAtXY(originalString, x + ((targetWidth - strlen(originalString)) / 2), y, backgroundColor, textColor);
}

void printCharAtXY(const char character, int x, int y, int backgroundColor, int textColor) {
	char text[2];
	sprintf(text, "%c", character);
	printAtXY(text, x, y, backgroundColor, textColor);
}

void clearAtXY(int x, int y, int clearWidth, int backgroundColor, int textColor) {
	for (int i = 0; i < clearWidth; i++) {
		printAtXY(" ", x + i, y, backgroundColor, textColor);
	}
}