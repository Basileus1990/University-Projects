#include <stdlib.h>
#include "Stone.h"
// Class used to acces and manipulate the game board

#define BoardBackgroundColor DARKGRAY
#define DefaultBoardTextColor BLACK
#define BetweenStonesCharacter char(196)
#define CursorCharacter '@'

class Board {
	// A one dimensional array for which acces is made as if it was 2D array
	Stone* _boardArray;
	int _size;
	int _drawnSizeX;
	int _drawnSizeY;

	int _cursorPosX = 0;
	int _cursorPosY = 0;
	int _cursorOffSetX = 0;
	int _cursorOffSetY = 0;

	Widget _boardWidget;

public:
	Board() {}

	void fillFields(int size, int newWidth, int newHeight, int x, int y) {
		_boardArray = (Stone*)(malloc(sizeof(Stone) * size * size));
		_size = size;
		_drawnSizeX = newWidth;
		_drawnSizeY = newHeight;

		_boardWidget.setFields(newWidth, newHeight, x, y, BoardBackgroundColor, DefaultBoardTextColor);

		_cursorPosX = _drawnSizeY / 2;
		_cursorPosY = _drawnSizeY / 2;

		fillBoard();
	}

	~Board() {
		free(_boardArray);
	}

private:
	// fills the board with Empty stones
	void fillBoard() {;
		for (int i = 0; i < _size; i++) {
			for (int j = 0; j < _size; j++) {
				Stone stone = Stone(i, j, _boardWidget);
				_boardArray[((_size * j) + i)] = stone;
			}

		}
	}

	void offSetStonesX(int offSet) {
		for (int i = 0; i < _size; i++) {
			for (int j = 0; j < _size; j++) {
				(*getBoardXY(i, j)).setOffsetX(offSet);
				(*getBoardXY(i, j)).printStone();
			}
		}
	}

	void offSetStonesY(int offSet) {
		for (int i = 0; i < _size; i++) {
			for (int j = 0; j < _size; j++) {
				(*getBoardXY(i, j)).setOffsetY(offSet);
				(*getBoardXY(i, j)).printStone();
			}
		}
	}

public:

	// get the board at XY
	Stone* getBoardXY(int x, int y) {
		if (x < 0 || y < 0 || x >= _size || y >= _size) {
			return nullptr;
		}
		return &_boardArray[((_size * y) + x)];
	}

	// returns pointer to boart at curren cursor position
	Stone* getBoardByCursorPos() {
		return getBoardXY(_cursorPosX + _cursorOffSetX, _cursorPosY + _cursorOffSetY);
	}

	// returns  pointer to boart at given cursor position
	Stone* getBoardByCursorPos(int cursorPosX, int cursorPosY) {
		return getBoardXY(cursorPosX + _cursorOffSetX, cursorPosY + _cursorOffSetY);
	}

	// prints whole board
	void printBoard() {
		_boardWidget.printBorder();
		// prints characters between the stones
		for (int i = 1; i < _drawnSizeX; i += 2) {
			for (int j = 0; j < _drawnSizeY; j++) {
				_boardWidget.printCharInWidget(BetweenStonesCharacter, i, j, BoardBackgroundColor, DefaultBoardTextColor);
			}
		}

		for (int i = 0; i < _size; i++) {
			for (int j = 0; j < _size; j++) {
				(* getBoardXY(i, j)).printStone();
			}
		}

		printCursor();
	}

	void setCursorX(int x) {
		if (x > _drawnSizeY - 1 || x < 0) {
			// checks if the cursor offset doesn't go off the boardArray
			if (_size != _drawnSizeY && ((_cursorOffSetX < _size - _drawnSizeY || (_cursorOffSetX == _size - _drawnSizeY && _cursorPosX - x != -1)) && (_cursorOffSetX > 0 || (_cursorOffSetX == 0 && _cursorPosX - x != 1)))) {
				_cursorOffSetX -= _cursorPosX - x;
				offSetStonesX(_cursorPosX - x);
				printCursor();
			}
			return;
		}

		(*getBoardXY(_cursorPosX + _cursorOffSetX, _cursorPosY + _cursorOffSetY)).printStone();
		_cursorPosX = x;
		printCursor();
	}

	void setCursorY(int y) {
		if (y > _drawnSizeY - 1 || y < 0) {
			// checks if the cursor offset doesn't go off the boardArray
			if (_size != _drawnSizeY && ((_cursorOffSetY < _size - _drawnSizeY || (_cursorOffSetY == _size - _drawnSizeY && _cursorPosY - y != -1)) && (_cursorOffSetY > 0 || (_cursorOffSetY == 0 && _cursorPosY - y != 1)))) {
				_cursorOffSetY -= _cursorPosY - y;
				offSetStonesY(_cursorPosY - y);
				printCursor();
			}
			return;
		}

		(*getBoardXY(_cursorPosX + _cursorOffSetX, _cursorPosY + _cursorOffSetY)).printStone();
		_cursorPosY = y;
		printCursor();
	}

	int getCursorX() {
		return _cursorPosX;
	}

	int getCursorY() {
		return _cursorPosY;
	}

	int getOffSettedCursorX() {
		return _cursorPosX + _cursorOffSetX;
	}

	int getOffSettedCursorY() {
		return _cursorPosY + _cursorOffSetY;
	}

	void printCursor() {
		_boardWidget.printCharInWidget(CursorCharacter, _cursorPosX * 2, _cursorPosY);
	}
};