#include "console.h"
#include <string.h>
#include <stdlib.h>

#define TopBorderChar char(196)
#define SideBorderChar char(179)
#define LeftTopBorderChar char(218)
#define LeftDownBorderChar char(192)
#define RightTopBorderChar char(191)
#define RightDownBorderChar char(217)

#define InputPosX startX
#define InputPosY startY + height + 1

class Widget {
private:
	int width;
	int height;
	int startX;
	int startY;
	int defaultTextColor;
	int defaultBgColor;

	// border determines whether to draw a border
public:
	Widget() {}

	void setFields(int newWidth, int newHeight, int x, int y, int defBgColor = BLACK, int defTextColor = WHITE) {
		width = newWidth;
		height = newHeight;
		startX = x;
		startY = y;
		defaultBgColor = defBgColor;
		defaultTextColor = defTextColor;

		colorTheWidget();
	}

private:
	bool isInside(int x, int y) {
		if (x < 0  || x >= width  || y < 0 || y >= height ) {
			return false;
		}
		return true;
	}

	// checks if whether default bg color should be used or a custom one was given
	int checkBgColor(int bgColor) {
		if (bgColor == NULL) {
			bgColor = defaultBgColor;
		}
		return bgColor;
	}

	// checks if whether default text color should be used or a custom one was given
	int checkTxtColor(int txtColor) {
		if (txtColor == NULL) {
			txtColor = defaultTextColor;
		}
		return txtColor;
	}

	bool checkIfStringFits(const char* originalString, int x) {
		if (strlen(originalString) + x > width) {
			return false;
		}
		return true;
	}

	void colorTheWidget() {
		for (int i = startY; i < startY + height; i++) {
			for (int j = startX; j < startX + width; j++) {
				printAtXY(" ", j, i, defaultBgColor, defaultTextColor);
			}
		}
	}

public:
	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	int getStartX() {
		return startX;
	}
	int getStartY() {
		return startY;
	}

	// prints border around the widget body
	void printBorder() {
		for (int i = 0; i < width; i++) {
			printCharAtXY(TopBorderChar, startX + i, startY - 1, defaultBgColor, defaultTextColor);
			printCharAtXY(TopBorderChar, startX + i, startY + height, defaultBgColor, defaultTextColor);
		}
		for (int i = 0; i < height; i++) {
			printCharAtXY(SideBorderChar, startX - 1, startY + i, defaultBgColor, defaultTextColor);
			printCharAtXY(SideBorderChar, startX + width, startY + i, defaultBgColor, defaultTextColor);
		}
		printCharAtXY(LeftTopBorderChar, startX - 1, startY - 1, defaultBgColor, defaultTextColor);
		printCharAtXY(LeftDownBorderChar, startX - 1, startY + height, defaultBgColor, defaultTextColor);
		printCharAtXY(RightTopBorderChar, startX + width, startY - 1, defaultBgColor, defaultTextColor);
		printCharAtXY(RightDownBorderChar, startX + width, startY + height, defaultBgColor, defaultTextColor);
	}

	// prints the text in given position relative to the widget
	// If the given position is outsite of the widget it doesn't write anything
	void printInWidget(const char* text, int x, int y, int backgroundColor = NULL, int textColor = NULL) {
		if (!isInside(x, y)) {
			return;
		}
		if (!checkIfStringFits(text, x)) {
			return;
		}
		printAtXY(text, x + startX, y + startY, checkBgColor(backgroundColor), checkTxtColor(textColor));
	}

	// prints the character in given position relative to the widget
	// If the given position is outsite of the widget it doesn't write anything
	void printCharInWidget(const char character, int x, int y, int backgroundColor = NULL, int textColor = NULL) {
		if (!isInside(x, y)) {
			return;
		}
		printCharAtXY(character, x + startX, y + startY, checkBgColor(backgroundColor), checkTxtColor(textColor));
	}

	// prints the text in the middle of the widget
	// If the given position is outsite of the widget it doesn't write anything
	void printCenteredInWidget(const char* text, int y, int backgroundColor = NULL, int textColor = NULL) {
		const int startingX = 0;
		if (!isInside(1, y)) {
			return;
		}
		if (!checkIfStringFits(text, startingX)) {
			return;
		}
		printCenteredString(text, startingX + startX, y + startY, width - 2, checkBgColor(backgroundColor), checkTxtColor(textColor));
	}

	// clears whole line in widget
	void clearLine(int y, int backgroundColor = NULL, int textColor = NULL) {  
		clearAtXY(startX, y, width, checkBgColor(backgroundColor), checkTxtColor(textColor));
	}

