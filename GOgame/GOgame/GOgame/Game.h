#include "conio2.h"
#include "Board.h"
#include <stdio.h>


#define AmountOfLegendLines 15
const char* GameLegendText[AmountOfLegendLines] = {
	"Author: Pawel Bogdanowicz 193123",
	"",
	"@      - cursor position",
	"",
	"Controls:",
	"Arrows - moving around the board",
	"I      - place a stone on the board",
	"ESC	- cancel current action",
	"Enter  - confirm input and end",
	"         player's turn",
	"F      - finish the game",
	"N      - start a new game",
	"S      - save the game state",
	"L      - load the game state",
	"Q      - exit the game"
};

#define PlayersScorePosY AmountOfLegendLines + 1
#define LegendHeight PlayersScorePosY + 7
#define LegendWidth 36

#define GameSizeHeight 26
#define GameSizeWidth GameSizeHeight * 2 - 1
#define MaxBoardSizeHeight GameSizeHeight - 2
#define MaxBoardSizeWidth (MaxBoardSizeHeight) * 2 - 1

#define LegendOnTheLeft true

#define ArrowDown 0x50
#define ArrowUp 0x48
#define ArrowLeft 0x4b
#define ArrowRight 0x4d

#define BufferCursorDefaultValue -1
#define KoDefaultValue -2

#define LoadMenuSizeWidth 40
#define LoadMenuSizeHeight 7
#define SaveNameFileName "savesNames.sve"
#define SaveFileName "saves.sve"
#define MaxSaveName 30

// Class responsible for starting and operating the game
class Game {
	float _komiBonus;
	int _gameSize;
	int _playerTurn; // tells which player trun it is
	float _whiteScore;
	float _blackScore;
	int _posKoX;
	int _posKoY;

	// Position of the stone placed by "I" key. If Enter is clicked this resets
	int _stoneBufferPosX = BufferCursorDefaultValue;
	int _stoneBufferPosY = BufferCursorDefaultValue;

	Widget _legendWidget;
	Widget _gameWidget;
	Board _board;

public:
	// constructor for loading game from menu
	Game() : _board(Board()), _legendWidget(Widget()), _gameWidget(Widget()) {
		if (LegendOnTheLeft) {
			_legendWidget.setFields(LegendWidth, LegendHeight, 0, 0);
			_gameWidget.setFields(GameSizeWidth, GameSizeHeight, LegendWidth + 2, 0);
		}
		else {
			_legendWidget.setFields(LegendWidth, LegendHeight, GameSizeWidth + 2, 0);
			_gameWidget.setFields(GameSizeWidth, GameSizeHeight, 0, 0);
		}

		clrscr();
		printLegend();
	}

	Game(int gameSize, float komiBonus = 6.5) : _board(Board()), _legendWidget(Widget()), _gameWidget(Widget()) {
		_gameSize = gameSize;
		_komiBonus = komiBonus;
		_playerTurn = Black;
		_whiteScore = _komiBonus;
		_blackScore = 0;
		_posKoX = KoDefaultValue;
		_posKoY = KoDefaultValue;

		setupGame();
		handleUserInput();
	}

