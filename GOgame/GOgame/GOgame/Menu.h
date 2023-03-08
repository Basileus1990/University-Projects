#include "Widget.h"
#include "conio2.h"
#include <string.h>

#define MenuLegendSizeWidth 35
enum MainMenuOption {
	STARTGAME,
	LOADGAME,
	EXIT,
};

enum SizeSelectionMenuOption {
	NINE,
	THIRTEEN,
	NINETEEN,
	OTHER
};

// Menu class for interaction and drawing with the menu
class Menu{
private:
	const char* legendText[5]{
		"Author: Pawel Bogdanowicz 193123",
		"Controls:",
		"Arrows - moving around the menu",
		"Enter  - select option",
		"Q      - exit the game"
	};
	int numberOfLegendLines = (sizeof(legendText) / sizeof(char*));

	// options displayed in the menu
	char** menuOptions;
	int numberOfMenuOptions;
	const char* optionHighlighter = "<---";
	char* title;

	// An ID of selected option
	int selectedOption = 0;

	//sposition of the first line of the menu, so the menu is centered vertically
	int indexOfFirstMenuLine;

	Widget menuWidget;
	Widget legend;

public:
	Menu(int newWidth, int newHeight, int x, int y, const char** options, int amountOfOptions, const char* newTitle ,bool legendOnRight = true, int defBgColor = BLACK, int defTextColor = WHITE):
	menuWidget(Widget()), legend(Widget())
	{
		// Sets options dynamically
		numberOfMenuOptions = amountOfOptions;
		menuOptions = (char**)(malloc(sizeof(char*) * numberOfMenuOptions));
		for (int i = 0; i < numberOfMenuOptions; i++) {
			menuOptions[i] = (char*)(malloc(sizeof(char) * strlen(options[i])));
			strcpy(menuOptions[i], options[i]);
		}

		title = (char*)(malloc(sizeof(char) * strlen(newTitle)));
		strcpy(title, newTitle);

		// sets legend and menu position
		if (legendOnRight) {
			menuWidget.setFields(newWidth, newHeight, x, y, defBgColor, defTextColor);
			legend.setFields(MenuLegendSizeWidth, newHeight, x + newWidth + 2, y, defBgColor, defTextColor);
		}
		else {
			menuWidget.setFields(newWidth, newHeight, x + MenuLegendSizeWidth + 2, y, defBgColor, defTextColor);
			legend.setFields(MenuLegendSizeWidth, newHeight, x, y, defBgColor, defTextColor);
		}

		indexOfFirstMenuLine = (menuWidget.getHeight() - numberOfMenuOptions) / 2;
		printMenu();
	}

private:
	// prints option from menuOptions[optionIndex] wiht the highlighter and in red color
	void printSelectedOption(int optionIndex) {
		int newTextLenght = strlen(menuOptions[optionIndex]) + strlen(optionHighlighter) + 2;
		char* text = new char[newTextLenght];
		int i = 0;
		for (int k = 0; k < strlen(menuOptions[optionIndex]); i++, k++) {
			text[i] = menuOptions[optionIndex][k];
		}
		text[i] = ' ';
		i++;
		for (int k = 0; k < strlen(optionHighlighter); i++, k++) {
			text[i] = optionHighlighter[k];
		}
		text[i] = '\0';

		menuWidget.printCenteredInWidget(text, optionIndex + indexOfFirstMenuLine, BLACK, RED);

		delete[] text;
	}

	void changeSelectedOption(int newOption) {
		menuWidget.clearLine(indexOfFirstMenuLine + selectedOption);
		menuWidget.clearLine(indexOfFirstMenuLine + newOption);

		menuWidget.printCenteredInWidget(menuOptions[selectedOption], selectedOption + indexOfFirstMenuLine);
		printSelectedOption(newOption);

		selectedOption = newOption;
	}

	void printLegend() {
		legend.printBorder();
		for (int i = 0; i < numberOfLegendLines; i++) {
			legend.printInWidget(legendText[i], 0, i);
		}
	}



public:
	// clears the screen and prints whole menu
	void printMenu() {
		clrscr();
		printLegend();
		menuWidget.printBorder();
		menuWidget.printCenteredInWidget(title, 0);
		for (int i = 0; i < numberOfMenuOptions; i++) {
			if (i == selectedOption) {
				printSelectedOption(i);
			}
			else {
				menuWidget.printCenteredInWidget(menuOptions[i], i + indexOfFirstMenuLine);
			}
		}
	}

	void goDown() {
		changeSelectedOption((selectedOption + 1) % numberOfMenuOptions);
	}

	void goUp() {
		if (selectedOption == 0) {
			changeSelectedOption(((selectedOption + numberOfMenuOptions - 1) % numberOfMenuOptions));
			return;
		}
		changeSelectedOption((selectedOption - 1)% numberOfMenuOptions);
	}

	int getMenuOption() {
		return selectedOption;
	}

	int getUserNumberInput(const char* questionText, int backgroundColor = NULL, int textColor = NULL) {
		return menuWidget.getUserNumberInput(questionText, backgroundColor, textColor);
	}
};