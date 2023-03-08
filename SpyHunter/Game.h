#include "Car.h"
#include "CarAI.h"
#include "Tree.h"

// state of the game
enum State {
	NONE,
	PAUSED,
	ALIVE,
	END
};

struct carSave {
	bool exists;
	pos position;
	double speed;
	CarStatus carStatus;
};

struct save {
	char saveName[50];
	State myState;
	double currentTime;
	double startTime;
	int distance;
	int previoutDistance;
	int score;
	double scoreDellay;
	int numberOfLifes;
	int addedLifes;
	carSave myCars[MAX_AMOUNT_OF_CARS];
};

const char ImplementedElements[] = {"abcdefghijlm"};


#define LEGEND_X 0
#define LEGEND_Y 0
#define LEGEND_WIDTH  SCREEN_WIDTH
#define LEGEND_HEIGHT  (LETTER_HEIGHT * 3)
#define GAME_LEGEND_COLOR BLACK
const char LegendTextAuthor[] = { "Pawel Bogdanowicz 193123" };

#define GAME_START_Y LEGEND_Y + LEGEND_HEIGHT + 1
#define GAME_WIDTH SCREEN_WIDTH
#define GAME_HEIGHT SCREEN_HEIGHT - (GAME_START_Y)

#define MAX_SPEED 10.0
#define MIN_SPEED 1.0
#define DEFAULT_SPEED_DIFFRENCE 1

#define PLAYER_START_X SCREEN_WIDTH / 2
#define PLAYER_START_Y SCREEN_HEIGHT - SCREEN_HEIGHT / 4

#define INITIAL_NUMBER_OF_LIFES 3
#define SCORE_FOR_ONE_LIFE 1000
#define SCORE_DELLAY 10 // in seconds
#define SCORE_FOR_ENEMY 100
#define TIME_OF_UNLIMITED_LIFES 10	// in seconds

#define CONTROLS_SENSIVITY 0.02

#define MAX_NUMBER_OF_TREES 4

#define TREE_PADDING_FROM_BORDER 60
#define ROAD_LENGTH_CALCULATIONS (100 * 1.4)

#define SAVE_FILE_NAME "saves.sv"

// class responsible for the displaying the game and it's logic
class Game {
	State _state = NONE;
	double _startTime;
	double* _currentTime;
	int _distance = 0;
	int _perviousDistance = 0; // distance after calculating score
	int _score = 0;
	double _scoreDellay = -SCORE_DELLAY;
	int _numberOfLifes = INITIAL_NUMBER_OF_LIFES;
	int _addedLifes = 0;

	// tell the game whether to move player car
	moveVector _carMove = { VSTATIONAL, VSTATIONAL };

	Screen* _screen;

	// _cars[0] --> player's car
	Car* _cars[MAX_AMOUNT_OF_CARS];
	Tree* _trees[MAX_NUMBER_OF_TREES];


public:
	Game(Screen* screen, double* time) {
		_screen = screen;
		_startTime = *time;
		_currentTime = time;

		// cars creation
		_cars[0] = new Car(_screen, Player, MIN_SPEED, PLAYER_START_X, PLAYER_START_Y);
		int k = 1;
		for (int i = 1; i < MAX_AMOUNT_OF_CARS; i++) {
			_cars[i] = NULL;
		}
		for (int i = 0; i < MAX_NUMBER_OF_TREES; i++) {
			_trees[i] = new Tree(_screen);
		}
	}

	~Game() {
		for (int i = 0; i < MAX_AMOUNT_OF_CARS; i++) {
			if (_cars[i] != NULL) {
				delete _cars[i];
			}
		}
		for (int i = 0; i < MAX_NUMBER_OF_TREES; i++) {
			delete _trees[i];
		}
	}