	// fills all the data game needs
	void setupGame() {
		if (LegendOnTheLeft) {
			_legendWidget.setFields(LegendWidth, LegendHeight, 0, 0);
			_gameWidget.setFields(GameSizeWidth, GameSizeHeight, LegendWidth + 2, 0);
		}
		else {
			_legendWidget.setFields(LegendWidth, LegendHeight, GameSizeWidth + 2, 0);
			_gameWidget.setFields(GameSizeWidth, GameSizeHeight, 0, 0);
		}

		if (_gameSize > MaxBoardSizeHeight) {
			_board.fillFields(_gameSize, MaxBoardSizeWidth, MaxBoardSizeHeight, _gameWidget.getStartX() + 1, _gameWidget.getStartY() + 1);
		}
		else {
			const int BoardWidth = _gameSize * 2 - 1;
			const int BoardHeight = _gameSize;
			_board.fillFields(_gameSize, BoardWidth, BoardHeight, _gameWidget.getStartX() + (GameSizeWidth - BoardWidth) / 2, _gameWidget.getStartY() + (GameSizeHeight - BoardHeight) / 2);
		}

		printGame();
	}

private:
	//Input Methods and starts the game
	void handleUserInput() {
		char input = NULL;
		while (input != 'q' && input != 'Q') {
			input = getch();
			if (input == 0) {
				handleArrows();
			}
			// place a stone and check if it can be placed. Check Ko rule
			else if (input == 'I' || input == 'i' && _board.getBoardByCursorPos()->getStatus() == None && _posKoX != _board.getCursorX() && _posKoY != _board.getCursorY()) {
				if (_posKoX > 0) {
					printf("%d %d", _posKoX, _board.getCursorX());
					getch();
				}
				_board.getBoardByCursorPos()->setStatus(_playerTurn);

				if (_stoneBufferPosX != BufferCursorDefaultValue) {
					_board.getBoardByCursorPos(_stoneBufferPosX, _stoneBufferPosY)->setStatus(None);
				}

				_stoneBufferPosX = _board.getCursorX();
				_stoneBufferPosY = _board.getCursorY();
			}
			else if (input == char(27)) { // ESQ key. Cleans user choice
				if (_stoneBufferPosX != BufferCursorDefaultValue) {
					_board.getBoardByCursorPos(_stoneBufferPosX, _stoneBufferPosY)->setStatus(None);
					_stoneBufferPosX = BufferCursorDefaultValue;
					_stoneBufferPosY = BufferCursorDefaultValue;
				}
			}
			else if (input == '\r') { // "Enter" key. Confirms user choice and proceeds to check if stone can be placed and if it kills any groups
				if (_stoneBufferPosX != BufferCursorDefaultValue) {
					_posKoX = KoDefaultValue;
					_posKoY = KoDefaultValue;
					int enemyColor;
					if (_playerTurn == Black) {
						enemyColor = White;
					}
					else {
						enemyColor = Black;
					}
					bool isKo = false;

					if (checkIfKo(_stoneBufferPosX, _stoneBufferPosY, enemyColor)) {
						isKo = true;
					}

					if (checkIfKilled(_stoneBufferPosX + 1, _stoneBufferPosY, enemyColor, _playerTurn)) {
						killStones(_stoneBufferPosX + 1, _stoneBufferPosY, enemyColor);
						if (isKo && checkIfKo(_stoneBufferPosX + 1, _stoneBufferPosY, _playerTurn)) {
							_posKoX = _stoneBufferPosX + 1;
							_posKoY = _stoneBufferPosY;
						}
					}
					if (checkIfKilled(_stoneBufferPosX - 1, _stoneBufferPosY, enemyColor, _playerTurn)) {
						killStones(_stoneBufferPosX - 1, _stoneBufferPosY, enemyColor);
						if (isKo && checkIfKo(_stoneBufferPosX - 1, _stoneBufferPosY, _playerTurn)) {
							_posKoX = _stoneBufferPosX - 1;
							_posKoY = _stoneBufferPosY;
						}
					}
					if (checkIfKilled(_stoneBufferPosX, _stoneBufferPosY + 1, enemyColor, _playerTurn)) {
						killStones(_stoneBufferPosX, _stoneBufferPosY + 1, enemyColor);
						if (isKo && checkIfKo(_stoneBufferPosX, _stoneBufferPosY + 1, _playerTurn)) {
							_posKoX = _stoneBufferPosX;
							_posKoY = _stoneBufferPosY + 1;
						}
					}
					if (checkIfKilled(_stoneBufferPosX, _stoneBufferPosY - 1, enemyColor, _playerTurn)) {
						killStones(_stoneBufferPosX, _stoneBufferPosY - 1, enemyColor);
						if (isKo && checkIfKo(_stoneBufferPosX, _stoneBufferPosY - 1, _playerTurn)) {
							_posKoX = _stoneBufferPosX;
							_posKoY = _stoneBufferPosY - 1;
						}
					}

					if (checkIfKilled(_stoneBufferPosX, _stoneBufferPosY, _playerTurn, enemyColor)) {
						_board.getBoardByCursorPos(_stoneBufferPosX, _stoneBufferPosY)->setStatus(None);
						if (_playerTurn == Black) {
							_playerTurn = White;
						}
						else {
							_playerTurn = Black;
						}
					}
				}
				
				_stoneBufferPosX = BufferCursorDefaultValue;
				_stoneBufferPosY = BufferCursorDefaultValue;

				//change player
				if (_playerTurn == Black) {
					_playerTurn = White;
				}
				else {
					_playerTurn = Black;
				}
				updateScores();
			}
			else if (input == 'F' || input == 'f') { // finish the game
				input = 'Q';
			}
			else if (input == 'N' || input == 'n') { // new game
				setupGame();
			}
			else if (input == 'S' || input == 's') { // save the gamestate
				if (_stoneBufferPosX != BufferCursorDefaultValue) {
					_board.getBoardByCursorPos(_stoneBufferPosX, _stoneBufferPosY)->setStatus(None);
				}
				saveGameState();
			}
			else if (input == 'L' || input == 'l') { // save the gamestate
				loadGameState();
			}
		}
	}