	// Placed bellow the widget
	// Asks user for input and if it doesn't get a number it displays a message and return 0
	int getUserNumberInput(const char* questionText, int backgroundColor = NULL, int textColor = NULL) {
		printAtXY(questionText, InputPosX, InputPosY, checkBgColor(backgroundColor), checkTxtColor(textColor));
		const int userInputSize = 5;
		int result = 0;
		char userInput[userInputSize];
		char input;
		int i = 0;
		for (; i < userInputSize - 1; i++) {
			input = getch();
			if (input == '\r') {  // enter
				userInput[i + 1] = '\0';
				result = atoi(userInput);
				break;
			}
			else if (input == char(8)) {  // enter
				if (strlen(userInput) == 0) break;
				i--;
				printAtXY(" ", InputPosX + strlen(questionText) + 1 + i, InputPosY, checkBgColor(backgroundColor), checkTxtColor(textColor));
				i--;
				continue;
			}
			userInput[i] = input;
			printCharAtXY(input, InputPosX + strlen(questionText) + 1 + i, InputPosY, checkBgColor(backgroundColor), checkTxtColor(textColor));
		}

		clearAtXY(InputPosX, InputPosY, strlen(questionText) + i + 1, checkBgColor(backgroundColor), checkTxtColor(textColor));
		if (result == 0) {
			const char* ErrorMessage[3]{
				"Your input has to be a number and it's",
				"value should be between 1 and 999!",
				"To continue press any key..."
			};

			printAtXY(ErrorMessage[0], InputPosX, InputPosY, checkBgColor(backgroundColor), checkTxtColor(textColor));
			printAtXY(ErrorMessage[1], InputPosX, InputPosY + 1, checkBgColor(backgroundColor), checkTxtColor(textColor));
			printAtXY(ErrorMessage[2], InputPosX, InputPosY + 2, checkBgColor(backgroundColor), checkTxtColor(textColor));
			getch();
			clearAtXY(InputPosX, InputPosY, strlen(ErrorMessage[0]) + i, checkBgColor(backgroundColor), checkTxtColor(textColor));
			clearAtXY(InputPosX, InputPosY + 1, strlen(ErrorMessage[1]) + i, checkBgColor(backgroundColor), checkTxtColor(textColor));
			clearAtXY(InputPosX, InputPosY + 2, strlen(ErrorMessage[2]) + i, checkBgColor(backgroundColor), checkTxtColor(textColor));
		}
		return result;
	}

	// Placed bellow the widget
	// Returns after we hit enter. When we go over the character limit it returns null after showing a message
	char* getUserStringInput(const char* questionText, int backgroundColor = NULL, int textColor = NULL) {
		printAtXY(questionText, InputPosX, InputPosY, checkBgColor(backgroundColor), checkTxtColor(textColor));
		const int userInputSize = 30;
		char userInput[userInputSize];
		char input;
		int i = 0;
		for (; i < userInputSize - 1; i++) {
			input = getch();
			if (input == '\r') {  // enter
				if (strlen(userInput) == 0) break;
				userInput[i + 1] = '\0';
				clearAtXY(InputPosX, InputPosY, strlen(questionText) + i + 1, checkBgColor(backgroundColor), checkTxtColor(textColor));
				return userInput;
			}
			else if (input == char(8)) {  // enter
				if (strlen(userInput) == 0) break;
				i--;
				printAtXY(" ", InputPosX + strlen(questionText) + 1 + i, InputPosY, checkBgColor(backgroundColor), checkTxtColor(textColor));
				i--;
				continue;
			}
			userInput[i] = input;
			printCharAtXY(input, InputPosX + strlen(questionText) + 1 + i, InputPosY, checkBgColor(backgroundColor), checkTxtColor(textColor));
		}

		const char* ErrorMessage[2] = {
			"Your input has to be less than 30 characters!",
			"To continue press any key..."
		};
		clearAtXY(InputPosX, InputPosY, strlen(questionText) + i, checkBgColor(backgroundColor), checkTxtColor(textColor));
		printAtXY(ErrorMessage[0], InputPosX, InputPosY, checkBgColor(backgroundColor), checkTxtColor(textColor));
		printAtXY(ErrorMessage[1], InputPosX, InputPosY + 1, checkBgColor(backgroundColor), checkTxtColor(textColor));
		getch();
		clearAtXY(InputPosX, InputPosY, strlen(ErrorMessage[0]) + i, checkBgColor(backgroundColor), checkTxtColor(textColor));
		clearAtXY(InputPosX, InputPosY + 1, strlen(ErrorMessage[1]) + i, checkBgColor(backgroundColor), checkTxtColor(textColor));

		return nullptr;
	}
};