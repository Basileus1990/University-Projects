#define TREE1 "./images/Tree1.bmp"
#define TREE2 "./images/Tree2.bmp"

// class resonsible for tree behaviour
class Tree {
	SDL_Surface* _treePhoto;

	pos _pos;
	Screen* _screen;


public:
	Tree(Screen* screen, int posX = NULL, int posY = NULL) {
		_screen = screen;

		_pos.x = posX;
		_pos.y = posY;

		if (rand() % 2) {
			_treePhoto = _screen->getBMP(TREE1);
		}
		else {
			_treePhoto = _screen->getBMP(TREE2);
		}
	}

	void drawTree() {
		if (_pos.x == NULL || _pos.y == NULL) {
			return;
		}
		_screen->drawSurface(_treePhoto, _pos.x, _pos.y);
	}

	void setPosition(int x, int y) {
		_pos.x = x;
		_pos.y = y;
	}

	void moveDown(double speed) {
		_pos.y += speed;
	}

	int getY() {
		return _pos.y;
	}

};