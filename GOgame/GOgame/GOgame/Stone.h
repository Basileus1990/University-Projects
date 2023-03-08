// class used to manipulate the stones
#define CharacterSign char(254)
#define NullCharacter char(197)


enum Status {
	None,
	Black,
	White
};

class Stone {
	int _posX;
	int _posY;

	bool _checked = false;

	int _status = None;
	int _textColor = NULL;
	Widget _gameWidget;

public:
	Stone() {}

	Stone(int posX, int posY, Widget &gameWidget) :_gameWidget(gameWidget) {
		_posX = posX;
		_posY = posY;
	}

	void printStone() {
		if (_status == None) {
			_gameWidget.printCharInWidget(NullCharacter, _posX * 2, _posY, NULL, NULL);
			return;
		}
		_gameWidget.printCharInWidget(CharacterSign, _posX * 2, _posY, NULL, _textColor);
	}

	//Sets one of 3 values: None, Black, White and Prints the updated stone
	void setStatus(int newStatus) {
		if (newStatus == None) {
			_status = None;
			_textColor = NULL;
		}
		else if (newStatus == Black) {
			_status = Black;
			_textColor = BLACK;
		}
		else if (newStatus == White) {
			_status = White;
			_textColor = WHITE;
		}
		printStone();
	}

	int getStatus() {
		return _status;
	}

	void setOffsetX(int offSet) {
		_posX += offSet;
	}

	void setOffsetY(int offSet) {
		_posY += offSet;
	}

	bool wasChecked() {
		return _checked;
	}

	void flipChecked() {
		if (_checked) {
			_checked = false;
		}
		else {
			_checked = true;
		}
	}

};