	void handleArrows() {
		char choice = getch();
		if (choice == ArrowDown) {
			_board.setCursorY(_board.getCursorY() + 1);
		}
		else if (choice == ArrowUp) {
			_board.setCursorY(_board.getCursorY() - 1);
		}
		else if (choice == ArrowLeft) {
			_board.setCursorX(_board.getCursorX() - 1);
		}
		else if (choice == ArrowRight) {
			_board.setCursorX(_board.getCursorX() + 1);
		}
		updateCursor();
	}

	// checks if tile should be killed. When it neighbours an ally stone it calls this method for them too
	bool checkIfKilled(int x, int y, int allyColor, int enemyColor) {
		bool isKilled = true;

		if (_board.getBoardByCursorPos(x, y) == nullptr || _board.getBoardByCursorPos(x, y)->getStatus() == enemyColor) {
			return true;
		}
		else if (_board.getBoardByCursorPos(x, y)->getStatus() == allyColor) {
			if (_board.getBoardByCursorPos(x, y)->wasChecked()) {
				return true;
			}
			_board.getBoardByCursorPos(x, y)->flipChecked();
			if		(!checkIfKilled(x + 1, y, allyColor, enemyColor)) isKilled = false;
			else if (!checkIfKilled(x - 1, y, allyColor, enemyColor)) isKilled = false;
			else if (!checkIfKilled(x, y + 1, allyColor, enemyColor)) isKilled = false;
			else if (!checkIfKilled(x, y - 1, allyColor, enemyColor)) isKilled = false;
		}
		else if (_board.getBoardByCursorPos(x, y)->getStatus() == None) {
			return false;
		}

		_board.getBoardByCursorPos(x, y)->flipChecked();
		return isKilled;
	}

	// Kills all stones in the group starting with XY
	void killStones(int x, int y, int allyColor) {
		bool toKilled = true;
		if (_board.getBoardByCursorPos(x, y) == nullptr || _board.getBoardByCursorPos(x, y)->wasChecked() || _board.getBoardByCursorPos(x, y)->getStatus() != allyColor) {
			return;
		}
		else if (_board.getBoardByCursorPos(x, y)->getStatus() == allyColor) {
			_board.getBoardByCursorPos(x, y)->flipChecked();
			_board.getBoardByCursorPos(x, y)->setStatus(None);
			if (allyColor == Black) {
				_whiteScore++;
			}
			else {
				_blackScore++;
			}
		}
		killStones(x + 1, y, allyColor);
		killStones(x - 1, y, allyColor);
		killStones(x, y + 1, allyColor);
		killStones(x, y - 1, allyColor);

		_board.getBoardByCursorPos(x, y)->flipChecked();
	}

	// chekcs if given stone is surrounded by nullptr or enemy stone
	bool checkIfKo(int x, int y, int enemyColor) {
		if (!(_board.getBoardByCursorPos(x + 1, y) != nullptr && _board.getBoardByCursorPos(x + 1, y)->getStatus() == enemyColor)) return false;
		if (!(_board.getBoardByCursorPos(x - 1, y) != nullptr && _board.getBoardByCursorPos(x - 1, y)->getStatus() == enemyColor)) return false;
		if (!(_board.getBoardByCursorPos(x, y + 1) != nullptr && _board.getBoardByCursorPos(x, y + 1)->getStatus() == enemyColor)) return false;
		if (!(_board.getBoardByCursorPos(x, y - 1) != nullptr && _board.getBoardByCursorPos(x, y - 1)->getStatus() == enemyColor)) return false;
		return true;
	}

