#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Constants.h"
#include "Window.h"
#include "Menu.h"
#include "Game.h"

#define ONE_HUNDRED_FPS_LIMIT 0.01

enum menuOption {
	START_GAME,
	LOAD,
	EXIT
};

int loadGame(Screen* screen, Game* game, save* saves, int numberOfSaves);

const int numberOfOptions = 3;
char** _menuOptions = new char* [numberOfOptions];

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv) {
	time_t t;
	srand(time(&t));
	_seed = rand();

	_menuOptions[0] = "Start game";
	_menuOptions[1] = "Load";
	_menuOptions[2] = "Exit";

	int t1, t2, quit, frames;
	double time, worldTime, fpsTimer, fps, distance, speed;

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	distance = 0;
	speed = 0;

	Screen* screen = new Screen();
	screen->fillScreen(BLUE);
	Menu* menu = new Menu(screen, _menuOptions, numberOfOptions, "Main menu");
	Game* game = new Game(screen, &worldTime);

	while (!quit) {
		t2 = SDL_GetTicks();

		// here t2-t1 is the time in milliseconds since
		// the last screen was drawn
		// delta is the same time in seconds
		time = (t2 - t1) * 0.001;
		// a fps limiter
		if (time < ONE_HUNDRED_FPS_LIMIT) {
			continue;
		}
		t1 = t2;

		worldTime += time;

		fpsTimer += time;
		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};

		screen->update();

		if (game->getState() == NONE) {
			// if handler returns true exit the program
			quit = menu->eventHandler(screen->getEvent());
			int option = menu->getMenuOption();
			switch (option) {
			case START_GAME:
				game->setState(ALIVE);
				break;
			case LOAD: {
				int numberOfSaves = 2;
				save* saves = game->loadGames(&numberOfSaves);
				int index = loadGame(screen, game, saves, numberOfSaves);
				game->loadGameFromTheSave(saves[index]);
				delete[] saves;
				break;
			}
			case EXIT:
				quit = 1;
				break;
			}
		}
		else {
			game->nextTick(fps);
			if (game->eventHandler(screen->getEvent()) == 1) {
				delete game;
				game = new Game(screen, &worldTime);
				menu->drawMenu();
			}
		}

		frames++;
	};

	delete game;
	delete screen;
	return 0;
};

// return index of user chosen save
int loadGame(Screen* screen, Game* game, save* saves, int numberOfSaves) {
	int quit = 0;
	char** menuOptions = new char* [numberOfSaves];
	for (int i = 0; i < numberOfSaves; i++) {
		menuOptions[i] = saves[i].saveName;
	}
	Menu* menu = new Menu(screen, menuOptions, numberOfSaves, "SAVED GAMES");

	int indexOfChosenSave = -1;

	while (!quit) {
		menu->drawMenu();
		screen->update();

		// if handler returns true exit the program
		quit = menu->eventHandler(screen->getEvent());
		int option = menu->getMenuOption();
		if (option != -1) {
			indexOfChosenSave = option;
			break;
		}
		
	}

	delete[] _menuOptions;
	return indexOfChosenSave;
}