	int eventHandler(SDL_Event* event) {
		while (SDL_PollEvent(event)) {
			switch ((*event).type) {
			case SDL_KEYDOWN:
				if ((*event).key.keysym.sym == SDLK_ESCAPE) {
					// quits the programm
					return 1;
				}
				else if ((*event).key.keysym.sym == SDLK_DOWN) {
					_carMove.y = VDown;
				}
				else if ((*event).key.keysym.sym == SDLK_UP) {
					_carMove.y = VUp;
				}
				else if ((*event).key.keysym.sym == SDLK_RIGHT) {
					_carMove.x = VRight;
				}
				else if ((*event).key.keysym.sym == SDLK_LEFT) {
					_carMove.x = VLeft;
				}
				// Pause switcher
				else if ((*event).key.keysym.sym == SDLK_p) {
					if (_state == ALIVE) {
						_state = PAUSED;
					}
					else {
						_state = ALIVE;
					}
				}
				// new game key
				else if ((*event).key.keysym.sym == SDLK_n) {
					return 1;
				}
				// finish button
				else if ((*event).key.keysym.sym == SDLK_f) {
					_state = END;
				}
				// save button
				else if ((*event).key.keysym.sym == SDLK_s) {
					saveGame();
				}
				break;
			case SDL_KEYUP:
				if ((*event).key.keysym.sym == SDLK_DOWN) {
					_carMove.y = VSTATIONAL;
				}
				else if ((*event).key.keysym.sym == SDLK_UP) {
					_carMove.y = VSTATIONAL;
				}
				else if ((*event).key.keysym.sym == SDLK_RIGHT) {
					_carMove.x = VSTATIONAL;
				}
				else if ((*event).key.keysym.sym == SDLK_LEFT) {
					_carMove.x = VSTATIONAL;
				}
				break;
			case SDL_QUIT:
				return 1;
			} 
		}
		return 0;
	}

	void nextTick(double fps) {
		if (_state == ALIVE) {
			checkIfCarsDied();
			
			// this if statement reduces the speed of the car by reducing the framerate of drawing the player car and the game world
			if ((int(*_currentTime * 1000) % 100) > (10 - _cars[0]->getSpeed() * 5)) {
				_distance += getSpeed();
				drawGame();
				drawTrees();
			}
			addAIRandomly();
			drawCars();
			movePlayerCar();
			moveAI();
		}
		else if(_state == PAUSED) {
			drawPaused();
		}
		else if (_state == END) {
			drawEnd();
			return;
		}
		updateScore();
		updateLives();
		drawLegend(fps);
		drawCompletedStepsOfTheProgram();
	}

	int getState() {
		return _state;
	}

	void setState(State state) {
		_state = state;
	}

	void setSpeed(double newSpeed) {
		if (newSpeed > MAX_SPEED || newSpeed < MIN_SPEED) {
			return;
		}
		// sets the speed to the player car
		_cars[0]->setSpeed(newSpeed);
	}

	double getSpeed() {
		return _cars[0]->getSpeed();
	}

	save* loadGames(int *numberOfSaves) {
		*numberOfSaves = 1;
		save* saves = new save[*numberOfSaves];

		FILE* file = fopen(SAVE_FILE_NAME, "rb");
		if (file != NULL) {
			int i = 0;
			for (; fread(&saves[i], sizeof(save), 1, file); i++) {
				if (i == *numberOfSaves - 1) {
					save* newSave = new save[*numberOfSaves + 1];
					for (int j = 0; j < *numberOfSaves; j++) {
						newSave[j] = saves[j];
					}
					*numberOfSaves += 1;
					delete[] saves;
					saves = newSave;
				}
			}
			fclose(file);
		}
		*numberOfSaves -= 1;
		return saves;
	}

	void loadGameFromTheSave(save save) {
		_state = save.myState;
		*_currentTime = save.currentTime;
		_startTime =save.startTime;
		_distance = save.distance;
		_perviousDistance = save.previoutDistance;
		_score = save.score;
		_scoreDellay = save.scoreDellay;
		_numberOfLifes = save.numberOfLifes;
		_addedLifes = save.addedLifes;
		for (int i = 0; i < MAX_AMOUNT_OF_CARS; i++) {
			if (save.myCars[i].exists) {
				if (_cars[i] == NULL) {
					_cars[i] = new Car(_screen, save.myCars[i].carStatus, save.myCars[i].speed, save.myCars[i].position.x, save.myCars[i].position.y);
					continue;
				}
				_cars[i]->setPosition(save.myCars[i].position.x, save.myCars[i].position.y);
				_cars[i]->setSpeed(save.myCars[i].speed);
				_cars[i]->setStatus(save.myCars[i].carStatus);
			}
			else {
				_cars[i] = NULL;
			}
		}
	}

private:
	void updateScore() {
		if (!_cars[0]->isOffRoad() && *_currentTime - _scoreDellay > SCORE_DELLAY) {
			_scoreDellay = -SCORE_DELLAY;
			if (_distance - _perviousDistance > 500) {
				_score += 50;
				_perviousDistance = _distance;
			}
		}
		else {
			_perviousDistance = _distance;
		}
	}

