const char* _legendText[4]{
	"Controls:",
	"Arrows - moving around the menu",
	"Enter  - select option",
	"ESQ    - exit the game"
};
const int _numberOfLegendLines = (sizeof(_legendText) / sizeof(char*));

#define OPTIONS_DEF_POSX (SCREEN_WIDTH / 2)
#define OPTIONS_DEF_POSY (SCREEN_HEIGHT / 2)
#define TITLE_BG_HEIGHT (LETTER_HEIGHT * 3)
#define TITLE_BG_WIDTH (SCREEN_WIDTH)
#define LEGEND_POSX (SCREEN_WIDTH - 40 * LETTER_WIDTH)
#define LEGEND_POSY (SCREEN_HEIGHT - 3 * LETTER_HEIGHT * _numberOfLegendLines)
#define LEGEND_MARGIN 5

// Menu class for interaction and drawing with the menu
class Menu {
private:

	// options displayed in the menu
	char** _menuOptions;
	int _numberOfMenuOptions;

	// the text which describes what the menu is about
	char* _title;
	// postiotion of the title
	pos _titlePos;

	// An ID of selected option
	int _highlightedOption = 0;
	int _selectedOption = -1;

	Screen* _screen;

public:
	Menu(Screen* screen, char** options, int amountOfOptions, const char* title)
	{
		_screen = screen;

		// Sets options dynamically
		_numberOfMenuOptions = amountOfOptions;
		_menuOptions = new char* [_numberOfMenuOptions];
		for (int i = 0; i < _numberOfMenuOptions; i++) {
			_menuOptions[i] = new char[strlen(options[i])];
			strcpy(_menuOptions[i], options[i]);
		}

		_title = new char[strlen(title)];
		strcpy(_title, title);
		_titlePos.x = (SCREEN_WIDTH / 2) - (strlen(title) * LETTER_WIDTH / 2);
		_titlePos.y = LETTER_HEIGHT;

		drawMenu();
	}

	// clears the screen and prints whole menu
	void drawMenu() {
		_screen->fillScreen(MENU_BACKGROUND_COLOR);
		_screen->drawRectangle(0, 0, TITLE_BG_WIDTH, TITLE_BG_HEIGHT, _screen->getColor(BLACK), _screen->getColor(BLACK));
		_screen->drawString(_titlePos.x, _titlePos.y, _title);

		drawLegend();
		for (int i = 0; i < _numberOfMenuOptions; i++) {
			pos optionPos;
			optionPos.x = OPTIONS_DEF_POSX - (strlen(_menuOptions[i]) * LETTER_WIDTH / 2);
			optionPos.y = OPTIONS_DEF_POSY + (i * LETTER_HEIGHT * 2) - (_numberOfMenuOptions * LETTER_HEIGHT);
			if (i == _highlightedOption) {
				_screen->drawRectangle(optionPos.x - LETTER_WIDTH / 2, optionPos.y - LETTER_HEIGHT / 2,
					(strlen(_menuOptions[i]) + 1) * LETTER_WIDTH, LETTER_HEIGHT * 2, _screen->getColor(BLACK), _screen->getColor(BLACK));
				_screen->drawString(optionPos.x, optionPos.y, _menuOptions[i]);
			}
			else {
				_screen->drawString(optionPos.x, optionPos.y, _menuOptions[i]);
			}
		}
	}

	// draw legend and creates a black rectangle which goes to the border of the screens
	void drawLegend() {
		for (int i = 0; i < _numberOfLegendLines; i++) {
			pos legendPos;
			legendPos.x = LEGEND_POSX;
			legendPos.y = LEGEND_POSY + i * LETTER_HEIGHT * 2;
			_screen->drawRectangle(legendPos.x, legendPos.y, SCREEN_WIDTH - legendPos.x, SCREEN_HEIGHT - legendPos.y, _screen->getColor(BLACK), _screen->getColor(BLACK));
			_screen->drawString(legendPos.x + LEGEND_MARGIN, legendPos.y + LEGEND_MARGIN, _legendText[i]);
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
				if ((*event).key.keysym.sym == SDLK_RETURN) {
					// sets the selected option
					_selectedOption = _highlightedOption;
					return 0;
				}
				if ((*event).key.keysym.sym == SDLK_DOWN) {
					goDown();
					return 0;
				}
				if ((*event).key.keysym.sym == SDLK_UP) {
					goUp();
					return 0;
				}
				break;
			case SDL_QUIT:
				return 1;
			}
		}
		return 0;
	}


	int getMenuOption() {
		int option = _selectedOption;
		// resets the selected option
		_selectedOption = -1;
		return option;
	}

private:
	void goDown() {
		_highlightedOption = (_highlightedOption + 1) % _numberOfMenuOptions;
		drawMenu();
	}

	void goUp() {
		if (_highlightedOption == 0) {
			_highlightedOption = (_highlightedOption + _numberOfMenuOptions - 1) % _numberOfMenuOptions;
			drawMenu();
			return;
		}
		_highlightedOption = (_highlightedOption - 1) % _numberOfMenuOptions;
		drawMenu();
	}
};