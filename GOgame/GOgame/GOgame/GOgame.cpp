#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "conio2.h"
#include "Menu.h"
#include "Game.h"

// If handleChoice return this value program ends
#define ExitValue -16546

#define ArrowDown 0x50
#define ArrowUp 0x48

#define MenuSizeWidth 40
#define MenuSizeHeight 7

const char* MainMenuOptions[3] = {
	"Start Game",
	"Load Game",
	"Exit"
};
const int numberOfMainMenuOptions = (sizeof(MainMenuOptions) / sizeof(char*));

const char* SizeSelectionMenuOptions[4] = {
	"9X9",
	"13X13",
	"19X19",
	"Other"
};
const int numberOfSizeSelectionMenuOptions = (sizeof(SizeSelectionMenuOptions) / sizeof(char*));


void handleArrows(Menu& menu);

int handleChoice(Menu& menu);

void startGame();

int getBoardSize();

int main() {
#ifndef __cplusplus
	Conio2_Init();
#endif
	consoleInit();

	// creates the menu and starts the program
	Menu menu = Menu(MenuSizeWidth, MenuSizeHeight, 1, 0, MainMenuOptions, numberOfMainMenuOptions, "GO Game");
	char choice = NULL;
	while (choice != 'q' && choice != 'Q') {
		choice = getch();
		if (choice == 0) {
			handleArrows(menu);
		}
		else if (choice == '\r') { // "Enter" key
			if (handleChoice(menu) == ExitValue) break; // if exit is selected it quits the loop
		}
	}
	
	consoleEnd();
	return 0;
}

void handleArrows(Menu& menu) {
	char choice = getch();
	if (choice == ArrowDown) {
		menu.goDown();
	}
	else if (choice == ArrowUp) {
		menu.goUp();
	}
}

// handles user main menu choice
// return whether exit was chosen or a game was ended. If so program should end
int handleChoice(Menu& menu) {
	if (menu.getMenuOption() == STARTGAME) {
		startGame();
		menu.printMenu();
		return 0;
	}
	else if (menu.getMenuOption() == LOADGAME) {
		Game game = Game();
		game.loadGameState();
		menu.printMenu();
		return 0;
	}
	else if (menu.getMenuOption() == EXIT) {
		return ExitValue;
	}
	return 0;
}

// gets board size from the user
int getBoardSize() {
	Menu menu = Menu(MenuSizeWidth, MenuSizeHeight, 1, 0, SizeSelectionMenuOptions, numberOfSizeSelectionMenuOptions, "Chose a board size");
	char choice = NULL;
	while (choice != 'q' && choice != 'Q') {
		choice = getch();
		if (choice == 0) {
			handleArrows(menu);
		}
		else if (choice == '\r') { // "Enter" key
			if (menu.getMenuOption() == NINE) {
				return 9;
			}
			else if (menu.getMenuOption() == THIRTEEN) {
				return 13;
			}
			else if (menu.getMenuOption() == NINETEEN) {
				return 19;
			}
			else if (menu.getMenuOption() == OTHER) {
				return menu.getUserNumberInput("Choose the board size bigger than 0:");
			}
		}
	}
}

// takes board size from the user and creates object of the game which starts the game
void startGame() {
	int boardSize = 0;
	while (boardSize < 1) {
		boardSize = getBoardSize();
	}

	Game game = Game(boardSize);
}