	void updateLives() {
		if (_score == (_addedLifes + 1)* SCORE_FOR_ONE_LIFE) {
			_numberOfLifes++;
			_addedLifes++;
		}
		if (_numberOfLifes <= 0) {
			_state = END;
		}
	}

	void movePlayerCar() {
		moveVector playerHorizontalMove = { _carMove.x * 2, VSTATIONAL};
		// a minimum move is 2px
		if (playerHorizontalMove.x > 0 && playerHorizontalMove.x < 2) {
			playerHorizontalMove.x = 2;
		}
		else if (playerHorizontalMove.x < 0 && playerHorizontalMove.x > -2) {
			playerHorizontalMove.x = -2;
		}
		_cars[0]->moveCar(playerHorizontalMove, _cars, _distance);

		if (_carMove.y == VUp) {
			setSpeed(getSpeed() + CONTROLS_SENSIVITY);
		}
		else if (_carMove.y == VDown) {
			setSpeed(getSpeed() - CONTROLS_SENSIVITY);
		}
	}

	void moveAI() {
		for (int i = 1; i < MAX_AMOUNT_OF_CARS; i++) {
			if (_cars[i] == NULL) {
				continue;
			}

			aiMoveCar(_distance, _cars[i], _cars);
			// deletes the AI car if it goes out of map range
			if (_cars[i]->getPos().y < 0 - OUTSIDE_THE_ROAD_DEATH_RADIUS || _cars[i]->getPos().y > SCREEN_HEIGHT + OUTSIDE_THE_ROAD_DEATH_RADIUS) {
				delete _cars[i];
				_cars[i] = NULL;
			}
		}
	}

	// assumes that player's car is always declared
	// new AI always take player's speed
	void addAIRandomly() {
		for (int i = 1; i < MAX_AMOUNT_OF_CARS; i++) {
			if (_cars[i] != NULL) {
				continue;
			}
			// this makes it so cars apperar at random intervals
			// these numbers indicate the probability of car beeing created
			if (rand() % 1000 > 995) {
				bool isCivilian = rand() % 2; // determines whether the car is an ally or an enemy
				double newSpeed = _cars[0]->getSpeed() - DEFAULT_SPEED_DIFFRENCE;
				if (isCivilian) {
					_cars[i] = new Car(_screen, Civilian, newSpeed, PLAYER_START_X, 0);
					if (checkIfCarSpawnsTooClose(_cars[i]) > -1) {
						delete _cars[i];
						_cars[i] = NULL;
						return;
					}
				}
				else {
					_cars[i] = new Car(_screen, Enemy, newSpeed, PLAYER_START_X, 0);
					if (checkIfCarSpawnsTooClose(_cars[i]) > -1) {
						delete _cars[i];
						_cars[i] = NULL;
						return;
					}
				}
			}
		}
	}

	// checks if the cars are dead and if they are it deletes them
	// If the car is player's car it deletes it and recreates it and decrements number of lifes
	// If the enemy is killed the score is increased, when civilian is killed then the score is halted
	void checkIfCarsDied() {
		for (int i = 1; i < MAX_AMOUNT_OF_CARS; i++) {
			if (_cars[i] == NULL) {
				continue;
			}
			if (_cars[i]->isDead()) {
				if (_cars[i]->getStatus() == Enemy) {
					_score += SCORE_FOR_ENEMY;
				}
				else {
					_scoreDellay = *_currentTime;
				}
				delete _cars[i];
				_cars[i] = NULL;
			}
		}

		// player handler
		if (_cars[0]->isDead()) {
			_numberOfLifes--;
			delete _cars[0];
			_cars[0] = new Car(_screen, Player, MIN_SPEED, PLAYER_START_X, PLAYER_START_Y);
			int tooCloseCar = checkIfCarSpawnsTooClose(_cars[0]);
			if (tooCloseCar >= 0) {
				delete _cars[tooCloseCar];
				_cars[tooCloseCar] = NULL;
				
				return;
			}
		}
	}