	void printGame() {
		clrscr();
		_board.printBoard();
		printLegend();
	}

	void updateScores() {
		_legendWidget.clearLine(PlayersScorePosY);
		_legendWidget.clearLine(PlayersScorePosY + 1);

		char *score = (char*)malloc(sizeof(char) * 15);
		sprintf(score, "Black score: %.1f", _blackScore);
		_legendWidget.printInWidget(score, 0, PlayersScorePosY);
		sprintf(score, "White score: %.1f", _whiteScore);
		_legendWidget.printInWidget(score, 0, PlayersScorePosY + 1);

	}

	void updateCursor() {
		_legendWidget.clearLine(PlayersScorePosY + 3);
		_legendWidget.clearLine(PlayersScorePosY + 4);

		char* cursorPos = (char*)malloc(sizeof(char) * 15);
		sprintf(cursorPos, "Cursor posX: %d", _board.getOffSettedCursorX());
		_legendWidget.printInWidget(cursorPos, 0, PlayersScorePosY + 3);
		sprintf(cursorPos, "Cursor posX: %d", _board.getOffSettedCursorY());
		_legendWidget.printInWidget(cursorPos, 0, PlayersScorePosY + 4);
	}

	void printLegend() {
		_legendWidget.printBorder();

		// Print legend text
		for (int i = 0; i < AmountOfLegendLines; i++) {
			_legendWidget.printInWidget(GameLegendText[i], 0, i);
		}
		
		// print player scores
		updateScores();
		updateCursor();
	}

	void saveGameState() {
		char saveFileName[MaxSaveName];
		while (strstr(saveFileName, ".txt") == NULL) {
			strcpy(saveFileName, _gameWidget.getUserStringInput("Save name in txt format:"));
		}
		saveFileName[strlen(saveFileName) - 1] = '\0';

		FILE* file = fopen(saveFileName, "wb");
		if (file != NULL) {
			fwrite(&_komiBonus, sizeof(float), 1, file);
			fwrite(&_gameSize, sizeof(int), 1, file);
			fwrite(&_playerTurn, sizeof(int), 1, file);
			fwrite(&_whiteScore, sizeof(float), 1, file);
			fwrite(&_blackScore, sizeof(float), 1, file);
			fwrite(&_posKoX, sizeof(int), 1, file);
			fwrite(&_posKoY, sizeof(int), 1, file);
			for (int i = 0; i < _gameSize; i++) {
				for (int j = 0; j < _gameSize; j++) {
					fwrite(_board.getBoardXY(i, j), sizeof(Stone), 1, file);
				}
			}
			fclose(file);
		}
	}

public:
	void loadGameState() {
		char saveFileName[MaxSaveName];
		while (strstr(saveFileName, ".txt") == NULL) {
			strcpy(saveFileName, _gameWidget.getUserStringInput("Load game from txt file:"));
		}
		saveFileName[strlen(saveFileName) - 1] = '\0';

		FILE* file = fopen(saveFileName, "rb");
		if (file != NULL) {
			fread(&_komiBonus, sizeof(float), 1, file);
			fread(&_gameSize, sizeof(int), 1, file);
			fread(&_playerTurn, sizeof(int), 1, file);
			fread(&_whiteScore, sizeof(float), 1, file);
			fread(&_blackScore, sizeof(float), 1, file);
			fread(&_posKoX, sizeof(int), 1, file);
			fread(&_posKoY, sizeof(int), 1, file);
			setupGame();
			for (int i = 0; i < _gameSize; i++) {
				for (int j = 0; j < _gameSize; j++) {
					fread(_board.getBoardXY(i, j), sizeof(Stone), 1, file);
				}
			}
			fclose(file);
		}
		else {
			return;
		}

		printGame();
		handleUserInput();
	}
};