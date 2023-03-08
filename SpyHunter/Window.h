#include<stdio.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

// A class for interaction with the screen
class Screen {
private:
	SDL_Event _event;
	SDL_Surface* _screen;
	SDL_Surface* _charset;
	SDL_Texture* _scrtex;
	SDL_Window* _window;
	SDL_Renderer* _renderer;

public:
	Screen() {

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			printf("SDL_Init error: %s\n", SDL_GetError());
			exit(1);
		}

		// fullscreen mode
		//int rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
		// 	&window, &renderer);
		int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
			&_window, &_renderer);
		if (rc != 0) {
			SDL_Quit();
			printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
			exit(1);
		};

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);

		SDL_SetWindowTitle(_window, "Pawel Bogdanowicz 193123");


		_screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
			0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

		_scrtex = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			SCREEN_WIDTH, SCREEN_HEIGHT);

		// cursor visibility
		SDL_ShowCursor(SDL_DISABLE);

		// the charset bmp image
		_charset = getBMP("./images/cs8x8.bmp");
	}

	~Screen() {
		SDL_FreeSurface(_screen);
		SDL_DestroyTexture(_scrtex);
		SDL_FreeSurface(_charset);
		SDL_DestroyRenderer(_renderer);
		SDL_DestroyWindow(_window);

		SDL_Quit();
	}

	SDL_Event* getEvent() {
		return &_event;
	}

	void update() {
		SDL_UpdateTexture(_scrtex, NULL, _screen->pixels, _screen->pitch);
		SDL_RenderCopy(_renderer, _scrtex, NULL, NULL);
		SDL_RenderPresent(_renderer);

	}

	// if retrns i != 0 program closes
	int handleEvents(int (*handler)(int e, SDL_Event)) {
		while (SDL_PollEvent(&_event)) {
			int quit = handler(_event.type, _event);
			if (quit) {
				return 1;
			}
		}
		return 0;
	}

	// fills the whole screen in given color
	void fillScreen(const int color[3]) {
		SDL_FillRect(_screen, NULL, getColor(color));
	}

	void fillRandomScreen(const int color[3]) {
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				drawPixel(i, j, getRandomColor(color));
			}
		}
	}

	int getColor(const int colorRGB[3]) {
		int returnValues[] = { colorRGB[0], colorRGB[1], colorRGB[2] };
		return SDL_MapRGB(_screen->format, returnValues[0], returnValues[1], returnValues[2]);
	}

	// return a random variation of given color
	int getRandomColor(const int colorRGB[3]) {
		return SDL_MapRGB(_screen->format, colorRGB[0] + (rand() % 50) - 8, colorRGB[1] + (rand() % 17) - 8, colorRGB[2] + (rand() % 17) - 8);
	}

	// draw a text txt on surface screen, starting from the point (x, y)
	// charset is a 128x128 bitmap containing character images
	void drawString(int x, int y, const char* text) {
		int px, py, c;
		SDL_Rect s, d;
		s.w = 8;
		s.h = 8;
		d.w = 8;
		d.h = 8;
		while (*text) {
			c = *text & 255;
			px = (c % 16) * 8;
			py = (c / 16) * 8;
			s.x = px;
			s.y = py;
			d.x = x;
			d.y = y;
			SDL_BlitSurface(_charset, &s, _screen, &d);
			x += 8;
			text++;
		};
	};

	// draw a surface sprite on a surface screen in point (x, y)
	// (x, y) is the center of sprite on screen
	void drawSurface(SDL_Surface* sprite, int x, int y) {
		SDL_Rect dest;
		dest.x = x - sprite->w / 2;
		dest.y = y - sprite->h / 2;
		dest.w = sprite->w;
		dest.h = sprite->h;
		SDL_BlitSurface(sprite, NULL, _screen, &dest);
	};

	// draw a single pixel
	void drawPixel(int x, int y, Uint32 color) {
		int bpp = _screen->format->BytesPerPixel;
		Uint8* p = (Uint8*)_screen->pixels + y * _screen->pitch + x * bpp;
		*(Uint32*)p = color;
	};

	// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
	// bπdü poziomie (gdy dx = 1, dy = 0)
	// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
	void drawLine(int x, int y, int l, int dx, int dy, Uint32 color) {
		for (int i = 0; i < l; i++) {
			drawPixel(x, y, color);
			x += dx;
			y += dy;
		};
	};

	// rysowanie prostokπta o d≥ugoúci bokÛw l i k
	// draw a rectangle of size l by k
	void drawRectangle(int x, int y, int l, int k,
		Uint32 outlineColor, Uint32 fillColor) {
		int i;
		drawLine(x, y, k, 0, 1, outlineColor);
		drawLine(x + l - 1, y, k, 0, 1, outlineColor);
		drawLine(x, y, l, 1, 0, outlineColor);
		drawLine(x, y + k - 1, l, 1, 0, outlineColor);
		for (i = y + 1; i < y + k - 1; i++)
			drawLine(x + 1, i, l - 2, 1, 0, fillColor);
	};

	SDL_Surface* getBMP(char* path, bool setColorKey = true) {
		SDL_Surface* BMP = SDL_LoadBMP(path);
		if (BMP == NULL) {
			printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
			exit(1);
		}
		if (setColorKey) {
			SDL_SetColorKey(BMP, true, 0x000000);
		}

		return BMP;
	}
};