	// return to index of the too close car
	// return -1 if not found any
	int checkIfCarSpawnsTooClose(Car* current) {
		for (int i = 0; i < MAX_AMOUNT_OF_CARS; i++) {
			if (_cars[i] == NULL || _cars[i] == current) {
				continue;
			}

			if (current->getVerticalDistanceToACar(_cars[i]) < CAR_HEIGHT + COLLISION_MARGIN) {
				return i;
			}
		}
		return -1;
	}

	void drawGame() {
		for (int i = 0; i < GAME_HEIGHT; i++) {
			int roadLength = getRoadLength(i);

			int j = 0;
			//grass
			for (; j < (GAME_WIDTH - roadLength) / 2; j++) {
				_screen->drawPixel(j, i + GAME_START_Y, _screen->getColor(GREEN));
			}
			// road
			for (; j < (GAME_WIDTH - roadLength) / 2 + roadLength; j++) {
				_screen->drawPixel(j, i + GAME_START_Y, _screen->getColor(GRAY));
			}
			// grass
			for (; j < GAME_WIDTH - 1; j++) {
				_screen->drawPixel(j, i + GAME_START_Y, _screen->getColor(GREEN));
			}
		}
	}

	// responsible for drawing the game when it is paused
	void drawPaused() {
		char text[] = { "Paused" };
		const int pauseX = SCREEN_WIDTH / 2 - strlen(text) * LETTER_WIDTH / 2;
		const int pauseY = SCREEN_HEIGHT / 2;

		 
		_screen->drawRectangle(pauseX - TEXT_MARGIN, pauseY - LETTER_HEIGHT, strlen(text) * LETTER_WIDTH + TEXT_MARGIN * 2, LETTER_HEIGHT * 3, _screen->getColor(WHITE), _screen->getColor(BLACK));
		_screen->drawString(pauseX, pauseY, text);
	}

	void drawEnd() {
		_screen->fillScreen(GRAY);
		_screen->drawRectangle(0, 0, LEGEND_WIDTH, LEGEND_HEIGHT, _screen->getColor(WHITE), _screen->getColor(BLACK));

		char text[32];
		int currentDrawY = TEXT_MARGIN + 3;
		const int posy = TEXT_MARGIN + 3;

		sprintf(text, "GAME OVER");
		int textX= SCREEN_WIDTH / 2 - strlen(text) * LETTER_WIDTH / 2;
		_screen->drawString(textX, currentDrawY, text);
		currentDrawY += LEGEND_HEIGHT;

		sprintf(text, "Final score: %d", _score);
		textX = SCREEN_WIDTH / 2 - strlen(text) * LETTER_WIDTH / 2;
		_screen->drawString(textX, currentDrawY, text);

	}

	void drawLegend(double fps) {
		_screen->drawRectangle(0, 0, LEGEND_WIDTH, LEGEND_HEIGHT, _screen->getColor(WHITE), _screen->getColor(BLACK));

		int currentDrawX = TEXT_MARGIN;
		const int posy = TEXT_MARGIN + 3;

		_screen->drawString(currentDrawX, posy, LegendTextAuthor);
		currentDrawX += strlen(LegendTextAuthor) * LETTER_WIDTH + 10 + TEXT_MARGIN;

		char text[32];
		
		if (*_currentTime > TIME_OF_UNLIMITED_LIFES) {
			sprintf(text, "Lifes: %d", _numberOfLifes);
			_screen->drawString(currentDrawX, posy, text);
			currentDrawX += strlen(text) * LETTER_WIDTH + 10 + TEXT_MARGIN;
		}
		else {
			// In case if during the time of invincibility the player dies
			if (_numberOfLifes != INITIAL_NUMBER_OF_LIFES) {
				_numberOfLifes = INITIAL_NUMBER_OF_LIFES;
			}

			sprintf(text, "Invicibility time: %d", TIME_OF_UNLIMITED_LIFES - int(*_currentTime));
			_screen->drawString(currentDrawX, posy, text);
			currentDrawX += strlen(text) * LETTER_WIDTH + 10 + TEXT_MARGIN;
		}

		sprintf(text, "Score: %d", _score);
		_screen->drawString(currentDrawX, posy, text);
		currentDrawX += strlen(text) * LETTER_WIDTH + 10 + TEXT_MARGIN;

		sprintf(text, "Time: %ds", int(*_currentTime));
		_screen->drawString(currentDrawX, posy, text);
		currentDrawX += strlen(text) * LETTER_WIDTH + 10 + TEXT_MARGIN;

		sprintf(text, "FPS: %d", int(fps));
		_screen->drawString(currentDrawX, posy, text);
		currentDrawX += strlen(text) * LETTER_WIDTH + 10 + TEXT_MARGIN;

	}

	void drawCompletedStepsOfTheProgram() {
		const int startX = SCREEN_WIDTH - 17 * LETTER_WIDTH;

		_screen->drawRectangle(startX, SCREEN_HEIGHT - LETTER_HEIGHT * 2, SCREEN_WIDTH - startX, LETTER_HEIGHT * 2, _screen->getColor(WHITE), _screen->getColor(BLACK));
		_screen->drawString(startX + TEXT_MARGIN, SCREEN_HEIGHT - LETTER_HEIGHT * 3 / 2, ImplementedElements);
	}

	void drawCars() {
		for (int i = 0; i < MAX_AMOUNT_OF_CARS; i++) {
			if (_cars[i] != NULL) {
				_cars[i]->drawCar();
			}
		}
	}

	// takes care of creating and moving trees
	void drawTrees() {
		for (int i = 0; i < MAX_NUMBER_OF_TREES; i++) {
			// destroy trees
			if (_trees[i]->getY() > SCREEN_HEIGHT + 100) {
				_trees[i]->setPosition(NULL, NULL);
			}
			// moves trees down
			else if (_trees[i]->getY() != NULL) {
				_trees[i]->moveDown(_cars[0]->getSpeed());
				_trees[i]->drawTree();
			}
			// generates first tree
			else if (i == 0) {
				_trees[i]->setPosition(TREE_PADDING_FROM_BORDER, 1);
			}
			// generates other trees
			else if (_trees[i]->getY() == NULL && 990 < rand() % 1000) {
				if (i % 2) {
					_trees[i]->setPosition(SCREEN_WIDTH - TREE_PADDING_FROM_BORDER, 1);
				}
				else {
					_trees[i]->setPosition(TREE_PADDING_FROM_BORDER, 1);
				}
			}
		}	
	}

	int getRoadLength(int offSet) {
		double roadSeed = randomDeterministicNumber(_distance - offSet);
		return int(roadSeed * ROAD_LENGTH_CALCULATIONS);
	}

	void saveGame() {
		save newSave;
		time_t curTime = time(NULL);
		struct tm myTime = *localtime(&curTime);
		sprintf(newSave.saveName, "%d-%02d-%02d_%02d:%02d:%02d\n", myTime.tm_year + 1900, myTime.tm_mon + 1, myTime.tm_mday, myTime.tm_hour, myTime.tm_min, myTime.tm_sec);
		newSave.myState = _state;
		newSave.currentTime = *_currentTime;
		newSave.startTime = _startTime;
		newSave.distance = _distance;
		newSave.previoutDistance = _perviousDistance;
		newSave.score = _score;
		newSave.scoreDellay = _scoreDellay;
		newSave.numberOfLifes = _numberOfLifes;
		newSave.addedLifes = _addedLifes;
		for (int i = 0; i < MAX_AMOUNT_OF_CARS; i++) {
			if (_cars[i] == NULL) {
				newSave.myCars[i].exists = false;
				continue;
			}
			newSave.myCars[i].exists = true;
			newSave.myCars[i].position = _cars[i]->getPos();
			newSave.myCars[i].speed = _cars[i]->getSpeed();
			newSave.myCars[i].carStatus = _cars[i]->getStatus();
		}

		FILE* file = fopen(SAVE_FILE_NAME, "ab");
		if (file != NULL) {
			fwrite(&newSave, sizeof(save), 1, file);
			fclose(file);
		}
	}
};